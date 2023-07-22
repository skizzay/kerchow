#pragma once

#include <type_traits>

namespace skizzay::kerchow::concepts {
template <typename T>
concept arithmetic = std::is_arithmetic_v<T>;
} // namespace skizzay::kerchow::concepts
