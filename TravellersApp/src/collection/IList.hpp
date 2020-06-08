//
// Created by Kalelzar on 26/03/2020.
//

#ifndef TRAVELLERSAPP_ILIST_HPP
#define TRAVELLERSAPP_ILIST_HPP

#include "../Nullable.hpp"
#include<memory>

/**
 * An interface for a generic list.
 * @tparam A the type of the elements in the list
 */
template<class A>
class IList {
private:

public:
    /**
     * Appends the given element to the list.
     * @param elem the element
     */
    virtual void append(A const &elem) = 0;

    /**
     * Prepends the given element to the list.
     * @param elem the element
     */
    virtual void prepend(A const &elem) = 0;

    /**
     * Inserts the given element in the list at the specified position.
     * @param elem the element
     * @param at the position
     */
    virtual void insert(A const &elem, unsigned at) = 0;

    /**
     * Appends the given element to the list.
     * @param elem the element
     */
    virtual void append(A&& elem) = 0;

    /**
     * Prepends the given element to the list.
     * @param elem the element
     */
    virtual void prepend(A&& elem) = 0;

    /**
     * Inserts the given element in the list at the specified position.
     * @param elem the element
     * @param at the position
     */
    virtual void insert(A&& elem, unsigned at) = 0;

    virtual ~IList() = default;

    /**
     * Returns the value associated with the given index if any.
     *
     * This is done by returning a pointer to a {@link Nullable} instance.
     * It is an instance of {@link Null} if the index doesn't exist
     * or an instance of {@link NotNull} wrapped around the value.
     *
     * Meant as some sort of cautious {@link get} in case for some reason
     * the length of the list is unknown.
     * @param index the index, whose value to get
     * @returns the value, if any
     */
    std::unique_ptr<Nullable<A>> operator[](unsigned index) const {
        if (index < length()) return std::make_unique<NotNull<A>>(get(index));
        return std::make_unique<Null<A>>();
    }

    /**
     * Remove the first occurence of the given element in the list.
     * All following elements are brought back by one index in order to fill
     * the hole left by the removal
     * @param elem the element to remove
     * @return the index of the removed element
     */
    virtual int remove(const A &elem) = 0;

    /**
     * Removes the value associated with the given index,
     * if any, and returns it.
     *
     * This is done by returning a pointer to a {@link Nullable} instance.
     * It is an instance of {@link Null} if the index doesn't exist
     * or an instance of {@link NotNull} wrapped around the value.
     *
     * All following elements are moved back by one index to fill
     * the hole left by the removal
     *
     * @param index the index, whose value to remove
     * @returns the value, if any
     */
    virtual std::unique_ptr<Nullable<A>> removeAt(unsigned ind) = 0;

    /**
     * Removes the last element of the list, if any, and returns it.
     *
     * This is done by returning a pointer to a {@link Nullable} instance.
     * It is an instance of {@link Null} if the list is empty
     * or an instance of {@link NotNull} wrapped around the value.
     *
     * @returns the value, if any
     */
    virtual std::unique_ptr<Nullable<A>> pop() = 0;

    /**
     * Removes the first element of the list, if any, and returns it.
     *
     * This is done by returning a pointer to a {@link Nullable} instance.
     * It is an instance of {@link Null} if the list is empty
     * or an instance of {@link NotNull} wrapped around the value.
     *
     * All following elements are moved back by one index to fill
     * the hole left by the removal
     *
     * @returns the value, if any
     */
    virtual std::unique_ptr<Nullable<A>> shift() = 0;

    /**
     * Returns the value at the index
     * @param index the index
     * @return the value
     */
    virtual A& get(unsigned index) const = 0;

    /**
     * Clears the list
     */
    virtual void clear() = 0;

    /**
     * Checks if the list is empty
     */
    bool isEmpty() const {
        return length() == 0;
    }

    /**
     * Returns true if the given element is contained in the list
     * @param elem the element to search for
     * @returns if the element is contained
     */
    virtual bool contains(A const &elem) const = 0;

    /**
     * Returns the amount of elements contained in the list
     */
    virtual unsigned length() const = 0;
};

#endif //TRAVELLERSAPP_ILIST_HPP
