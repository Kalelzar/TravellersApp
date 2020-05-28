#include "Token.hpp"

#include<cstring>



void Token::create(TokenType _t, const char *_lexeme, int _line) {
    t = _t;
    line = _line;
    int length = strlen(_lexeme);
    lexeme = new char[length + 1];
    strncpy(lexeme, _lexeme, length);
    lexeme[length] = '\0';
}

void Token::copy(Token const &other) {
    create(other.t, other.lexeme,  other.line);
}


Token::Token(TokenType _t, const char *_lexeme, int _line) {
    create(_t, _lexeme, _line);
}

Token::~Token() {
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

// Allow TokenType to be directly passed to ostream
// It's ugly but it is what it is.
static std::ostream& operator<<(std::ostream& out, TokenType const& type){
    switch(type){
    case TokenType::AVAILABILITY:
        out<<"(AVAILABILITY)";
    case TokenType::ADD:
        out<<"(ADD)";
        break;
    case TokenType::REMOVE:
        out<<"(REMOVE)";
        break;
    case TokenType::OPEN:
        out<<"(OPEN)";
        break;
    case TokenType::CLOSE:
        out<<"(CLOSE)";
        break;
    case TokenType::SAVE:
        out<<"(SAVE)";
        break;
    case TokenType::SAVE_AS:
        out<<"(SAVE AS)";
        break;
    case TokenType::HELP:
        out<<"(HELP)";
        break;
    case TokenType::EXIT:
        out<<"(EXIT)";
        break;
    case TokenType::CHECKIN:
        out<<"(CHECK IN)";
        break;
    case TokenType::CHECKOUT:
        out<<"(CHECK OUT)";
        break;
    case TokenType::REPORT:
        out<<"(REPORT)";
        break;
    case TokenType::FIND:
        out<<"(FIND)";
        break;
    case TokenType::FIND_F:
        out<<"(FIND!)";
        break;
    case TokenType::UNAVAILABLE:
        out<<"(UNAVAILABLE)";
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
    case TokenType::LIST:
        out<<"(LIST)";
        break;
    case TokenType::NUMERIC_RANGE:
        out<<"(NUMERIC RANGE)";
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

static std::ostream& operator<<(std::ostream &out, Token const &token) {
    out << token.t << " " << token.line << ": " << token.lexeme;
    return out;
}
