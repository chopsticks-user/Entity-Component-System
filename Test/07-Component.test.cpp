#include "TestUtils.hpp"

struct CustomConfig3 {
  using SignatureType = std::bitset<3>;
  using EIDGeneratorType = T_EIDGen;
};

using T_CustomRecord3 = ushi::internal::impl::ComponentRecord<CustomConfig3>;

TEST_CASE("Case #01: IsComponent", "[require]") {
  REQUIRE_FALSE(ushi::internal::impl::IsComponent<T_Component>);
  REQUIRE(ushi::internal::impl::IsComponent<Motion>);
  REQUIRE(ushi::internal::impl::IsComponent<Assets>);
  REQUIRE(ushi::internal::impl::IsComponent<Animation>);
  REQUIRE(ushi::internal::impl::IsComponent<Texture>);
  REQUIRE(ushi::internal::impl::IsComponent<Audio>);
}

TEST_CASE("Case #02: ComponentRecord<T>::maxComponents", "[require]") {
  REQUIRE(T_Record::maxComponents == T_Signature{}.size());
  REQUIRE(T_CustomRecord3::maxComponents ==
          CustomConfig3::SignatureType{}.size());
}

TEST_CASE("Case #03: ComponentRecord", "[require]") {
  auto record = T_CustomRecord3{};
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
  REQUIRE(record.getIndex<Audio>() == T_CustomRecord3::maxComponents);
}

TEST_CASE("Case #03: ComponentTable", "[require]") {
  T_CustomTable table{};
  //
}