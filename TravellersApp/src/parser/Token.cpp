#include "Token.hpp"

#include<cstring>
#include<iostream>
#include "../debug.hpp"

// Allow TokenType to be directly passed to ostream
// It's ugly but it is what it is.
static std::ostream& operator<<(std::ostream& out, TokenType const& type){
    switch(type){
    case TokenType::SAVE:
        out<<"(SAVE)";
        break;
    case TokenType::HELP:
        out<<"(HELP)";
        break;
    case TokenType::EXIT:
        out<<"(EXIT)";
        break;
    case TokenType::STRING:
        out<<"(STRING)";
        break;
    case TokenType::NUMBER:
        out<<"(NUMBER)";
        break;
    case TokenType::DATE:
        out<<"(DATE)";
        break;
    case TokenType::ERROR:
        out<<"(ERROR)";
        break;
    case TokenType::EOF_T:
        out<<"(EOF)";
        break;
    }
    return out;
}



void Token::create(TokenType _t, const char *_lexeme, int _line) {
    LOG(INFO, "Create token { " << _t << ", " << _lexeme << ", "
        << _line <<" }");
    t = _t;
    line = _line;
    int length = strlen(_lexeme);
    lexeme = new char[length + 1];
    strncpy(lexeme, _lexeme, length);
    lexeme[length] = '\0';
}

void Token::copy(Token const &other) {
    LOG(INFO, "Copy token");
    create(other.t, other.lexeme,  other.line);
}


Token::Token(TokenType _t, const char *_lexeme, int _line) {
    create(_t, _lexeme, _line);
}

Token::~Token() {
    LOG(INFO, "Destructor called.");
    delete[] lexeme;
}

Token::Token() {
    create(TokenType::EOF_T, "", 0);
}

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
    return t == other.t &&
        line == other.line && strcmp(lexeme, other.lexeme) == 0;
}

static std::ostream& operator<<(std::ostream &out, Token const &token) {
    out << token.t << " " << token.line << ": " << token.lexeme;
    return out;
}
