//
// Created by Kalelzar on 25/03/2020.
//

#ifndef TRAVELLERAPP_HTLINTERPRETER_HPP
#define TRAVELLERAPP_HTLINTERPRETER_HPP

#include "Token.hpp"
#include "../ReachedEndOfStreamException.hpp"
#include "../WrongTokenTypeException.hpp"
#include "../InvalidArgumentException.hpp"
#include "../TravelState.hpp"
#include "CommandList.hpp"
#include <cstring>
#include <iostream>
#include <cstdlib>

namespace Travel {

    class HTLInterpreter {
    private:
        std::shared_ptr<ArrayList<Token>> tokens;
        unsigned index = 0;


        bool isAtEnd() const;
        Token next();
        Token peek() const;
        void error(int line, const char* msg) const;
        Token consume(TokenType type, const char* msg);
        bool matches(TokenType type) const;
        Token prev() const;

    public:

        bool errorflag = false;

        explicit HTLInterpreter(std::shared_ptr<ArrayList<Token>> _tokens)
            : tokens(_tokens) {}


        bool parse(TravelState &state);

    };

}
#endif //TRAVELLERAPP_HTLINTERPRETER_HPP
