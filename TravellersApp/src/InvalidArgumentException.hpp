#ifndef TRAVELLERAPP_INVALIDARGUMENTEXCEPTION_HPP
#define TRAVELLERAPP_INVALIDARGUMENTEXCEPTION_HPP

#include <iostream>
#include <cstring>
#include <exception>

class InvalidArgumentException : public std::exception {
private:
    static const char *DEFAULT_MESSAGE;

    char *msg;

public:
    InvalidArgumentException(const char *_msg) {
        msg = new char[strlen(_msg) + 1];
        strcpy(msg, _msg);
    }

    virtual const char *what() const throw() {
        return msg;
    }


    ~InvalidArgumentException() {
        delete[] msg;
    }

    InvalidArgumentException() : InvalidArgumentException(DEFAULT_MESSAGE) {}

};


#endif //TRAVELLERAPP_INVALIDARGUMENTEXCEPTION_HPP
