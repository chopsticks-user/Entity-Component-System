#include "ECS.hpp"

#include <glm/glm.hpp>

struct Motion : public ecs::Component {
  glm::vec3 position;
  glm::vec3 velocity;
  glm::vec3 acceleration;
};

struct Mesh : public ecs::Component {
  glm::vec4 color;
};

struct Texture : public ecs::Component {
  glm::vec4 coordinates;
  glm::vec4 color;
};

DECLARE_SIMPLE_ENTITY(NPC);
DECLARE_SIMPLE_ENTITY(Player);
DECLARE_SIMPLE_ENTITY(Tree);
DECLARE_SIMPLE_ENTITY(Enemy);

struct Renderable : public ecs::System {
  void function(ecs::World &world) {
    for (auto entityID : entityIDs) {
      // auto &motion = world.get<Motion>(entityID);
      auto &mesh = world.get<Mesh>(entityID);
      auto &texture = world.get<Texture>(entityID);
      // motion.position = {3, 0, 5};
      mesh.color = {1, 1, 1, 1};
      texture.color = {};
    }
  }
};

int protected_main([[maybe_unused]] int argc, [[maybe_unused]] char **argv) {
  using namespace ecs;

  World scence;

  //* Add 3 components
  scence.registr<Motion>();
  scence.registr<Mesh>();
  scence.registr<Texture>();

  //* Add 1 system
  scence.registr<Renderable, Mesh, Texture>();

  //* Add 4 entities
  NPC npc = scence.add<NPC>();
  Player player = scence.add<Player>();
  Tree tree = scence.add<Tree>();
  Enemy enemy = scence.add<Enemy>();

  //* Assign some components to each entity

  return EXIT_SUCCESS;
}

int main([[maybe_unused]] int argc, [[maybe_unused]] char **argv) {
  try {
    return protected_main(argc, argv);
  } catch (std::exception &e) {
    std::cerr << e.what() << std::endl;
  } catch (...) {
    std::cerr << "Uncaught exception" << std::endl;
  }
  return EXIT_FAILURE;
}