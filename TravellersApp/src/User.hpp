#ifndef TRVL_USER_HPP
#define TRVL_USER_HPP

#include <filesystem>

#include "parser/CSV.hpp"
#include "collection/ArrayList.hpp"

class User {
private:

    char* username = nullptr;
    char* friendDBPath = nullptr;
    char* usrDBPath = nullptr;
    ArrayList<char*> friends;

    forType<char*>::repeat<5>::apply<CSV> destinations{"dest", "from", "to",
                                                       "rating", "comment"};
    HashMap<SimpleString, ArrayList<char*>> pics;

    void setUserName(const char* _username){
        if(username) delete [] username;
        if(friendDBPath) delete [] friendDBPath;
        username = new char[strlen(_username) + 1];
        // /friend.db - 10 chars
        friendDBPath = new char[strlen(_username)+10+1];
        usrDBPath = new char[strlen(_username)+3+1];
        strcpy(username, _username);
        strcpy(friendDBPath, _username);
        strcpy(friendDBPath+strlen(_username),
               "/friend.db");
        strcpy(usrDBPath, _username);
        strcpy(usrDBPath+strlen(_username),
               ".db");
    }

public:

    User() = delete;

    User(const char* name){
        setUserName(name);
        CSV<const char*> friendsCSV("name");
        friendsCSV.load(friendDBPath);
        auto names = friendsCSV.getKeys();
        if(names.get()){
            for(int i = 0; i < names->length(); i++){
                addFriend(names->get(i));
            }
        }
    }

    const char* getName() const {
        return username;
    }

    void addFriend(const char* name){
        auto userNameInd = friends
            .find([name](char* const (&key)){
                       return strcmp(key, name) == 0;
                   });
        if(userNameInd >= 0 ) {
            std::cerr<<name<<" is already your friend."<<std::endl;
            return;
        }
        auto _name = new char[strlen(name) + 1];
        strcpy(_name, name);
        friends.append(_name);
    }

    void removeFriend(const char* name){
        unique_ptr<Nullable<char*>> rem =
            friends.removeIf([name](char* fr){
                                  return strcmp(name, fr) == 0;
                         });

        if(rem->isDefined()){
            delete [] rem->get();
        }
    }

    void listFriends(){
        friends.foreach([](char* fr){
                            std::cout<<fr<<std::endl;
                        });
    }


    bool operator==(User const& other){
        return strcmp(getName(), other.getName())==0;
    }

    ~User(){
        CSV<char*> friendsCSV("name");
        for(int i = 0; i < friends.length(); i++){
            friendsCSV.addEntry(friends.get(i));
        }
        if(std::filesystem::exists(username) ||
           std::filesystem::create_directory(username)){
            friendsCSV.save(friendDBPath);
            destinations.save(usrDBPath);
            auto kd = pics.keys();
            auto vd = pics.values();
            auto usrln = strlen(username);
            for(int i = 0; i < kd->length(); i++){
                auto key = kd->get(i);
                auto value = vd->get(i);
                auto kpath = new char[usrln+1+key.length()+3+1];
                strcpy(kpath, username);
                strcpy(kpath+usrln, "/");
                strcpy(kpath+usrln+1, key.get());
                strcpy(kpath+usrln+1+key.length(), ".db");
                ofstream file(kpath);
                if(file.is_open()){
                    file<<"pics"<<std::endl;
                    value
                        .foreach([&file](char* const(&pic)) {
                                      file<<"\""<<pic<<"\""<<std::endl;
                                  });
                }else{
                    std::cerr<<"Could not open "<<kpath
                             <<" for write."<<std::endl;
                }
                file.close();
                delete [] kpath;
            }
        }else{
            std::cerr<<"Failed to create directory:"
                     <<username
                     <<". Cannot save userdata";

        }


        if(username) delete [] username;
        if(friendDBPath) delete [] friendDBPath;
        if(usrDBPath) delete [] usrDBPath;
    }

};

#endif //TRVL_USER_HPP
