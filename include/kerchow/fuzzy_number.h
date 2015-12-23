// vim: sw=3 ts=3 expandtab smartindent autoindent cindent
#ifndef KERCHOW_FUZZY_NUMBER_H__
#define KERCHOW_FUZZY_NUMBER_H__

#include "kerchow/uniform_distribution.h"
#include <complex>

namespace kerchow {

template<class ArithmeticType,
         class DistributionType=typename details_::uniform_distribution<ArithmeticType>::type,
         class GeneratorType=std::mt19937_64>
class fuzzy_number {
public:
   typedef DistributionType distribution_type;
   typedef GeneratorType generator_type;
   typedef typename distribution_type::result_type result_type;
   typedef typename generator_type::result_type seed_type;

   template<class... DistributionArgs>
   inline fuzzy_number(generator_type &g, DistributionArgs ...distribution_args) :
      generator{g},
      distribution{distribution_args...}
   {
   }

   inline result_type next() {
      return distribution(generator);
   }

   inline operator result_type() {
      return next();
   }

   const distribution_type & get_distribution() const {
      return distribution;
   }

private:
   generator_type &generator;
   distribution_type distribution;
};

}

#endif
