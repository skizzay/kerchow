// vim: sw=3 ts=3 expandtab smartindent autoindent cindent
#ifndef KERCHOW_PICKER_H__
#define KERCHOW_PICKER_H__

#include "kerchow/fuzzy_container.h"
#include "kerchow/fuzzy_number.h"
#include <iterator>
#include <limits>

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


template<class Generator>
class basic_random_picker {
public:
   typedef Generator generator_type;
   typedef typename generator_type::result_type result_type;

   inline basic_random_picker(result_type seed_value=generator_type::default_seed) :
      generator(seed_value)
   {
   }

   inline basic_random_picker(std::seed_seq &sseq) :
      generator(sseq)
   {
   }

   static constexpr result_type min() {
      return generator_type::min();
   }

   static constexpr result_type max() {
      return generator_type::max();
   }

   void seed(result_type seed) {
      generator.seed(seed);
   }

   inline result_type operator()() {
      return generator();
   }

   inline void discard(unsigned long long z) {
      generator.discard(z);
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

      ArithmeticType a = pick(low, high);
      ArithmeticType b = pick(low, high);
      while (b == a) {
         b = pick(low, high);
      }

      return fuzzy_number<ArithmeticType>{generator, std::min(a, b), std::max(a, b)};
   }

   template<class T, class ... ValueArgs>
   fuzzy_container<T, generator_type> create_fuzzy_container(ValueArgs && ... value_args) {
      return fuzzy_container<T, generator_type>{generator, std::forward<ValueArgs>(value_args)...};
   }

   template<class T, class U=T>
   fuzzy_container<T, generator_type> create_fuzzy_container(const std::initializer_list<U> &values) {
      return fuzzy_container<T, generator_type>{generator, values};
   }

private:
   generator_type generator;
};


template<>
class basic_random_picker<std::random_device> {
public:
   typedef std::random_device generator_type;
   typedef generator_type::result_type result_type;

   inline basic_random_picker() = default;
   inline basic_random_picker(const std::string &token) :
      generator(token)
   {
   }

   static constexpr result_type min() {
      return generator_type::min();
   }

   static constexpr result_type max() {
      return generator_type::max();
   }

   inline result_type operator()() {
      return generator();
   }

   inline double entropy() const noexcept {
      return generator.entropy();
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

private:
   generator_type generator;
};


typedef basic_random_picker<std::mt19937_64> random_picker;


extern random_picker picker;

}

#endif
