#ifndef TRAVELLERAPP_TUPLE_HPP
#define TRAVELLERAPP_TUPLE_HPP

#include <iostream>
#include <cstdlib>
#include <type_traits>

#include <iostream>
#include <cstdlib>
#include <type_traits>



/**
 * Tuple implementation partially taken from
 * www.stroustrup.com/C++11FAQ.html#variadic-templates
 * though I couldn't get it to compile as is.
 * So I had to take some time to reimplement it.
 * I did learn a whole lot more about variadic templates and functions. So.
 * Silver lining.
 */
template <typename ... Tail>
class Tuple {
public:
    Tuple(){}
    bool operator==(Tuple<> const& other) const {
        return true;
    }
};

template < typename Head, typename ... Tail >
class Tuple <Head, Tail...> : private Tuple<Tail...> {
    using type = Tuple<Head, Tail...>;
    using inherited  = Tuple<Tail...>;
protected:
    Head _head;
public:
    Tuple() = delete;

    template <typename... VariadicValues>
    Tuple(Head const& head, VariadicValues...tail)
        : _head(head), inherited(tail...) {}

    template <typename... VariadicValues>
    Tuple(Head&& head, VariadicValues&&...tail)
        : _head(std::move(head)), inherited(std::move(tail...)) {}

    Tuple(type const& other)
        : _head(other.head()), inherited(other.tail()){}

    Head& head(){
        return _head;
    }

    const Head& head() const {
        return _head;
    }

    inherited& tail() {
        return *this;
    }

    const inherited& tail() const {
        return *this;
    }

    bool operator==(type const& other) const {
        return head() == other.head() && tail() == other.tail();
    }

};

template<typename Head, typename ...Tail>
static std::ostream& operator<<(std::ostream& out,
                                Tuple<Head, Tail...> const& tuple) {
    out<<tuple.head();
    out<<"::";
    out<<tuple.tail();
    return out;
}


static std::ostream& operator<<(std::ostream& out,
                                Tuple<> const& tuple) {
    out<<"Nil";
    return out;
}

#endif //TRAVELLERAPP_TUPLE_HPP
