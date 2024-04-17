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

  REQUIRE(table.size() == 3);

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
  REQUIRE(table.size() == 2);
  REQUIRE_THROWS(table.at<double>(12) == 1.1);

  REQUIRE_NOTHROW(table.remove(99));
  REQUIRE(table.size() == 2);

  table.clear();
  REQUIRE(table.size() == 0);
}