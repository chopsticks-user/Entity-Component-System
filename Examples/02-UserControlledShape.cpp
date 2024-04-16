#include <SFML/Graphics.hpp>
#include <Ushi/Ushi.hpp>

#include <iostream>

using ushi::u64;

static constexpr u64 nVertices = 4;

struct CInput : public ushi::Component {
  bool arrowLeft = false;
  bool arrowRight = false;
  bool arrowUp = false;
  bool arrowDown = false;
};

struct CMotion : public ushi::Component {
  sf::Vector2f velocity;
  sf::Vector2f acceleration;
};

template <u64 nVerts> //
struct CMesh : public ushi::Component {
  sf::Vector2f position[nVerts];
  sf::Color color[nVerts];
  sf::Vector2f texCoords[nVerts];
};

struct SGetInput {
  static void function(CInput &input) {
    input.arrowUp = sf::Keyboard::isKeyPressed(sf::Keyboard::Up);
    input.arrowDown = sf::Keyboard::isKeyPressed(sf::Keyboard::Down);
    input.arrowRight = sf::Keyboard::isKeyPressed(sf::Keyboard::Right);
    input.arrowLeft = sf::Keyboard::isKeyPressed(sf::Keyboard::Left);
  }
};

struct SMove {
  static void function(CMotion &motion, CMesh<nVertices> &mesh, CInput &input) {

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

    for (u64 i = 0; i < nVertices; ++i) {
      mesh.position[i] += motion.velocity;
    }
  }
};

struct SRender {
  static sf::RenderWindow *pRenderer;
  static void function(CMesh<nVertices> &mesh) {
    if (pRenderer == nullptr) {
      return;
    }

    sf::VertexArray shape(sf::TriangleStrip, nVertices);

    for (u64 i = 0; i < nVertices; ++i) {
      shape[i].position = mesh.position[i];
      shape[i].color = mesh.color[i];
      shape[i].texCoords = mesh.texCoords[i];
    }

    pRenderer->draw(shape);
  }
};
sf::RenderWindow *SRender::pRenderer = nullptr;

struct CustomConfig {
  using SignatureType = std::bitset<3>; // 3 components at most
  using EIDGeneratorType = ushi::DefaultConfig::EIDGeneratorType;
};

int main([[maybe_unused]] int argc, [[maybe_unused]] char **argv) {
  sf::RenderWindow window(sf::VideoMode(1024, 768), "User-controlled Shape");

  auto pWorld = ushi::World<CustomConfig>::instance();

  pWorld->record<CMotion, CMesh<nVertices>, CInput>();

  pWorld->regster<SGetInput, CInput>();
  pWorld->regster<SMove, CMotion, CMesh<nVertices>, CInput>();
  pWorld->regster<SRender, CMesh<nVertices>>();

  CMotion initialMotion{};
  initialMotion.velocity = {0.0f, 0.0f};
  initialMotion.acceleration = {0.0f, 0.0f};

  CMesh<nVertices> mesh{};
  mesh.position[0] = sf::Vector2f(50.f, 50.f);
  mesh.position[1] = sf::Vector2f(10.f, 50.f);
  mesh.position[2] = sf::Vector2f(50.f, 10.f);
  mesh.position[3] = sf::Vector2f(10.f, 10.f);
  mesh.color[0] = sf::Color::Green;
  mesh.color[1] = sf::Color::Green;
  mesh.color[2] = sf::Color::Green;
  mesh.color[3] = sf::Color::Green;

  auto eTriangle = pWorld->create<CMotion, CMesh<nVertices>, CInput>(
      initialMotion, mesh, {});

  SRender::pRenderer = &window;
  while (window.isOpen()) {
    sf::Event event;
    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed)
        window.close();
    }
    window.clear();

    pWorld->execute<SGetInput>();
    pWorld->execute<SMove>();
    pWorld->execute<SRender>();

    window.display();
  }

  return 0;
}