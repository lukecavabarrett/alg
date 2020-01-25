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
  assert(K - 1 <= N);
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

template<typename InputIt, typename OutputIt>
void compress_suffix_nlogn(InputIt first, InputIt last, OutputIt out) {
  typedef typename std::iterator_traits<OutputIt>::value_type size_t;
  typedef typename std::iterator_traits<InputIt>::value_type size_t_;
  static_assert(std::is_same_v<size_t, size_t_>);
  static_assert(std::is_integral_v<size_t>);
  static_assert(std::is_unsigned_v<size_t>);
  // assume values in [0,K)
  const size_t N = std::distance(first, last);
  size_t K = block_radix_compress<2>(first,last,out);
  OutputIt out_last = out + N;
  while (K < N){
    K = block_radix_compress<2>(out,out_last,out);
  }
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
  assert(std::distance(first, last) < std::numeric_limits<size_t>::max());
}

}

#endif //ALG_SUFFIX_ARRAY_H
