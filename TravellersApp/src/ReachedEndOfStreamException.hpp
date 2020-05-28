#ifndef TRAVELLERAPP_REACHEDENDOFSTREAMEXCEPTION_HPP
#define TRAVELLERAPP_REACHEDENDOFSTREAMEXCEPTION_HPP

#include <iostream>
#include <cstring>
#include <exception>

using namespace std;

class ReachedEndOfStreamException : public std::exception {
private:
    static const char *DEFAULT_MESSAGE;


    char *msg;

public:
    virtual const char *what() const throw() {
        return msg;
    }


    ReachedEndOfStreamException(const char *_msg) {
        msg = new char[strlen(_msg) + 1];
        strcpy(msg, _msg);
    }

    ~ReachedEndOfStreamException() {
        delete[] msg;
    }

    ReachedEndOfStreamException() : ReachedEndOfStreamException(DEFAULT_MESSAGE) {}

};


#endif //TRAVELLERAPP_REACHEDENDOFSTREAMEXCEPTION_HPP
