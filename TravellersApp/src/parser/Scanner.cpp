//
// Created by Kalelzar on 25/03/2020.
//

#include "Scanner.hpp"

Travel::Scanner::Scanner(ScannerContext sc, std::istream *input, CommandList const &cl)
{
    this->sc = sc;
    this->input = input;
    this->cl = cl;
}


std::shared_ptr<ArrayList<Token>> Travel::Scanner::scan(){
    error = false;
    std::shared_ptr<ArrayList<Token>> list = std::make_shared<ArrayList<Token>>();
    while (!input->eof()) {
        char sline[513];
        input->getline(sline, 512);
        if (input->eof()) break;
        list->appendAll(*scanLine(sline));
        line++;
    }
    list->append(Token{TokenType::EOF_T, "", line});
    return list;
}

std::shared_ptr<ArrayList<Token>> Travel::Scanner::scanNext(){
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
    std::shared_ptr<ArrayList<Token>> list = std::make_shared<ArrayList<Token>>();
    int linelen = strlen(line);

    int index = 0;
    while (index < linelen) {
        if (line[index] == '"') {

            index++;
            int start = index;
            while (line[index] != '"') {

                index++;
                if (index >= linelen) {

                    const char errorMsg[] = "Unterminated string.";
                    list->append({TokenType::ERROR,
                                  errorMsg, this->line});
                    error = true;
                    return list;
                }
            }

            if (index + 1 != linelen && line[index + 1] != ' ') {
                const char errorMsg[] = "Expected whitespace or EOL after string.";
                list->append({TokenType::ERROR,
                              errorMsg, this->line});
                error = true;
                return list;
            }

            char* lexeme = new char[index - start + 1];
            strncpy(lexeme, line + start, index - start);
            lexeme[index - start] = '\0';
            list->append({TokenType::STRING, lexeme, this->line});
			delete[] lexeme;
            index++;
        } else if (isdigit(line[index]) ||
                   (line[index] == '-' && index + 1 < linelen && isdigit(line[index + 1]))) {

            int start = index;
            if(line[index] == '-') index++;
            TokenType tt = TokenType::NUMBER;
            while (isdigit(line[index])) {

                index++;
                if (line[index] == '-' && tt == TokenType::NUMBER) {

                    tt = TokenType::NUMERIC_RANGE;
                    index++;
                } else if (line[index] == '-' && tt == TokenType::NUMERIC_RANGE) {
                    tt = TokenType::DATE;
                    index++;
                } else if (line[index] == '-') {
                    const char errorMsg[] = "Malformed date.";
                    list->append({TokenType::ERROR,
                                  errorMsg, this->line});
                    error = true;
                    return list;
                }
            }


            if (line[index - 1] == '-' || (index != linelen && line[index] != ' ')) {
                const char errorMsg[] = "Malformed number or numeric range.";
                list->append({TokenType::ERROR,
                              errorMsg, this->line});
                error = true;
                return list;
            }
			char* lexeme = new char[index - start + 1];
            strncpy(lexeme, line + start, index - start);
            lexeme[index - start] = '\0';
            list->append({tt, lexeme, this->line});
			delete[] lexeme;
            index++;
        } else if (line[index] == ' ') {
            index++;
        } else{
            int start = index;
            index++;
            while (index < linelen && line[index] != ' ') {
                index++;
            }

            if (index != linelen && line[index] != ' ') {
                const char errorMsg[] = "Expected whitespace or EOL after identifier.";
                list->append({TokenType::ERROR,
                              errorMsg, this->line});
                error = true;
                return list;
            }

			char* lexeme = new char[index - start + 1];
            strncpy(lexeme, line + start, index - start);
            lexeme[index - start] = '\0';
            std::shared_ptr<Nullable<TokenType>> tt = cl.tokenFor(lexeme);
            if (tt->isDefined())
            {
                ScannerContext tctx = cl.contextFor(tt->get())
                    ->getOrElse(ScannerContext::UNDEFINED);
                if(tctx == ScannerContext::ALL || tctx == sc || sc == ScannerContext::ALL){
                    list->append({tt->get(), lexeme, this->line});
                }else{
                    const char errorMsg[] = "Command used in the wrong context.";
                    list->append({TokenType::ERROR,
                                  errorMsg, this->line});
                    error = true;
					delete[] lexeme;
                    return list;
                }
            } else {
                list->append({TokenType::STRING,
                              lexeme, this->line});
            }
			delete[] lexeme;
            index++;
        }
    }

    return list;
}
