#include "Include/ECS.hpp"

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

struct RenderSystem : public ecs::System {
  static void function(ecs::World &world,
                       const ecs::SparseVector<ecs::u64> &entityIDs) {
    for (auto entityID : entityIDs) {
      auto &mesh = world.getComponent<Mesh>(entityID);
      auto &texture = world.getComponent<Texture>(entityID);
      mesh.color = {5, -6, 2, 3};
      texture.color = {2, -3, 5, 1};
    }
  }
};

struct PhysicsSystem : public ecs::System {
  static void function(ecs::World &world,
                       const ecs::SparseVector<ecs::u64> &entityIDs) {
    for (auto entityID : entityIDs) {
      auto &motion = world.getComponent<Motion>(entityID);
      motion.position = {67, 8, -9};
    }
  }
};

int protected_main([[maybe_unused]] int argc, [[maybe_unused]] char **argv) {
  using namespace ecs;

  auto timer1 = new ScopedTimer();

  World scence;

  //* Add 3 components
  scence.registerComponent<Motion>();
  scence.registerComponent<Mesh>();
  scence.registerComponent<Texture>();

  //* Add 2 systems
  scence.registerSystem<RenderSystem, Mesh, Texture>();
  scence.registerSystem<PhysicsSystem, Motion>();

  //* Add 4 entities
  NPC npc = scence.addEntity<NPC>();
  Player player = scence.addEntity<Player>();
  Tree tree = scence.addEntity<Tree>();
  Enemy enemy = scence.addEntity<Enemy>();

  delete timer1;

  //* Assign some components to each entity
  auto timer2 = new ScopedTimer();

  // scence.addComponent<Motion>(npc);
  // scence.addComponent<Mesh>(npc);
  // scence.addComponent<Texture>(npc);

  scence.addComponent<Motion, Mesh, Texture>(npc, {}, {}, {});
  scence.addComponent<Motion, Mesh, Texture>(player, {}, {}, {});
  scence.addComponent<Mesh, Texture>(tree, {}, {});
  scence.addComponent<Motion, Mesh, Texture>(enemy, {}, {}, {});

  scence.removeComponent<Mesh>(player);

  int n = 100000;
  while (n--) {
    scence.execute<RenderSystem>();
    scence.execute<PhysicsSystem>();
  }

  delete timer2;

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