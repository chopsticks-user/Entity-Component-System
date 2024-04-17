#include "TestUtils.hpp"

TEST_CASE("Case #01: IsConfig", "[require]") {
  REQUIRE(ushi::internal::impl::IsConfig<T_Config>);
  REQUIRE(ushi::internal::impl::IsConfig<CustomConfig>);
  REQUIRE_FALSE(ushi::internal::impl::IsConfig<NotAConfig>);
}

TEST_CASE("Case #02: IsEntity", "[require]") {
  REQUIRE(ushi::internal::impl::IsEntity<T_Entity>);
  REQUIRE(ushi::internal::impl::IsEntity<T_CustomEntity>);
}