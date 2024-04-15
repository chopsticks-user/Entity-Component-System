#include "TestUtils.hpp"

TEST_CASE("Case #01: Concepts", "[require]") {
  REQUIRE(ushi::internal::impl::IsConfig<T_Config>);
  REQUIRE(ushi::internal::impl::IsConfig<CustomConfig>);
  REQUIRE(ushi::internal::impl::IsEntity<T_Entity>);
  REQUIRE(ushi::internal::impl::IsEntity<T_CustomEntity>);
  REQUIRE_FALSE(ushi::internal::impl::IsConfig<NotAConfig>);
}

TEST_CASE("Case #01: EntityIDGenerator", "[require]") {
  T_EIDGen idGenerator;
  REQUIRE(idGenerator() == 0);
  REQUIRE(idGenerator() == 1);
  REQUIRE(idGenerator() == 2);
}

TEST_CASE("Case #02: EntityFactory", "[require]") {
  REQUIRE(ushi::internal::impl::IsEntity<T_Entity>);
  REQUIRE(ushi::internal::impl::IsEntity<T_CustomEntity>);

  T_EntityFactory factory;

  // Implicitly convertible to ushi::EntityID
  REQUIRE(factory.create<T_Entity>() == 0ul);

  auto player1 = factory.create<T_Entity>();
  REQUIRE(player1.id() == 1ul);

  static constexpr T_Signature bitset = 0xf1a0;

  // Clone
  auto player2 = factory.create<T_Entity>(bitset);
  REQUIRE(player2.id() == 2ul);
  REQUIRE(player2.signature() == bitset);

  auto player3 = factory.clone<T_Entity>(player2);
  REQUIRE(player3.id() == 3ul);
  REQUIRE(player3.signature() == bitset);

  // Create an Entity using a Config template
  auto player4 = factory.create<CustomConfig>();
  REQUIRE(player4.id() == 4ul);
  REQUIRE(player4.maxComponents() == CustomConfig::SignatureType{}.size());
}

TEST_CASE("Case #03: EntityManager", "[require]") {
  T_EntityManager manager{};

  auto entity1 = manager.create();
  REQUIRE(manager.contains(entity1));

  static T_Signature bitset1 = 0xfa342;

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

  static T_Signature bitset2 = 0xffbff;
  manager.setSignature(entity2, bitset2);

  // update
  entity2 = manager.get(entity2);
  REQUIRE(entity2.signature() == bitset2);
}
