#define CATCH_CONFIG_MAIN
#define CATCH_CONFIG_ENABLE_BENCHMARKING
#include <catch2/catch.hpp>

#include <Ushi/Ushi.hpp>

using T_Record = ushi::ComponentRecord<ushi::DefaultConfig>;

struct Motion : public ushi::Component {};

struct Texture : public ushi::Component {};

TEST_CASE("Case #01: IsComponent", "[require]") {
  REQUIRE_FALSE(ushi::IsComponent<ushi::Component>);
  REQUIRE(ushi::IsComponent<Motion>);
  REQUIRE(ushi::IsComponent<Texture>);
}

TEST_CASE("Case #02: ComponentRecord", "[require]") {
  auto record = T_Record{};
  REQUIRE(T_Record::maxComponents ==
          ushi::DefaultConfig::SignatureType{}.size());
}