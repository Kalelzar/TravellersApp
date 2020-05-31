#ifndef TTYPE_HPP
#define TTYPE_HPP

template<typename Type>
struct forType{

    template<int times, typename...Tail>
    struct repeat : repeat<times-1, Type, Tail...>{
        static_assert(times > 0,
                      "Cannot repeat a type a negative number of times.");

        template < template<typename...A> class applicant >
        using apply =
            typename repeat<times-1, Type, Tail...>::template apply<applicant>;
    };

    template<typename...Tail>
    struct repeat<0, Tail...>{
        template < template<typename...A> class applicant >
        using apply = applicant<Tail...>;
    };

};

#endif // TTYPE_HPP
