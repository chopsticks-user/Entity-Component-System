#define CATCH_CONFIG_MAIN
#define CATCH_CONFIG_ENABLE_BENCHMARKING
#include <catch2/catch.hpp>

#include <Ushi/Ushi.hpp>

#include <any>
#include <functional>
#include <ranges>
#include <string>
#include <variant>

struct CustomConfig {
  using SignatureType = std::bitset<5>;
  using EIDGeneratorType = ushi::DefaultConfig::EIDGeneratorType;
};

using T_Record = ushi::ComponentRecord<CustomConfig>;
using T_Group = ushi::Group<CustomConfig>;

struct Motion : public ushi::Component {};

struct Assets : public ushi::Component {};

struct Animation : public Assets {};

struct Texture : public Assets {};

struct Audio : public Assets {};

TEST_CASE("Case #01: Group", "[require]") {
  auto record = T_Record{};
  record.regster<Motion>();
  record.regster<Assets>();
  record.regster<Animation>();
  record.regster<Texture>();
  record.regster<Audio>();

  auto factory = ushi::GroupFactory<CustomConfig>{record};

  auto group1 = factory.create<Motion, Texture, Animation>();
  group1.qualifiedChildren(CustomConfig::SignatureType{0x982});
  // REQUIRE_NOTHROW(group1)
}

void for_each_in_a_view(std::ranges::ref_view<std::vector<int>> &&view) {
  for (auto &element : view) {
    std::cout << element << ' ';
  }
  std::cout << std::endl;
}

template <std::convertible_to<std::ranges::ref_view<std::vector<int>>>... Args>
void for_each_in_combined_view(Args &&...views) {
  (for_each_in_a_view(views), ...);
}

TEST_CASE("Case #02: ", "[require]") {
  std::vector<int> v1(5);
  std::vector<int> v2(10);

  // auto view1 = std::views::all(v1);
  // auto view2 = std::views::all(v2);
  // auto combinedView = view1 | std::views::join;

  for_each_in_combined_view(v1, v1);
}