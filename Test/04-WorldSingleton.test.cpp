#define CATCH_CONFIG_MAIN
#define CATCH_CONFIG_ENABLE_BENCHMARKING
#include <catch2/catch.hpp>

#include <Ushi/Ushi.hpp>

struct CustomConfig {
  using SignatureType = std::bitset<128>;
};

template <typename T1, typename T2> //
constexpr auto comparePtr(std::shared_ptr<T1> p1, std::shared_ptr<T2> p2)
    -> bool {
  return std::static_pointer_cast<void>(p1) ==
         std::static_pointer_cast<void>(p2);
}

TEST_CASE("Case #01: Behavior", "[require]") {
  auto world1 = ushi::World<ushi::DefaultConfig>::instance();
  auto world2 = ushi::World<ushi::DefaultConfig>::instance();
  REQUIRE(world1 != nullptr);
  REQUIRE(comparePtr(world1, world2));

  auto world3 = ushi::World<CustomConfig>::instance();
  REQUIRE(world3 != nullptr);
  REQUIRE_FALSE(comparePtr(world3, world1));
}