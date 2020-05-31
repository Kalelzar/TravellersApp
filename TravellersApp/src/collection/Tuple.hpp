#ifndef TRAVELLERAPP_TUPLE_HPP
#define TRAVELLERAPP_TUPLE_HPP


#include <iostream>
#include <cstdlib>
#include <type_traits>
#include <functional>
#include "../ttype.hpp"

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

public:
    template<int ind, typename dummy = int>
    struct typeOf{
        using type = void;
    };

    template<int ind>
    void* get(){
        return nullptr;
    }

    template<typename T>
    Tuple(T** tptr){}

    template<typename T>
    constexpr int indexOf(const T& elem){
        return 1;
    }

    template<int ind>
    const void* get() const {
        return nullptr;
    }

    template<typename T>
    T* at(int ind) const {
        return nullptr;
    }

    constexpr size_t size() const{
        return _size;
    }

    void free(){

    }



    Tuple(){}
    bool operator==(Tuple<> const& other) const {
        return true;
    }
    // unsigned size(){
    //     return 0;
    // }
};


static std::ostream& operator<<(std::ostream& out,
                                Tuple<> const& tuple) {
    return out;
}

template<typename Head, typename ...Tail>
static std::ostream& operator<<(std::ostream& out,
                                Tuple<Head, Tail...> const& tuple) {
    out<<"\""<<tuple.head()<<"\"";
    if(tuple.size() > 1) out<<" ";
    out<<tuple.tail();
    return out;
}




template < typename Head, typename ... Tail >
class Tuple <Head, Tail...> : private Tuple<Tail...> {
private:
    using type = Tuple<Head, Tail...>;
    using inherited  = Tuple<Tail...>;
    static constexpr size_t _size = 1+sizeof...(Tail);

public:


    template<int ind, typename dummy=int>
    struct typeOf{
        using type = typename inherited::template typeOf<ind-1, dummy>::type;
    };

    template<typename dummy>
    struct typeOf<0, dummy>{
        using type = Head;
    };

    constexpr size_t size() const {
        return _size;
    }

    template<int ind>
    std::add_const_t<typename typeOf<ind>::type>* get() const {
        if(ind <= 0)
            return reinterpret_cast<std::add_const_t<typename typeOf<ind>::type>*>
                         (&head());
        return reinterpret_cast<std::add_const_t<typename typeOf<ind>::type>*>
            (tail().template get<ind-1>());
    }

    template<typename T,
             std::enable_if_t<std::is_same<type,
                                   typename Tuple<std::remove_reference_t<T>>::
                                           template ofSize<_size>::type>::value>* = nullptr>
    T const* at(int ind) const {
        if(ind == 0) return &head();
        else return tail().template at<T>(ind-1);
    }

    template<int ind>
    typename typeOf<ind>::type* get() {
        if(ind <= 0) return reinterpret_cast<typename typeOf<ind>::type*>
                         (&head());
        return reinterpret_cast<typename typeOf<ind>::type*>
            (tail().template get<ind-1>());
    }


    // unsigned size(){
    //     return 1 + tail().size();
    // }

    template<int i>
    struct ofSize{
        using type =
            typename forType<Head>::template repeat<i>::template apply<Tuple>;
    };

    Tuple() = delete;

    template <typename... VariadicValues>
    Tuple(Head const& head, VariadicValues...tail)
        : _head(head), inherited(tail...) {}

    template <typename... VariadicValues>
    Tuple(Head&& head, VariadicValues&&...tail)
        : _head(std::move(head)), inherited(tail...) {}

    // template <typename T,
    //           std::enable_if_t<std::is_same<type, typename Tuple<Head>::template
    //                                       ofSize<_size>::type>::value>* =nullptr>
    // typename Tuple<T>::template ofSize<_size>::type
    //   map(std::function<T(Head&)> mapper){
    //     return collect()->map(mapper)
    //         ->template slice<_size>()
    //         ->get(0);
    // }

    // template <typename T,
    //          std::enable_if_t<std::is_same<type, typename Tuple<T>::template
    //                                        ofSize<_size>::type>::value>* =nullptr>
    // unique_ptr<ArrayList<T>> collect(){
    //     auto list = make_unique<ArrayList<T>>();
    //     list.append(head());
    //     list.appendAll(tail().collect());
    //     return list;
    // }

    template <typename T,
              std::enable_if_t<std::is_same<type, typename Tuple<T>::template
                                         ofSize<_size>::type>::value>* =nullptr>
    Tuple(T** tptr) : _head(**tptr), inherited(tptr+1){}

    Tuple(type const& other)
        : _head(other.head()), inherited(other.tail()){}

    Head& head(){
        return _head;
    }

    template<typename T,
             std::enable_if_t<!std::is_same<Head, T>::value> * = nullptr>
    constexpr int indexOf(T const& elem){
        int ind = 1 + tail().template indexOf<T>(elem);
        return ind >= size() || ind < 0 ? -2 : ind;
    }

    template<typename T,
             std::enable_if_t<std::is_same<Head, T>::value> * = nullptr>
    constexpr int indexOf(Head const& elem){
        int ind = elem == head() ? 0 : 1 + tail().template indexOf<T>(elem);
        return ind > size() || ind < 0 ? -2 : ind;
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

    void free(){
        delete [] _head;
        tail().free();
    }

protected:
    Head _head;

};



#endif //TRAVELLERAPP_TUPLE_HPP
