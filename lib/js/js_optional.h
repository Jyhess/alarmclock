#pragma once

#include "js_types.h"
#include "js_default_value.h"

template <class T>
class Optional
{
private:
    T _value;
    bool _has_value;

public:
    constexpr Optional() : _value(default_value<T>::value), _has_value(false) {}
    Optional(const T &value) : _value(value), _has_value(true) {}

    Optional &operator=(const Optional &other)
    {
        if (&other != this)
        {
            _has_value = other._has_value;
            _value = other._value;
        }
        return *this;
    }
    Optional &operator=(const T &value)
    {
        set_value(value);
        return *this;
    }
    inline const T &operator*() const { return _value; }
    inline const T *operator->() const { return &_value; }
    inline T *operator->() { return &_value; }

    inline bool has_value() const { return _has_value; }
    inline void reset() { _has_value = false; }
    inline const T &value() const { return _value; }
    inline T &value() { return _value; }
    inline void set_value(const T &value)
    {
        _value = value;
        _has_value = true;
    }

    bool operator==(const Optional<T> &other) const { return _has_value == other._has_value && _value == other._value; }
    bool operator!=(const Optional<T> &other) const { return !operator==(other); }
    bool operator==(const T &value) const { return _has_value == true && _value == value; }
    bool operator!=(const T &value) const { return !operator==(value); }
};
