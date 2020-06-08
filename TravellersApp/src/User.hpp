#ifndef TRVL_USER_HPP
#define TRVL_USER_HPP

#include <filesystem>

#include "parser/CSV.hpp"
#include "collection/ArrayList.hpp"
#include "VisitBuilder.hpp"

#include <cstdlib>

#include <stdio.h>
#ifdef TRVL_PLATFORM_LINUX
#include <unistd.h>
#define workingDirectory(PATH, LENGTH) getcwd(PATH, LENGTH)
#elif TRVL_PLATFORM_WINDOWS
#undef _HAS_STD_BYTE
#include <direct.h>
//I still have no idea why Windows dislikes being POSIX-compliant
#define workingDirectory(PATH, LENGTH) _getcwd(PATH, LENGTH)
#else
#error "Your platform is not supported."
#endif

class User {
private:

    char* username = nullptr;
    char* friendDBPath = nullptr;
    char* usrDBPath = nullptr;
    ArrayList<char*> friends;

    forType<char*>::repeat<5>::apply<CSV> destinations{"dest", "from", "to",
                                                       "rating", "comment"};
    HashMap<SimpleString, ArrayList<SimpleString>> pics;

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
               "/db.friend");
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
        destinations.load(usrDBPath);
        auto names = friendsCSV.getKeys();
        if(names.get()){
            for(int i = 0; i < names->length(); i++){
                addFriend(names->get(i));
            }
        }

        // char* path = new char[FILENAME_MAX+1];
        // if(!workingDirectory(path, FILENAME_MAX)){
        //     std::cerr<<"Failed to read current working directory"<<std::endl;
        // }


        for (const auto & entry : std::filesystem::directory_iterator(username)){
            auto epath = entry.path();
            if(epath.extension().string().compare(".db") != 0) continue;
            auto dest = SimpleString(epath.filename().stem().c_str());
            auto path = epath.c_str();
            CSV<char*> s("pics");
            s.load(path);
            ArrayList<SimpleString> l = *s.getKeys()
                ->template map<SimpleString>([]
                                    (char* const(&s)){
                                        return SimpleString(s);
                                    });
            pics.put(dest, l);
        }

        // delete [] path;
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

    void addVisit(Travel::VisitBuilder& vb){
        char* dest = vb.getDestination();
        Travel::Date from = vb.getFrom();
        Travel::Date to   = vb.getTo();
        int rating = vb.getRating();
        char* comment = vb.getComment() ? vb.getComment() : new char[1];
        if(!vb.getComment())comment[0] = '\0';
        char* sfrom = new char[11];
        char* sto = new char[11];

        from.getString(sfrom);
        to.getString(sto);

        char* srating = new char[3];
        if(rating == -1){
            srating[0] = '-';
            srating[1] = '1';
            srating[2] = '\0';

        }else{
            srating[0] = (char) (48 + rating);
            srating[1] = '\0';
        }


        destinations.addEntry(dest, sfrom , sto, srating,
                              comment);

        pics.put(dest, vb.getPics());
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
                    file<<"\"pics\""<<std::endl;
                    value
                        .foreach([&file](SimpleString const(&pic)) {
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
