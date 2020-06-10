#ifndef TRAVELLERAPP_VISITBUILDER_HPP
#define TRAVELLERAPP_VISITBUILDER_HPP

#include "Date.hpp"
#include "SimpleString.hpp"
#include "collection/ArrayList.hpp"

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
    if (setFrom) {
      std::cerr << "Already set 'from' to " << _from << ". Changing value."
                << std::endl;
    }
    _from = Travel::Date(fdate);
    setFrom = true;
  }

  /// Set the end of visit.
  /// TODO: Make sure from is before to
  void to(char *fdate) {
    if (setTo) {
      std::cerr << "Already set 'to' to " << _to << ". Changing value."
                << std::endl;
    }
    _to = Travel::Date(fdate);
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
  void photoAdd(SimpleString &&uri) { _photos.append(std::move(uri)); }

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
