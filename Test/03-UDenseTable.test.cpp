#include "TestUtils.hpp"

#include "Container/UDenseTypeTable.hpp"

using ushi::internal::container::UDenseTypeTable;

TEST_CASE("Case #01: Add and Remove", "[require]") {
  auto table = UDenseTypeTable<ushi::u64>{};
  table.init<int, double, float>();
  REQUIRE(table.nTypes() == 3);

  table.add<int, double, float>(12, 1, 1.1, 1.2f);
  table.add(15, double{2.2}, int{2}, float{2.4f});
  table.add<int, float, double>(1);
  table.add<int, double, float>(3, 3, 3.3, 3.6f);

  REQUIRE(table.size() == 4);

  REQUIRE(table.at<int>(12) == 1);
  REQUIRE(table.at<double>(12) == 1.1);
  REQUIRE(table.at<float>(12) == 1.2f);

  REQUIRE(table.at<int>(15) == 2);
  REQUIRE(table.at<double>(15) == 2.2);
  REQUIRE(table.at<float>(15) == 2.4f);

  REQUIRE(table.at<int>(1) == 0);
  REQUIRE(table.at<double>(1) == 0.0);
  REQUIRE(table.at<float>(1) == 0.0f);

  table.remove(12);
  REQUIRE(table.size() == 3);
  REQUIRE_THROWS(table.at<double>(12) == 1.1);

  REQUIRE_NOTHROW(table.remove(99));
  REQUIRE(table.size() == 3);

  auto [i1, d1, f1, c1] = table.pop<int, double, float, char>(15);
  REQUIRE(table.size() == 2);
  REQUIRE(table.validate());
  REQUIRE(i1 == 2);
  REQUIRE(d1 == 2.2);
  REQUIRE(f1 == 2.4f);
  REQUIRE(c1 == char{});

  REQUIRE_THROWS(table.pop<int, double, float, char>(99));

  auto [i2, d2] = table.pop<int, double>(3);
  REQUIRE(table.size() == 1);
  REQUIRE(table.validate());
  REQUIRE(i2 == 3);
  REQUIRE(d2 == 3.3);

  table.clear();
  REQUIRE(table.size() == 0);
}