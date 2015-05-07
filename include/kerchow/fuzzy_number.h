// vim: sw=3 ts=3 expandtab smartindent autoindent cindent
#ifndef KERCHOW_FUZZY_NUMBER_H__
#define KERCHOW_FUZZY_NUMBER_H__

#include "kerchow/random_seed.h"
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
   inline fuzzy_number(seed_type seed, DistributionArgs ...distribution_args) :
      generator{seed},
      distribution{distribution_args...}
   {
   }

   template<class... DistributionArgs>
   inline fuzzy_number(DistributionArgs ...distribution_args) :
      fuzzy_number{static_cast<seed_type>(random_seed.value()), distribution_args...}
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
   distribution_type distribution;
   generator_type generator;
};


#if 0
template<class ArithmeticType, class DistributionType, class GeneratorType>
class fuzzy_number<std::complex<ArithmeticType>, DistributionType, GeneratorType> {
public:
   typedef DistributionType distribution_type;
   typedef GeneratorType generator_type;
   typedef typename distribution_type::result_type std::complex<ArithmeticType>;
   typedef typename generator_type::result_type seed_type;

   template<class... DistributionArgs>
   inline fuzzy_number(seed_type seed, DistributionArgs ...distribution_args) :
      generator{seed},
      distribution{distribution_args...}
   {
   }

   template<class... DistributionArgs>
   inline fuzzy_number(DistributionArgs ...distribution_args) :
      fuzzy_number{static_cast<seed_type>(random_seed.value()), distribution_args...}
   {
   }

   inline result_type next() {
      return {distribution(generator), distribution(generator)};
   }

   inline operator result_type() {
      return next();
   }

private:
   distribution_type distribution;
   generator_type generator;
};
#endif

}

#endif
