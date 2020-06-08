#ifndef TRAVELLERAPP_VISITBUILDER_HPP
#define TRAVELLERAPP_VISITBUILDER_HPP

#include "Date.hpp"
#include "SimpleString.hpp"
#include "collection/ArrayList.hpp"

namespace Travel {
    class VisitBuilder {
    private:
        char* _destination = nullptr;
        Date _from;
        bool setFrom = false;
        Date _to;
        bool setTo = false;
        int _rating = -1;
        char* _comment = nullptr;
        ArrayList<SimpleString> _photos;
    public:

        ArrayList<SimpleString> getPics(){
            return _photos;
        }

        char* getComment() const {
            return _comment;
        }

        int getRating() const {
            return _rating;
        }

        Date getTo() const {
            return _to;
        }
        Date getFrom() const {
            return _from;
        }

        char* getDestination() const {
            return _destination;
        }
        void from(char* fdate){
            if(setFrom){
                std::cerr<<"Already set 'from' to "<<_from
                <<". Changing value."<<std::endl;
            }
            _from = Travel::Date(fdate);
            setFrom = true;
        }

        void to(char* fdate){
            if(setTo){
                std::cerr<<"Already set 'to' to "<<_to
                <<". Changing value."<<std::endl;
            }
            _to = Travel::Date(fdate);
            setTo = true;
        }

        void rating(int i){
            if(i < 1 || i > 5){
                std::cerr<<"Rating must be between 1 - 5 inclusive"<<std::endl;
                return;
            }
            if(_rating != -1){
                std::cerr<<"Already rated "<<_rating
                         <<". Changing value."<<std::endl;
            }
            _rating = i;
        }

        void comment(const char* comment){
            if(_comment){
                std::cerr<<"Already commented "<<_comment
                         <<". Changing value."<<std::endl;
                delete [] _comment;
            }
            _comment = new char[strlen(comment)+1];
            strcpy(_comment, comment);
        }

        void photoAdd(SimpleString&& uri){
            _photos.append(std::move(uri));
        }

        void photoDelete(SimpleString&& uri){
            auto res = _photos.remove(std::move(uri));
            if(res == -1){
                std::cerr<<"This photo does not exists."<<std::endl;
            }
        }

        void photoShow(){
            std::cout<<"Photos uploaded: "<<std::endl;
            _photos.foreach(
                [](SimpleString const& ss){
                    std::cout<<ss<<std::endl;
                }
                );
        }

        void destination(const char* destination){
            if(_destination){
                std::cerr<<"Already chose "<<_destination
                         <<" as destination. Changing value."<<std::endl;
                delete [] _destination;
            }
            _destination = new char[strlen(destination)+1];
            strcpy(_destination, destination);
        }

        void clear(){
            setFrom = false;
            setTo = false;
            if(_destination){
                delete [] _destination;
                _destination = nullptr;
            }
            if(_comment){
                delete [] _comment;
                _comment = nullptr;
            }
            _rating = -1;
            _photos.clear();
        }
    };
}


#endif //TRAVELLERAPP_VISITBUILDER_HPP
