#ifndef TRAVELLERAPP_INTERPRETER_HPP
#define TRAVELLERAPP_INTERPRETER_HPP

#include "../InvalidArgumentException.hpp"
#include "../ReachedEndOfStreamException.hpp"
#include "../WrongTokenTypeException.hpp"
#include "CommandList.hpp"
#include "Token.hpp"
#include <cstdlib>
#include <cstring>
#include <iostream>

/// A interface for command-line interpreters.
template <typename Controller> class Interpreter {

protected:
  std::shared_ptr<ArrayList<Token>> tokens;
  unsigned index = 0;

  /// Has the interpreter consumed all the tokens
  /// it was given
  bool isAtEnd() const { return index >= tokens->length(); }

  /// Pop the next token.
  Token next() {
    if (isAtEnd())
      throw ReachedEndOfStreamException{};
    else {
      index++;
      return tokens->get(index - 1);
    }
  }
  /// Peek at the next token
  Token peek() const {
    if (isAtEnd())
      throw ReachedEndOfStreamException{};
    else {
      return tokens->get(index);
    }
  }

  /// Throw an error at the given line with the given message.
  ///@throws WrongTokenTypeException
  void error(int line, const char *msg) const {
    char sline[32];
    sprintf(sline, "%d", line);
    char *nmsg = new char[6 + strlen(sline) + 3 + strlen(msg) + 1];
    strcpy(nmsg, "Line: ");
    strcpy(nmsg + 6, sline);
    strcpy(nmsg + 6 + strlen(sline), " - ");
    strcpy(nmsg + 6 + strlen(sline) + 3, msg);
    // std::cerr<<nmsg<<std::endl;
    WrongTokenTypeException e(nmsg);
    delete[] nmsg;
    throw e;
  }

  /// If the next token is of the provided {@link TokenType}
  /// return the next token else throw an error with the
  /// provided message.
  /// @throws WrongTokenTypeException
  Token consume(TokenType type, const char *msg) {
    if (!matches(type)) {
      error(peek().line, msg);
    } else {
      return next();
    }
    // Never used. Just here to suppress a compiler warning
    return {TokenType::EOF_T, "", 0};
  }

  /// Is the next token of the provided {@link TokenType}
  bool matches(TokenType type) const { return peek().t == type; }

  /// Return the previous token
  Token prev() const { return tokens->get(index - 1); }

public:
  bool errorflag = false;
  /// Parse the provided tokens into the given controller
  virtual bool parse(Controller &state) = 0;

  explicit Interpreter(std::shared_ptr<ArrayList<Token>> _tokens)
      : tokens(_tokens) {}
};
#endif // TRAVELLERAPP_INTERPRETER_HPP
