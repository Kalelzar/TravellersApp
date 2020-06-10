#ifndef TRAVELLERAPP_STACK_HPP
#define TRAVELLERAPP_STACK_HPP

#include <memory>
#include "../Nullable.hpp"
#include "ArrayList.hpp"

/// An implementation of a stack leveraging {@link ArrayList}.
/// Basically an ArrayList but most methods are private
/// and some others exist with different names
template <class A> class Stack : private ArrayList<A> {
public:
  using ArrayList<A>::length;
  using ArrayList<A>::pop;

  Stack(Stack<A> const &stack) : ArrayList<A>(stack) {}

  Stack(int capacity) : ArrayList<A>(capacity) {}

  Stack() : ArrayList<A>() {}

  /// Push an element onto the stack
  void push(A const &elem) { this->append(elem); }

  /// Push multiple elements on top of the stack
  void pushAll(Stack<A> const &stack) {
    for (unsigned i = 0; i < stack.length(); i++)
      push(stack.get(i));
  }

  /// Convert to list
  std::unique_ptr<ArrayList<A>> asList() {
    std::unique_ptr<ArrayList<A>> list =
        std::make_unique<ArrayList<A>>(ArrayList<A>::capacity());
    for (int i = 0; i < length(); i++) {
      list->append(ArrayList<A>::get(i));
    }
    return list;
  }

  /// Peek at the top element of the stack
  std::unique_ptr<Nullable<A>> peek() const { return (*this)[length() - 1]; }
};

#endif // TRAVELLERAPP_STACK_HPP
