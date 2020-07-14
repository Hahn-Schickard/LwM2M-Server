#ifndef __UTILTIY_UNIQUE_PTR_CAST_HPP
#define __UTILTIY_UNIQUE_PTR_CAST_HPP

#include <memory>

namespace utility {
/**
 * @brief Casts a polymorphic unique_ptr base into a given child
 *
 * @tparam R - Child Type
 * @tparam T - Base Type
 * @param target
 * @return std::unique_ptr<R>
 */
template <typename R, typename T>
std::unique_ptr<R> static_pointer_cast(std::unique_ptr<T> target) {
  if (R *target_base = static_cast<R *>(target.get())) {
    std::unique_ptr<R> result(target_base);
    if (!result)
      throw std::bad_cast();
    target.release();
    return result;
  }
  throw std::bad_cast();
}
} // namespace utility
#endif //__UTILTIY_UNIQUE_PTR_CAST_HPP