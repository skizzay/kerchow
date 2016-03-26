# kerchow
Kerchow is a fuzzy data generator.  It can be used to generate a fuzzy number or random element from a container.  It has a number of interfaces available that will best suit your application's needs.  In some cases it'll be used as part of a unit test suite, Monte Carlo simulations, or just to properly create a simple random number.  It's C++11 compliant.

# Quick Start
For most users the `random_picker` will be the de facto choice of generating data.  Its interface is simple enough to generate random data for most use cases.  There is a globally instantiated instance that is available which properly takes care of seeding the random number generator.

In this example, we're going to use the random number generation mechanism to simulate rolling a pair of dice.
```cpp
#include <kerchow/kerchow.h>

int roll_single_die() {
  return kerchow::picker.pick(1, 6);
}

int roll_dice() {
  return roll_single_die() + roll_single_die();
}
```

We sometimes want to randomly pick from a set of values.  If we think of a die of as a set of values {1, 2, 3, 4, 5, 6}, then we could randomly pick from one of the values from the set.
```cpp
int roll_single_die() {
  return kerchow::picker.pick({1, 2, 3, 4, 5, 6});
}
```

Kerchow isn't limited to numeric values.  We can use any data type from which to choose.  Suppose we wanted to pick a random color.
```cpp
const char *get_color() {
  return kerchow::picker.pick({"red", "blue", "green", "yellow", "purple", "pink", "black", "white"});
}
```
