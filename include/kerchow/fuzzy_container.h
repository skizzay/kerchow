// vim: sw=3 ts=3 expandtab smartindent autoindent cindent
#ifndef KERCHOW_FUZZY_CONTAINER_H__
#define KERCHOW_FUZZY_CONTAINER_H__

#include "kerchow/picker.h"
#include <cassert>
#include <vector>

namespace kerchow {

template<class> class fuzzy_container;

namespace details_ {

template<class T>
class fuzzy_iterator : std::iterator<std::input_iterator_tag, T> {
public:
   typedef fuzzy_container<typename std::remove_const<T>::type> container_type;

   inline fuzzy_iterator(container_type &c) :
      items_remaining{c.size()},
      container(&c)
   {
      next();
   }

   inline fuzzy_iterator() :
      items_remaining(0),
      container(nullptr)
   {
   }

   T * operator->() {
      return current;
   }

   T & operator* () {
      return *current;
   }

   fuzzy_iterator<T> & operator++() {
      next();
      --items_remaining;
      return *this;
   }

   // postfix is just annoying
   fuzzy_iterator<T> operator++(int) = delete;

   inline bool operator==(const fuzzy_iterator<T> &r) const {
      return items_remaining == r.items_remaining;
   }

   inline bool operator!=(const fuzzy_iterator<T> &r) const {
      return items_remaining != r.items_remaining;
   }

private:
   inline void next();

   std::size_t items_remaining;
   fuzzy_container<T> *container;
   T *current = nullptr;
};

}


template<class T>
class fuzzy_container {
public:
   typedef typename fuzzy_number<std::size_t>::seed_type seed_type;
   typedef typename std::vector<T>::const_reference result_type;
   typedef details_::fuzzy_iterator<T> iterator;
   typedef details_::fuzzy_iterator<const T> const_iterator;

   typedef typename std::vector<T>::value_type value_type;
   typedef typename std::vector<T>::difference_type difference_type;
   typedef typename std::vector<T>::size_type size_type;
   typedef typename std::vector<T>::allocator_type allocator_type;
   typedef typename std::vector<T>::reference reference;
   typedef typename std::vector<T>::const_reference const_reference;
   typedef typename std::vector<T>::pointer pointer;
   typedef typename std::vector<T>::const_pointer const_pointer;

   template<class... ValueArgs>
   inline fuzzy_container(ValueArgs && ... value_args) :
      values{std::forward<ValueArgs>(value_args)...},
      iteration_count{values.size()}
   {
   }

   template<class U>
   inline fuzzy_container(const std::initializer_list<U> &value_args) :
      values{value_args},
      iteration_count{values.size()}
   {
   }

   inline result_type next() noexcept {
      assert(!empty() && "Cannot get result on empty container.");
      return values.size() == 1 ? values[0] : values[picker.pick<size_type>(0, values.size() - 1)];
   }

   inline void set_iteration_count(size_type n) noexcept {
      iteration_count = n;
   }

   inline void sync_to_element_count() noexcept {
      set_iteration_count(values.size());
   }

   inline allocator_type get_allocator() const noexcept {
      return values.get_allocator();
   }

   inline size_type max_size() const noexcept {
      return std::numeric_limits<size_type>::max();
   }

   inline size_type size() const noexcept {
      return values.empty() ? 0 : iteration_count;
   }

   inline bool empty() const noexcept {
      return size() == static_cast<size_type>(0);
   }

   inline iterator begin() noexcept {
      return {*this};
   }

   inline iterator end() noexcept {
      return {};
   }

   inline const_iterator cbegin() const noexcept {
      return {*const_cast<fuzzy_container<T> *>(this)};
   }

   inline const_iterator cend() const noexcept {
      return {};
   }

   inline const_iterator begin() const noexcept {
      return cbegin();
   }

   inline const_iterator end() const noexcept {
      return cend();
   }

   inline void push(const T &value) {
      values.push_back(value);
      ++iteration_count;
   }

   template<class ... Args>
   inline void emplace(Args && ...args) {
      values.emplace_back(std::forward<Args>(args)...);
      ++iteration_count;
   }

private:
   std::vector<T> values;
   size_type iteration_count;
};


namespace details_ {

template<class T>
void fuzzy_iterator<T>::next() {
   current = &const_cast<T &>(container->next());
}

}

}

#endif
