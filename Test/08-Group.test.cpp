#include "TestUtils.hpp"

#include <iostream>

using ushi::internal::impl::Group;

struct CustomConfig1 {
  using SignatureType = std::bitset<5>;
  using EIDGeneratorType = ushi::DefaultConfig::EIDGeneratorType;
};

using ushi::internal::container::PolyTypeTable;
using ushi::internal::container::VectorWrapper;
using ushi::internal::container::VectorWrapperBase;
using ushi::internal::impl::Group;

using TTable =
    std::unordered_map<std::type_index,
                       std::shared_ptr<VectorWrapperBase<ushi::Component>>>;

TEST_CASE("Case #01 : Group", "[require]") {
  TTable table1{};
  table1[typeid(Motion)] =
      std::make_shared<VectorWrapper<Motion, ushi::Component>>();
  table1[typeid(Animation)] =
      std::make_shared<VectorWrapper<Animation, ushi::Component>>();
  table1[typeid(Texture)] =
      std::make_shared<VectorWrapper<Texture, ushi::Component>>();

  Group<CustomConfig> group1{table1};

  auto table2 = group1.constructLargerTableWith<Audio, Assets>();
  REQUIRE(table2.size() == 5);
  REQUIRE(table2.contains(typeid(Motion)));
  REQUIRE(table2.contains(typeid(Audio)));
  REQUIRE(table2.contains(typeid(Assets)));
  REQUIRE(table2.contains(typeid(Animation)));
  REQUIRE(table2.contains(typeid(Texture)));

  auto table3 = group1.constructSmallerTableWithout<Animation>();
  REQUIRE(table3.size() == 2);
  REQUIRE(table3.contains(typeid(Motion)));
  REQUIRE(table3.contains(typeid(Texture)));
  REQUIRE_FALSE(table3.contains(typeid(Animation)));
}

TEST_CASE("Case #02 : GroupNetwork", "[require]") {}