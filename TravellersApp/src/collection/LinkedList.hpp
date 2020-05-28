//
// Created by Kalelzar on 26/03/2020.
//

#ifndef TRAVELLERAPP_LINKEDLIST_HPP
#define TRAVELLERAPP_LINKEDLIST_HPP

#include "IList.hpp"
#include <iostream>

template<class A>
struct Node {
    A elem;
    Node *next;
};

template<class A>
class LinkedList : public IList<A> {
private:

    void free() {
        Node<A> *cur = _head;
        while (cur != nullptr) {
            Node<A> *next = cur->next;
            delete cur;
            cur = next;
        }
        _head = nullptr;
        _last = nullptr;
        elemCount = 0;
    }

protected:

    Node<A> *_head;
    Node<A> *_last;

    int elemCount{};

public:

    LinkedList() {
        _head = nullptr;
        _last = nullptr;
    }

    void append(A const &elem) override {
        insert(elem, length());
    }

    void prepend(A const &elem) override {
        insert(elem, 0);
    }

    ~LinkedList() {
        free();
    }

    void insert(A const &elem, unsigned at) override {

        if (at < 0) return;

        if (_head == nullptr) {
            Node<A> *s = new Node<A>{elem, nullptr};
            _head = _last = s;
        } else if (at == 0) {
            Node<A> *s = new Node<A>{elem, _head};
            _head = s;
        } else if (at >= length()) {
            Node<A> *s = new Node<A>{elem, nullptr};
            _last->next = s;
            _last = s;
        } else {
            Node<A> *cur = _head;
            for (int i = 0; i < at - 1; i++) {
                cur = cur->next;
            }
            Node<A> *s = new Node<A>{elem, cur->next};
            cur->next = s;
        }

        elemCount++;
    }

    void appendAll(LinkedList<A> const &l) {
        for (int i = 0; i < l.length(); i++) {
            append(l.get(i));
        }
    }

    void prependAll(LinkedList<A> const &l) {
        for (int i = 0; i < l.length(); i++) {
            prepend(l.get(i));
        }
    }

    unsigned remove(const A &elem) override {
        if (_head == nullptr) return 0;
        unsigned foundAt = 0;
        Node<A> *cur = _head;
        while (cur != nullptr) {
            if (cur->next->elem == elem) {
                Node<A> *temp = cur->next;
                cur->next = temp->next;
                delete temp;
                foundAt++;
                elemCount--;
                break;
            }
            cur = cur->next;
        }
        return foundAt;
    }

    bool contains(A const &elem) const override {
        Node<A> *cur = _head;
        while (cur != nullptr) {
            if (cur->elem == elem) return true;
            cur = cur->next;
        }
        return false;
    }

    A get(unsigned index) const override {
        if (index < 0) return get(0);
        if (index > length() - 1 && length() - 1 > 0) return get(length() - 1);

        if (index == 0) return _head->elem;
        if (index == length() - 1) return _last->elem;

        Node<A> *cur = _head;
        A value{};
        int counter = 0;

        while (cur != nullptr) {
            if (counter == index) {
                value = cur->elem;
                break;
            }
            cur = cur->next;
            counter++;
        }

        return value;
    }

    unsigned int length() const override {
        return elemCount;
    }

    void clear() override {
        free();
    }

    std::unique_ptr<Nullable<A>> removeAt(unsigned at) override {
        if (_head == nullptr) return std::make_unique<Null<A>>();
        if (at > length() - 1) return removeAt(length() - 1);

        A ret;
        if (at == 0) {
            Node<A> *temp = _head;
            _head = _head->next;
            ret = temp->elem;
            delete temp;
        } else {
            int counter = 0;
            Node<A> *cur = _head;
            while (cur != nullptr) {
                if (counter + 1 == at) {
                    Node<A> *temp = cur->next;
                    cur->next = temp->next;
                    if (at == length() - 1) {
                        _last = cur;
                    }
                    ret = temp->elem;
                    delete temp;
                    break;
                }
                counter++;
                cur = cur->next;
            }
        }

        elemCount--;
        return std::make_unique<NotNull<A>>(ret);
    }

    std::unique_ptr<Nullable<A>> pop() override {
        return removeAt(length() - 1);
    }

    std::unique_ptr<Nullable<A>> shift() override {
        return removeAt(0);
    }

    std::unique_ptr<Nullable<A>> head() const {
        if (_head == nullptr) return std::make_unique<Null<A>>();
        return std::make_unique<NotNull<A>>(_head->elem);
    }

    std::unique_ptr<Nullable<A>> last() const {
        if (_last == nullptr) return std::make_unique<Null<A>>();
        return std::make_unique<NotNull<A>>(_last->elem);
    }

// TODO: Figure out a way to do this safely
//    void appendAll(LinkedList<A> const &list){
//        _last->next = list._head;
//        _last = list._last;
//    }
//    void prependAll(LinkedList<A> const &list){
//        _last->next = list._head;
//        _last = list._last;
//    }

};


#endif //TRAVELLERAPP_LINKEDLIST_HPP
