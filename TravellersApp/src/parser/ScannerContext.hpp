//
// Created by Kalelzar on 25/03/2020.
//

#ifndef TRAVELLERAPP_SCANNERCONTEXT_HPP
#define TRAVELLERAPP_SCANNERCONTEXT_HPP

#include <iostream>

enum class ScannerContext {
  ATOPEN = 0,    ///< For commands issued during the login prompt
  GENERAL,       ///< For commands not in any other category
  VISIT,         ///< For commands issued in visit mode
  ALL,           ///< All commands
  UNDEFINED = -1 ///< Errors
};

static std::ostream &operator<<(std::ostream &out, ScannerContext const &ctx) {
  out << "ScannerContext";
  return out;
}

#endif // TRAVELLERAPP_SCANNERCONTEXT_HPP
