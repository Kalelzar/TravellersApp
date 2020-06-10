#include "LoginInterpreter.hpp"

namespace Travel {

bool LoginInterpreter::login(TravelState &state) {
  auto user = consume(TokenType::STRING, "Expected username");
  if (matches(TokenType::STRING) || matches(TokenType::NUMBER)) {
    auto password = next();
    return state.login(user.lexeme, password.lexeme);
  } else {
    error(peek().line, "Expected password");
    return false;
  }
}

bool LoginInterpreter::reg(TravelState &state) {
  auto email = consume(TokenType::STRING, "Expected email");
  auto user = consume(TokenType::STRING, "Expected username");
  if (matches(TokenType::STRING) || matches(TokenType::NUMBER)) {
    auto password = next();
    return state.uregister(email.lexeme, user.lexeme, password.lexeme);
  } else {
    error(peek().line, "Expected password");
    return false;
  }
}

bool LoginInterpreter::parse(TravelState &state) {
  errorflag = false;
  while (!isAtEnd()) {
    try {
      switch (next().t) {
      case TokenType::LOGIN:
        return !login(state);
        break;
      case TokenType::REGISTER:
        return !reg(state);
        break;
      case TokenType::NUMBER:
        errorflag = true;
        std::cerr << "(Line " << prev().line << ") "
                  << "Unexpected Number -> " << prev().lexeme << std::endl;
        break;
      case TokenType::DATE:
        errorflag = true;
        std::cerr << "(Line " << prev().line << ") "
                  << "Unexpected Date -> " << prev().lexeme << std::endl;
        break;
      case TokenType::ERROR:
        errorflag = true;
        std::cerr << "(Line " << prev().line << ") "
                  << "Parsing error: " << prev().lexeme << std::endl;
        break;
      case TokenType::EOF_T:
        return true;
        break;
      default:
        errorflag = true;
        std::cerr << "(Line " << prev().line << ") "
                  << "Unexpected String -> " << prev().lexeme << std::endl;
        break;
      }
    } catch (NoValueException &e) {
      errorflag = true;
      std::cerr << e.what() << std::endl;
    } catch (InvalidArgumentException &e) {
      errorflag = true;
      std::cerr << e.what() << std::endl;
    } catch (ReachedEndOfStreamException &e) {
      errorflag = true;
      std::cerr << e.what() << std::endl;
      return true;
    } catch (WrongTokenTypeException &e) {
      errorflag = true;
      std::cerr << e.what() << std::endl;
      while (peek().t == TokenType::STRING || peek().t == TokenType::NUMBER ||
             peek().t == TokenType::DATE || peek().t == TokenType::ERROR) {
        Token n = next();
        if (n.t == TokenType::ERROR) {
          std::cerr << "Error: " << n.lexeme << std::endl;
        }
      }
    }
  }
  return true;
}

} // namespace Travel
