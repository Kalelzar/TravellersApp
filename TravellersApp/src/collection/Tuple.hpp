#ifndef TRAVELLERAPP_TUPLE_HPP
#define TRAVELLERAPP_TUPLE_HPP

// #include <iostream>
// #include <cstdlib>
// #include <type_traits>


// // Tuple implementation partially taken from
// // www.stroustrup.com/C++11FAQ.html#variadic-templates

// template <typename Head, typename ... Tail>
// class Tuple : private Tuple< Tail... >{
// private:
//     using Inherited = Tuple<Tail...>;
// public:

//     Tuple(){}

// // Construct tuple from separate arguments:
//     Tuple(typename std::add_lvalue_reference_t<Head> v,
//           typename std::add_lvalue_reference_t<Tail>... vtail)
//         : _head(v), Inherited(vtail...) { }

//     // Construct tuple from another tuple:
//     template<typename... VValues>
//     Tuple(const Tuple<VValues...>& other)
//         : _head(other.head()), Inherited(other.tail()) { }

//     template<typename... VValues>
//     Tuple& operator=(const Tuple<VValues...>& other) // assignment
//         {
//             _head = other.head();
//             tail() = other.tail();
//             return *this;
//         }

//     typename std::add_lvalue_reference_t<Head> head() { return _head; }
//     typename std::add_lvalue_reference_t<const Head> head()const{ return _head;}

//     Inherited& tail() { return *this; }
//     const Inherited& tail() const { return *this; }

// protected:
//     Head _head;

// };

// template <typename Head>
// class Tuple<Head> {
// public:
//     Tuple(){}

// // Construct tuple from separate arguments:
//     Tuple(typename std::add_lvalue_reference_t<Head> v)
//         : _head(v) { }

//     // Construct tuple from another tuple:
//     template<typename Value>
//     Tuple(const Tuple<Value>& other)
//         : _head(other.head()) { }

//     template<typename Value>
//     Tuple& operator=(const Tuple<Value>& other)// assignment
//         {
//             _head = other.head();
//             return *this;
//         }

//     typename std::add_lvalue_reference_t<Head> head() { return _head; }
//     typename std::add_lvalue_reference_t<const Head> head()const{ return _head;}
// protected:
//     Head _head;
// };

#endif //TRAVELLERAPP_TUPLE_HPP
