#pragma once

#include "skizzay/kerchow/uniform_distribution.h"
#include <algorithm>
#include <random>
#include <utility>
#include <vector>

namespace skizzay {
namespace kerchow {

template <class T,
          std::uniform_random_bit_generator Generator = std::mt19937_64>
struct fuzzy_container
    : std::ranges::view_interface<fuzzy_container<T, Generator>> {
  using generator_type = Generator;

  inline fuzzy_container(generator_type &g, auto &&...value_args)
      : fuzzy_container{g, std::vector<T>(std::forward<decltype(value_args)>(
                               value_args)...)} {}

  template <typename U = T>
  inline fuzzy_container(generator_type &g,
                         std::initializer_list<U> const value_args)
      : fuzzy_container{g, std::vector<T>(value_args)} {}

  inline auto max_size() const noexcept { return values_.max_size(); }

  inline auto begin() noexcept { return std::ranges::begin(values_); }

  inline auto end() noexcept { return std::ranges::end(values_); }

  inline auto begin() const noexcept { return std::ranges::begin(values_); }

  inline auto end() const noexcept { return std::ranges::end(values_); }

  inline void push(T const &value) {
    values_.push_back(value);
    shuffle();
  }

  inline void push(std::ranges::input_range auto const &values) {
    values_.insert(end(), std::ranges::begin(values), std::ranges::end(values));
    shuffle();
  }

  template <class... Args> inline void emplace(Args &&...args) {
    values_.emplace_back(std::forward<Args>(args)...);
    shuffle();
  }

private:
  explicit inline fuzzy_container(generator_type &g,
                                  std::vector<T> &&values) noexcept
      : generator_{&g}, values_{std::move(values)} {
    shuffle();
  }

  inline void shuffle() noexcept { std::ranges::shuffle(values_, *generator_); }

  generator_type *generator_;
  std::vector<T> values_;
};

} // namespace kerchow
} // namespace skizzay