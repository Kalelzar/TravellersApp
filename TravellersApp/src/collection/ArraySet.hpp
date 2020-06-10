//
// Created by Kalelzar on 26/03/2020.
//

#ifndef TRAVELLERAPP_ARRAYSET_HPP
#define TRAVELLERAPP_ARRAYSET_HPP

#include "ArrayList.hpp"

/**
 * Literally an {@link ArrayList} but when adding items it checks if the element
 * is already in the list.
 */
template <class A> class ArraySet : public ArrayList<A> {
public:
  ArraySet(ArraySet<A> const &set) : ArrayList<A>(set) {}

  ArraySet(int capacity) : ArrayList<A>(capacity) {}

  ArraySet() : ArrayList<A>() {}

  void insert(A const &elem, unsigned at) override {

    if (at < 0)
      return;

    for (unsigned i = 0; i < this->length(); i++) {
      if (this->get(i) == elem) {
        return;
      }
    }

    ArrayList<A>::insert(elem, at);
  }
};

#endif // TRAVELLERAPP_ARRAYSET_HPP
