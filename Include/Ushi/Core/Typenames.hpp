#ifndef USHI_INCLUDE_USHI_CORE_TYPENAMES_HPP
#define USHI_INCLUDE_USHI_CORE_TYPENAMES_HPP

#include "Platforms.hpp"

#include <algorithm>
#include <atomic>
#include <bitset>
#include <cstdint>
#include <memory>
#include <typeindex>
#include <unordered_map>
#include <unordered_set>
#include <vector>

namespace ushi {

constexpr bool cppAtLeast14 = __cplusplus >= 201402L;
constexpr bool cppAtLeast17 = __cplusplus >= 201703L;
constexpr bool cppAtLeast20 = __cplusplus >= 202002L;
constexpr bool cpp14 = cppAtLeast14 && !cppAtLeast17;
constexpr bool cpp17 = cppAtLeast17 && !cppAtLeast20;

typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef float f32;
typedef double f64;

typedef bool b8;
typedef uint32_t b32;

typedef const char *cString;

} // namespace ushi

#endif // USHI_INCLUDE_USHI_CORE_TYPENAMES_HPP
