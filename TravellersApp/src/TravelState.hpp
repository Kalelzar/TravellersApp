#ifndef TRAVELLERAPP_TRAVELSTATE_HPP
#define TRAVELLERAPP_TRAVELSTATE_HPP

#include "parser/CSV.hpp"
#include "VisitBuilder.hpp"
#include "User.hpp"


namespace Travel {
    class TravelState {
    private:
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wwrite-strings"
        static constexpr char* usersDatabase = "users.db";
#pragma GCC diagnostic pop
        CSV<const char*, const char*,const char*> csv{"user", "pass", "email"};
        HashMap<SimpleString, User*> users;
        User* loggedInUser = nullptr;

    public:

        TravelState(){
            csv.load(TravelState::usersDatabase);
            auto keys = csv.getKeys();
            for(int i = 0; i < csv.size(); i++){
                users.put(keys->get(i), new User(keys->get(i)));
            }
        }

        ~TravelState(){
            users.values()
                ->foreach([](User* const (&usr)){
                              delete usr;
                          });

            csv.save(TravelState::usersDatabase);
        }

        void save(VisitBuilder& vb);
        bool login(const char* username, const char* password);
        bool uregister(const char* email,
                       const char* username,
                       const char* password);

        void friendAdd(const char* name);
        void friendRemove(const char* name);
        void friendList();
    };
}


#endif //TRAVELLERAPP_TRAVELSTATE_HPP
