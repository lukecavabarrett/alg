#ifndef AUX_PRETTY_PRINT_H
#define AUX_PRETTY_PRINT_H

#include <iostream>
#include <tuple>

namespace aux {
template<std::size_t...>
struct seq {
};

template<std::size_t N, std::size_t... Is>
struct gen_seq : gen_seq<N - 1, N - 1, Is...> {
};

template<std::size_t... Is>
struct gen_seq<0, Is...> : seq<Is...> {
};

template<class Ch, class Tr, class Tuple, std::size_t... Is>
void print_tuple(std::basic_ostream<Ch, Tr> &os, Tuple const &t, seq<Is...>) {
  using swallow = int[];
  (void) swallow{0, (void(os << (Is == 0 ? "" : ", ") << std::get<Is>(t)), 0)...};
}

template< class Ch, class Tr,typename InputIt>
auto print_range(std::basic_ostream<Ch, Tr> &os,InputIt first, InputIt last) -> std::basic_ostream<Ch, Tr> & {
  os << "[";
  for(auto it = first; it != last; ++it){
    os << (it == first ? "" : ", ") << *it;
  }
  return os << "]";
}
}

template<class Ch, class Tr, class... Args>
auto operator<<(std::basic_ostream<Ch, Tr> &os, std::tuple<Args...> const &t)
-> std::basic_ostream<Ch, Tr> & {
  os << "(";
  aux::print_tuple(os, t, aux::gen_seq<sizeof...(Args)>());
  return os << ")";
}

template<class Ch, class Tr, class T, class... Args>
auto operator<<(std::basic_ostream<Ch, Tr> &os, std::vector<T, Args...> const &v)
-> std::basic_ostream<Ch, Tr> & {
  os << "[";
  for(auto it = v.cbegin(); it != v.cend(); ++it){
    os << (it == v.cbegin() ? "" : ", ") << *it;
  }
  return os << "]";
}

#endif //AUX_PRETTY_PRINT_H