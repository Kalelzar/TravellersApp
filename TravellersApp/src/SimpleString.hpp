#ifndef TRAVELLERAPP_SIMPLESTRING_HPP
#define TRAVELLERAPP_SIMPLESTRING_HPP

#include<cstring>
#include<iostream>

class SimpleString {

private:
    char *inner;
    size_t len;

    void init(const char *str) {
        len = strlen(str);
        inner = new char[len + 1];
        strcpy(inner, str);
    }

public:

    SimpleString() {
        init("");
    }

    ~SimpleString() {
        delete[] inner;
    }

    SimpleString(const char *str) {
        init(str);
    }

    SimpleString(SimpleString const &str) {
        init(str.inner);
    }

    SimpleString &operator=(SimpleString const &str) {
        if (&str != inner) {
            delete[] inner;
            init(str.inner);
        }
        return *this;
    }

    SimpleString &operator=(const char *str) {
        delete[] inner;
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


    char *operator&() {
        return inner;
    }

    const char *operator&() const {
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
