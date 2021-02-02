//
// Created by Hameer Abbasi on 19.02.21.
//

#ifndef TACO2_TEMPLATE_UTILS_H
#define TACO2_TEMPLATE_UTILS_H
#include <tuple>
#include <type_traits>

namespace taco2::util
{
  template<typename T, template<typename...> class TT>
  struct is_specialization_of : std::false_type
  {
  };

  template<template<typename...> class TT, typename... Ts>
  struct is_specialization_of<TT<Ts...>, TT> : std::true_type
  {
  };

  template<typename T, template<typename...> class TT>
  inline constexpr bool is_specialization_of_v = is_specialization_of<T, TT>::value;

  template <typename>
  struct is_tuple_with_integral_template_arguments
    : std::false_type {};

  template <typename... Ts>
  struct is_tuple_with_integral_template_arguments<std::tuple<Ts...>>
    : std::bool_constant<(std::is_integral_v<Ts> && ...)> {};

  template<typename T>
  inline constexpr bool is_tuple_with_integral_template_arguments_v = is_tuple_with_integral_template_arguments<T>::value;
}
#endif  // TACO2_TEMPLATE_UTILS_H
