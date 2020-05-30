#ifndef TRAVELLERSAPP_CSV_HPP
#define TRAVELLERSAPP_CSV_HPP

#include "../collection/Tuple.hpp"

template<typename...Types>
class CSV {
private:
    using CSVHeader = typename Tuple<const char*>::ofSize<sizeof...(Types)>;
    CSVHeader _header;
public:


    CSV() = delete;
    CSV(CSVHeader header) : _header(header) {};

    CSVHeader header(){ return _header; }


};
#endif //TRAVELLERSAPP_CSV_HPP
