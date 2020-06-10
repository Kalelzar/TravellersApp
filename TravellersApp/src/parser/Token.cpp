#include "Token.hpp"

#include "../debug.hpp"
#include <cstring>
#include <iostream>

void Token::create(TokenType _t, const char *_lexeme, int _line) {
  // LOG(INFO, "Create token { " << _t << ", " << _lexeme << ", "
  //        << _line <<" }");
  t = _t;
  line = _line;
  int length = strlen(_lexeme);
  lexeme = new char[length + 1];
  strncpy(lexeme, _lexeme, length);
  lexeme[length] = '\0';
}

void Token::copy(Token const &other) {
  // LOG(INFO, "Copy token");
  create(other.t, other.lexeme, other.line);
}

Token::Token(TokenType _t, const char *_lexeme, int _line) {
  create(_t, _lexeme, _line);
}

Token::~Token() {
  // LOG(INFO, "Destructor called.");
  delete[] lexeme;
}

Token::Token() { create(TokenType::EOF_T, "", 0); }

Token::Token(const Token &other) {
  if (this != &other)
    copy(other);
}

Token &Token::operator=(Token const &other) {
  if (this != &other) {
    delete[] lexeme;
    copy(other);
  }
  return *this;
}

bool Token::operator==(Token const &other) const {
  return t == other.t && line == other.line &&
         strcmp(lexeme, other.lexeme) == 0;
}
