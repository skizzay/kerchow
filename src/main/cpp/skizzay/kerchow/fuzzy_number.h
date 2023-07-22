#pragma once

#include "skizzay/kerchow/arithmetic.h"
#include "skizzay/kerchow/distribution.h"
#include "skizzay/kerchow/uniform_distribution.h"
#include <random>

namespace skizzay {
namespace kerchow {

template <concepts::arithmetic ArithmeticType,
          concepts::random_number_distribution DistributionType =
              uniform_distribution<ArithmeticType>,
          std::uniform_random_bit_generator GeneratorType = std::mt19937_64>
class fuzzy_number {
public:
  using generator_type = GeneratorType;
  using distribution_type = DistributionType;
  using result_type = typename distribution_type::result_type;

  inline fuzzy_number(generator_type &g, auto &&...distribution_args)
      : generator_{g}, distribution_{std::forward<decltype(distribution_args)>(
                           distribution_args)...} {
    next();
  }

  inline void next() noexcept { current_ = distribution_(generator_); }

  inline result_type value() const noexcept { return current_; }

  inline operator result_type() const noexcept { return value(); }

  inline typename distribution_type::param_type
  distribution_parameters() const {
    return distribution_.param();
  }

private:
  generator_type &generator_;
  distribution_type distribution_;
  result_type current_;
};
} // namespace kerchow
} // namespace skizzay
