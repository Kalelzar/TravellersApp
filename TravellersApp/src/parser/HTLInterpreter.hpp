//
// Created by Kalelzar on 25/03/2020.
//

#ifndef TRAVELLERAPP_HTLINTERPRETER_HPP
#define TRAVELLERAPP_HTLINTERPRETER_HPP

#include "../InvalidArgumentException.hpp"
#include "../ReachedEndOfStreamException.hpp"
#include "../TravelState.hpp"
#include "../WrongTokenTypeException.hpp"
#include "CommandList.hpp"
#include "Interpreter.hpp"
#include "Token.hpp"
#include <cstdlib>
#include <cstring>
#include <iostream>

namespace Travel {

/**
 * An interpreter for general commands
 */
class HTLInterpreter : public Interpreter<TravelState> {

private:
  /// Interprets all commands not in a prefix
  bool general(TravelState &state);
  /// Interprets all commands in the friend prefix
  void cfriend(TravelState &state);
  /// Interprets all commands in the destination prefix
  void destination(TravelState &state);
  /// Starts visit mode
  void visit(TravelState &state);

  /// Interprets the friend add command
  void friendAdd(TravelState &state);
  /// Interprets the friend remove command
  void friendRemove(TravelState &state);
  /// Interprets the friend list command
  void friendList(TravelState &state);
  /// Interprets the friend visited command
  void friendVisited(TravelState &state);

  /// Interprets the destination exists command
  void destinationExists(TravelState &state);
  /// Interprets the destination rating command
  void destinationRating(TravelState &state);
  /// Interprets the destination comments command
  void destinationComments(TravelState &state);
  /// Interprets the destination visited command
  void destinationVisited(TravelState &state);
  /// Interprets the destination list command
  void destinationList(TravelState &state);

public:
  /// Parses the provided tokens into the given {@link Travel::TravelState}
  bool parse(TravelState &state) override;

  /// Inherits the constructors of Interpreter
  using Interpreter::Interpreter;
};

} // namespace Travel
#endif // TRAVELLERAPP_HTLINTERPRETER_HPP
