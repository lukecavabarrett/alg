#ifndef ALG_SUFFIX_ARRAY_H
#define ALG_SUFFIX_ARRAY_H

#include <numeric>
#include "pretty_print.h"
namespace alg {

template<typename InputIt, typename OutputIt>
typename std::iterator_traits<OutputIt>::value_type single_compress(InputIt first, InputIt last, OutputIt out) {
  typedef typename std::iterator_traits<OutputIt>::value_type size_t;
  typedef typename std::iterator_traits<InputIt>::value_type size_t_;
  static_assert(std::is_same_v<size_t, size_t_>);
  static_assert(std::is_integral_v<size_t>);
  static_assert(std::is_unsigned_v<size_t>);
  if (first == last)return 0;
  const size_t N = std::distance(first, last), K = *std::max_element(first, last) + 1;
  size_t count[K];
  std::fill(count, count + K, 0);
  for (InputIt i = first; i != last; ++i)++count[*i];
  std::copy_backward(count, count + K - 1, count + K);
  count[0] = 0;
  std::partial_sum(count, count + K, count);
  size_t sorted[N];
  {
    size_t i = 0;
    for (InputIt it = first; it != last; ++it, ++i) {
      sorted[count[*it]++] = i;
    }
  }
  std::adjacent_difference(count, count + K, count);
  size_t val = 0, b = 0;
  while (!count[b])++b;
  for (size_t s : sorted) {
    out[s] = val;
    if (--count[b] == 0) {
      ++val;
      while (!count[b])++b;
    }
  }
  assert(std::accumulate(count, count + K, true, [](auto p, auto x) { return p && (x == 0); }));
  return val;
}

template<typename InputIt, typename OutputIt, typename BaseT = uint8_t>
typename std::iterator_traits<OutputIt>::value_type radix_compress(InputIt first, InputIt last, OutputIt out) {
  typedef typename std::iterator_traits<InputIt>::value_type char_t;
  typedef typename std::iterator_traits<OutputIt>::value_type size_t;
  typedef std::make_unsigned_t<char_t> uchar_t;
  static_assert(std::is_integral_v<char_t>);
  static_assert(std::is_integral_v<size_t>);
  static_assert(std::is_unsigned_v<size_t>);
  //if constexpr (std::is_signed_v<char_t>) return 0;
  if (first == last)return 0;
  static_assert(sizeof(size_t) > sizeof(BaseT));
  const size_t N = std::distance(first, last);
  constexpr size_t Bbits = sizeof(BaseT) * 8;
  constexpr size_t K = size_t(1) << Bbits;
  constexpr BaseT MSB = BaseT(1) << (Bbits - 1);
  constexpr size_t Bmsk = K - 1;
  constexpr size_t P = sizeof(char_t) / sizeof(BaseT); // number of passes
  static_assert(P > 0);
  size_t sorted[N];
  std::iota(sorted, sorted + N, 0);
  for (size_t p = 0, sh = 0; p < P; ++p, sh += Bbits) {
    BaseT bsw = std::is_unsigned_v<char_t> ? 0 : (p == P - 1) ? MSB : 0;
    size_t count[K];
    std::fill(count, count + K, 0);
    for (InputIt i = first; i != last; ++i)++count[static_cast<BaseT>(static_cast<uchar_t>(*i) >> sh) ^ bsw];
    std::copy_backward(count, count + K - 1, count + K);
    count[0] = 0;
    std::partial_sum(count, count + K, count);
    size_t sorted_[N];
    for (size_t s : sorted) {
      sorted_[count[static_cast<BaseT>(static_cast<uchar_t>(first[s]) >> sh) ^ bsw]++] = s;
    }
    std::copy(sorted_, sorted_ + N, sorted);
  }
  size_t val = 0;
  char_t prv = first[sorted[0]];
  for (size_t s : sorted) {
    if (first[s] != prv) {
      ++val;
      prv = first[s];
    }
    out[s] = val;
  }
  return val + 1;
}

template<uint64_t BlockLength, typename InputIt, typename OutputIt >
typename std::iterator_traits<OutputIt>::value_type block_radix_compress(InputIt first, InputIt last, OutputIt out) {
  typedef typename std::iterator_traits<OutputIt>::value_type size_t;
  typedef typename std::iterator_traits<InputIt>::value_type size_t_;
  static_assert(std::is_same_v<size_t, size_t_>);
  static_assert(std::is_integral_v<size_t>);
  static_assert(std::is_unsigned_v<size_t>);
  static_assert(BlockLength > 0);
  if (first == last)return 0;
  const size_t N = std::distance(first, last), K = *std::max_element(first, last) + 2;
  assert(K - 1 <= N*2);
  size_t sorted[N];
  std::iota(sorted, sorted + N, 0);
  size_t first_[N + BlockLength - 1];
  std::copy(first, last, first_);
  for (size_t i = 0; i < N; ++i)++first_[i];
  std::fill(first_ + N, first_ + N + BlockLength - 1, 0);
  for (size_t p = BlockLength; p; --p) {
    size_t count[K];
    std::fill(count, count + K, 0);
    for (size_t i = 0; i < N; ++i)++count[first_[i + p -1]];
    std::copy_backward(count, count + K - 1, count + K);
    count[0] = 0;
    std::partial_sum(count, count + K, count);
    size_t sorted_[N];
    for (size_t s : sorted) {
      sorted_[count[first_[s + p -1]]++] = s;
    }
    std::copy(sorted_, sorted_ + N, sorted);
  }

  size_t val = 0, prv_s = sorted[0];
  for (size_t s : sorted) {
    for (size_t p = 0; p < BlockLength; ++p)
      if (first_[s + p] != first_[prv_s + p]) {
        ++val;
        break;
      }
    prv_s = s;
    out[s] = val;
  }
  return val + 1;
}


template<typename InputIt, typename OutputIt >
typename std::iterator_traits<OutputIt>::value_type couple_radix_compress(InputIt first, InputIt last, OutputIt out, size_t W) {
  typedef typename std::iterator_traits<OutputIt>::value_type size_t;
  typedef typename std::iterator_traits<InputIt>::value_type size_t_;
  static_assert(std::is_same_v<size_t, size_t_>);
  static_assert(std::is_integral_v<size_t>);
  static_assert(std::is_unsigned_v<size_t>);
  assert(W > 0);
  if (first == last)return 0;
  const size_t N = std::distance(first, last), K = *std::max_element(first, last) + 2;
  assert(K - 1 <= N);

  size_t sorted[N];
  //LSB
  {
    size_t count[K];
    std::fill(count, count + K, 0);
    for (size_t i = W; i < N; ++i)++count[first[i]+1];
    count[0]=W;
    std::copy_backward(count, count + K - 1, count + K);
    count[0] = 0;
    std::partial_sum(count, count + K, count);
    for (size_t s = 0; s+W < N; ++s) {
      sorted[count[first[s + W]+1]++] = s;
    }
    for (size_t s = N-W; s < N; ++s) {
      sorted[count[0]++] = s;
    }
  }
  //MSB
  {
    size_t count[K];
    std::fill(count, count + K, 0);
    for (size_t i = 0; i < N; ++i)++count[first[i]];
    std::copy_backward(count, count + K - 1, count + K);
    count[0] = 0;
    std::partial_sum(count, count + K, count);
    size_t sorted_[N];
    for (size_t s : sorted) {
      sorted_[count[first[s]]++] = s;
    }
    std::copy(sorted_, sorted_ + N, sorted);
  }
  size_t out_[N];
  size_t val = 0, prv_val_0 = first[sorted[0]], prv_val_1 = sorted[0]+W < N ? first[sorted[0]+W]+1 : 0;
  for (size_t s : sorted) {
    size_t val_0 = first[s];
    size_t val_1 = s+W < N ? first[s+W]+1 : 0;
    if(val_0 != prv_val_0 || val_1 != prv_val_1){
      ++val;
      prv_val_0 = val_0;
      prv_val_1 = val_1;
    }
    out_[s] = val;
  }
  std::copy(out_,out_+N,out);
  return val + 1;
}

template<typename InputIt, typename OutputIt>
void compress_suffix_insertion(InputIt first, InputIt last, OutputIt out) {
  typedef typename std::iterator_traits<OutputIt>::value_type size_t;
  typedef typename std::iterator_traits<InputIt>::value_type size_t_;
  static_assert(std::is_same_v<size_t, size_t_>);
  static_assert(std::is_integral_v<size_t>);
  static_assert(std::is_unsigned_v<size_t>);
  // assume values in [0,K)
  const size_t N = std::distance(first, last);
  size_t sorted[N];
  for(size_t idx = 0; idx < N; ++idx){
    size_t *l=sorted, *r=sorted+idx;
    auto rng = std::make_tuple(l-sorted,r-sorted);
    for(size_t i = 0; idx+i<N && l!=r; ++i){
      auto lr = std::equal_range(l,r,idx,[&first, i](const auto& id1,const auto& id2){return first[id1+i]<first[id2+i];});
      l = lr.first;
      r = lr.second;
      rng = std::make_tuple(l-sorted,r-sorted);
    }
    std::copy_backward(l,sorted+idx,sorted+idx+1);
    *l = idx;
  }
  size_t val  = 0;
  for (size_t s : sorted)out[s]=val++;
}

template<typename InputIt, typename OutputIt>
void compress_suffix_nlogn(InputIt first, InputIt last, OutputIt out) {
  typedef typename std::iterator_traits<OutputIt>::value_type size_t;
  typedef typename std::iterator_traits<InputIt>::value_type size_t_;
  static_assert(std::is_same_v<size_t, size_t_>);
  static_assert(std::is_integral_v<size_t>);
  static_assert(std::is_unsigned_v<size_t>);
  // assume values in [0,K)
  const size_t N = std::distance(first, last);
  size_t K = couple_radix_compress(first,last,out,1);
  OutputIt out_last = out + N;
  size_t W = 2;
  while (K < N){
    K = couple_radix_compress(out,out_last,out,W);
    W<<=1;
  }
  return;
}

template<typename InputIt>
void qd_suffix_insertion(InputIt a, typename std::iterator_traits<InputIt>::value_type N) {
  typedef typename std::iterator_traits<InputIt>::value_type size_t;
  static_assert(std::is_integral_v<size_t>);
  static_assert(std::is_unsigned_v<size_t>);
  size_t sorted[N];
  for(size_t idx = 0; idx < N; ++idx){
    size_t *l=sorted, *r=sorted+idx;
    auto rng = std::make_tuple(l-sorted,r-sorted);
    for(size_t i = 0; idx+i<N && l!=r; ++i){
      auto lr = std::equal_range(l,r,idx,[&a, i](const auto& id1,const auto& id2){return a[id1+i]<a[id2+i];});
      l = lr.first;
      r = lr.second;
      rng = std::make_tuple(l-sorted,r-sorted);
    }
    std::copy_backward(l,sorted+idx,sorted+idx+1);
    *l = idx;
  }
  std::copy(sorted,sorted+N,a);
}

template<typename InputIt>
void ks_suffix_array(InputIt a, typename std::iterator_traits<InputIt>::value_type N){
  typedef typename std::iterator_traits<InputIt>::value_type size_t;
  static_assert(std::is_integral_v<size_t>);
  static_assert(std::is_unsigned_v<size_t>);


  constexpr size_t quadratic_threshold = 5;
  bool force_quad = false;
  if(N<=quadratic_threshold || force_quad)return qd_suffix_insertion(a, N);
  const size_t MAX_EXP = (N>>1)*3 + (N&1)<<1;
  const size_t MAX = *std::max_element(a,a+N);
  assert(MAX<=MAX_EXP);
  size_t a12[N];
  block_radix_compress<3>(a,a+N,a12);//TODO: do here
  const size_t N_0 = (N+2)/3, N_1 = (N+1)/3, N_2 = N/3;
  size_t sorted12[N_1+N_2];
  const bool invert_12 = (N%3 == 1);
  if(invert_12){
    for(size_t i = 0,i2=2; i < N_2; ++i,i2+=3)sorted12[i]=a12[i2];
    for(size_t i = 0,i1=1; i < N_1; ++i,i1+=3)sorted12[N_2+i]=a12[i1];
  } else {
    for(size_t i = 0,i1=1; i < N_1; ++i,i1+=3)sorted12[i]=a12[i1];
    for(size_t i = 0,i2=2; i < N_2; ++i,i2+=3)sorted12[N_1+i]=a12[i2];
  }
  //recursive call
  ks_suffix_array(sorted12,N_1+N_2);

  if(invert_12){
    for(size_t &s : sorted12)s= s<N_2 ? s*3+2 : (s-N_2)*3+1;
  } else {
    for(size_t &s : sorted12)s= s<N_1 ? s*3+1 : (s-N_1)*3+2;
  }
  size_t sorted0[N_0];
  {
    // prepare MSD pass
    size_t const K = MAX+1;
    size_t count[K];
    std::fill(count, count + K, 0);
    for (size_t i = 0; i < N; i += 3)++count[a[i]];
    std::copy_backward(count, count + K - 1, count + K);
    count[0] = 0;
    std::partial_sum(count, count + K, count);
    //TODO: avoid modulus in following part - fill in while unrolling sorted12
#define push_cnt(X) sorted0[count[a[ X ]]++] = X
    if (invert_12) {
      assert(N_0 == N_1 + 1);
      push_cnt(N-1);
      for (size_t s : sorted12)if (s % 3 == 1) push_cnt(s - 1);
    } else {
      assert(N_0 == N_1);
      for (size_t s : sorted12){
        if (s % 3 == 1){
          push_cnt(s - 1);
        }
      }
    }
#undef push_cnt
  }
  {
    size_t val = 1;
    for(size_t s12 : sorted12)a12[s12]=val++;
  }
  //merge
  size_t sorted012[N];
  size_t i0 = 0, i12 = 0;
  for(size_t &s_out : sorted012){
    if(i0==N_0)s_out=sorted12[i12++];
    else if(i12==N_1+N_2)s_out=sorted0[i0++];
    else{
      const size_t s0 = sorted0[i0];
      const size_t s12 = sorted12[i12];
      bool cmp;
      if(a[s0]==a[s12]){
      if(s12%3==1){
        size_t val_0 = s0==N-1 ? 0 : a12[s0+1];
        size_t val_1 = s12==N-1 ? 0 : a12[s12+1];
        cmp = val_0 < val_1;
      } else {
        size_t a0_1 = s0==N-1 ? 0 : a[s0+1]+1;
        size_t a12_1 = s12==N-1 ? 0 : a[s12+1]+1;
        if(a0_1==a12_1){
          a0_1 = s0==N-2 ? 0 : a12[s0+2];
          a12_1 = s12==N-2 ? 0 : a12[s12+2];
          cmp = a0_1<a12_1;
        }else cmp = a0_1<a12_1;
      }} else {
        cmp = a[s0]<a[s12];
      }

      if(cmp)s_out=sorted0[i0++]; else s_out=sorted12[i12++];

    }
  }
  std::copy(sorted012,sorted012+N,a);
}

template<typename It>
struct unsigned_iterator : It {
  typedef std::make_unsigned_t<typename It::value_type> value_type;
  unsigned_iterator(It it) : It(it) {}
};

template<typename InputIt, typename OutputIt>
void suffix_array(InputIt first, InputIt last, OutputIt out) {
  typedef typename std::iterator_traits<OutputIt>::value_type size_t;
  typedef typename std::iterator_traits<InputIt>::value_type char_t;
  static_assert(std::is_integral_v<size_t>);
  static_assert(std::is_integral_v<char_t>);
  auto N = std::distance(first, last);
  assert(N < std::numeric_limits<size_t>::max());
  radix_compress(first,last,out);
  ks_suffix_array(out,size_t(N));
}

}

#endif //ALG_SUFFIX_ARRAY_H
