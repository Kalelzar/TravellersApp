#ifndef TRVL_USER_HPP
#define TRVL_USER_HPP

#include <filesystem>

#include "VisitBuilder.hpp"
#include "collection/ArrayList.hpp"
#include "parser/CSV.hpp"

#include <cstdlib>

#include <stdio.h>
#ifdef TRVL_PLATFORM_LINUX
#include <unistd.h>
#define workingDirectory(PATH, LENGTH) getcwd(PATH, LENGTH)
#elif TRVL_PLATFORM_WINDOWS
#undef _HAS_STD_BYTE
#include <direct.h>
// I still have no idea why Windows dislikes being POSIX-compliantsd
#define workingDirectory(PATH, LENGTH) _getcwd(PATH, LENGTH)
#else
#error "Your platform is not supported."
#endif

/**
 * This class is responsible for storing and loading information regarding
 * the user. All reading/writing of information is reliant on {@link CSV CSV}.
 * All information is stored as a char*, so additional conversion
 * (such as to int or {@link Travel::Date Date}) may be necessary.
 */
class User {
private:
  char *username = nullptr;
  char *friendDBPath = nullptr;
  char *usrDBPath = nullptr;
  ArrayList<char *> friends;

  forType<char *>::repeat<5>::apply<CSV> destinations{"dest", "from", "to",
                                                      "rating", "comment"};
  HashMap<SimpleString, ArrayList<SimpleString>> pics;

  /**
   * Sets the username. Also generates the paths to where information
   * for the user is stored.
   * @param _username the new username
   */
  void setUserName(const char *_username) {
    if (username)
      delete[] username;
    if (friendDBPath)
      delete[] friendDBPath;
    username = new char[strlen(_username) + 1];
    // /friend.db - 10 chars
    friendDBPath = new char[strlen(_username) + 10 + 1];
    usrDBPath = new char[strlen(_username) + 3 + 1];
    strcpy(username, _username);
    strcpy(friendDBPath, _username);
    strcpy(friendDBPath + strlen(_username), "/db.friend");
    strcpy(usrDBPath, _username);
    strcpy(usrDBPath + strlen(_username), ".db");
  }

public:
  /**
   * Return a reference to the list of friend names.
   * @return the list of friends
   */
  ArrayList<char *> &getFriends() { return friends; }

  /**
   * Return a reference to the information about a visit to a
   * provided destination stored in a tuple.
   ******************************************************
   * INDEX *      0      *  1   *  2 *    3   *    4    *
   ******************************************************
   * STORE * DESTINATION * FROM * TO * RATING * COMMENT *
   ******************************************************
   * @param dest the destination whose information to return
   * @return the information for the destination
   */
  forType<char *>::repeat<5>::apply<Tuple> &destinationStats(const char *dest) {
    auto ind = destinations.getKeys()->find(
        [&dest](char *const(&key)) { return strcmp(dest, key) == 0; });
    return destinations.getRow(ind);
  }

  /**
   * Checks if user has visited the provided destination.
   * @param dest the destination to check.
   * @return has the user visited the destination
   */
  bool hasVisited(const char *dest) {
    return destinations.getKeys()->find([&dest](char *const(&key)) {
      return strcmp(dest, key) == 0;
    }) >= 0;
  }

  User() = delete;

  User(const char *name) {
    setUserName(name);
    CSV<const char *> friendsCSV("name");
    friendsCSV.load(friendDBPath);
    destinations.load(usrDBPath);
    auto names = friendsCSV.getKeys();
    if (names.get()) {
      for (int i = 0; i < names->length(); i++) {
        addFriend(names->get(i));
      }
    }

    for (const auto &entry : std::filesystem::directory_iterator(username)) {
      auto epath = entry.path();
      if (epath.extension().string().compare(".db") != 0)
        continue;
      auto dest = SimpleString(epath.filename().stem().c_str());
      auto path = epath.c_str();
      CSV<char *> s("pics");
      s.load(path);
      ArrayList<SimpleString> l = *s.getKeys()->template map<SimpleString>(
          [](char *const(&s)) { return SimpleString(s); });
      pics.put(dest, l);
    }
  }

  /**
   * Return the name of the user.
   */
  const char *getName() const { return username; }

  /**
   * Add the provided user as a friend.
   * @param name the name of the user to befriend
   */
  void addFriend(const char *name) {
    auto userNameInd = friends.find(
        [name](char *const(&key)) { return strcmp(key, name) == 0; });
    if (userNameInd >= 0) {
      std::cerr << name << " is already your friend." << std::endl;
      return;
    }
    auto _name = new char[strlen(name) + 1];
    strcpy(_name, name);
    friends.append(_name);
  }

  /**
   * Remove the provided user from the user's list of friends.
   * @param name the name of the user to befriend
   */
  void removeFriend(const char *name) {
    auto rem =
        friends.removeIf([name](char *fr) { return strcmp(name, fr) == 0; });

    if (rem->isDefined()) {
      delete[] rem->get();
    }
  }

  /// Print the user's friends to stdout
  void listFriends() {
    friends.foreach ([](char *fr) { std::cout << fr << std::endl; });
  }

  /**
   * Check if the provided user is a friend of the current user.
   * @param the name of the user to check
   */
  bool hasFriend(const char *name) {
    return friends.find([&name](char *fr) { return strcmp(name, fr) == 0; }) >=
           0;
  }

  /**
   * Users are only differentiated via their username.
   * Hence the name being unique.
   */
  bool operator==(User const &other) {
    return strcmp(getName(), other.getName()) == 0;
  }

  /**
   * Register the visit described by the provided
   * {@link Travel::VisitBuilder VisitBuilder} with the user.
   * @param vb the VisitBuilder.
   */
  void addVisit(Travel::VisitBuilder &vb) {
    char *dest = vb.getDestination();
    if (!dest)
      return;
    Travel::Date from = vb.getFrom();
    Travel::Date to = vb.getTo();
    int rating = vb.getRating();
    char *comment = vb.getComment() ? vb.getComment() : new char[1];
    if (!vb.getComment())
      comment[0] = '\0';
    char *sfrom = new char[11];
    char *sto = new char[11];

    from.getString(sfrom);
    to.getString(sto);

    char *srating = new char[3];
    if (rating == -1) {
      srating[0] = '-';
      srating[1] = '1';
      srating[2] = '\0';

    } else {
      srating[0] = (char)(48 + rating);
      srating[1] = '\0';
    }

    destinations.addEntry(dest, sfrom, sto, srating, comment);

    pics.put(dest, vb.getPics());
  }

  ~User() {
    CSV<char *> friendsCSV("name");
    for (int i = 0; i < friends.length(); i++) {
      friendsCSV.addEntry(friends.get(i));
    }
    if (std::filesystem::exists(username) ||
        std::filesystem::create_directory(username)) {
      friendsCSV.save(friendDBPath);
      destinations.save(usrDBPath);
      auto kd = pics.keys();
      auto vd = pics.values();
      auto usrln = strlen(username);
      for (int i = 0; i < kd->length(); i++) {
        auto key = kd->get(i);
        auto value = vd->get(i);
        auto kpath = new char[usrln + 1 + key.length() + 3 + 1];
        strcpy(kpath, username);
        strcpy(kpath + usrln, "/");
        strcpy(kpath + usrln + 1, key.get());
        strcpy(kpath + usrln + 1 + key.length(), ".db");
        std::ofstream file(kpath);
        if (file.is_open()) {
          file << "\"pics\"" << std::endl;
          value.foreach ([&file](SimpleString const(&pic)) {
            file << "\"" << pic << "\"" << std::endl;
          });
        } else {
          std::cerr << "Could not open " << kpath << " for write." << std::endl;
        }
        file.close();
        delete[] kpath;
      }
    } else {
      std::cerr << "Failed to create directory:" << username
                << ". Cannot save userdata";
    }

    if (username)
      delete[] username;
    if (friendDBPath)
      delete[] friendDBPath;
    if (usrDBPath)
      delete[] usrDBPath;
  }
};

#endif // TRVL_USER_HPP
