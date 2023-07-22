#pragma once

#include "skizzay/kerchow/arithmetic.h"
#include <concepts>
#include <random>

namespace skizzay::kerchow::concepts {
template <typename D>
concept random_number_distribution = std::regular<D> &&
    arithmetic<typename D::result_type> &&
    std::semiregular<typename D::param_type> &&
    requires(D d, D const x, std::random_device &g) {
  {d.reset()};
  { x.param() } -> std::same_as<typename D::param_type>;
  { x.min() } -> std::same_as<typename D::result_type>;
  { x.max() } -> std::same_as<typename D::result_type>;
  { d(g) } -> std::same_as<typename D::result_type>;
  { d(g, d.param()) } -> std::same_as<typename D::result_type>;
};
} // namespace skizzay::kerchow::concepts
