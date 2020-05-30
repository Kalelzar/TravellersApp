#ifndef TRAVELLERAPP_INTERPRETER_HPP
#define TRAVELLERAPP_INTERPRETER_HPP

#include "Token.hpp"
#include "../ReachedEndOfStreamException.hpp"
#include "../WrongTokenTypeException.hpp"
#include "../InvalidArgumentException.hpp"
#include "CommandList.hpp"
#include <cstring>
#include <iostream>
#include <cstdlib>

template <typename Controller>
class Interpreter {

protected:
    std::shared_ptr<ArrayList<Token>> tokens;
    unsigned index = 0;

    bool isAtEnd() const {
        return index >= tokens->length();
    }


    Token next(){
        if(isAtEnd()) throw ReachedEndOfStreamException{};
        else {
            index++;
            return tokens->get(index-1);
        }
    }

    Token peek() const {
        if(isAtEnd()) throw ReachedEndOfStreamException{};
        else {
            return tokens->get(index);
        }
    }

    void error(int line, const char* msg) const {
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

    Token consume(TokenType type, const char* msg){
        if(!matches(type)){
            error(peek().line, msg);
        }else{
            return next();
        }
        // Never used. Just here to suppress a compiler warning
        return {TokenType::EOF_T, "", 0};
    }

    bool matches(TokenType type) const {
        return peek().t == type;
    }

    Token prev() const {
        return tokens->get(index-1);
    }


public:
    bool errorflag = false;
    virtual bool parse(Controller &state) = 0;

    explicit Interpreter(std::shared_ptr<ArrayList<Token>> _tokens)
        : tokens(_tokens) {}
};
#endif //TRAVELLERAPP_INTERPRETER_HPP
