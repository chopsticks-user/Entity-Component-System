#define CATCH_CONFIG_MAIN
#define CATCH_CONFIG_ENABLE_BENCHMARKING
#include <catch2/catch.hpp>

#include <Ushi/Ushi.hpp>

#include <any>
#include <functional>
#include <string>
#include <variant>

struct CustomConfig {
  using SignatureType = std::bitset<5>;
  using EIDGeneratorType = ushi::DefaultConfig::EIDGeneratorType;
};

using T_Record = ushi::ComponentRecord<CustomConfig>;
using T_Group = ushi::Group<CustomConfig>;

struct Motion : public ushi::Component {};

struct Assets : public ushi::Component {};

struct Animation : public Assets {};

struct Texture : public Assets {};

struct Audio : public Assets {};

TEST_CASE("Case #01: Group", "[require]") {
  auto record = T_Record{};
  record.regster<Motion>();
  record.regster<Assets>();
  record.regster<Animation>();
  record.regster<Texture>();
  record.regster<Audio>();

  auto factory = ushi::GroupFactory<CustomConfig>{};

  auto group1 = factory.create<Motion, Texture, Animation>(record);
  // REQUIRE_NOTHROW(group1)
}

TEST_CASE("Case #02: ", "[require]") {}