#pragma once

template <class T>
class default_value
{
    static const T value = T();
};

template <>
class default_value<int>
{
    static const int value = 0;
};

template <>
class default_value<long>
{
    static const long value = 0;
};

template <>
class default_value<unsigned long>
{
    static const unsigned long value = 0;
};

template <>
class default_value<unsigned int>
{
    static const unsigned int value = 0;
};
