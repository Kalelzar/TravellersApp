#ifndef TRAVELLERAPP_NOVALUEEXCEPTION_HPP
#define TRAVELLERAPP_NOVALUEEXCEPTION_HPP

#include <iostream>
#include <cstring>
#include <exception>

using namespace std;

class NoValueException : public std::exception {
private:
    static const char *DEFAULT_MESSAGE;

    char *msg;

public:

    virtual const char *what() const throw() {
        return msg;
    }

    NoValueException(const char *_msg) {
        msg = new char[strlen(_msg) + 1];
        strcpy(msg, _msg);
    }

    ~NoValueException() {
        delete[] msg;
    }

    NoValueException() : NoValueException(DEFAULT_MESSAGE) {}

};


#endif //TRAVELLERAPP_NOVALUEEXCEPTION_HPP
