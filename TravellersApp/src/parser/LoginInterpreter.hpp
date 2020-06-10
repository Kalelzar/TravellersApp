//
// Created by Kalelzar on 25/03/2020.
//

#ifndef TRAVELLERAPP_LOGININTERPRETER_HPP
#define TRAVELLERAPP_LOGININTERPRETER_HPP

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

/// Interprets all commands in the {@link ScannerContext::ATOPEN ATOPEN}
/// ScannerContext.
class LoginInterpreter : public Interpreter<TravelState> {

private:
  /// Interprets the login command
  bool login(TravelState &state);
  /// Interprets the register command
  bool reg(TravelState &state);

public:
  /// Parses the provided tokens into the given {@link Travel::TravelState}
  bool parse(TravelState &state) override;
  /// Inherits the constructors of Interpreter
  using Interpreter::Interpreter;
};

} // namespace Travel
#endif // TRAVELLERAPP_LOGININTERPRETER_HPP
