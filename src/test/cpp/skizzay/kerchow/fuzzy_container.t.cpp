#include "skizzay/kerchow/fuzzy_container.h"

#include <catch2/catch_all.hpp>
#include <skizzay/kerchow/kerchow.h>

using namespace skizzay::kerchow;

TEST_CASE("fuzzy container", "[unit]") {
  random_picker picker = {get_picker_seed()};

  SECTION("size given empty container should be 0") {
    // Arrange
    auto target = picker.create_fuzzy_container<char const *>();

    // Act
    std::size_t const actual = target.size();

    // Assert
    REQUIRE(0 == actual);
  }
  
  SECTION("size given initialized container should use size of initialized "
          "values") {
    // Arrange
    auto target =
        picker.create_fuzzy_container<char const *>({"hello", "world"});
    std::size_t const expected = 2;

    // Act
    std::size_t const actual = target.size();

    // Assert
    REQUIRE(expected == actual);
  }

  SECTION("pushing an element increases the size by 1") {
    // Arrange
    auto target =
        picker.create_fuzzy_container<char const *>({"hello", "world"});
    std::size_t const expected = std::ranges::size(target) + 1;
    target.push("frog");

    // Act
    std::size_t const actual = target.size();

    // Assert
    REQUIRE(expected == actual);
  }

  SECTION("size after pushing should be incremented by 1") {
    // Arrange
    auto target =
        picker.create_fuzzy_container<char const *>({"hello", "world"});
    std::size_t const expected = target.size() + 1;
    target.push("blammo");

    // Act
    std::size_t const actual = target.size();

    // Assert
    REQUIRE(expected == actual);
  }

  SECTION(
      "size after pushing a range should be incremented by the range's size") {
    // Arrange
    auto target =
        picker.create_fuzzy_container<char const *>({"hello", "world"});
    auto const range = std::vector{"blammo", "frog"};
    auto const expected = std::ranges::size(target) + std::ranges::size(range);

    // Act
    target.push(range);
    std::size_t const actual = target.size();

    // Assert
    REQUIRE(expected == actual);
  }

  SECTION("size after pushing an initializer_list should be incremented by the "
          "list's size") {
    // Arrange
    auto target =
        picker.create_fuzzy_container<char const *>({"hello", "world"});
    std::initializer_list const list = {"blammo", "frog"};
    auto const expected = std::ranges::size(target) + std::ranges::size(list);

    // Act
    target.push(list);
    std::size_t const actual = target.size();

    // Assert
    REQUIRE(expected == actual);
  }

  SECTION("size after emplacing should be incremented by 1") {
    // Arrange
    auto target =
        picker.create_fuzzy_container<char const *>({"hello", "world"});
    std::size_t const expected = target.size() + 1;
    target.emplace("blammo");

    // Act
    std::size_t const actual = target.size();

    // Assert
    REQUIRE(expected == actual);
  }
}