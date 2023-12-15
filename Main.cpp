#include "ECS.hpp"

#include <glm/glm.hpp>

class Motion : public ecs::Component {
  glm::vec3 position;
  glm::vec3 velocity;
  glm::vec3 acceleration;
};

class Mesh : public ecs::Component {
  glm::vec4 color;
};

DECLARE_SIMPLE_ENTITY(NPC);
DECLARE_SIMPLE_ENTITY(Player);

int protected_main([[maybe_unused]] int argc, [[maybe_unused]] char **argv) {
  using namespace ecs;
  World scence;

  scence.mComponentTable->reg<Motion>();
  scence.mComponentTable->reg<Mesh>();

  scence.mEntityManager->setNComponents(
      scence.mComponentTable->getNComponents());
  auto e = scence.mEntityManager->createEntity<NPC>(scence);
  scence.mComponentTable->add<Motion>(e);
  scence.mEntityManager->setSignature(e, 1, true);
  scence.mEntityManager->destroyEntity(e);

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