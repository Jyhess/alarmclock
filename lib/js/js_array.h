#pragma once

#include "js_move.h"
#include "js_types.h"

template <typename Tp_, size_t n_>
class Array
{
    Tp_ _data[n_];

public:
    Array(void) = default;
    Array(const Array &) = default;
    Array(Array &&) = default;
    ~Array(void) = default;

    Array &operator=(const Array &) = default;
    Array &operator=(Array &&) = default;

    template <typename... Tps_>
    Array(Tps_ &&...values)
        : _data{move(values)...}
    {
    }

    //============================================
    //             ** Data Access **
    //============================================

    Tp_ &operator[](const size_t &index) { return Array::_data[index]; }
    const Tp_ &operator[](const size_t &index) const { return Array::_data[index]; }

    Tp_ *begin(void) { return Array::_data; }
    Tp_ *end(void) { return Array::_data + n_; }

    const Tp_ *begin(void) const { return Array::_data; }
    const Tp_ *end(void) const { return Array::_data + n_; }

    Tp_ &front(void) { return *Array::_data; }
    Tp_ &back(void) { return *(Array::_data + n_ - 1); }

    const Tp_ &front(void) const { return *Array::_data; }
    const Tp_ &back(void) const { return *(Array::_data + n_ - 1); }

    Tp_ *data(void) { return Array::_data; }
    const Tp_ *data() const { return Array::_data; }

    static constexpr size_t size(void) { return n_; }
};