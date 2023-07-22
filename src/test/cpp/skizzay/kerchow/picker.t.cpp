#include "skizzay/kerchow/picker.h"

#include <catch2/catch_all.hpp>
#include <skizzay/kerchow/kerchow.h>

using namespace skizzay::kerchow;

TEST_CASE("picker", "[unit]") {
  // Arrange
  random_picker target = {get_picker_seed()};

  SECTION("pick with no arguments should return a value in the range of the "
          "generator") {
    // Act
    auto const actual = target.pick();

    // Assert
    REQUIRE(target.min() <= actual);
    REQUIRE(actual <= target.max());
  }

  SECTION("picker is a random number generator") {
    REQUIRE(std::uniform_random_bit_generator<random_picker>);
  }

  SECTION("pick with two integers should return a value in the range of the "
          "generator") {
    // Arrange
    int const a = -10;
    int const b = 10;

    // Act
    auto const actual = target.pick(a, b);

    // Assert
    REQUIRE(a <= actual);
    REQUIRE(actual <= b);
  }

  SECTION("pick with two floats should return a value in the range of the "
          "generator") {
    // Arrange
    float const a = -1.f;
    float const b = 1.f;

    // Act
    auto const actual = target.pick(a, b);

    // Assert
    REQUIRE(a <= actual);
    REQUIRE(actual <= b);
  }

  SECTION("pick with a range should return an interator in the range") {
   // Arrange
   auto const range = std::ranges::iota_view{0, 10};

   // Act
   auto const actual = target.pick(range);

   // Assert
   REQUIRE(std::ranges::begin(range) <= actual);
  }
}