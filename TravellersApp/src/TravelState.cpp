#include "TravelState.hpp"
#include <cstring>
#include <filesystem>

bool Travel::TravelState::login(const char *username, const char *password) {
  if (loggedInUser) {
    std::cerr << "Already logged in into " << loggedInUser->getName()
              << std::endl;
    return true;
  }

  auto userNameInd = csv.getKeys()->find([username](const char *const(&key)) {
    return strcmp(key, username) == 0;
  });
  if (userNameInd < 0) {
    std::cerr << "Username or password doesn't match our database."
              << std::endl;
    return false;
  }

  auto userEntry = csv.getRow(userNameInd);

  if (strcmp(*userEntry.get<1>(), password) != 0) {
    std::cerr << "Username or password doesn't match our database."
              << std::endl;
    return false;
  }

  loggedInUser = users.get(username)->get();

  return true;
}

void Travel::TravelState::save(VisitBuilder &vb) { loggedInUser->addVisit(vb); }

bool Travel::TravelState::uregister(const char *email, const char *username,
                                    const char *password) {
  if (loggedInUser) {
    std::cerr << "Already logged in into " << loggedInUser->getName()
              << std::endl;
    return true;
  }
  auto userNameInd = csv.getKeys()->find([username](const char *const(&key)) {
    return strcmp(key, username) == 0;
  });

  if (userNameInd >= 0) {
    std::cerr << "Username is taken." << std::endl;
    return false;
  }

  char *_usrcpy = new char[strlen(username) + 1];
  strcpy(_usrcpy, username);
  char *_passcpy = new char[strlen(password) + 1];
  strcpy(_passcpy, password);
  char *_mailcpy = new char[strlen(email) + 1];
  strcpy(_mailcpy, email);

  const char *usrcpy = _usrcpy;
  const char *passcpy = _passcpy;
  const char *mailcpy = _mailcpy;
  std::filesystem::create_directory(username);
  csv.addEntry(usrcpy, passcpy, mailcpy);
  users.put(username, new User(username));

  loggedInUser = users.get(username)->get();
  return true;
}

void Travel::TravelState::friendAdd(const char *name) {
  auto userNameInd = csv.getKeys()->find(
      [name](const char *const(&key)) { return strcmp(key, name) == 0; });
  if (userNameInd < 0) {
    std::cerr << "User with the name " << name << " doesn't exist" << std::endl;
    return;
  }
  loggedInUser->addFriend(name);
}

void Travel::TravelState::friendRemove(const char *name) {
  auto userNameInd = csv.getKeys()->find(
      [name](const char *const(&key)) { return strcmp(key, name) == 0; });
  if (userNameInd < 0) {
    std::cerr << "User with the name " << name << " doesn't exist" << std::endl;
    return;
  }

  loggedInUser->removeFriend(name);
}

void Travel::TravelState::friendList() { loggedInUser->listFriends(); }

void Travel::TravelState::userVisited(const char *dest, const char *name) {

  auto user = users.get(name);
  if (user->isEmpty()) {
    std::cerr << "User with the name " << name << " doesn't exist."
              << std::endl;
    return;
  }

  auto userg = user->get();

  if (!userg->hasVisited(dest)) {
    std::cout << name << " hasn't visited " << dest << std::endl;
    return;
  }

  auto deststats = userg->destinationStats(dest);
  auto from = *deststats.get<1>();
  auto to = *deststats.get<2>();
  auto rating = atoi(*deststats.get<3>());
  auto comment = *deststats.get<4>();

  std::cout << name;
  if (rating > 0) {
    std::cout << " (";
    for (int i = 1; i <= rating; i++) {
      std::cout << "*";
    }
    std::cout << ")";
  }

  std::cout << " [";
  if (strcmp(from, to) == 0) {
    std::cout << from;
  } else {
    std::cout << from << " - " << to;
  }
  std::cout << "]";

  if (strlen(comment) > 0) {
    std::cout << " says: " << std::endl;
    std::cout << comment;
  }

  std::cout << std::endl;
}

void Travel::TravelState::friendVisited(const char *dest, const char *name) {
  bool hasfr = loggedInUser->hasFriend(name);
  if (!hasfr) {
    std::cerr << name << " is not your friend." << std::endl;
    return;
  }

  userVisited(dest, name);
}

void Travel::TravelState::friendVisited(const char *dest) {
  auto fr = loggedInUser->getFriends();
  fr.foreach ([&dest, this](char *const(&f)) { userVisited(dest, f); });
}

void Travel::TravelState::destinationComments(const char *dest) {
  users.values()
      ->filter([&dest](User *const(&u)) { return u->hasVisited(dest); })
      ->foreach (
          [&dest, this](User *const(&u)) { userVisited(dest, u->getName()); });
}

void Travel::TravelState::destinationRating(const char *dest) {
  auto ratings =
      users.values()
          ->filter([&dest](User *const(&u)) { return u->hasVisited(dest); })
          ->map<int>([&dest](User *const(&u)) {
            auto stats = u->destinationStats(dest);
            return atoi(*stats.get<3>());
          })
          ->filter([](int const &i) { return i > 0; });
  int ratingCount = ratings->length();
  if (ratingCount == 0) {
    std::cout << "Nobody has rated this destination yet." << std::endl;
    return;
  }
  int sum = ratings->fold(0, [](int const &a, int const &b) { return a + b; });
  std::cout << "Average rating of " << dest << " is "
            << (((double)sum) / ratingCount) << std::endl;
}

void Travel::TravelState::destinationList() {
  std::cout << "The following destinations are registered." << std::endl;
  users.values()
      ->map<ArrayList<char *>>(
          [](User* const (&user)) { return *(user->getDestinations()); })
      ->flatten<char*>()
      ->distinct()
      ->foreach ([](char *const(&s)) { std::cout << s << std::endl; });
}

bool Travel::TravelState::destinationExists(const char *dest) {
  return users.values()->find(
             [&dest](User *const(&u)) { return u->hasVisited(dest); }) >= 0;
}
