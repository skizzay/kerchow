#include "skizzay/kerchow/kerchow.h"
#include <algorithm>
#include <gtest/gtest.h>

namespace {

using namespace skizzay::kerchow;


TEST(fuzzy_number_tests, uniform_distribution_provides_valid_range) {
   // Arrange
   fuzzy_number<int> target = picker.create_fuzzy_number(-10, 10);

   // Act
   int actual = target.next();

   // Assert
   ASSERT_TRUE((target.get_distribution().a() <= actual) && (actual <= target.get_distribution().b()));
}


TEST(fuzzy_container_tests, size_given_initialized_container_should_use_size_of_initialized_values) {
   // Arrange
   auto target = picker.create_fuzzy_container<const char *>({"hello", "world"});
   size_t expected = 2;

   // Act
   size_t actual = target.size();

   // Assert
   ASSERT_EQ(expected, actual);
}


TEST(fuzzy_container_tests, size_given_specified_iteration_count_should_use_iteration_count) {
   // Arrange
   auto target = picker.create_fuzzy_container<const char *>({"hello", "world"});
   size_t expected = picker.pick<size_t>();
   target.set_iteration_count(expected);

   // Act
   size_t actual = target.size();

   // Assert
   ASSERT_EQ(expected, actual);
}


TEST(fuzzy_container_tests, size_after_pushing_should_be_incremented_by_1) {
   // Arrange
   auto target = picker.create_fuzzy_container<const char *>({"hello", "world"});
   size_t expected = target.size() + 1;
   target.push("blammo");

   // Act
   size_t actual = target.size();

   // Assert
   ASSERT_EQ(expected, actual);
}


TEST(fuzzy_container_tests, size_after_emplacing_should_be_incremented_by_1) {
   // Arrange
   auto target = picker.create_fuzzy_container<const char *>({"hello", "world"});
   size_t expected = target.size() + 1;
   target.emplace("blammo");

   // Act
   size_t actual = target.size();

   // Assert
   ASSERT_EQ(expected, actual);
}


TEST(fuzzy_container_tests, empty_given_no_iteration_count_should_be_true) {
   // Arrange
   auto target = picker.create_fuzzy_container<const char *>({"hello", "world"});
   bool expected = true;
   target.set_iteration_count(0);

   // Act
   bool actual = target.empty();

   // Assert
   ASSERT_EQ(expected, actual);
}


TEST(fuzzy_container_tests, empty_given_an_iteration_count_should_be_false) {
   // Arrange
   auto target = picker.create_fuzzy_container<const char *>({"hello", "world"});
   bool expected = false;
   target.set_iteration_count(picker.pick<size_t>(1));

   // Act
   bool actual = target.empty();

   // Assert
   ASSERT_EQ(expected, actual);
}


TEST(fuzzy_container_tests, iteration_over_container_iterates_the_size_of_container) {
   // Arrange
   std::vector<const char *> values{{"hello", "world", "car", "boot", "run", "jump"}};
   auto target = picker.create_fuzzy_container<const char *>(std::begin(values), std::end(values));
   size_t expected = picker.pick<size_t>(target.size(), 3 * target.size());
   target.set_iteration_count(expected);
   size_t actual = 0;

   // Act
   for (const char *value : target) {
      ++actual;
      ASSERT_NE(std::find(values.begin(), values.end(), value), values.end());
   }

   // Assert
   ASSERT_EQ(expected, actual);
}


TEST(fuzzy_container_tests, container_size_matches_distance_of_iterators) {
   // Arrange
   std::vector<const char *> const values{{"hello", "world", "car", "boot", "run", "jump"}};
   auto const target = picker.create_fuzzy_container<const char *>(std::begin(values), std::end(values));
   std::size_t const expected = target.size();

   // Act
   std::size_t const actual = std::distance(std::begin(target), std::end(target));

   // Assert
   ASSERT_EQ(expected, actual);
}

}
