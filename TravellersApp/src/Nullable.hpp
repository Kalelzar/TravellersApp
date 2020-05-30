#ifndef TRAVELLERAPP_NULLABLE_HPP
#define TRAVELLERAPP_NULLABLE_HPP

#include "NoValueException.hpp"
#include <iostream>
#include "debug.hpp"

template<class A>
class Nullable {
private:
protected:
public:
    virtual ~Nullable() = default;

    virtual bool isEmpty() const = 0;

    bool isDefined() const {
        return !isEmpty();
    }

    virtual A const& get() const = 0;

    A const& getOrElse(A const& def) const {
        if (isEmpty()) return def;
        return get();
    }
};

template<class A>
class NotNull : public Nullable<A> {
private:
    const A& value;
public:

    //Why did I do this?
    //NotNull(A const&&) = delete;

    explicit NotNull(A const &_value) : value(_value) {
        LOG(INFO, "Created NotNull.");
    }

    bool isEmpty() const override {
        return false;
    }

    A const& get() const override {
        return value;
    }
};

template<class A>
class Null : public Nullable<A> {
public:

    Null(){
        LOG(INFO, "Created Null.");
    }

    bool isEmpty() const override {
        return true;
    }

    A const& get() const override {
        throw NoValueException();
    }
};


#endif // TRAVELLERAPP_NULLABLE_HPP
