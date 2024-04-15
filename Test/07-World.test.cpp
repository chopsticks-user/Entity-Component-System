#include "TestUtils.hpp"

template <typename T1, typename T2> //
constexpr auto comparePtr(std::shared_ptr<T1> p1, std::shared_ptr<T2> p2)
    -> bool {
  return std::static_pointer_cast<void>(p1) ==
         std::static_pointer_cast<void>(p2);
}

TEST_CASE("Case #01: Singleton", "[require]") {
  auto world1 = ushi::World<ushi::DefaultConfig>::instance();
  auto world2 = ushi::World<ushi::DefaultConfig>::instance();
  REQUIRE(world1 != nullptr);
  REQUIRE(comparePtr(world1, world2));

  auto world3 = ushi::World<CustomConfig>::instance();
  REQUIRE(world3 != nullptr);
  REQUIRE_FALSE(comparePtr(world3, world1));
}

TEST_CASE("Case #02: Entity-Component behaviors", "[require]") {
  auto world = ushi::World<ushi::DefaultConfig>::instance();

  world->record<Motion>();
  world->record<Assets, Animation, Texture, Audio>();

  auto entity1 = world->create();

  REQUIRE_FALSE(world->hasComponents<Motion>(entity1));
  world->addComponentsToEntity(entity1, Motion{});
  REQUIRE(world->hasComponents<Motion>(entity1));

  REQUIRE_FALSE(world->hasComponents<Motion, Texture>(entity1));
  world->addComponentsToEntity(entity1, Texture{});
  REQUIRE(world->hasComponents<Motion, Texture>(entity1));

  auto entity2 = world->create(Motion{}, Texture{});
  REQUIRE(world->hasComponents<Motion, Texture>(entity2));
}