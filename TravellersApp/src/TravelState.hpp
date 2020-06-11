#ifndef TRAVELLERAPP_TRAVELSTATE_HPP
#define TRAVELLERAPP_TRAVELSTATE_HPP

#include "User.hpp"
#include "VisitBuilder.hpp"
#include "parser/CSV.hpp"

namespace Travel {
    ///The core of the application.
    ///Doubles as UI API
class TravelState {
private:
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wwrite-strings"
  static constexpr char *usersDatabase = "users.db";
#pragma GCC diagnostic pop

  CSV<const char *, const char *, const char *> csv{"user", "pass", "email"};
  HashMap<SimpleString, User *> users;
  User *loggedInUser = nullptr;

public:
  /// Initialize the travel state.
  /// This loads and initializes all users from the user database
  TravelState() {
    csv.load(TravelState::usersDatabase);
    auto keys = csv.getKeys();
    for (int i = 0; i < csv.size(); i++) {
      users.put(keys->get(i), new User(keys->get(i)));
    }
  }

  ~TravelState() {
    users.values()->foreach ([](User *const(&usr)) { delete usr; });

    csv.save(TravelState::usersDatabase);
  }

  /// Save the visit stored in the provided {@link VisitBuilder}
  /// to the logged-in user's visit history
  void save(VisitBuilder &vb);
  /// Attempt to log into the given user with the given password
  ///@return true upon success and false otherwise
  bool login(const char *username, const char *password);
  /// Register an account with the provided email, username and password
  /// The username must not already be taken
  bool uregister(const char *email, const char *username, const char *password);

  /// Prints the visit stats of the given user for the given desination
  void userVisited(const char *dest, const char *name);

  /// Add given user to the logged-in user's friends list
  void friendAdd(const char *name);
  /// Remove given user from the logged-in user's friends list
  void friendRemove(const char *name);
  /// List the users in the logged-in user's friends list
  void friendList();
  /// Prints the visit stats of the given user for the given desination
  /// if that user is in your friends list.
  /// @see userVisited
  void friendVisited(const char *dest, const char *name);
  /// Print the visit stats of all users in the logged in user's
  /// friend list for the given destination.
  void friendVisited(const char *dest);

  /// Returns true if the given destination is registered in at least
  /// one user's visit history
  bool destinationExists(const char *dest);
  /// Prints the average rating of the given destination to stdout
  void destinationRating(const char *dest);
  /// Print the visit stats for the given destination
  /// of all users who have visited it
  void destinationComments(const char *dest);
};
} // namespace Travel

#endif // TRAVELLERAPP_TRAVELSTATE_HPP
