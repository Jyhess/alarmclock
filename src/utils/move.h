#pragma once

template <class T>
struct remove_reference
{
    using type = T;
};

//Specialized version
template <class T>
struct remove_reference<T &>
{
    using type = T;
};

template <class T>
struct remove_reference<T &&>
{
    using type = T;
};

template <typename T>
typename remove_reference<T>::type &&move(T &&arg)
{
    return static_cast<typename remove_reference<T>::type &&>(arg);
}
