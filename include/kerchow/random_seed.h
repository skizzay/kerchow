// vim: sw=3 ts=3 expandtab smartindent autoindent cindent
#ifndef KERCHOW_RANDOM_SEED_H__
#define KERCHOW_RANDOM_SEED_H__

#include <random>
#include <type_traits>

namespace kerchow {

namespace details_ {

template<class ArithmeticType>
struct uniform_distribution {
   static_assert(std::is_arithmetic<ArithmeticType>::value, "Uniform distribution is only applicable to arithmetic types.");
   typedef typename std::conditional<std::is_integral<ArithmeticType>::value,
                                     std::uniform_int_distribution<ArithmeticType>,
                                     std::uniform_real_distribution<ArithmeticType>>::type
                                type;
};

}


class random_seed_holder {
public:
   random_seed_holder() :
      seed{0},
      generator{}
   {
      reseed();
   }

   inline void reset_within_range(unsigned int low, unsigned int high) {
      while (!(low <= seed) && (seed <= high)) {
         reseed();
      }
   }

   inline void update(unsigned int value) {
      seed = value;
   }

   inline unsigned int value() const {
      return seed;
   }

private:
   random_seed_holder(const random_seed_holder &) = delete;
   random_seed_holder(random_seed_holder &&) = delete;

   inline void reseed() {
      update(generator());
   }

   unsigned int seed = 0;
   std::random_device generator;
} random_seed;

}

#endif
