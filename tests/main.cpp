// vim: sw=3 ts=3 expandtab smartindent autoindent cindent
#include <iostream>
#include "kerchow/kerchow.h"
#include <algorithm>
#include <gtest/gtest.h>

namespace {

using namespace kerchow;

void seed_rng() {
   unsigned int seed = ::testing::GTEST_FLAG(random_seed);
   if (seed == 0) {
      random_seed.reset_within_range(1U, 99999U);
      ::testing::GTEST_FLAG(random_seed) = random_seed.value();
   }
   else {
      random_seed.update(seed);
   }
   picker.reseed();
}


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
   fuzzy_container<const char *> target{{"hello", "world"}};
   size_t expected = 2;

   // Act
   size_t actual = target.size();

   // Assert
   ASSERT_EQ(expected, actual);
}


TEST(fuzzy_container_tests, size_given_specified_iteration_count_should_use_iteration_count) {
   // Arrange
   fuzzy_container<const char *> target{{"hello", "world"}};
   size_t expected = picker.pick<size_t>();
   target.set_iteration_count(expected);

   // Act
   size_t actual = target.size();

   // Assert
   ASSERT_EQ(expected, actual);
}


TEST(fuzzy_container_tests, size_after_pushing_should_be_incremented_by_1) {
   // Arrange
   fuzzy_container<const char *> target{{"hello", "world"}};
   size_t expected = target.size() + 1;
   target.push("blammo");

   // Act
   size_t actual = target.size();

   // Assert
   ASSERT_EQ(expected, actual);
}


TEST(fuzzy_container_tests, size_after_emplacing_should_be_incremented_by_1) {
   // Arrange
   fuzzy_container<const char *> target{{"hello", "world"}};
   size_t expected = target.size() + 1;
   target.emplace("blammo");

   // Act
   size_t actual = target.size();

   // Assert
   ASSERT_EQ(expected, actual);
}


TEST(fuzzy_container_tests, empty_given_no_iteration_count_should_be_true) {
   // Arrange
   fuzzy_container<const char *> target{{"hello", "world"}};
   bool expected = true;
   target.set_iteration_count(0);

   // Act
   bool actual = target.empty();

   // Assert
   ASSERT_EQ(expected, actual);
}


TEST(fuzzy_container_tests, empty_given_an_iteration_count_should_be_false) {
   // Arrange
   fuzzy_container<const char *> target{{"hello", "world"}};
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
   fuzzy_container<const char *> target(std::begin(values), std::end(values));
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

}


int main(int argc, char **argv) {
   ::testing::InitGoogleTest(&argc, argv);
   seed_rng();
   return RUN_ALL_TESTS();
}
