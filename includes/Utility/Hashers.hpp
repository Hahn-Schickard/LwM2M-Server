#ifndef __COMMON_HASHER_FUNCTOR_HPP
#define __COMMON_HASHER_FUNCTOR_HPP

#include <cstdint>
#include <functional>
#include <vector>

namespace std {
template <> struct hash<vector<uint8_t>> {
  size_t operator()(vector<uint8_t> const &vec) const {
    size_t seed = vec.size();
    for (auto &i : vec) {
      seed ^= i + 0x9e3779b9 + (seed << 6) + (seed >> 2);
    }
    return seed;
  }
};
} // namespace std

#endif //__COMMON_HASHER_FUNCTOR_HPP