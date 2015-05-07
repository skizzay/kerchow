// vim: sw=3 ts=3 expandtab smartindent autoindent cindent
#ifndef KERCHOW_PICKER_H__
#define KERCHOW_PICKER_H__

#include "kerchow/fuzzy_number.h"
#include <cassert>
#include <iterator>
#include <limits>
#include <random>
#include <type_traits>

namespace kerchow {

namespace details_ {

template<class Iterator>
struct is_forward_iterator : std::is_convertible<typename std::iterator_traits<Iterator>::iterator_category,
                                                 std::forward_iterator_tag> {
                                                 
};


template<class ForwardIterator, class=typename std::enable_if<!std::is_arithmetic<ForwardIterator>::value>::type>
struct enable_forward_iterator : std::enable_if<is_forward_iterator<ForwardIterator>::value,
                                                ForwardIterator> {
};


template<class ArithmeticType>
struct enable_arithmetic : std::enable_if<std::is_arithmetic<ArithmeticType>::value, ArithmeticType> {
};

}


class basic_picker {
   std::mt19937_64 generator;

public:
   typedef std::mt19937_64::result_type seed_type;

   inline basic_picker(seed_type seed=random_seed.value()) :
      generator{seed}
   {
   }

   inline void reseed(seed_type seed=random_seed.value()) {
      generator.seed(seed);
   }

   template<class ArithmeticType>
   typename details_::enable_arithmetic<ArithmeticType>::type
         pick(ArithmeticType low=std::numeric_limits<ArithmeticType>::min(),
              ArithmeticType high=std::numeric_limits<ArithmeticType>::max()) {
      assert((low < high) && "Low must strictly be less than high.");

      typename details_::uniform_distribution<ArithmeticType>::type distribution{low, high};
      return distribution(generator);
   }

   template<class ForwardIterator>
   typename details_::enable_forward_iterator<ForwardIterator>::type
         pick(ForwardIterator begin, ForwardIterator end) {
      using std::distance;
      using std::next;

      auto range = distance(begin, end);
      return next(begin, pick(static_cast<decltype(range)>(0), range));
   }

   template<class Container>
   auto pick(const Container &container) ->
         typename details_::enable_forward_iterator<decltype(std::begin(container))>::type {
      using std::begin;
      using std::end;

      return pick(begin(container), end(container));
   }

   template<class ArithmeticType>
   fuzzy_number<ArithmeticType> create_fuzzy_number(ArithmeticType low=std::numeric_limits<ArithmeticType>::min(),
                                                    ArithmeticType high=std::numeric_limits<ArithmeticType>::max()) {
      assert((low < high) && "Low must strictly be less than high.");
      typedef typename fuzzy_number<ArithmeticType>::seed_type fuzzy_seed_type;

      ArithmeticType a = pick(low, high);
      ArithmeticType b = pick(low, high);

      return fuzzy_number<ArithmeticType>{pick<fuzzy_seed_type>(), std::min(a, b), std::max(a, b)};
   }
} picker;

}

#endif
