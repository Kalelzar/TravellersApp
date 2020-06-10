#ifndef TRAVELLERAPP_NULLABLE_HPP
#define TRAVELLERAPP_NULLABLE_HPP

#include "NoValueException.hpp"
#include "debug.hpp"
#include <iostream>

/**
 * An optional value container inspired by Haskell's Maybe and Scala's Option.
 * It stores either a value of type A (wrapped into a {@link NotNull} instance)
 * or an empty (represented by {@link Null}) value.
 * It is intended to provide more safety when returning empty values than
 * returning nullptr would. It <b>not</b> mean you shouldn't check if a value is
 * null. Rather the entire purpose of the class is for when you do forget to
 * check the value, you don't end up spending 2 hours finding out which nullptr
 * you tried to dereference.
 */
template <class A> class Nullable {
private:
protected:
public:
  virtual ~Nullable() = default;

  /// Is this value Null?
  virtual bool isEmpty() const = 0;

  /// Is this value NotNull?
  bool isDefined() const { return !isEmpty(); }

  /**
   * Return the value contained if such exists, else throw a NoValueException.
   * @throws NoValueException
   * @return the stored value
   */
  virtual A const get() const = 0;

  /**
   * Return the value contained if such exists, else return the provided value
   * @param def the default value to return
   * @return the stored value or the default value.
   */
  A const getOrElse(A const &def) const {
    if (isEmpty())
      return def;
    return get();
  }
};

/**
 * This class represents a wrapper around a concrete value.
 * Meant to be used in conjunction with {@link Null} to represent optional
 * types.
 */
template <class A> class NotNull : public Nullable<A> {
private:
  const A value;

public:
  // Why did I do this?
  // NotNull(A const&&) = delete;

  explicit NotNull(A const _value) : value(_value) {}

  bool isEmpty() const override { return false; }

  A const get() const override { return value; }
};

/**
 * This class represents nothing (null).
 * Meant to be used in conjunction with {@link NotNull} to represent optional
 * types.
 */
template <class A> class Null : public Nullable<A> {
public:
  bool isEmpty() const override { return true; }

  A const get() const override { throw NoValueException(); }
};

#endif // TRAVELLERAPP_NULLABLE_HPP
