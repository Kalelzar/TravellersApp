#ifndef TRAVELLERAPP_STACK_HPP
#define TRAVELLERAPP_STACK_HPP

#include "ArrayList.hpp"

template<class A>
class Stack : private ArrayList<A> {
public:
    using ArrayList<A>::length;
    using ArrayList<A>::pop;

    Stack(Stack<A> const &stack) : ArrayList<A>(stack) {}

    Stack(int capacity) : ArrayList<A>(capacity) {}

    Stack() : ArrayList<A>() {}

    void push(A const &elem) {
        this->append(elem);
    }

    void pushAll(Stack<A> const &stack) {
        for (unsigned i = 0; i < stack.length(); i++)
            push(stack.get(i));
    }

    std::unique_ptr<ArrayList<A>> asList() {
        std::unique_ptr<ArrayList<A>> list = std::make_unique<ArrayList<A>>(ArrayList<A>::capacity());
        for(int i = 0; i < length(); i++){
            list->append(ArrayList<A>::get(i));
        }
        return list;
    }

    std::unique_ptr<Nullable<A>> peek() const {
        return (*this)[length() - 1];
    }

};

#endif //TRAVELLERAPP_STACK_HPP
