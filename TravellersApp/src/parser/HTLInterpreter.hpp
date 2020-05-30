//
// Created by Kalelzar on 25/03/2020.
//

#ifndef TRAVELLERAPP_HTLINTERPRETER_HPP
#define TRAVELLERAPP_HTLINTERPRETER_HPP

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

    class HTLInterpreter : public Interpreter<TravelState>{

    private:

        bool general(TravelState& state);
        void cfriend(TravelState& state);
        void destination(TravelState& state);
        void visit(TravelState& state);


        void friendAdd(TravelState& state);
        void friendRemove(TravelState& state);
        void friendList(TravelState& state);
        void friendVisited(TravelState& state);

        void destinationExists(TravelState& state);
        void destinationRating(TravelState& state);
        void destinationComments(TravelState& state);
        void destinationVisited(TravelState& state);




    public:

        bool parse(TravelState &state) override;

        using Interpreter::Interpreter;


    };

}
#endif //TRAVELLERAPP_HTLINTERPRETER_HPP
