#include "../Include/ECS/ECS.hpp"

#include <SFML/Graphics.hpp>

#include <iostream>

static constexpr ecs::u64 nVertices = 4;

struct CInput : public ecs::Component {
  bool arrowLeft = false;
  bool arrowRight = false;
  bool arrowUp = false;
  bool arrowDown = false;
};

struct CMotion : public ecs::Component {
  sf::Vector2f velocity;
  sf::Vector2f acceleration;
};

template <ecs::u64 nVerts> //
struct CMesh : public ecs::Component {
  sf::Vector2f position[nVerts];
  sf::Color color[nVerts];
  sf::Vector2f texCoords[nVerts];
};

ECS_SIMPLE_ENTITY_CLASS(ERect);

struct SGetInput : public ecs::System {
  static void function(ecs::World &world,
                       const ecs::SparseVector<ecs::u64> &entityIDs) {
    for (auto entityID : entityIDs) {
      auto &input = world.getComponent<CInput>(entityID);
      input.arrowUp = sf::Keyboard::isKeyPressed(sf::Keyboard::Up);
      input.arrowDown = sf::Keyboard::isKeyPressed(sf::Keyboard::Down);
      input.arrowRight = sf::Keyboard::isKeyPressed(sf::Keyboard::Right);
      input.arrowLeft = sf::Keyboard::isKeyPressed(sf::Keyboard::Left);
    }
  }
};

struct SFallDown : public ecs::System {
  static void function(ecs::World &world,
                       const ecs::SparseVector<ecs::u64> &entityIDs) {
    for (auto entityID : entityIDs) {
      auto &motion = world.getComponent<CMotion>(entityID);
      auto &mesh = world.getComponent<CMesh<nVertices>>(entityID);
      auto &input = world.getComponent<CInput>(entityID);

      // motion.velocity += motion.acceleration;

      if (!input.arrowDown && !input.arrowUp) {
        motion.velocity.y = {};
      } else if (input.arrowUp) {
        motion.velocity.y -= 1e-5f;
      } else {
        motion.velocity.y += 1e-5f;
      }

      if (!input.arrowRight && !input.arrowLeft) {
        motion.velocity.x = {};
      } else if (input.arrowLeft) {
        motion.velocity.x -= 1e-5f;
      } else {
        motion.velocity.x += 1e-5f;
      }

      for (ecs::u64 i = 0; i < nVertices; ++i) {
        mesh.position[i] += motion.velocity;
      }
    }
  }
};

struct SRender : public ecs::System {
  static void function(ecs::World &world,
                       const ecs::SparseVector<ecs::u64> &entityIDs,
                       sf::RenderWindow &renderer) {
    for (auto entityID : entityIDs) {
      auto &mesh = world.getComponent<CMesh<nVertices>>(entityID);
      sf::VertexArray shape(sf::TriangleStrip, nVertices);

      for (ecs::u64 i = 0; i < nVertices; ++i) {
        shape[i].position = mesh.position[i];
        shape[i].color = mesh.color[i];
        shape[i].texCoords = mesh.texCoords[i];
      }

      renderer.draw(shape);
    }
  }
};

void mainLoop() {
  sf::RenderWindow window(sf::VideoMode(1024, 768), "User-controlled Shape");

  ecs::World scence;

  scence.registerComponent<CMotion>();
  scence.registerComponent<CMesh<nVertices>>();
  scence.registerComponent<CInput>();

  scence.registerSystem<SGetInput, CInput>();
  scence.registerSystem<SFallDown, CMotion, CMesh<nVertices>, CInput>();
  scence.registerSystem<SRender, CMesh<nVertices>>();

  ERect myTriangle = scence.addEntity<ERect>();
  scence.addComponents<CMotion, CMesh<nVertices>, CInput>(
      myTriangle,
      CMotion{
          .velocity = {0.0f, 0.0f},
          .acceleration = {0.0f, 0.0f},
      },
      CMesh<nVertices>{
          .position =
              {
                  sf::Vector2f(50.f, 50.f),
                  sf::Vector2f(10.f, 50.f),
                  sf::Vector2f(50.f, 10.f),
                  sf::Vector2f(10.f, 10.f),
              },
          .color =
              {
                  sf::Color::Red,
                  sf::Color::Blue,
                  sf::Color::Green,
                  sf::Color::Yellow,
              },
          .texCoords = {},
      },
      {});

  while (window.isOpen()) {
    sf::Event event;
    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed)
        window.close();
    }
    window.clear();

    scence.execute<SGetInput>();
    scence.execute<SFallDown>();
    scence.execute<SRender>(window);

    window.display();
  }
}

int main([[maybe_unused]] int argc, [[maybe_unused]] char **argv) {
  mainLoop();

  return 0;
}