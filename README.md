# kerchow
Kerchow is a fuzzy data generator.  It can be used to generate a fuzzy number or random element from a container.  It has a number of interfaces available that will best suit your application's needs.  In some cases it'll be used as part of a unit test suite, Monte Carlo simulations, or just to properly create a simple random number.  It's C++11 compliant.

## Quick Start
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

## basic_random_picker
The `basic_random_picker` class template is used to generate random values.  You can use the globally instantiated object `picker` or you can instantiate your own.  Using the provided, global instance simplifies seeding the instance.  Think of it as a replacement for `std::rand` (which has been deprecated and soon to be removed altogether).  In addition to generating random integers, it can also generate floating point numbers.
```cpp
double random_value() {
  return kerchow::picker.pick(0.0, 1.0);
}
```

The `basic_random_picker` class template also fulfills the interface requirements to be a random number engine.  As such, you can use your own distribution (as opposed to a uniform distribution) in conjunction with the picker.  This is best achieved with using the global instance so that you do not have to worry about seeding the generator.
```cpp
double random_value() {
  constexpr double mean = 0.0;
  constexpr double standard_deviation = 1.0;
  std::normal_distribution<double> distribution{mean, standard_distribution};
  return distribution(kerchow::picker);
}
```

The underlying generator can be seeded with the system default seed, a seed sequence, or if the generator is a random device, then a token (which takes on different meanings for each system).  You can reseed the instance at anytime as according to the random number engine concept.  The global instance, `picker`, is automatically seeded at application startup.  The seed will be generated using the source of most entropy available on the system.  This is typically `/dev/urandom` or `/dev/random` if those are available.  If you need reproducibility, then you can start your application with the `KERCHOW_SEED` environment variable set to an unsigned integer.
