#pragma once

#include <type_traits>

#define BIT(n) (1ULL << (n))

#define DECLARE_FLAG_OPERATORS(Type)                                                    \
inline constexpr Type operator|(Type a, Type b) noexcept {                              \
    using U = std::underlying_type_t<Type>;                                             \
    return static_cast<Type>(static_cast<U>(a) | static_cast<U>(b));                    \
}                                                                                       \
inline constexpr Type operator&(Type a, Type b) noexcept {                              \
    using U = std::underlying_type_t<Type>;                                             \
    return static_cast<Type>(static_cast<U>(a) & static_cast<U>(b));                    \
}                                                                                       \
inline constexpr Type operator^(Type a, Type b) noexcept {                              \
    using U = std::underlying_type_t<Type>;                                             \
    return static_cast<Type>(static_cast<U>(a) ^ static_cast<U>(b));                    \
}                                                                                       \
inline constexpr Type operator~(Type a) noexcept {                                      \
    using U = std::underlying_type_t<Type>;                                             \
    return static_cast<Type>(~static_cast<U>(a));                                       \
}                                                                                       \
inline constexpr Type& operator|=(Type& a, Type b) noexcept { return a = a | b; }       \
inline constexpr Type& operator&=(Type& a, Type b) noexcept { return a = a & b; }       \
inline constexpr Type& operator^=(Type& a, Type b) noexcept { return a = a ^ b; }       \
inline constexpr bool HAS_FLAG(Type value, Type mask) noexcept {                        \
    using U = std::underlying_type_t<Type>;                                             \
    return (static_cast<U>(value) & static_cast<U>(mask)) != 0;                         \
}                                                                                       \
inline constexpr void SET_FLAG(Type& value, Type mask) noexcept { value |= mask; }      \
inline constexpr void CLEAR_FLAG(Type& value, Type mask) noexcept { value &= ~mask; }   \
inline constexpr void TOGGLE_FLAG(Type& value, Type mask) noexcept { value ^= mask; }