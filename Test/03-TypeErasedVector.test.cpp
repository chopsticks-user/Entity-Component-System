#include "TestUtils.hpp"

using ushi::internal::container::TypeErasedVector;

struct A3 {
  int m[3] = {};
};

struct A5 {
  int m[5] = {};
};

TypeErasedVector v1{sizeof(A3)};
TypeErasedVector v2{sizeof(A3)};

TEST_CASE("Case #01: Add", "[require]") {
  REQUIRE(v1.size() == 0);

  v1.add(A3{.m = {0, 0, 0}});
  REQUIRE(v1.size() == 1);
  REQUIRE(v1.at<A3>(0).m[0] == 0);

  v1.add(A3{.m = {3, 3, 3}});
  REQUIRE(v1.size() == 2);
  REQUIRE(v1.at<A3>(1).m[0] == 3);

  v1.add(A3{.m = {6, 6, 6}});
  REQUIRE(v1.size() == 3);
  REQUIRE(v1.at<A3>(2).m[0] == 6);

  v1.add(A3{.m = {9, 9, 9}});
  REQUIRE(v1.size() == 4);
  REQUIRE(v1.at<A3>(3).m[0] == 9);

  v2.add(A3{.m = {0, 0, 0}});
  v2.add(A3{.m = {5, 5, 5}});
  v2.add(A3{.m = {11, 11, 11}});
  REQUIRE(v2.size() == 3);

  v2.replace(2, A3{.m = {10, 10, 10}});
  REQUIRE(v2.size() == 3);
  REQUIRE(v2.at<A3>(2).m[0] == 10);

  v1.merge(std::move(v2));
  REQUIRE(v1.size() == 7);
  REQUIRE(v1.at<A3>(5).m[0] == 5);
  REQUIRE(v1.at<A3>(6).m[0] == 10);

  REQUIRE_THROWS(v1.at<A5>(2));
  REQUIRE_THROWS(v1.at<A3>(99));
  REQUIRE_THROWS(v2.replace(2, A5{.m = {10, 10, 10, 10, 10}}));
}

TEST_CASE("Case #02: Remove", "[require]") {
  REQUIRE(v1.size() == 7);

  auto value = v1.remove<A3>(0);
  REQUIRE(v1.size() == 6);
  REQUIRE(value.m[0] == 0);

  value = v1.remove<A3>(3);
  REQUIRE(v1.size() == 5);
  REQUIRE(value.m[0] == 9);

  v1.clear();
  REQUIRE(v1.size() == 0);
}