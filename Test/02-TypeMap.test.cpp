#define CATCH_CONFIG_MAIN
#define CATCH_CONFIG_ENABLE_BENCHMARKING
#include <catch2/catch.hpp>

#include <Tora/Core/TypeMap.hpp>

using tora::cString;
using tora::f64;
using tora::u64;

tora::TypeMap m;
u64 nTypes = 0;
u64 nElements[3] = {0, 0, 0};
u64 nTotalElements = 0;

template <typename T> //
void verifyAdd(u64 id) {
  REQUIRE(m.nTypes() == nTypes);
  REQUIRE(m.nElements() == nTotalElements);
  REQUIRE(m.exists<T>(id));
}

TEST_CASE("Case #01: TypeMap.add", "[require]") {
  m.add<u64>(0, 0);
  ++nTypes;
  ++nTotalElements;
  verifyAdd<u64>(0);

  m.add<u64>(2, 2);
  ++nTotalElements;
  verifyAdd<u64>(2);

  m.add<u64>(4, 4);
  ++nTotalElements;
  verifyAdd<u64>(4);

  m.add<f64>(0, 0.0);
  ++nTypes;
  ++nTotalElements;
  verifyAdd<f64>(0);

  m.add<f64>(2, 2.0);
  ++nTotalElements;
  verifyAdd<f64>(2);

  m.add<f64>(4, 4.0);
  ++nTotalElements;
  verifyAdd<f64>(4);

  m.add<cString>(0, "0.0");
  ++nTypes;
  ++nTotalElements;
  verifyAdd<cString>(0);

  m.add<cString>(2, "2.0");
  ++nTotalElements;
  verifyAdd<cString>(2);

  m.add<cString>(4, "4.0");
  ++nTotalElements;
  verifyAdd<cString>(4);

  m.add<cString>(0, "3.0");
  verifyAdd<cString>(0);
}

TEST_CASE("Case #02: TypeMap.get", "[require]") {
  REQUIRE(m.get<u64>(0) == 0);
  REQUIRE(m.get<u64>(2) == 2);
  REQUIRE(m.get<u64>(4) == 4);

  REQUIRE(m.get<f64>(0) == 0.0);
  REQUIRE(m.get<f64>(2) == 2.0);
  REQUIRE(m.get<f64>(4) == 4.0);

  REQUIRE(std::string{m.get<cString>(0)} == std::string{"3.0"});
  REQUIRE(std::string{m.get<cString>(2)} == std::string{"2.0"});
  REQUIRE(std::string{m.get<cString>(4)} == std::string{"4.0"});
}

template <typename T> //
void verifyRemove(u64 id) {
  REQUIRE(m.nTypes() == nTypes);
  REQUIRE(m.nElements() == nTotalElements);
  REQUIRE_FALSE(m.exists<T>(id));
}

TEST_CASE("Case #03: TypeMap.remove", "[require]") {
  m.remove<u64>(0);
  --nTotalElements;
  verifyRemove<u64>(0);

  m.remove<u64>(2);
  --nTotalElements;
  verifyRemove<u64>(2);

  m.remove<u64>(4);
  --nTotalElements;
  m.remove<u64>();
  --nTypes;
  verifyRemove<u64>(4);
  REQUIRE(m.empty<u64>());

  m.remove<f64>(0);
  --nTotalElements;
  verifyRemove<f64>(0);

  m.remove<f64>(2);
  --nTotalElements;
  verifyRemove<f64>(2);

  m.remove<f64>(4);
  --nTotalElements;
  m.remove<f64>();
  --nTypes;
  verifyRemove<f64>(4);
  REQUIRE(m.empty<f64>());

  m.remove<cString>(0);
  --nTotalElements;
  verifyRemove<cString>(0);

  m.remove<cString>(2);
  --nTotalElements;
  verifyRemove<cString>(2);

  m.remove<cString>(2);
  verifyRemove<cString>(2);

  m.remove<cString>(4);
  --nTotalElements;
  m.remove<cString>();
  --nTypes;
  verifyRemove<cString>(4);
  REQUIRE(m.empty<cString>());

  REQUIRE(m.empty());
}