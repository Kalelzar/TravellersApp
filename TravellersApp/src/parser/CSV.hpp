#ifndef TRAVELLERSAPP_CSV_HPP
#define TRAVELLERSAPP_CSV_HPP

#include "../collection/Tuple.hpp"
#include "../collection/ArrayList.hpp"
#include "../ttype.hpp"
#include "../SimpleString.hpp"
#include "Scanner.hpp"
#include <fstream>


template<typename...Types>
class CSV {
private:
    static constexpr int columns = sizeof...(Types);
    static_assert(columns > 0, "CSV file needs at least one column");
    using CSVHeader=typename Tuple<const char*>::ofSize<columns>::type;
    CSVHeader _header;
    using Entry= Tuple<Types...>;
    ArrayList<Entry> entries;

public:

    CSV() = delete;
    CSV(CSVHeader const& header) : _header(header) {};

    template<typename...Ts>
    CSV(Ts...ts) : _header(CSVHeader(ts...)) {}


    void load(const char* filepath){
        ifstream file(filepath);
        if(file.is_open()){
            Travel::CommandList cl;
            Travel::Scanner scanner(ScannerContext::ALL, &file, &cl);
            auto tokens = scanner.scan()
                ->filter([](Token const& t){
                             return t.t != TokenType::EOF_T;
                         })
                ->template map<const char*>(
                                      [](Token const& t){
                                          char* s = t.lexeme;
                                          char* d = new char[strlen(s)+1];
                                          strcpy(d, s);
                                          return d;
                                      });
            if(tokens->length() < columns){
                std::cerr<<filepath<<" is empty"<<std::endl;
                file.close();
                return;
            }
            for(int i = 0; i < columns; i++){
                delete [] tokens->get(i);
            }
            auto readEntries = tokens->template slice<columns>();
            for(int i = 0; i < readEntries->length(); i++){
                auto k = getKeys();
                if(k->find([&readEntries, i](const char* const (&entry)){
                               return strcmp(readEntries->get(i).head(), entry) == 0;
                            }) >= 0 ){
                    std::cerr<<"Key " << readEntries->get(i).head()
                             << " already exists. Skipping"<<std::endl;
                    readEntries->get(i).free();
                    continue;
                }
                entries.append(readEntries->get(i));
            }
        }else{
            std::cerr<<"Cannot open " << filepath <<" for read."<<std::endl;
        }
        file.close();
    }

    void save(const char* filepath){
        ofstream file(filepath);
        if(file.is_open()){
            file<<header()<<std::endl;
            for(int i = 0; i < size(); i++){
                file<<entries.get(i)<<std::endl;
            }
        }else{
            std::cerr<<"Cannot open " << filepath <<" for write."<<std::endl;
        }
        file.close();
    }


    CSVHeader header(){ return _header; }

    void addEntry(Types&...data){
        entries.append({data...});
    }

    Tuple<Types...>& getRow(unsigned ind) {
        return entries.get(ind);
    }

    unsigned size(){
        return entries.length();
    }

    auto getKeys()
        -> unique_ptr<ArrayList<typename Entry::template typeOf<0>::type>>{
        return getColumn<0>();
    }


    template<int ind>
    auto getColumn()
        -> unique_ptr<ArrayList<typename Entry::template typeOf<ind>::type>>{
        return entries.template map<typename Entry::template
                             typeOf<ind>::type>(
                                                [](Entry const& t){
                                                    return *t.template
                                                        get<ind>();
                                                });

    }

    ~CSV(){
        for(int i = 0; i < entries.length(); i++){
            entries.get(i).free();
        }
    }

    auto getByColumnHeader(const char* s){
        int ind = header().template indexOf<const char*>(s);
        if(entries.length() == 0) {
            throw NoValueException("CSV contains no entries");
        }
        auto t = *entries.get(0).template at<decltype(s)>(ind);
        return entries.template map<decltype(t)>
            ([ind](const Entry& e){
                 return *e.template at<decltype(s)>(ind);
            });
    }


};
#endif //TRAVELLERSAPP_CSV_HPP
