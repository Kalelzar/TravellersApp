#ifndef TRAVELLERAPP_TOKEN_HPP
#define TRAVELLERAPP_TOKEN_HPP

#include <iostream>

/**
 * Describes the different tokens that {@link Travel::Scanner Scanner}
 * recognises.
 * Some may need to be provided via {@link Travel::CommandList CommandList}
 */
enum class TokenType {

  HELP, /**< Matches the <code>help</code> command. Console Only */
  EXIT, /**< Matches the <code>exit</code> command. Console Only */

  REGISTER,
  LOGIN,
  FRIEND,
  ADD,
  REMOVE,
  LIST,
  VISITED,

  VISIT,
  FROM,
  TO,
  RATE,
  COMMENT,
  PHOTO,
  UPLOAD,
  DELETE,
  SHOW,
  CANCEL,
  SAVE,

  DESTINATION,
  EXISTS,
  RATING,
  COMMENTS,

  // Types? I guess
  STRING, /**< Any single non-reserved word and everything between " " is
           considered a string.*/
  NUMBER, /**< A signed integer. */
  DATE,   /**< Date in the ISO 8601 format (year-month-day). */

  // Parser/Scanner info
  ERROR, /**< Stores information for errors encountered by the
          {@link Travel::Scanner Scanner}.*/
  EOF_T  /**< The end of the input stream */
};

/**
 * Generic token for use in scanners/parsers.
 * It stores the type of the token, it's original text representation
 * and the line it was found on.
 */
class Token {
private:
  /**
   * Convenience method for initializing a token
   */
  void create(TokenType _t, const char *_lexeme, int _line);

  /**
   * Convenience method for copying another token.
   * It simply calls {@link create} with the other token's fields
   */
  void copy(Token const &other);

public:
  /** The token type of this token */
  TokenType t;
  /** The original text representation of this token */
  char *lexeme;
  /** The line it was found on */
  int line;

  Token(TokenType _t, const char *_lexeme, int _line);

  /**
   * Frees the dynamic memory associated with the {@link lexeme}
   */
  ~Token();

  /**
   * Creates a default token.
   * Equivalent to <code> Token(TokenType::EOF_T, "", 0) </code>
   */
  Token();

  /**
   * Copy constructor.
   */
  Token(const Token &other);

  Token &operator=(Token const &other);

  /**
   * Tests for the equality of two tokens.
   * Two tokens are considered equal
   * when they are of the same type
   * have the same lexeme
   * and are found on the same line
   */
  bool operator==(Token const &other) const;
};

// Allow TokenType to be directly passed to ostream
// It's ugly but it is what it is.
static std::ostream &operator<<(std::ostream &out, TokenType const &type) {
  switch (type) {
  case TokenType::FRIEND:
    out << "(FRIEND)";
    break;
  case TokenType::SAVE:
    out << "(SAVE)";
    break;
  case TokenType::HELP:
    out << "(HELP)";
    break;
  case TokenType::EXIT:
    out << "(EXIT)";
    break;
  case TokenType::STRING:
    out << "(STRING)";
    break;
  case TokenType::NUMBER:
    out << "(NUMBER)";
    break;
  case TokenType::DATE:
    out << "(DATE)";
    break;
  case TokenType::ERROR:
    out << "(ERROR)";
    break;
  case TokenType::EOF_T:
    out << "(EOF)";
    break;
  }
  return out;
}

static std::ostream &operator<<(std::ostream &out, Token const &token) {
  out << token.t << " " << token.line << ": " << token.lexeme;
  return out;
}

#endif // TRAVELLERAPP_TOKEN_HPP
