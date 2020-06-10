#ifndef TRAVELLERAPP_RANGEBINARYTREE_HPP
#define TRAVELLERAPP_RANGEBINARYTREE_HPP

#include <iomanip>
#include <iostream>
#include <memory>

#include "BinaryNode.hpp"
#include "ArrayList.hpp"


/// A binary search tree. Sort of.
template <typename B, typename A> class RangeBinaryTree {
private:
  BinaryNode<B, A> *root = nullptr;

  /// Free all nodes.
  void free() {
    if (root)
      delete root;
  }

public:
  ~RangeBinaryTree() { free(); }

  RangeBinaryTree() { root = nullptr; }

  /// Are there no nodes?
  bool isEmpty() const { return root == nullptr; }

  /**
   * Set the head to the provided head.
   * This frees the original head so be careful
   */
  void setHead(BinaryNode<B, A> newHead) {
    free();
    root = newHead;
  }

  /// Return a const pointer to the head.
  const BinaryNode<B, A> *viewHead() const { return root; }

  /// Add a new node with the provided tag and value.
  void populate(B tag, A value) {
    if (root == nullptr) {
      root = new BinaryNode<B, A>(tag, value);
    } else {
      root->populate(tag, value);
    }
  }

  /**
   * Adds the node to the binary node structure using a range to gauge
   * where it should go.
   * @see BinaryNode::populateWithRange
   */
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

  /// Print the tree to stdout
  void print() const { root->print(); }

  /// Return the values of all nodes whose tag lies inside the given
  /// (inclusive)
  /// range
  std::unique_ptr<ArrayList<A>> inRange(B const &lower, B const &upper) const {
    if (root == nullptr)
      return nullptr;
    return root->allInRange(lower, upper);
  }

  /// Return the values of all nodes whose tag lies inside the given
  /// (inclusive)
  /// range
  template <typename C>
  std::unique_ptr<ArrayList<A>> inRangeT(C const &lower, C const &upper) const {
    if (root == nullptr)
      return nullptr;
    return root->template allInRangeT<C>(lower, upper);
  }

  /// Return the head
  BinaryNode<B, A> *getHead() { return root; }

  /// Return the first node whose tag matches the given predicate
  const BinaryNode<B, A> *findFirst(bool predicate(B)) {
    return root->firstMatch(predicate);
  }
  /// Return the first node whose tag is equal to the given target
  const BinaryNode<B, A> *findFirst(B target) {
    return root->firstMatch(target);
  }
};

#endif // TRAVELLERAPP_RANGEBINARYTREE_HPP
