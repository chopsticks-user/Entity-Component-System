#define CATCH_CONFIG_MAIN
#define CATCH_CONFIG_ENABLE_BENCHMARKING
#include <catch2/catch.hpp>

#include <Ushi/Ushi.hpp>

using ushi::u64;

ushi::UnorderedDenseMap<u64, u64> v{};
u64 sizeCounter = 0;

void verifyAdd(u64 id, u64 value, u64 increment) {
  sizeCounter += increment;
  REQUIRE(v.size() == sizeCounter);
  REQUIRE(v.contains(id));
  REQUIRE(v[id] == value);
}

TEST_CASE("Case #01: UnorderedDenseMap.add", "[require]") {
  REQUIRE(v.size() == sizeCounter);
  REQUIRE(v.empty());

  v.add(5, 6);
  verifyAdd(5, 6, 1);

  v.add(9, 2);
  verifyAdd(9, 2, 1);

  v.add(5, 7);
  verifyAdd(5, 7, 0);

  v.add(1, 1);
  v.add(1, 5);
  verifyAdd(1, 5, 1);
}

void verifyRemove(u64 id, u64 decrement, bool empty = false) {
  sizeCounter -= decrement;
  if (!empty) {
    REQUIRE(v.size() == sizeCounter);
  } else {
    REQUIRE(v.empty());
    REQUIRE(v.size() == 0);
  }
  REQUIRE_FALSE(v.contains(id));
  REQUIRE_THROWS_AS(v[id], std::out_of_range);
}

TEST_CASE("Case #02: UnorderedDenseMap.remove", "[require]") {
  v.remove(5);
  verifyRemove(5, 1);

  v.remove(1);
  verifyRemove(1, 1);

  v.remove(5);
  verifyRemove(5, 0);

  v.remove(9);
  verifyRemove(9, 1, true);
}

TEST_CASE("Case #03: Element order", "[require]") {
  v.clear();
  sizeCounter = 0;
  REQUIRE(v.empty());
  REQUIRE(v.size() == 0);

  for (; sizeCounter < 5; ++sizeCounter) {
    v.add(sizeCounter, sizeCounter);
  }

  for (u64 i = 0; i < sizeCounter; ++i) {
    REQUIRE(v[i] == i);
    v.remove(i);
  }
  REQUIRE(v.empty());
}
