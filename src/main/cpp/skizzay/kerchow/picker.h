#pragma once

#include "skizzay/kerchow/coalesce.h"
#include "skizzay/kerchow/fuzzy_container.h"
#include "skizzay/kerchow/fuzzy_number.h"
#include <skizzay/kerchow/arithmetic.h>

#include <cassert>
#include <charconv>
#include <cstring>
#include <filesystem>
#include <iterator>
#include <limits>
#include <optional>
#include <random>
#include <ranges>

namespace skizzay {
namespace kerchow {

template <std::uniform_random_bit_generator Generator>
struct basic_random_picker {
public:
  using generator_type = Generator;
  using result_type = typename generator_type::result_type;

  inline basic_random_picker(
      result_type seed_value = generator_type::default_seed)
      : generator(seed_value) {}

  inline basic_random_picker(std::seed_seq &sseq) : generator(sseq) {}

  static constexpr result_type min() { return generator_type::min(); }

  static constexpr result_type max() { return generator_type::max(); }

  inline void seed(result_type seed) requires
      requires(Generator &g, result_type seed) {
    {g.seed(seed)};
  }
  { generator.seed(seed); }

  template <class Sseq>
  inline void seed(Sseq &seq) requires requires(Generator &g, Sseq &seq) {
    {g.seed(seq)};
  }
  { generator.seed(seq); }

  inline result_type operator()() { return generator(); }

  inline void discard(unsigned long long const z) requires
      requires(Generator &g, unsigned long long const z) {
    {g.discard(z)};
  }
  { generator.discard(z); }

  template <concepts::arithmetic ArithmeticType1 = result_type,
            concepts::arithmetic ArithmeticType2 = ArithmeticType1>
  std::common_type_t<ArithmeticType1, ArithmeticType2>
  pick(ArithmeticType1 const low = std::numeric_limits<ArithmeticType1>::min(),
       ArithmeticType2 const high =
           std::numeric_limits<ArithmeticType2>::max()) {
    using t = std::common_type_t<ArithmeticType1, ArithmeticType2>;
    assert((t(low) < t(high)) && "Low must strictly be less than high.");

    uniform_distribution<t> distribution{t(low), t(high)};
    return distribution(generator);
  }

  template <std::forward_iterator ForwardIterator,
            std::sentinel_for<ForwardIterator> Sentinel>
  ForwardIterator pick(ForwardIterator const begin, Sentinel const end) {
    using std::distance;
    using std::next;

    auto const range = distance(begin, end);
    return next(begin, pick(static_cast<decltype(range)>(0), range));
  }

  auto pick(std::ranges::range auto const &container) {
    using std::begin;
    using std::end;

    return pick(begin(container), end(container));
  }

  template <concepts::arithmetic ArithmeticType>
  fuzzy_number<ArithmeticType> create_fuzzy_number(
      ArithmeticType const low = std::numeric_limits<ArithmeticType>::min(),
      ArithmeticType const high = std::numeric_limits<ArithmeticType>::max()) {
    assert((low < high) && "Low must strictly be less than high.");

    return fuzzy_number<ArithmeticType>{generator, low, high};
  }

  template <class T, class... ValueArgs>
  fuzzy_container<T, generator_type>
  create_fuzzy_container(ValueArgs &&...value_args) {
    return fuzzy_container<T, generator_type>{
        generator, std::forward<ValueArgs>(value_args)...};
  }

  template <class T, class U = T>
  fuzzy_container<T, generator_type>
  create_fuzzy_container(std::initializer_list<U> const &values) {
    return fuzzy_container<T, generator_type>{generator, values};
  }

private:
  generator_type generator;
};

typedef basic_random_picker<std::mt19937_64> random_picker;

inline constinit auto get_picker_seed = [] {
  auto seed_from_env_value = [](char const *const env_var)
      -> std::optional<std::mt19937_64::result_type> {
    char *const buffer = ::getenv(env_var);

    if (nullptr != buffer) {
      std::mt19937_64::result_type value = {};
      auto const result =
          std::from_chars(buffer, buffer + std::strlen(buffer), value);
      return result.ec == std::errc{} ? std::make_optional(value)
                                      : std::nullopt;
    } else {
      return std::nullopt;
    }
  };
  return coalesce(
             std::make_optional(std::mt19937_64::default_seed),
             [=] { return seed_from_env_value("SKIZZAY_KERCHOW_SEED"); },
             [=]() -> std::optional<std::mt19937_64::result_type> {
               char *const buffer = ::getenv("SKIZZAY_KERCHOW_SEED_VAR");
               if (nullptr != buffer) {
                 return seed_from_env_value(buffer);
               } else {
                 return std::nullopt;
               }
             },
             []() -> std::optional<std::mt19937_64::result_type> {
               return std::random_device{}();
             })
      .value();
};

inline random_picker picker{get_picker_seed()};
} // namespace kerchow
} // namespace skizzay