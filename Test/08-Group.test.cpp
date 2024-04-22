#include "TestUtils.hpp"

#include <iostream>

using ushi::internal::impl::Group;

struct CustomConfig1 {
  using SignatureType = std::bitset<5>;
  using EIDGeneratorType = ushi::DefaultConfig::EIDGeneratorType;
};

TEST_CASE("Case #01 : GroupNetwork", "[require]") {
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