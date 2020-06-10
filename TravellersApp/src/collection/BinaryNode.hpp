//
// Created by Kalelzar on 30/03/2020.
//

#ifndef TRAVELLERAPP_BINARYNODE_HPP
#define TRAVELLERAPP_BINARYNODE_HPP

#include <memory>
#include <functional>
#include <iomanip>
#include <iostream>

#include "ArrayList.hpp"
#include "Stack.hpp"

/**
 * A node for use in a binary tree. It has a left element and a right element.
 * It also knows it's parent element. It has a tag that is used to put it in the
 * correct place and a list of values.
 */
template <typename B, typename A> class BinaryNode {
public:
  B tag;
  ArrayList<A> value;
  BinaryNode<B, A> *left;
  BinaryNode<B, A> *right;
  BinaryNode<B, A> *parent;

  BinaryNode(B const &tag, A const &value, BinaryNode<B, A> *parent = nullptr) {
    this->tag = tag;
    this->value.append(value);
    left = right = nullptr;
    this->parent = parent;
  }

  ~BinaryNode() {
    delete left;
    delete right;
  }

  /**
   * Return a new dynamically allocated instance of BinaryNode with the
   * provided parameters.
   */
  static BinaryNode<B, A> *instance(B const &tag, A const &value,
                                    BinaryNode<B, A> *parent = nullptr) {
    return new BinaryNode(tag, value, parent);
  }

  /// Prints the node to stdout
  void print(int depth = 0) {
    for (int i = 0; i < depth; i++) {
      cout << ' ';
    }
    cout << tag << endl;
    if (left != nullptr) {
      left->print(depth + 2);
    } else {
      for (int i = 0; i < depth + 2; i++) {
        cout << ' ';
      }
      cout << "XXX" << endl;
    }

    if (right != nullptr) {
      right->print(depth + 2);
    } else {
      for (int i = 0; i < depth + 2; i++) {
        cout << ' ';
      }
      cout << "XXX" << endl;
    }
  }

  /// Add a new node with the provided tag and value.
  void populate(B const &ntag, A const &nvalue) {
    populate(BinaryNode<B, A>::instance(ntag, nvalue, nullptr));
  }

  /**
   * Adds the node to the binary node structure.
   * If the provided node's tag matches this node's tag add it's values to
   * this node's values.
   * If the provided node's tag is lesser than this node's tag attempt to add
   * the node to this node's left child if such exists, else set it as the
   * left child.
   * If the provided node's tag is greater than this node's tag attempt to add
   * the node to this node's right child if such exists, else set it as the
   * right child.
   */
  void populate(BinaryNode<B, A> *node) {
    if (!node)
      return;
    if (tag == node->tag) {
      value.appendAll(node->value);
      delete node;
    } else if (tag > node->tag) {
      if (left == nullptr) {
        left = node;
        node->parent = this;
      } else
        left->populate(node);
    } else {
      if (right == nullptr) {
        right = node;
        node->parent = this;
      } else
        right->populate(node);
    }
  }

  /**
   * Adds the node to the binary node structure using a range to gauge
   * where it should go.
   * If this node's tag is equal to either end of the range add the provided
   * value to this node's values.
   * If this node's tag is greater than the end of the range attempt to add
   * the new node to this node's left child if such exists, else set it as the
   * left child.
   * If this node's tag is lesser than the start of the range attempt to add
   * the new node to this node's right child if such exists, else set it as
   * the right child.
   * If the tag lies within the range:
   * First check if a node with tag == end || tag == start
   * exists further down and if it does add the value to it.
   * else set this as the current node's parent
   */
  void populateWithRange(B const &start, B const &end, A const nvalue) {
    if (tag == start || tag == end) {
      value.append(nvalue);
    } else if (tag > end) {
      if (left == nullptr) {
        left = BinaryNode::instance(start, nvalue, this);
      } else
        left->populateWithRange(start, end, nvalue);
    } else if (tag < start) {
      if (right == nullptr) {
        right = BinaryNode::instance(end, nvalue, this);
      } else
        right->populateWithRange(start, end, nvalue);
    } else {
      // The current node lies inside the range
      // First check if a node with tag == end || tag == start exists further
      // down
      BinaryNode<B, A> *exists = firstMatch(end);
      if (exists) {
        exists->value.append(nvalue);
        return;
      }
      exists = firstMatch(start);
      if (exists) {
        exists->value.append(nvalue);
        return;
      }

      bool isLeft = (!parent && std::abs(tag - start) <= std::abs(tag - end)) ||
                    (parent && parent->left == this);
      if (isLeft) {
        BinaryNode<B, A> *node = BinaryNode::instance(start, nvalue, parent);
        if (parent)
          parent->left = node;
        node->repopulate(this);
      } else {
        BinaryNode<B, A> *node = BinaryNode::instance(end, nvalue, parent);
        if (parent)
          parent->right = node;
        node->repopulate(this);
      }
    }
  }

  /// Collect all binary nodes further down.
  std::unique_ptr<Stack<BinaryNode<B, A> *>> collect() {
    std::unique_ptr<Stack<BinaryNode<B, A> *>> stack =
        std::make_unique<Stack<BinaryNode<B, A> *>>();
    stack->push(this);
    if (left)
      stack->pushAll(*left->collect());
    if (right)
      stack->pushAll(*right->collect());
    return stack;
  }

  /// Remove all nodes further down and re-add them.
  void repopulate(BinaryNode<B, A> *node) {
    std::unique_ptr<Stack<BinaryNode<B, A> *>> nodes = node->collect();
    while (nodes->length()) {
      BinaryNode<B, A> *n = nodes->pop()->get();
      n->left = nullptr;
      n->right = nullptr;
      n->parent = nullptr;
      populate(n);
    }
  }

  /// Return the first node whose tag matches the given predicate
  BinaryNode<B, A> *firstMatch(bool predicate(B)) {
    if (predicate(tag))
      return this;
    if (left != nullptr) {
      BinaryNode<B, A> *res = left->firstMatch(predicate);
      if (res != nullptr)
        return res;
    }
    if (right != nullptr) {
      BinaryNode<B, A> *res = right->firstMatch(predicate);
      if (res != nullptr)
        return res;
    }
    return nullptr;
  }

  /// Return the first node whose tag is equal to the given target
  BinaryNode<B, A> *firstMatch(B target) {
    if (tag == target)
      return this;
    if (left != nullptr) {
      BinaryNode<B, A> *res = left->firstMatch(target);
      if (res != nullptr)
        return res;
    }
    if (right != nullptr) {
      BinaryNode<B, A> *res = right->firstMatch(target);
      if (res != nullptr)
        return res;
    }
    return nullptr;
  }

  /// Return the values of all nodes whose tag lies inside the given (inclusive)
  /// range
  std::unique_ptr<ArrayList<A>> allInRange(B const &lower,
                                           B const &upper) const {
    std::unique_ptr<ArrayList<A>> range = std::make_unique<ArrayList<A>>();

    if (tag >= lower && tag <= upper) {
      range->appendAll(value);
      if (right != nullptr)
        range->appendAll(*right->allInRange(lower, upper));
      if (left != nullptr)
        range->appendAll(*left->allInRange(lower, upper));
    }

    if (tag > upper) {
      if (left != nullptr)
        range->appendAll(*left->allInRange(lower, upper));
    }
    if (tag < lower) {
      if (right != nullptr)
        range->appendAll(*right->allInRange(lower, upper));
    }

    return range;
  }

  /// Return the values of all nodes whose tag lies inside the given
  /// (inclusive) range.
  template <typename C>
  std::unique_ptr<ArrayList<A>> allInRangeT(C const &lower,
                                            C const &upper) const {
    std::unique_ptr<ArrayList<A>> range = std::make_unique<ArrayList<A>>();
    // std::cout<<"All in range "<<lower<<" to " <<upper<<std::endl;
    if (tag >= lower && tag <= upper) {
      // std::cout<<tag<<" is between "<<lower<<" and "<<upper<<std::endl;
      range->appendAll(value);
      if (right != nullptr)
        range->appendAll(*right->allInRangeT<C>(lower, upper));
      if (left != nullptr)
        range->appendAll(*left->allInRangeT<C>(lower, upper));
    } else if (tag > upper) {
      // std::cout<<tag<<" is greater than "<<upper<<std::endl;
      if (left != nullptr)
        range->appendAll(*left->allInRangeT<C>(lower, upper));
    } else if (tag < lower) {
      // std::cout<<tag<<" is less than "<<lower<<std::endl;
      if (right != nullptr)
        range->appendAll(*right->allInRangeT<C>(lower, upper));
    } else {
      std::cout << "Maldefined comparison operators" << std::endl;
    }

    return range;
  }
};

#endif // TRAVELLERAPP_BINARYNODE_HPP
