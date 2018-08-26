#pragma once

#include <random>
#include <type_traits>

namespace skizzay {
namespace kerchow {

namespace details_ {

template<class ArithmeticType>
struct uniform_distribution {
   static_assert(std::is_arithmetic<ArithmeticType>::value, "Uniform distribution is only applicable to arithmetic types.");

   using type = typename std::conditional<
      std::is_integral<ArithmeticType>::value,
      std::uniform_int_distribution<ArithmeticType>,
      std::uniform_real_distribution<ArithmeticType>
   >::type;
};

}

}
}
