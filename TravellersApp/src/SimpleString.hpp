#ifndef TRAVELLERAPP_SIMPLESTRING_HPP
#define TRAVELLERAPP_SIMPLESTRING_HPP

#include<cstring>
#include<iostream>
#include "debug.hpp"

class SimpleString {

private:
    char *inner = nullptr;
    size_t len = 0;

    void init(const char *str) {
        LOG(INFO, "Creating SimpleString instance.");
        len = strlen(str);
        inner = new char[len + 1];
        strcpy(inner, str);
    }

public:

    SimpleString() {
        inner = new char[1];
        inner[0] = '\0';
    }

    ~SimpleString() {
        LOG(INFO, "Destructor called.");
        delete[] inner;
    }

    SimpleString(SimpleString&& str){
        LOG(INFO, "Moving SimpleString");
        len = str.len;
        inner = str.inner;

        str.len = 0;
        str.inner = nullptr;
    }

    SimpleString &operator=(SimpleString&& str) {
        if (&str != inner) {
            LOG(INFO, "Move(assign) SimpleString");
            if(inner) delete[] inner;
            len = str.len;
            inner = str.inner;

            str.len = 0;
            str.inner = nullptr;
        }
        return *this;
    }


    SimpleString(const char (&&str)[]) {
        LOG(INFO, "Moving cstring into SimpleString");
        len = strlen(str);
        inner = const_cast<char*>(str);
    }

    SimpleString(const char* str) {
        LOG(INFO, "Copying SimpleString from cstring");
        init(str);
    }

    SimpleString(SimpleString const& str) {
        LOG(INFO, "Copying SimpleString");
        init(str.inner);
    }

    SimpleString &operator=(SimpleString const &str) {
        if (&str != inner) {
            LOG(INFO, "Assigning SimpleString");
            if(inner) delete[] inner;
            init(str.inner);
        }
        return *this;
    }

    SimpleString &operator=(const char str[]) {
        LOG(INFO, "Assigning SimpleString from cstring");
        if(inner) delete[] inner;
        init(str);
        return *this;
    }

    bool operator==(SimpleString const &str) const {
        if (length() != str.length()) return false;
        return strcmp(inner, str.inner) == 0;
    }

    bool operator==(const char *str) const {
        if (length() != strlen(str)) return false;
        return strcmp(inner, str) == 0;
    }

    bool operator!=(SimpleString const &str) const {
        return !(*this == str);
    }

    bool operator!=(const char *str) const {
        return !(*this == str);
    }

    bool operator>(SimpleString const& other) const {
        return strcmp(inner, other.get()) > 0;
    }


    char *operator&() {
        LOG(WARN, "Calling override of operator&.");
        return inner;
    }

    const char *operator&() const {
        LOG(WARN, "Calling override of operator&");
        return inner;
    }


    const char *get() const {
        return inner;
    }

    friend std::ostream &operator<<(std::ostream &out, SimpleString str) {
        return out << str.inner;
    }

    size_t length() const {
        return len;
    }

};

#endif //TRAVELLERAPP_SIMPLESTRING_HPP
