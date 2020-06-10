#ifndef TRAVELLERAPP_TUPLE_HPP
#define TRAVELLERAPP_TUPLE_HPP

#include "../ttype.hpp"
#include <cstdlib>
#include <functional>
#include <iostream>
#include <type_traits>

/**
 * Tuple implementation partially taken from
 * www.stroustrup.com/C++11FAQ.html#variadic-templates
 * It wasn't working out of the box since it was meant as an example
 * of variadic templates rather than an actually usable tuple class
 * but I patched it up and added a whole ton of additional (badly designed)
 * features.
 */
template <typename... Tail> class Tuple {
private:
  size_t _size = 0;

protected:
public:
  /// End of recursive template of typeOf
  template <int ind, typename dummy = int> struct typeOf { using type = void; };

  /// Return a pointer to the element with the given index
  ///@bug if the element overloads operator& this breaks. Horribly
  template <int ind> void *get() { return nullptr; }

  template <typename T> Tuple(T **tptr) {}

  /// Return the index of the given element
  template <typename T> constexpr int indexOf(const T &elem) { return 1; }

  /// Return a const pointer to the element with the given index
  ///@bug if the element overloads operator& this breaks. Horribly
  template <int ind> const void *get() const { return nullptr; }

  /// Return the element at the provided index
  template <typename T> T *at(int ind) const { return nullptr; }

  /// Return the amount of elements in the tuple
  constexpr size_t size() const { return _size; }

  /// Free the elements in the tuple
  void free() {}

  Tuple() {}
  bool operator==(Tuple<> const &other) const { return true; }
  // unsigned size(){
  //     return 0;
  // }
};

static std::ostream &operator<<(std::ostream &out, Tuple<> const &tuple) {
  return out;
}

template <typename Head, typename... Tail>
static std::ostream &operator<<(std::ostream &out,
                                Tuple<Head, Tail...> const &tuple) {
  out << "\"" << tuple.head() << "\"";
  if (tuple.size() > 1)
    out << " ";
  out << tuple.tail();
  return out;
}

template <typename Head, typename... Tail>
class Tuple<Head, Tail...> : private Tuple<Tail...> {
private:
  /// The type of the tuple
  using type = Tuple<Head, Tail...>;
  /// The type if the tuple this tuple inherits from
  using inherited = Tuple<Tail...>;
  /// The size of the tuple
  static constexpr size_t _size = 1 + sizeof...(Tail);

public:
  /// Return the type of the element at the provided index
  template <int ind, typename dummy = int> struct typeOf {
    using type = typename inherited::template typeOf<ind - 1, dummy>::type;
  };

  /// Bottom o recursion
  template <typename dummy> struct typeOf<0, dummy> { using type = Head; };

  constexpr size_t size() const { return _size; }

  template <int ind> std::add_const_t<typename typeOf<ind>::type> *get() const {
    if (ind <= 0)
      return reinterpret_cast<std::add_const_t<typename typeOf<ind>::type> *>(
          &head());
    return reinterpret_cast<std::add_const_t<typename typeOf<ind>::type> *>(
        tail().template get<ind - 1>());
  }

  template <typename T,
            std::enable_if_t<std::is_same<
                type, typename Tuple<std::remove_reference_t<T>>::
                          template ofSize<_size>::type>::value> * = nullptr>
  T const *at(int ind) const {
    if (ind == 0)
      return &head();
    else
      return tail().template at<T>(ind - 1);
  }

  template <int ind> typename typeOf<ind>::type *get() {
    if (ind <= 0)
      return reinterpret_cast<typename typeOf<ind>::type *>(&head());
    return reinterpret_cast<typename typeOf<ind>::type *>(
        tail().template get<ind - 1>());
  }

  // unsigned size(){
  //     return 1 + tail().size();
  // }

  /// Construct the type of tuple of the given size by repeating the Head type
  /// of tuple used to refer to this struct that many times
  ///@see forType::repeat
  template <int i> struct ofSize {
    using type =
        typename forType<Head>::template repeat<i>::template apply<Tuple>;
  };

  /// Issue a compiler error if tuple not constructed with enough elements.
  Tuple() = delete;

  template <typename... VariadicValues>
  Tuple(Head const &head, VariadicValues... tail)
      : _head(head), inherited(tail...) {}

  template <typename... VariadicValues>
  Tuple(Head &&head, VariadicValues &&... tail)
      : _head(std::move(head)), inherited(tail...) {}

  // template <typename T,
  //           std::enable_if_t<std::is_same<type, typename
  //           Tuple<Head>::template
  //                                       ofSize<_size>::type>::value>*
  //                                       =nullptr>
  // typename Tuple<T>::template ofSize<_size>::type
  //   map(std::function<T(Head&)> mapper){
  //     return collect()->map(mapper)
  //         ->template slice<_size>()
  //         ->get(0);
  // }

  // template <typename T,
  //          std::enable_if_t<std::is_same<type, typename Tuple<T>::template
  //                                        ofSize<_size>::type>::value>*
  //                                        =nullptr>
  // unique_ptr<ArrayList<T>> collect(){
  //     auto list = make_unique<ArrayList<T>>();
  //     list.append(head());
  //     list.appendAll(tail().collect());
  //     return list;
  // }

  template <typename T,
            std::enable_if_t<std::is_same<
                type, typename Tuple<T>::template ofSize<_size>::type>::value>
                * = nullptr>
  Tuple(T **tptr) : _head(**tptr), inherited(tptr + 1) {}

  Tuple(type const &other) : _head(other.head()), inherited(other.tail()) {}

  /// The head element
  Head &head() { return _head; }

  template <typename T,
            std::enable_if_t<!std::is_same<Head, T>::value> * = nullptr>
  constexpr int indexOf(T const &elem) {
    int ind = 1 + tail().template indexOf<T>(elem);
    return ind >= size() || ind < 0 ? -2 : ind;
  }

  template <typename T,
            std::enable_if_t<std::is_same<Head, T>::value> * = nullptr>
  constexpr int indexOf(Head const &elem) {
    int ind = elem == head() ? 0 : 1 + tail().template indexOf<T>(elem);
    return ind > size() || ind < 0 ? -2 : ind;
  }

  /// The head element
  const Head &head() const { return _head; }

  /// All elements except the first one
  inherited &tail() { return *this; }

  /// All elements except the first one
  const inherited &tail() const { return *this; }

  bool operator==(type const &other) const {
    return head() == other.head() && tail() == other.tail();
  }

  void free() {
    delete[] _head;
    tail().free();
  }

protected:
  Head _head;
};

#endif // TRAVELLERAPP_TUPLE_HPP
