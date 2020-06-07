#include "Scanner.hpp"
#include "../debug.hpp"

Travel::Scanner::Scanner(ScannerContext sc, std::istream *input, CommandList* cl)
{
    LOG(INFO, "Initialize scanner.");
    this->sc = sc;
    this->input = input;
    this->cl = cl;
}


std::shared_ptr<ArrayList<Token>> Travel::Scanner::scan(){
    LOG(INFO, "Scanning..");
    error = false;
    std::shared_ptr<ArrayList<Token>> list = std::make_shared<ArrayList<Token>>();
    while (!input->eof()) {
        LOG(INFO, "Reading line from input.");
        char sline[513];
        input->getline(sline, 512);
        if (input->eof()) break;
        list->appendAll(*scanLine(sline));
        line++;
    }
    LOG(VERBOSE, "Reached end of input");
    list->append(Token{TokenType::EOF_T, "", line});
    return list;
}

std::shared_ptr<ArrayList<Token>> Travel::Scanner::scanNext(){
    LOG(INFO, "Scanning next line...");
    error = false;
    std::shared_ptr<ArrayList<Token>> list = std::make_shared<ArrayList<Token>>();
    char sline[513];
    input->getline(sline, 512);
    if (input->eof()) {
        list->append(Token{TokenType::EOF_T, "", line});
        return list;
    }
    list->appendAll(*scanLine(sline));
    line++;
    list->append(Token{TokenType::EOF_T, "", line});
    return list;
}

std::shared_ptr<ArrayList<Token>> Travel::Scanner::scanLine(const char *line){
    LOG(VERBOSE, "Read: ");
    LOG(VERBOSE, line);
    LOG(VERBOSE, "Tokenizing line...");
    std::shared_ptr<ArrayList<Token>> list =
        std::make_shared<ArrayList<Token>>();
    int linelen = strlen(line);

    int index = 0;
    while (index < linelen) {
        LOG(INFO, "======================================================");
        if (line[index] == '"') {
            LOG(INFO, "Found beginning of string literal.");
            index++;
            int start = index;
            while (line[index] != '"') {

                index++;
                if (index >= linelen) {
                    LOG(ERROR, "Unterminated string.");
                    const char errorMsg[] = "Unterminated string.";
                    list->append({TokenType::ERROR,
                                  errorMsg, this->line});
                    error = true;
                    return list;
                }
            }

            LOG(VERBOSE, "Reached end of string");

            if (index + 1 != linelen && line[index + 1] != ' ') {
                LOG(ERROR, "Expected whitespace or EOL after string.");
                const char errorMsg[] =
                    "Expected whitespace or EOL after string.";
                list->append({TokenType::ERROR,
                              errorMsg, this->line});
                error = true;
                return list;
            }

            char* lexeme = new char[index - start + 1];
            LOG(VERBOSE, "Copying string... ");
            strncpy(lexeme, line + start, index - start);
            lexeme[index - start] = '\0';
            LOG(VERBOSE, "Generating token...");
            list->append({TokenType::STRING, lexeme, this->line});
            delete[] lexeme;
            LOG(INFO, "String token generated successfully");
            index++;
        } else if (isdigit(line[index]) ||
                   (line[index] == '-' && index + 1 < linelen &&
                    isdigit(line[index + 1]))) {
            LOG(INFO, "Found beginning of number");
            int start = index;
            bool maybeDate = false;
            if(line[index] == '-') index++;
            TokenType tt = TokenType::NUMBER;
            while (isdigit(line[index])) {

                index++;
                if (line[index] == '-' &&
                    tt == TokenType::NUMBER
                    && index+1 < linelen
                    && isdigit(line[index+1])) {
                    tt = TokenType::DATE;
                    index++;
                    maybeDate = true;
                }else if(line[index] == '-' &&
                         maybeDate &&
                         index+1 < linelen &&
                         isdigit(line[index+1])){
                    index++;
                    maybeDate = false;
                }else if(line[index] == '-'){
                    index++;
                    tt=TokenType::STRING;
                    break;
                }
            }

            LOG(VERBOSE, "Reached end of number");

            while (index < linelen && line[index] != ' ') {
                tt=TokenType::STRING;
                index++;
            }

            char* lexeme = new char[index - start + 1];
            LOG(VERBOSE, "Copying number...");
            strncpy(lexeme, line + start, index - start);
            lexeme[index - start] = '\0';
            LOG(VERBOSE, "Generating token...");
            list->append({tt, lexeme, this->line});
            delete[] lexeme;
            LOG(INFO, "Number token generated successfully");
            index++;
        } else if (line[index] == ' ') {
            LOG(VERBOSE, "Skipping whitespace");
            index++;
        } else{
            LOG(INFO, "Found either a reserved keyword or a single-word string");
            int start = index;
            index++;
            while (index < linelen && line[index] != ' ') {
                index++;
            }
            LOG(VERBOSE, "Found end of identifier");

            if (index != linelen && line[index] != ' ') {
                LOG(ERROR, "Expected whitespace or EOL after identifier.");
                const char errorMsg[] =
                    "Expected whitespace or EOL after identifier.";
                list->append({TokenType::ERROR,
                              errorMsg, this->line});
                error = true;
                return list;
            }

            char* lexeme = new char[index - start + 1];
            LOG(VERBOSE, "Copying identifier...");
            strncpy(lexeme, line + start, index - start);
            lexeme[index - start] = '\0';
            LOG(INFO, "Checking if identifier is a reserved keyword...");
            std::shared_ptr<Nullable<TokenType>> tt = cl->tokenFor(lexeme);
            if (tt->isDefined())
            {
                LOG(INFO, "Found reserved keyword.");
                LOG(VERBOSE, "Acquiring Scanner Context for keyword.");
                ScannerContext tctx = cl->contextFor(tt->get())
                    ->getOrElse(ScannerContext::UNDEFINED);
                if(tctx == ScannerContext::ALL || tctx == sc || sc == ScannerContext::ALL){
                    LOG(VERBOSE, "Keyword used in proper scanner context.");
                    LOG(VERBOSE, "Generating token...");
                    list->append({tt->get(), lexeme, this->line});
                    LOG(INFO, "Keyword token generated successfully");
                }else{
                    LOG(ERROR, "Command used in the wrong context.");
                    const char errorMsg[] = "Command used in the wrong context.";
                    list->append({TokenType::ERROR,
                                  errorMsg, this->line});
                    error = true;
                    delete[] lexeme;
                    return list;
                }
            } else {
                LOG(INFO, "Not a reserved keyword.");
                LOG(VERBOSE, "Generating token...");
                list->append({TokenType::STRING,
                              lexeme, this->line});
                LOG(INFO, "String token generated successfully");
            }
            delete[] lexeme;
            index++;
        }
    }
    LOG(INFO, "Line tokenized...");
    return list;
}
