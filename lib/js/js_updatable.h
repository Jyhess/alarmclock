#pragma once

template <class T>
class Updatable
{
private:
    T _value;
    bool _updated;

public:
    Updatable() : _updated(true) {}
    Updatable(const T &value) : _value(value), _updated(true) {}

    Updatable &operator=(const Updatable &other)
    {
        if (&other != this)
        {
            _updated = other._updated;
            _value = other._value;
        }
        return *this;
    }
    Updatable &operator=(const T &value)
    {
        set_value(value);
        return *this;
    }
    inline const T &operator*() const { return _value; }
    inline const T *operator->() const { return &_value; }
    inline T const *operator->() { return &_value; }

    inline bool is_updated() const
    {
        return _updated;
    }
    void clear_updated() { _updated = false; }
    inline const T &value() const { return _value; }
    inline void set_value(const T &value)
    {
        if (_value != value)
        {
            _updated = true;
            _value = value;
        }
    }
};
