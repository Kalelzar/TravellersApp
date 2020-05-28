//
// Created by Kalelzar on 30/03/2020.
//

#ifndef TRAVELLERAPP_BINARYNODE_HPP
#define TRAVELLERAPP_BINARYNODE_HPP

#include <functional>
#include "ArrayList.hpp"
#include "Stack.hpp"

template<typename B, typename A>
class BinaryNode {
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

    static BinaryNode<B, A> *instance(B const &tag, A const &value, BinaryNode<B, A> *parent = nullptr) {
        return new BinaryNode(tag, value, parent);
    }


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

    void populate(B const &ntag, A const &nvalue) {
        populate(BinaryNode<B, A>::instance(ntag, nvalue, nullptr));
    }

    void populate(BinaryNode<B, A> *node) {
        if (!node) return;
        if (tag == node->tag) {
            value.appendAll(node->value);
            delete node;
        } else if (tag > node->tag) {
            if (left == nullptr) {
                left = node;
                node->parent = this;
            } else left->populate(node);
        } else {
            if (right == nullptr) {
                right = node;
                node->parent = this;
            } else right->populate(node);
        }
    }

    void populateWithRange(B const &start, B const &end, A const nvalue) {
        if (tag == start || tag == end) {
            value.append(nvalue);
        } else if (tag > end) {
            if (left == nullptr) {
                left = BinaryNode::instance(start, nvalue, this);
            } else left->populateWithRange(start, end, nvalue);
        } else if (tag < start) {
            if (right == nullptr) {
                right = BinaryNode::instance(end, nvalue, this);
            } else right->populateWithRange(start, end, nvalue);
        } else {
            //The current node lies inside the range
            //First check if a node with tag == end || tag == start exists further down
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

            bool isLeft = (!parent && std::abs(tag - start) <= std::abs(tag - end)) || (parent && parent->left == this);
            if (isLeft) {
                BinaryNode<B, A> *node = BinaryNode::instance(start, nvalue, parent);
                if (parent) parent->left = node;
                node->repopulate(this);
            } else {
                BinaryNode<B, A> *node = BinaryNode::instance(end, nvalue, parent);
                if (parent) parent->right = node;
                node->repopulate(this);
            }
        }
    }

    std::unique_ptr<Stack<BinaryNode<B, A> *>> collect() {
        std::unique_ptr<Stack<BinaryNode<B, A> *>> stack = std::make_unique<Stack<BinaryNode<B, A> *>>();
        stack->push(this);
        if (left) stack->pushAll(*left->collect());
        if (right) stack->pushAll(*right->collect());
        return stack;
    }

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

    BinaryNode<B, A> *firstMatch(bool predicate(B)) {
        if (predicate(tag)) return this;
        if (left != nullptr) {
            BinaryNode<B, A> *res = left->firstMatch(predicate);
            if (res != nullptr) return res;
        }
        if (right != nullptr) {
            BinaryNode<B, A> *res = right->firstMatch(predicate);
            if (res != nullptr) return res;
        }
        return nullptr;
    }

    BinaryNode<B, A> *firstMatch(B target) {
        if (tag == target) return this;
        if (left != nullptr) {
            BinaryNode<B, A> *res = left->firstMatch(target);
            if (res != nullptr) return res;
        }
        if (right != nullptr) {
            BinaryNode<B, A> *res = right->firstMatch(target);
            if (res != nullptr) return res;
        }
        return nullptr;
    }


    std::unique_ptr<ArrayList<A>> allInRange(B const &lower, B const &upper) const {
        std::unique_ptr<ArrayList<A>> range = std::make_unique<ArrayList<A>>();

        if (tag >= lower && tag <= upper) {
            range->appendAll(value);
            if (right != nullptr) range->appendAll(*right->allInRange(lower, upper));
            if (left != nullptr) range->appendAll(*left->allInRange(lower, upper));
        }

        if (tag > upper) {
            if (left != nullptr) range->appendAll(*left->allInRange(lower, upper));
        }
        if (tag < lower) {
            if (right != nullptr) range->appendAll(*right->allInRange(lower, upper));
        }

        return range;
    }

    template<typename C>
    std::unique_ptr<ArrayList<A>> allInRangeT(C const &lower, C const &upper) const {
        std::unique_ptr<ArrayList<A>> range = std::make_unique<ArrayList<A>>();
        //std::cout<<"All in range "<<lower<<" to " <<upper<<std::endl;
        if (tag >= lower && tag <= upper) {
            //std::cout<<tag<<" is between "<<lower<<" and "<<upper<<std::endl;
            range->appendAll(value);
            if (right != nullptr) range->appendAll(*right->allInRangeT<C>(lower, upper));
            if (left != nullptr) range->appendAll(*left->allInRangeT<C>(lower, upper));
        } else if (tag > upper) {
            //std::cout<<tag<<" is greater than "<<upper<<std::endl;
            if (left != nullptr) range->appendAll(*left->allInRangeT<C>(lower, upper));
        } else if (tag < lower) {
            //std::cout<<tag<<" is less than "<<lower<<std::endl;
            if (right != nullptr) range->appendAll(*right->allInRangeT<C>(lower, upper));
        } else {
            std::cout << "Maldefined comparison operators" << std::endl;
        }

        return range;
    }


};

#include <iostream>
#include <iomanip>
#include "Stack.hpp"

#endif //TRAVELLERAPP_BINARYNODE_HPP
