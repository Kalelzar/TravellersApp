//
// Created by Kalelzar on 25/03/2020.
//

#ifndef TRAVELLERAPP_SCANNERCONTEXT_HPP
#define TRAVELLERAPP_SCANNERCONTEXT_HPP

#include <iostream>

enum class ScannerContext {
    ATOPEN = 0,
    GENERAL,
    VISIT,
    ALL,
    UNDEFINED = -1
};

static std::ostream& operator<<(std::ostream& out, ScannerContext const& ctx){
    out<<"ScannerContext";
    return out;
}

#endif //TRAVELLERAPP_SCANNERCONTEXT_HPP
