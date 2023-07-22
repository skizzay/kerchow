#pragma once

#include <skizzay/kerchow/arithmetic.h>

#include <random>
#include <type_traits>

namespace skizzay {
namespace kerchow {

template <concepts::arithmetic Arithmetic>
using uniform_distribution =
    std::conditional_t<std::is_integral_v<Arithmetic>,
                       std::uniform_int_distribution<Arithmetic>,
                       std::uniform_real_distribution<Arithmetic>>;

} // namespace kerchow
} // namespace skizzay
