#include "TestUtils.hpp"

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

  T_CustomSystemManager systemManager{};
  systemManager.regster<Renderable, Animation, Texture>(componentTable);
  systemManager.execute<Renderable>(componentTable);
}
