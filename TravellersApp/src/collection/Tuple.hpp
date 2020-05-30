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
private:
    size_t _size = 0;
protected:
    template<int ind, int dummy = 0>
    struct Resolve{
        using type = void;
    };
public:

    template<int ind>
    void* get(){
        return nullptr;
    }

    size_t size(){
        return _size;
    }

    Tuple(){}
    bool operator==(Tuple<> const& other) const {
        return true;
    }
    // unsigned size(){
    //     return 0;
    // }
};

template < typename Head, typename ... Tail >
class Tuple <Head, Tail...> : private Tuple<Tail...> {
private:
    using type = Tuple<Head, Tail...>;
    using inherited  = Tuple<Tail...>;
    size_t _size = 1+sizeof...(Tail);


protected:
    Head _head;

    template<int ind, int dummy=0>
    struct Resolve{
        using type = typename inherited::template Resolve<ind-1, dummy>::type;
    };

    template<int dummy>
    struct Resolve<0, dummy>{
        using type = Head;
    };

public:


    size_t size(){
        return _size;
    }

    template<int ind>
    typename Resolve<ind, 0>::type* get(){
        if(ind <= 0) return reinterpret_cast<typename Resolve<ind, 0>::type*>
                         (&head());
        return reinterpret_cast<typename Resolve<ind, 0>::type*>
            (tail().template get<ind-1>());
    }


    // unsigned size(){
    //     return 1 + tail().size();
    // }

    template<int i, typename...LTail>
    struct ofSize{
        using type = typename ofSize<i-1, Head, LTail...>::type;
    };

    template<typename...LTail>
    struct ofSize<0, LTail...>{
        using type = Tuple<LTail...>;
    };

    Tuple() = delete;

    template <typename... VariadicValues>
    Tuple(Head const& head, VariadicValues...tail)
        : _head(head), inherited(tail...) {}

    template <typename... VariadicValues>
    Tuple(Head&& head, VariadicValues&&...tail)
        : _head(std::move(head)), inherited(tail...) {}

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
