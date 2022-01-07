#pragma once

#include "js_types.h"

template <class T>
struct default_value
{
    static constexpr T value = {};
};

template <>
struct default_value<int>
{
    static constexpr int value = 0;
};

template <>
struct default_value<long>
{
    static constexpr long value = 0;
};

template <>
struct default_value<unsigned long>
{
    static constexpr unsigned long value = 0;
};

template <>
struct default_value<uint8_t>
{
    static constexpr int value = 0;
};

template <>
struct default_value<uint16_t>
{
    static constexpr int value = 0;
};
