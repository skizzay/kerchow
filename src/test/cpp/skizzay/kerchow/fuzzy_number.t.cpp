#include "skizzay/kerchow/fuzzy_number.h"

#include <catch2/catch_all.hpp>
#include <skizzay/kerchow/kerchow.h>

using namespace skizzay::kerchow;

TEST_CASE("fuzzy number", "[unit]") {
  // Arrange
  random_picker picker = {get_picker_seed()};
  fuzzy_number<int> target = picker.create_fuzzy_number(-10, 10);

  SECTION("value provides number in the range provided") {
    // Act
    int const actual = target.value();

    // Assert
    REQUIRE(target.distribution_parameters().a() <= actual);
    REQUIRE(actual <= target.distribution_parameters().b());
  }

  SECTION("value conversion provides number in the range provided") {
    // Act
    int const actual = target;

    // Assert
    REQUIRE(target.distribution_parameters().a() <= actual);
    REQUIRE(actual <= target.distribution_parameters().b());
  }

  SECTION("value and value conversion produce same result") {
    // Act
    int const actual_value = target.value();
    int const actual_conversion = target;

    // Assert
    REQUIRE(actual_value == actual_conversion);
  }

  SECTION("repeated calls to value produces same result") {
    // Act
    int const actual1 = target.value();
    int const actual2 = target.value();

    // Assert
    REQUIRE(actual1 == actual2);
  }

  SECTION("next will eventually generate a different value") {
    // Act
    int const actual1 = target.value();
    target.next();
    // We're expecting that the underlying URNG will eventually generate a
    // different value after 5 attempts
    for (std::size_t i = 0; i < 5 && actual1 == target.value(); ++i) {
      target.next();
    }

    // Assert
    REQUIRE(actual1 != target.value());
  }
}