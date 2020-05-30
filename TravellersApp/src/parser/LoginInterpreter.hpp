//
// Created by Kalelzar on 25/03/2020.
//

#ifndef TRAVELLERAPP_LOGININTERPRETER_HPP
#define TRAVELLERAPP_LOGININTERPRETER_HPP

#include "Interpreter.hpp"
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

    class LoginInterpreter : public Interpreter<TravelState>{

    private:
        bool login(TravelState& state);
        bool reg(TravelState& state);





    public:

        bool parse(TravelState &state) override;

        using Interpreter::Interpreter;


    };

}
#endif //TRAVELLERAPP_LOGININTERPRETER_HPP
