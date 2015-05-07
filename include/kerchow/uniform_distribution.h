// vim: sw=3 ts=3 expandtab smartindent autoindent cindent
#ifndef KERCHOW_UNIFORM_DISTRIBUTION_H__
#define KERCHOW_UNIFORM_DISTRIBUTION_H__

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

}

#endif
