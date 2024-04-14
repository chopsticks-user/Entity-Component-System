#define CATCH_CONFIG_MAIN
#define CATCH_CONFIG_ENABLE_BENCHMARKING
#include <catch2/catch.hpp>

#include <Ushi/Ushi.hpp>

struct CustomConfig {
  using SignatureType = std::bitset<5>;
  using EIDGeneratorType = ushi::DefaultConfig::EIDGeneratorType;
};

using T_Record = ushi::ComponentRecord<ushi::DefaultConfig>;
using T_CustomTable = ushi::ComponentTable<CustomConfig>;
using T_SystemManager = ushi::SystemManager<CustomConfig>;

struct Motion : public ushi::Component {};
struct Assets : public ushi::Component {};
struct Animation : public Assets {};
struct Texture : public Assets {};
struct Audio : public Assets {};

struct Renderable {
  static void function([[maybe_unused]] Animation &a,
                       [[maybe_unused]] Texture &t) {
    std::cout << "function\n";
  }
};

TEST_CASE("Case #01:", "[require]") {
  T_CustomTable componentTable{};
  componentTable.regster<Motion>();
  componentTable.regster<Assets>();
  componentTable.regster<Animation>();
  componentTable.regster<Texture>();
  componentTable.regster<Audio>();

  componentTable.addEntityWith<Animation, Texture>(10, {}, {});
  componentTable.addEntityWith<Animation, Texture>(11, {}, {});
  componentTable.addEntityWith<Animation, Texture>(11, {}, {});

  T_SystemManager systemManager{};
  systemManager.regster<Renderable, Animation, Texture>(componentTable);
  systemManager.execute<Renderable>(componentTable);

  std::cout << sizeof(ushi::World<CustomConfig>) << std::endl;
}

TEST_CASE("Case #02: ComponentRecord<T>::maxComponents", "[require]") {}

TEST_CASE("Case #03: ComponentRecord", "[require]") {}
