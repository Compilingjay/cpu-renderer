#ifndef TEMPLATE_CONCEPTS_H
#define TEMPLATE_CONCEPTS_H

#include <concepts>
#include <limits>
#include <type_traits>

// template <typename T>
// concept Floating = requires(T param) {
//     std::is_floating_point_v<T>;
// };

template <typename T>
concept SizedNumeric = requires(T param) {
    sizeof(T) >= 4 && (std::is_floating_point_v<T> || std::numeric_limits<T>::is_integer);
};

#endif