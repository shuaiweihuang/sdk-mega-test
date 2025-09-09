#pragma once

namespace mega
{
namespace common
{
namespace detail
{

template<typename T>
struct IsExpected;

} // detail

template<typename E, typename T>
class Expected;

template<typename T>
struct IsExpected;

template<typename T>
constexpr auto IsExpectedV = IsExpected<T>::value;

} // common
} // mega

