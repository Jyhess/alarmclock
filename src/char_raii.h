#pragma once
#include <string.h>

class CharRaii
{
private:
    char *_data;

    CharRaii(const CharRaii &other) = delete;
    const CharRaii &operator=(const CharRaii &other) = delete;

public:
    inline CharRaii(int size)
        : _data(new char[size])
    {
    }
    // To bypass lack of std::move
    explicit inline CharRaii(char *other) : _data(other)
    {
    }
    inline CharRaii(CharRaii &&other) : _data(other._data)
    {
        other._data = nullptr;
    }
    inline ~CharRaii()
    {
        if (_data)
        {
            delete[] _data;
        }
    }
    char *data()
    {
        return _data;
    }
    const char *data() const
    {
        return _data;
    }
    char *move()
    {
        char *tmp = _data;
        _data = nullptr;
        return tmp;
    }
};
