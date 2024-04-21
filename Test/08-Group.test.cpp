#include "TestUtils.hpp"

#include <iostream>

using ushi::internal::impl::Group;

struct CustomConfig1 {
  using SignatureType = std::bitset<5>;
  using EIDGeneratorType = ushi::DefaultConfig::EIDGeneratorType;
};

TEST_CASE("Case #01 : Group", "[require]") {
  // TTable table1{};
  // table1[typeid(Motion)] =
  //     std::make_shared<VectorWrapper<Motion, ushi::Component>>();
  // table1[typeid(Animation)] =
  //     std::make_shared<VectorWrapper<Animation, ushi::Component>>();
  // table1[typeid(Texture)] =
  //     std::make_shared<VectorWrapper<Texture, ushi::Component>>();

  // Group<CustomConfig> group1{table1};

  // auto table2 = group1.constructLargerTableWith<Audio, Assets>();
  // REQUIRE(table2.size() == 5);
  // REQUIRE(table2.contains(typeid(Motion)));
  // REQUIRE(table2.contains(typeid(Audio)));
  // REQUIRE(table2.contains(typeid(Assets)));
  // REQUIRE(table2.contains(typeid(Animation)));
  // REQUIRE(table2.contains(typeid(Texture)));

  // auto table3 = group1.constructSmallerTableWithout<Animation>();
  // REQUIRE(table3.size() == 2);
  // REQUIRE(table3.contains(typeid(Motion)));
  // REQUIRE(table3.contains(typeid(Texture)));
  // REQUIRE_FALSE(table3.contains(typeid(Animation)));
}

TEST_CASE("Case #02 : GroupNetwork", "[require]") {
  T_CustomRecord record{};
  record.regster<Motion>();
  record.regster<Assets>();
  record.regster<Animation>();
  record.regster<Texture>();
  record.regster<Audio>();

  T_CustomEntityManager manager{};
  auto e1 = manager.create(record.signature<Motion, Texture>());
  auto e2 = manager.create(record.signature<Motion, Texture>());
  auto e3 = manager.create(record.signature<Motion, Texture>());

  T_CustomGroupNetwork network{};
  // network.addEntityWith(record, e1, Motion{1}, Texture{2});
  // network.addEntityWith(record, e2, Motion{2}, Texture{4});
  // network.addEntityWith(record, e3, Motion{3}, Texture{6});

  // REQUIRE(e1.signature() == record.signature<Motion, Texture>());

  // auto [rMotions, rTextures] =
  //     network.entityView(e1.signature(), std::make_tuple(Motion{},
  //     Texture{}));

  // for (const auto &motion : rMotions.get()) {
  //   std::cout << motion.value << ' ';
  // }
  // std::cout << '\n';
  // for (const auto &texture : rTextures.get()) {
  //   std::cout << texture.value << ' ';
  // }
  // std::cout << '\n';
}