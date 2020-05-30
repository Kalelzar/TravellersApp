//
// Created by Kalelzar on 25/03/2020.
//

#ifndef TRAVELLERAPP_VISITINTERPRETER_HPP
#define TRAVELLERAPP_VISITINTERPRETER_HPP

#include "Interpreter.hpp"
#include "Token.hpp"
#include "../ReachedEndOfStreamException.hpp"
#include "../WrongTokenTypeException.hpp"
#include "../InvalidArgumentException.hpp"
#include "../TravelState.hpp"
#include "../VisitBuilder.hpp"
#include "CommandList.hpp"
#include <cstring>
#include <iostream>
#include <cstdlib>

namespace Travel {

    class VisitInterpreter : public Interpreter<VisitBuilder>{

    private:
        bool general(VisitBuilder& vb);
        void from(VisitBuilder& vb);
        void to(VisitBuilder& vb);
        void rate(VisitBuilder& vb);
        void comment(VisitBuilder& vb);

        void photo(VisitBuilder& vb);
        void photoDelete(VisitBuilder& vb);
        void photoShow(VisitBuilder& vb);
        void photoUpload(VisitBuilder& vb);




    public:

        bool parse(VisitBuilder &state) override;

        using Interpreter::Interpreter;


    };

}
#endif //TRAVELLERAPP_VISITINTERPRETER_HPP
