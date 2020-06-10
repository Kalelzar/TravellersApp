//
// Created by Kalelzar on 25/03/2020.
//

#ifndef TRAVELLERAPP_VISITINTERPRETER_HPP
#define TRAVELLERAPP_VISITINTERPRETER_HPP

#include "../InvalidArgumentException.hpp"
#include "../ReachedEndOfStreamException.hpp"
#include "../TravelState.hpp"
#include "../VisitBuilder.hpp"
#include "../WrongTokenTypeException.hpp"
#include "CommandList.hpp"
#include "Interpreter.hpp"
#include "Token.hpp"
#include <cstdlib>
#include <cstring>
#include <iostream>

namespace Travel {

/**
 * An interpreter for the visit mode started by
 * the visit <destination> command.
 */
class VisitInterpreter : public Interpreter<VisitBuilder> {

private:
  /// Interprets all commands not part of a prefix.
  /// (from, to, rate, comment)
  bool general(VisitBuilder &vb);
  /// Interprets the from command.
  void from(VisitBuilder &vb);
  /// Interprets the to command.
  void to(VisitBuilder &vb);
  /// Interprets the rate command.
  void rate(VisitBuilder &vb);
  /// Interprets the comment command
  void comment(VisitBuilder &vb);

  /// Interprets all commands that are part of the photo prefix.
  ///(delete, show, upload)
  void photo(VisitBuilder &vb);
  /// Interprets the photo delete command.
  void photoDelete(VisitBuilder &vb);
  /// Interprets the photo show command.
  void photoShow(VisitBuilder &vb);
  /// Interprets the photo upload command.
  void photoUpload(VisitBuilder &vb);

public:
  /// Parses the provided tokens into the given {@link Travel::VisitBuilder}
  bool parse(VisitBuilder &state) override;

  /// Inherits the constructors of Interpreter
  using Interpreter::Interpreter;
};

} // namespace Travel
#endif // TRAVELLERAPP_VISITINTERPRETER_HPP
