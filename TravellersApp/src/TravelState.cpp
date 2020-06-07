#include "TravelState.hpp"
#include <cstring>

bool Travel::TravelState::login(const char* username, const char* password) {
    if(loggedInUser) {
        std::cerr<<"Already logged in into "<<loggedInUser->getName()<<std::endl;
        return true;
    }

    auto userNameInd = csv.getKeys()
        ->find([username](const char* const (&key)){
                   return strcmp(key, username) == 0;
                });
    if(userNameInd < 0) {
        std::cerr<<"Username or password doesn't match our database."
                 <<std::endl;
        return false;
    }

    auto userEntry = csv.getRow(userNameInd);

    if(strcmp(*userEntry.get<1>(), password) != 0){
        std::cerr<<"Username or password doesn't match our database."
                 <<std::endl;
        return false;
    }

    loggedInUser = users.get(username)->get();

    return true;
}

bool Travel::TravelState::uregister(const char* email,
                                    const char* username,
                                    const char* password) {
    if(loggedInUser) {
        std::cerr<<"Already logged in into "<<loggedInUser->getName()<<std::endl;
        return true;
    }
    auto userNameInd = csv.getKeys()
        ->find([username](const char* const (&key)){
                   return strcmp(key, username) == 0;
               });

    if(userNameInd >= 0) {
        std::cerr<<"Username is taken."
                 <<std::endl;
        return false;
    }

    char* _usrcpy = new char[strlen(username)+1];
    strcpy(_usrcpy, username);
    char* _passcpy = new char[strlen(password)+1];
    strcpy(_passcpy, password);
    char* _mailcpy = new char[strlen(email)+1];
    strcpy(_mailcpy, email);

    const char* usrcpy = _usrcpy;
    const char* passcpy = _passcpy;
    const char* mailcpy = _mailcpy;

    csv.addEntry(usrcpy, passcpy, mailcpy);
    users.put(username, new User(username));

    loggedInUser = users.get(username)->get();
    return true;
}

void Travel::TravelState::friendAdd(const char* name) {
    auto userNameInd = csv.getKeys()
        ->find([name](const char* const (&key)){
                   return strcmp(key, name) == 0;
               });
    if(userNameInd < 0){
        std::cerr<<"User with the name "<<name<<" doesn't exist"<<std::endl;
        return;
    }
    loggedInUser->addFriend(name);
}

void Travel::TravelState::friendRemove(const char* name) {
    auto userNameInd = csv.getKeys()
        ->find([name](const char* const (&key)){
                   return strcmp(key, name) == 0;
               });
    if(userNameInd < 0){
        std::cerr<<"User with the name "<<name<<" doesn't exist"<<std::endl;
        return;
    }

    loggedInUser->removeFriend(name);
}


void Travel::TravelState::friendList(){
    loggedInUser->listFriends();
}
