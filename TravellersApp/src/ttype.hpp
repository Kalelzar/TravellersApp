#ifndef TTYPE_HPP
#define TTYPE_HPP

/// A collection of template magic that operates on the provided type
/// There is only a single piece of magic here right now
///@tparam Type the type to operate on
template <typename Type> struct forType {

  /// A template magic spell that will repeat the given type the provided
  /// number of types. This can later be applied to any type that accepts
  /// a vararg template
  ///@tparam times the amount of times to repeat the type
  template <int times, typename... Tail>
  struct repeat : repeat<times - 1, Type, Tail...> {
    static_assert(times > 0,
                  "Cannot repeat a type a negative number of times.");
    /// A way to apply the repeated type to a class
    ///@tparam applicant the type to apply the repeated type to
    template <template <typename... A> class applicant>
    using apply =
        typename repeat<times - 1, Type, Tail...>::template apply<applicant>;
  };

  /// Root of the recursion for {@link repeat}
  template <typename... Tail> struct repeat<0, Tail...> {
    template <template <typename... A> class applicant>
    using apply = applicant<Tail...>;
  };
};

#endif // TTYPE_HPP
