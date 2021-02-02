//
// Created by Hameer Abbasi on 29.03.21.
//

#ifndef TACO2_LOCATE_HPP
#define TACO2_LOCATE_HPP

#include <type_traits>

template <typename T, typename = void>
struct has_locate : std::false_type {};

template <typename T>
struct has_locate<T, decltype(T::locate)> : std::true_type {};

template<typename T>
inline constexpr bool has_locate_v = has_locate<T>::value;

#endif  // TACO2_LOCATE_HPP
