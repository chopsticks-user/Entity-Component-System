#ifndef USHI_TEST_TEST_UTILS_HPP
#define USHI_TEST_TEST_UTILS_HPP

#define CATCH_CONFIG_MAIN
#define CATCH_CONFIG_ENABLE_BENCHMARKING
#include <catch2/catch.hpp>

#include <Ushi/Ushi.hpp>

struct CustomConfig {
  using SignatureType = std::bitset<3>;
  using EIDGeneratorType = ushi::DefaultConfig::EIDGeneratorType;
};

using T_Record = ushi::ComponentRecord<ushi::DefaultConfig>;
using T_CustomRecord = ushi::ComponentRecord<CustomConfig>;
using T_CustomTable = ushi::ComponentTable<CustomConfig>;

struct Motion : public ushi::Component {};

struct Assets : public ushi::Component {};

struct Animation : public Assets {};

struct Texture : public Assets {};

struct Audio : public Assets {};

#endif // USHI_TEST_TEST_UTILS_HPP