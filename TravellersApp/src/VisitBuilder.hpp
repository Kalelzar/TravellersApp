#ifndef TRAVELLERAPP_VISITBUILDER_HPP
#define TRAVELLERAPP_VISITBUILDER_HPP

#include "Date.hpp"
#include "ArrayList.hpp"

namespace Travel {
    class VisitBuilder {
    private:
        char* _destination = nullptr;
        Travel::Date _from;
        bool setFrom = false;
        Travel::Date _to;
        bool setTo = false;
        int _rating = -1;
        char* _comment = nullptr;
        ArrayList<char*> _pics;
        bool picsAddeded = true
    public:
        void from(char* fdate){
            if(setFrom){
                std::cerr<<"Already set 'from' to "<<_from
                <<". Changing value."<<std::endl;
            }
            setFrom=true;
            _from = Travel::Date(fdate);
        }
        void to(char* fdate){
            if(setTo){
                std::cerr<<"Already set 'to' to "<<_to
                <<". Changing value."<<std::endl;
            }
            setTo=true;
            _to = Travel::Date(fdate);
        }
    };
}


#endif //TRAVELLERAPP_VISITBUILDER_HPP
