#pragma once

#include <concepts>
#include <functional>
#include <type_traits>
#include <utility>

namespace skizzay::kerchow {
namespace coalesce_details_ {
struct coalesce_fn final {
  template <std::invocable... Fns>
  requires std::default_initializable<
      std::common_type_t<std::invoke_result_t<Fns>...>>
  constexpr auto operator()(Fns &&...fns) const
      noexcept(std::conjunction_v<std::is_nothrow_invocable<Fns>...>) {
    return calculate(std::common_type_t<std::invoke_result_t<Fns>...>{},
                     std::forward<decltype(fns)>(fns)...);
  }

  template <std::invocable... Fns>
  constexpr auto operator()(auto default_value, Fns &&...fns) const
      noexcept(std::conjunction_v<std::is_nothrow_invocable<Fns>...>) {
    return calculate(std::move(default_value),
                     std::forward<decltype(fns)>(fns)...);
  }

private:
  constexpr auto calculate(auto default_value,
                           std::invocable auto &&...fns) const
      noexcept(
          std::conjunction_v<std::is_nothrow_invocable<decltype(fns)>...>) {
    auto const impl = [default_value = std::move(default_value)](
                          auto &impl, auto &&fn, auto &&...fns) {
      auto const result = std::invoke(std::forward<decltype(fn)>(fn));
      if (result) {
        return result;
      } else if constexpr (0 == sizeof...(fns)) {
        return default_value;
      } else {
        return impl(impl, std::forward<decltype(fns)>(fns)...);
      }
    };
    return impl(impl, std::forward<decltype(fns)>(fns)...);
  }
};
} // namespace coalesce_details_

inline namespace coalesce_fn_ns {
inline constexpr coalesce_details_::coalesce_fn coalesce = {};
} // namespace coalesce_fn_ns

} // namespace skizzay::kerchow
