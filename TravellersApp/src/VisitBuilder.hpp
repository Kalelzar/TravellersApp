#ifndef TRAVELLERAPP_VISITBUILDER_HPP
#define TRAVELLERAPP_VISITBUILDER_HPP

#include "Date.hpp"
#include "SimpleString.hpp"
#include "collection/ArrayList.hpp"
#include <regex>

/**
 * Collects all code that is not necessarily easily reusable.
 * (i. e. is too specialized to the project)
 */
namespace Travel {

/**
 * Holds all information necessary to construct a visit.
 * Meant as disposable way to package information that will be passed
 * from the UI to {@link Travel::TravelState TravelState} in a single
 * function call.
 */
class VisitBuilder {
private:
  char *_destination = nullptr;
  Date _from;
  bool setFrom = false;
  Date _to;
  bool setTo = false;
  int _rating = -1;
  char *_comment = nullptr;
  ArrayList<SimpleString> _photos;

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wwrite-strings"
  static constexpr char *regexstr =
      "([A-Z]:)?([/\\\\]?[a-zA-Z._]+)+\\.(jpg|png)";
#pragma GCC diagnostic pop
  static const std::regex path;

public:
  /// Return the list of paths to uploaded photos
  ArrayList<SimpleString> getPics() { return _photos; }

  /// Return the visit comment
  char *getComment() const { return _comment; }

  /// Return the rating
  int getRating() const { return _rating; }

  /// Return end of visit
  Date getTo() const { return _to; }

  /// Return start of visit
  Date getFrom() const { return _from; }

  /// Return visit destination
  char *getDestination() const { return _destination; }

  /// Set the start of visit.
  /// TODO: Make sure from is before to
  void from(char *fdate) {
    Travel::Date f = Travel::Date(fdate);
    if (setTo && f > _to) {
      std::cerr << "Your visit cannot begin after it ends." << std::endl;
      return;
    }
    if (setFrom) {
      std::cerr << "Already set 'from' to " << _from << ". Changing value."
                << std::endl;
    }
    _from = f;
    setFrom = true;
  }

  /// Set the end of visit.
  /// TODO: Make sure from is before to
  void to(char *fdate) {

    Travel::Date t = Travel::Date(fdate);
    if (setFrom && t < _from) {
      std::cerr << "Your visit cannot end before it begins." << std::endl;
      return;
    }
    if (setTo) {
      std::cerr << "Already set 'to' to " << _to << ". Changing value."
                << std::endl;
    }
    _to = t;
    setTo = true;
  }

  /// Set the rating (1 - 5)
  void rating(int i) {
    if (i < 1 || i > 5) {
      std::cerr << "Rating must be between 1 - 5 inclusive" << std::endl;
      return;
    }
    if (_rating != -1) {
      std::cerr << "Already rated " << _rating << ". Changing value."
                << std::endl;
    }
    _rating = i;
  }

  /// Set the comment
  void comment(const char *comment) {
    if (_comment) {
      std::cerr << "Already commented " << _comment << ". Changing value."
                << std::endl;
      delete[] _comment;
    }
    _comment = new char[strlen(comment) + 1];
    strcpy(_comment, comment);
  }

  /// Add photo
  void photoAdd(SimpleString &&uri) {
    if (!std::regex_match(uri.get(), path)) {
      std::cerr << uri << " is not a valid path to a photo." << std::endl;
      return;
    }
    _photos.append(std::move(uri));
  }

  /// Remove photo
  void photoDelete(SimpleString &&uri) {
    auto res = _photos.remove(std::move(uri));
    if (res == -1) {
      std::cerr << "This photo does not exists." << std::endl;
    }
  }

  /// Print a list of the uploaded photos to stdout
  void photoShow() {
    std::cout << "Photos uploaded: " << std::endl;
    _photos.foreach (
        [](SimpleString const &ss) { std::cout << ss << std::endl; });
  }

  /// Set destination
  void destination(const char *destination) {
    if (_destination) {
      std::cerr << "Already chose " << _destination
                << " as destination. Changing value." << std::endl;
      delete[] _destination;
    }
    _destination = new char[strlen(destination) + 1];
    strcpy(_destination, destination);
  }

  /// Reset all values to their default state
  void clear() {
    setFrom = false;
    setTo = false;
    if (_destination) {
      delete[] _destination;
      _destination = nullptr;
    }
    if (_comment) {
      delete[] _comment;
      _comment = nullptr;
    }
    _rating = -1;
    _photos.clear();
  }
};
} // namespace Travel

#endif // TRAVELLERAPP_VISITBUILDER_HPP
