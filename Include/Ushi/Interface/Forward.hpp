#ifndef USHI_INCLUDE_USHI_INTERFACE_FORWARD_HPP
#define USHI_INCLUDE_USHI_INTERFACE_FORWARD_HPP

#include <cstdint>

namespace ushi {

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

namespace container {
template <typename T_Key> struct UnorderedDenseMapBase;
template <typename T_Key, typename T_Value> class UnorderedDenseMap;
} // namespace container

struct DefaultConfig;

struct Component;
template <typename T_Config> class ComponentRecord;
template <typename T_Config> class ComponentTable;

using EntityID = u64;
template <typename T_Config> class Entity;
template <typename T_EID> class EntityIDGenerator;
template <typename T_Config> class EntityManager;

template <typename T_Config> class SystemManager;

template <typename T_Config> class World;

} // namespace ushi

#endif // USHI_INCLUDE_USHI_INTERFACE_FORWARD_HPP