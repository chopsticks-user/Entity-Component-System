#include "TestUtils.hpp"

using ushi::internal::impl::Group;

struct CustomConfig1 {
  using SignatureType = std::bitset<5>;
  using EIDGeneratorType = ushi::DefaultConfig::EIDGeneratorType;
};

TEST_CASE("Case #01 : Group behaviors", "[require]") {
  // Group<CustomConfig1> group{};
  // REQUIRE(group.level() == 0);

  // group.init<Motion, Texture, Animation>();
  // REQUIRE(group.level() == 3);

  //   group.addEntity(10, Motion{20}, Texture{30}, Animation{40});
  //   group.addEntity(15, Motion{30}, Animation{45}, Texture{60});
  //   group.addEntity<Motion, Animation, Texture>(20, {40}, {60}, {80});
  //   group.addEntity<Motion, Animation, Texture>(25, {}, {}, {});
  //   REQUIRE(group.size() == 4);

  //   auto [a1, t1] = group.yield<Animation, Texture>(10);
  //   REQUIRE(a1.value == 40);
  //   REQUIRE(t1.value == 30);

  //   auto [a2, m2, t2, as2] = group.yield<Animation, Motion, Texture,
  //   Assets>(15); REQUIRE(a2.value == 45); REQUIRE(m2.value == 30);
  //   REQUIRE(t2.value == 60);

  //   auto [a3, m3, t3] = group.yield<Animation, Motion, Texture>(20);
  //   REQUIRE(a3.value == 60);
  //   REQUIRE(m3.value == 40);
  //   REQUIRE(t3.value == 80);

  //   REQUIRE_THROWS(group.addEntity(11, Audio{}));
  //   REQUIRE_THROWS(group.addEntity<Motion, Texture, Assets>(11, Motion{},
  //                                                           Texture{},
  //                                                           Assets{}));
  //   REQUIRE_THROWS(group.yield<Animation, Motion, Texture>(20));
}

// TEST_CASE("Case #02 : Add and Remove", "[require]") {
//   Group<CustomConfig1> group{};
//   REQUIRE(group.level() == 0);

//   group.init<Motion, Texture, Animation>();
//   REQUIRE(group.level() == 3);

//   group.addEntity(10, Motion{}, Texture{}, Animation{});
//   REQUIRE_THROWS(group.addEntity(11, Audio{}));
//   REQUIRE_THROWS(group.addEntity(11, Motion{}, Texture{}, Assets{}));
// }