#ifndef TRAVELLERAPP_SIMPLESTRING_HPP
#define TRAVELLERAPP_SIMPLESTRING_HPP

#include "debug.hpp"
#include <cstring>
#include <iostream>

/**
 * An exceedingly simple implementation of a string class.
 * It's only purpose is to allow strings to be compared via ==.
 * It also contains a (horrifically hacky) workaround that allows strings to
 * be used as keys in a {@link HashMap}.
 * <b> Use char* over this wherever possible </b>
 * @bug Because of the HashMap workaround trying to retrieve SimpleString from
 * {@link Tuple} via the {@link Tuple::get} method will fail as it will attempt
 * to reinterpret_cast a char* to SimpleString*.
 */
class SimpleString {

private:
  char *inner = nullptr;
  size_t len = 0;

  /**
   * Initialize the string from a cstring.
   * @param str the cstring to copy
   */
  void init(const char *str) {
    // LOG(INFO, "Creating SimpleString instance.");
    len = strlen(str);
    inner = new char[len + 1];
    strcpy(inner, str);
  }

public:
  /// Initialize with an empty string
  SimpleString() {
    inner = new char[1];
    inner[0] = '\0';
  }

  ~SimpleString() {
    // LOG(INFO, "Destructor called.");
    delete[] inner;
  }

  /// Move constructor from SimpleString.
  SimpleString(SimpleString &&str) {
    // LOG(INFO, "Moving SimpleString");
    len = str.len;
    inner = str.inner;

    str.len = 0;
    str.inner = nullptr;
  }

  /// Move the provided SimpleString to this one
  SimpleString &operator=(SimpleString &&str) {
    if (&str != inner) {
      // LOG(INFO, "Move(assign) SimpleString");
      if (inner)
        delete[] inner;
      len = str.len;
      inner = str.inner;

      str.len = 0;
      str.inner = nullptr;
    }
    return *this;
  }

  /// Move constructor from cstring
  SimpleString(const char(&&str)[]) {
    // LOG(INFO, "Moving cstring into SimpleString");
    len = strlen(str);
    inner = const_cast<char *>(str);
  }

  /// Copy constructor from cstring.
  SimpleString(const char *str) {
    // LOG(INFO, "Copying SimpleString from cstring");
    init(str);
  }

  /// Copy constructor from SimpleString
  SimpleString(SimpleString const &str) {
    // LOG(INFO, "Copying SimpleString");
    init(str.inner);
  }

  /// Copy the provided SimpleString to this one.
  SimpleString &operator=(SimpleString const &str) {
    if (&str != inner) {
      // LOG(INFO, "Assigning SimpleString");
      if (inner)
        delete[] inner;
      init(str.inner);
    }
    return *this;
  }

  /// Assign from cstring
  SimpleString &operator=(const char str[]) {
    // LOG(INFO, "Assigning SimpleString from cstring");
    if (inner)
      delete[] inner;
    init(str);
    return *this;
  }

  /// Equality with SimpleString. Utilizes std::strcmp
  bool operator==(SimpleString const &str) const {
    if (length() != str.length())
      return false;
    return strcmp(inner, str.inner) == 0;
  }

  /// Equality with cstring. Utilizes std::strcmp
  bool operator==(const char *str) const {
    if (length() != strlen(str))
      return false;
    return strcmp(inner, str) == 0;
  }

  /// Inequality with SimpleString. Utilizes std::strcmp
  bool operator!=(SimpleString const &str) const { return !(*this == str); }

  /// Inequality with cstring. Utilizes std::strcmp
  bool operator!=(const char *str) const { return !(*this == str); }

  /// Is this string greater than another? Utilizes std::strcmp
  bool operator>(SimpleString const &other) const {
    return strcmp(inner, other.get()) > 0;
  }

  /** HACK: Needed so this class can be used as a HashMap key.
   * @bug Messes up {@link Tuple::get}.
   * @return the underlying char*
   */
  char *operator&() {
    // LOG(WARN, "Calling override of operator&.");
    return inner;
  }

  /** HACK: Needed so this class can be used as a HashMap key.
   * @bug Messes up {@link Tuple::get}.
   * @return the underlying char*
   */
  const char *operator&() const {
    // LOG(WARN, "Calling override of operator&");
    return inner;
  }

  /// Returns the underlying char*.
  const char *get() const { return inner; }

  /// Passes the underlying char* to the provided stream
  friend std::ostream &operator<<(std::ostream &out, SimpleString str) {
    return out << str.inner;
  }

  /// Returns the length of the string (Excluding the \0)
  size_t length() const { return len; }
};

#endif // TRAVELLERAPP_SIMPLESTRING_HPP
