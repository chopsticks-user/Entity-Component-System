#include "TestUtils.hpp"

template <typename T1, typename T2> //
constexpr auto comparePtr(std::shared_ptr<T1> p1, std::shared_ptr<T2> p2)
    -> bool {
  return std::static_pointer_cast<void>(p1) ==
         std::static_pointer_cast<void>(p2);
}

TEST_CASE("Case #01: World is singleton", "[require]") {
  auto world1 = ushi::World<ushi::DefaultConfig>::instance();
  auto world2 = ushi::World<ushi::DefaultConfig>::instance();
  REQUIRE(world1 != nullptr);
  REQUIRE(comparePtr(world1, world2));

  auto world3 = ushi::World<CustomConfig>::instance();
  REQUIRE(world3 != nullptr);
  REQUIRE_FALSE(comparePtr(world3, world1));
}

TEST_CASE("Case #02: Create 1 entity", "[require]") {
  auto world = ushi::World<ushi::DefaultConfig>::instance();

  world->record<Motion>();
  world->record<Assets, Animation, Texture, Audio>();

  REQUIRE(world->nRecordedComponents() == 5);

  auto entity1 = world->create();

  REQUIRE_FALSE(world->hasComponents<Motion>(entity1));
  world->addComponents(entity1, Motion{});
  REQUIRE(world->hasComponents<Motion>(entity1));

  REQUIRE_FALSE(world->hasComponents<Motion, Texture>(entity1));
  world->addComponents(entity1, Texture{});
  REQUIRE(world->hasComponents<Motion, Texture>(entity1));

  auto entity2 = world->create(Motion{}, Texture{});
  REQUIRE(world->hasComponents<Motion, Texture>(entity2));

  auto entity3 = world->create<Motion, Texture, Audio>({}, {}, {});
  REQUIRE(world->hasComponents<Motion, Texture, Audio>(entity3));

  auto entity4 = world->create<Motion, Texture, Animation>();
  REQUIRE(world->hasComponents<Motion, Texture, Animation>(entity4));
}

TEST_CASE("Case #02: Create multiple entities", "[require]") {
  auto world = ushi::World<ushi::DefaultConfig>::instance();

  world->record<Motion>();
  world->record<Assets, Animation, Texture, Audio>();

  REQUIRE(world->nRecordedComponents() == 5);

  auto entitySet1 = world->create(5);

  REQUIRE_FALSE(world->hasComponents<Motion>(entitySet1));
  world->addComponents<Motion>(entitySet1);
  REQUIRE(world->hasComponents<Motion>(entitySet1));

  REQUIRE_FALSE(world->hasComponents<Texture>(entitySet1));
  world->addComponents<Texture>(entitySet1, Texture{});
  REQUIRE(world->hasComponents<Motion, Texture>(entitySet1));

  auto entitySet2 = world->create<Texture, Motion, Animation>(5);
  REQUIRE(world->hasComponents<Motion, Animation, Texture>(entitySet2));

  auto entitySet3 = world->create(5, Texture{}, Motion{}, Audio{});
  REQUIRE(world->hasComponents<Motion, Texture, Audio>(entitySet3));
}