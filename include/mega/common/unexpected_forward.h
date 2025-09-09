#pragma once

namespace mega
{
namespace common
{
namespace detail
{

template<typename T>
struct IsUnexpected;

} // detail

template<typename T>
struct IsUnexpected;

template<typename T>
class Unexpected;

template<typename T>
constexpr auto IsUnexpectedV = IsUnexpected<T>::value;

template<typename T>
auto unexpected(T&& value);

} // common
} // mega
