#ifndef __VARIANT_VISITOR_HPP
#define __VARIANT_VISITOR_HPP

#include <variant>

namespace detail {

template <class... Ts> struct overloaded : Ts... { using Ts::operator()...; };

template <class... Ts> overloaded(Ts...)->overloaded<Ts...>;

} // namespace detail
template <typename Variant, typename... Matchers>
auto match(Variant &&variant, Matchers &&... matchers) {
  return std::visit(detail::overloaded{std::forward<Matchers>(matchers)...},
                    std::forward<Variant>(variant));
}

#endif //__VARIANT_VISITOR_HPP