#include "VisitInterpreter.hpp"

namespace Travel {

bool VisitInterpreter::general(VisitBuilder &vb) {
  switch (next().t) {
  case TokenType::FROM:
    from(vb);
    break;
  case TokenType::TO:
    to(vb);
    break;
  case TokenType::COMMENT:
    comment(vb);
    break;
  case TokenType::RATE:
    rate(vb);
    break;
  case TokenType::HELP:
    // TODO: Maybe filter the commands so only the right ones are
    // shown?
    CommandList::getCommandList()->printCommandsWithDescription();
    break;
  case TokenType::CANCEL:
    vb.clear();
  case TokenType::SAVE:
    return false;
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
  return true;
}

void VisitInterpreter::from(VisitBuilder &vb) {
  auto from = consume(TokenType::DATE, "Expected Date");
  vb.from(from.lexeme);
}

void VisitInterpreter::to(VisitBuilder &vb) {
  auto to = consume(TokenType::DATE, "Expected Date");
  vb.to(to.lexeme);
}

void VisitInterpreter::rate(VisitBuilder &vb) {
  auto rating = consume(TokenType::NUMBER, "Expected Number(1 - 5)");
  vb.rating(atoi(rating.lexeme));
}

void VisitInterpreter::comment(VisitBuilder &vb) {
  auto comment = consume(TokenType::STRING, "Expected Comment");
  vb.comment(comment.lexeme);
}

void VisitInterpreter::photo(VisitBuilder &vb) {
  if (isAtEnd())
    throw ReachedEndOfStreamException();

  switch (next().t) {
  case TokenType::UPLOAD:
    photoUpload(vb);
    break;
  case TokenType::DELETE:
    photoDelete(vb);
    break;
  case TokenType::SHOW:
    photoShow(vb);
    break;
  case TokenType::EOF_T:
    std::cout << "Photo is a command prefix. It doesn't do anything"
                 " by itself."
              << std::endl;
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
  default:
    errorflag = true;
    std::cerr << "(Line " << prev().line << ") "
              << "Unexpected String -> " << prev().lexeme << std::endl;
    break;
  }
}

void VisitInterpreter::photoDelete(VisitBuilder &vb) {
  auto uri = consume(TokenType::STRING, "Expected path to photo");
  std::cout << "Deleting " << uri.lexeme << std::endl;
  vb.photoDelete(uri.lexeme);
}

void VisitInterpreter::photoShow(VisitBuilder &vb) { vb.photoShow(); }

void VisitInterpreter::photoUpload(VisitBuilder &vb) {
  auto uri = consume(TokenType::STRING, "Expected path to photo");
  std::cout << "Uploading " << uri.lexeme << std::endl;
  vb.photoAdd(uri.lexeme);
}

bool VisitInterpreter::parse(VisitBuilder &vb) {
  errorflag = false;
  // vb.destination(destination);

  while (!isAtEnd()) {
    try {
      if (matches(TokenType::PHOTO)) {
        LOG(INFO, "Photo prefix");
        next();
        photo(vb);
      } else {
        LOG(INFO, "General");
        if (!general(vb))
          return false;
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
