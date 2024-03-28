#define CATCH_CONFIG_MAIN
#define CATCH_CONFIG_ENABLE_BENCHMARKING
#include <catch2/catch.hpp>

#include <Ushi/Ushi.hpp>

using Entity = ushi::Entity<ushi::DefaultConfig>;
using Bitset = ushi::DefaultConfig::SignatureType;

struct CustomConfig {
  using SignatureType = std::bitset<128>;
  using EIDGeneratorType = ushi::DefaultConfig::EIDGeneratorType;
};

struct NotAConfig {
  using Signature = ushi::u64;
};

TEST_CASE("Case #01: Concepts", "[require]") {
  REQUIRE(ushi::IsValidConfig<ushi::DefaultConfig>);
  REQUIRE(ushi::IsValidConfig<CustomConfig>);
  REQUIRE(ushi::IsValidEntity<Entity>);
  REQUIRE(ushi::IsValidEntity<ushi::Entity<CustomConfig>>);
  REQUIRE_FALSE(ushi::IsValidConfig<NotAConfig>);
}

TEST_CASE("Case #01: EntityIDGenerator", "[require]") {
  ushi::EntityIDGenerator<ushi::u64> idGenerator;
  REQUIRE(idGenerator() == 0);
  REQUIRE(idGenerator() == 1);
  REQUIRE(idGenerator() == 2);
}

TEST_CASE("Case #02: EntityFactory", "[require]") {
  REQUIRE(ushi::IsValidEntity<Entity>);
  REQUIRE(ushi::IsValidEntity<ushi::Entity<CustomConfig>>);

  ushi::EntityFactory<ushi::EntityIDGenerator<ushi::u64>> factory;

  // Implicitly convertible to ushi::EntityID
  REQUIRE(factory.create<Entity>() == 0ul);

  auto player1 = factory.create<Entity>();
  REQUIRE(player1.id() == 1ul);

  static constexpr Bitset bitset = 0xf1a0;

  // Clone
  auto player2 = factory.create<Entity>(bitset);
  REQUIRE(player2.id() == 2ul);
  REQUIRE(player2.signature() == bitset);

  auto player3 = factory.clone<Entity>(player2);
  REQUIRE(player3.id() == 3ul);
  REQUIRE(player3.signature() == bitset);

  // Create an Entity using a Config template
  auto player4 = factory.create<CustomConfig>();
  REQUIRE(player4.id() == 4ul);
  REQUIRE(player4.maxComponents() == CustomConfig::SignatureType{}.size());
}

TEST_CASE("Case #03: EntityManager", "[require]") {
  ushi::EntityManager<ushi::DefaultConfig> manager{};

  auto entity1 = manager.create();
  REQUIRE(manager.contains(entity1));

  static Bitset bitset1 = 0xfa342;

  auto entity2 = manager.create(0xfa342);
  REQUIRE(manager.contains(entity2));
  REQUIRE(entity2.signature() == bitset1);

  auto entity3 = manager.clone(entity2);
  REQUIRE(manager.contains(entity3));
  REQUIRE(entity2.signature() == entity2.signature());

  manager.destroy(entity1);
  REQUIRE_FALSE(manager.contains(entity1));
  REQUIRE_THROWS(manager.get(entity1));
  REQUIRE_THROWS(manager.clone(entity1.id()));

  static Bitset bitset2 = 0xffbff;
  manager.setSignature(entity2, bitset2);

  // update
  entity2 = manager.get(entity2);
  REQUIRE(entity2.signature() == bitset2);
}
