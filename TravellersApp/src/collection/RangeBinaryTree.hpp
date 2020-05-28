#ifndef TRAVELLERAPP_RANGEBINARYTREE_HPP
#define TRAVELLERAPP_RANGEBINARYTREE_HPP

#include "BinaryNode.hpp"
#include <iostream>
#include <iomanip>

template<typename B, typename A>
class RangeBinaryTree {
private:
    BinaryNode<B, A> *root = nullptr;

    void free() {
        delete root;
    }

public:

    ~RangeBinaryTree() {
        free();
    }

    RangeBinaryTree() {
        root = nullptr;
    }

    bool isEmpty() const {
        return root == nullptr;
    }

    /**
     * This frees the original head so be careful
     */
    void setHead(BinaryNode<B, A> newHead) {
        free();
        root = newHead;
    }

    const BinaryNode<B, A> *viewHead() const {
        return root;
    }

    void populate(B tag, A value) {
        if (root == nullptr) {
            root = new BinaryNode<B, A>(tag, value);
        } else {
            root->populate(tag, value);
        }
    }

    void populateWithRange(B start, B end, A value) {
        if (root == nullptr) {
            root = new BinaryNode<B, A>(start, value);
        } else {
            root->populateWithRange(start, end, value);
            while (root->parent != nullptr) {
                root = root->parent;
            }
        }
    }

    void print() const {
        root->print();
    }

    std::unique_ptr<ArrayList<A>> inRange(B const &lower, B const &upper) const {
        if (root == nullptr) return nullptr;
        return root->allInRange(lower, upper);
    }

    template<typename C>
    std::unique_ptr<ArrayList<A>> inRangeT(C const &lower, C const &upper) const {
        if (root == nullptr) return nullptr;
        return root->template allInRangeT<C>(lower, upper);
    }

    BinaryNode<B, A> *getHead() {
        return root;
    }

    const BinaryNode<B, A> *findFirst(bool predicate(B)) {
        return root->firstMatch(predicate);
    }

    const BinaryNode<B, A> *findFirst(B target) {
        return root->firstMatch(target);
    }

};

#endif //TRAVELLERAPP_RANGEBINARYTREE_HPP
