#pragma once

#include "default_value.h"

template <class T, T Min, T Max>
class Range
{
private:
    T _value;

public:
    Range() : _value(default_value<T>::value) {}
    Range(const Range &other) = default;
    Range(const T &value) : _value(value)
    {
        set_value(_value);
    }
    Range &operator=(const Range &other) = default;
    Range &operator=(const T &value)
    {
        set_value(value);
        return *this;
    }
    inline const T &value() const
    {
        return _value;
    }
    inline void set_value(const T &value)
    {
        _value = value;
        if (_value > Max)
            _value = Max;
        else if (_value < Min)
            _value = Min;
    }
    Range &operator+=(T value)
    {
        _value += value;
        if (_value > Max)
            _value = Max;
        return *this;
    }
    Range &operator-=(T value)
    {
        _value += value;
        if (_value < Min)
            _value = Min;
        return *this;
    }
    inline Range &operator++()
    {
        return operator+=(1);
    }
    inline Range &operator--()
    {
        return operator-=(1);
    }
    inline Range operator++(int)
    {
        Range result = *this;
        operator+=(1);
        return result;
    }
    inline Range operator--(int)
    {
        Range result = *this;
        operator-=(1);
        return result;
    }
    inline bool operator==(const Range &other) const { return _value == other._value; }
    inline bool operator!=(const Range &other) const { return !operator==(other); }
};

template <class T, T Min, T Max>
Range<T, Min, Max> operator+(const Range<T, Min, Max> &range, const T &value)
{
    return Range<T, Min, Max>(range + value);
}
template <class T, T Min, T Max>
Range<T, Min, Max> operator-(const Range<T, Min, Max> &range, const T &value)
{
    return Range<T, Min, Max>(range - value);
}
