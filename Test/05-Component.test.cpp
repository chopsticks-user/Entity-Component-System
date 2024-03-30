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

struct Motion : public ushi::Component {};

struct Assets : public ushi::Component {};

struct Animation : public Assets {};

struct Texture : public Assets {};

struct Audio : public Assets {};

TEST_CASE("Case #01: IsComponent", "[require]") {
  REQUIRE_FALSE(ushi::IsComponent<ushi::Component>);
  REQUIRE(ushi::IsComponent<Motion>);
  REQUIRE(ushi::IsComponent<Assets>);
  REQUIRE(ushi::IsComponent<Animation>);
  REQUIRE(ushi::IsComponent<Texture>);
  REQUIRE(ushi::IsComponent<Audio>);
}

TEST_CASE("Case #02: ComponentRecord<T>::maxComponents", "[require]") {
  REQUIRE(T_Record::maxComponents ==
          ushi::DefaultConfig::SignatureType{}.size());
  REQUIRE(T_CustomRecord::maxComponents ==
          CustomConfig::SignatureType{}.size());
}

TEST_CASE("Case #03: ComponentRecord", "[require]") {
  auto record = T_CustomRecord{};
  REQUIRE(record.size() == 0);

  record.regster<Motion>();
  REQUIRE(record.size() == 1);
  REQUIRE(record.getIndex<Motion>() == 0);

  record.regster<Texture>();
  record.regster<Texture>();
  REQUIRE(record.size() == 2);
  REQUIRE(record.getIndex<Texture>() == 1);

  record.regster<Animation>();
  REQUIRE(record.size() == 3);
  REQUIRE(record.getIndex<Animation>() == 2);
  REQUIRE(record.full());

  record.regster<Audio>();
  REQUIRE(record.size() == 3);
  REQUIRE(record.getIndex<Audio>() == T_CustomRecord::maxComponents);
}