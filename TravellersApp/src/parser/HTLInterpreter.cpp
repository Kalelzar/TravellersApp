//
// Created by Kalelzar on 25/03/2020.
//

#include "HTLInterpreter.hpp"

namespace Travel {

    bool HTLInterpreter::isAtEnd() const {
        return index >= tokens->length();
    }


    Token HTLInterpreter::next(){
        if(isAtEnd()) throw ReachedEndOfStreamException{};
        else {
            index++;
            return tokens->get(index-1);
        }
    }

    Token HTLInterpreter::peek() const {
        if(isAtEnd()) throw ReachedEndOfStreamException{};
        else {
            return tokens->get(index);
        }
    }

    void HTLInterpreter::error(int line, const char* msg) const {
        char sline[32];
        sprintf(sline, "%d", line);
        char* nmsg = new char[6 + strlen(sline)+ 3 + strlen(msg) + 1];
        strcpy(nmsg, "Line: ");
        strcpy(nmsg+6, sline);
        strcpy(nmsg+6+strlen(sline), " - ");
        strcpy(nmsg+6+strlen(sline)+3, msg);
        //std::cerr<<nmsg<<std::endl;
        WrongTokenTypeException e(nmsg);
        delete[] nmsg;
        throw e;
    }

    Token HTLInterpreter::consume(TokenType type, const char* msg){
        if(!matches(type)){
            error(peek().line, msg);
        }else{
            return next();
        }
        // Never used. Just here to suppress a compiler warning
        return {TokenType::EOF_T, "", 0};
    }

    bool HTLInterpreter::matches(TokenType type) const {
        return peek().t == type;
    }

    Token HTLInterpreter::prev() const {
        return tokens->get(index-1);
    }

    bool HTLInterpreter::parse(TravelState& state) {
        errorflag = false;
        while(!isAtEnd()){
            try{
                switch(next().t){
                case TokenType::HELP:
                    CommandList::getCommandList()->printCommandsWithDescription ();
                    break;
                case TokenType::EXIT:
                    return false;
                    break;
                case TokenType::STRING:
                    errorflag = true;
                    std::cerr<<"(Line "<<prev().line<<") "<<"Unexpected String -> " << prev().lexeme <<std::endl;
                    break;
                case TokenType::NUMBER:
                    errorflag = true;
                    std::cerr<<"(Line "<<prev().line<<") "<<"Unexpected Number -> " << prev().lexeme <<std::endl;
                    break;
                case TokenType::DATE:
                    errorflag = true;
                    std::cerr<<"(Line "<<prev().line<<") "<<"Unexpected Date -> " << prev().lexeme <<std::endl;
                    break;
                case TokenType::ERROR:
                    errorflag = true;
                    std::cerr<<"(Line "<<prev().line<<") "<<"Parsing error: " << prev().lexeme << std::endl;
                    break;
                case TokenType::EOF_T:
                    return true;
                    break;
                }
            }catch(NoValueException& e){
                errorflag = true;
                std::cerr<<e.what()<<std::endl;
            }catch(InvalidArgumentException& e){
                errorflag = true;
                std::cerr<<e.what()<<std::endl;
            }catch(ReachedEndOfStreamException& e){
                errorflag = true;
                std::cerr<<e.what()<<std::endl;
                return true;
            }catch(WrongTokenTypeException& e){
                errorflag = true;
                std::cerr<<e.what()<<std::endl;
                while(peek().t == TokenType::STRING ||
                      peek().t == TokenType::NUMBER ||
                      peek().t == TokenType::DATE ||
                      peek().t == TokenType::ERROR){
                    Token n = next();
                    if(n.t == TokenType::ERROR){
                        std::cerr<<"Error: "<<n.lexeme<<std::endl;
                    }
                }
            }
        }
        return true;
    }

}
