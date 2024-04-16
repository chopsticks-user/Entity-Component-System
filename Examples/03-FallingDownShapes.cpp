#include <SFML/Graphics.hpp>
#include <Ushi/Ushi.hpp>

#include <iostream>
#include <random>

using ushi::f32;
using ushi::f64;
using ushi::u64;

static constexpr ushi::u64 nVertices = 4;

struct CMotion : public ushi::Component {
  sf::Vector2f velocity;
};

template <ushi::u64 nVerts> //
struct CMesh : public ushi::Component {
  sf::Vector2f position[nVerts];
  sf::Color color[nVerts];
  sf::Vector2f texCoords[nVerts];
};

struct SReappear {
  static f32 screenHeight;
  static void function(CMesh<nVertices> &mesh) {
    if (mesh.position[nVertices - 1].y > screenHeight) {
      for (ushi::u64 i = 0; i < nVertices; ++i) {
        mesh.position[i].y -= screenHeight;
      }
    }
  }
};
f32 SReappear::screenHeight = 0.0f;

struct SFallDown {
  static f32 downwardAccel;
  static void function(CMotion &motion, CMesh<nVertices> &mesh) {
    motion.velocity.y += downwardAccel;

    for (ushi::u64 i = 0; i < nVertices; ++i) {
      mesh.position[i] += motion.velocity;
    }
  }
};
f32 SFallDown::downwardAccel = 0.0f;

struct SRender {
  static sf::RenderWindow *pRenderer;
  static void function(CMesh<nVertices> &mesh) {
    sf::VertexArray shape(sf::TriangleStrip, nVertices);

    for (ushi::u64 i = 0; i < nVertices; ++i) {
      shape[i].position = mesh.position[i];
      shape[i].color = mesh.color[i];
      shape[i].texCoords = mesh.texCoords[i];
    }

    pRenderer->draw(shape);
  }
};
sf::RenderWindow *SRender::pRenderer = nullptr;

struct CustomConfig {
  using SignatureType = std::bitset<2>; // 2 components at most
  using EIDGeneratorType = ushi::DefaultConfig::EIDGeneratorType;
};

sf::Color getRandomColor() {
  const static sf::Color colorArray[] = {
      sf::Color::Red,   sf::Color::Green,  sf::Color::Blue,
      sf::Color::White, sf::Color::Yellow,
  };
  static std::mt19937 gen(std::random_device{}());
  static std::uniform_int_distribution<> colorValue(0, 4);

  return colorArray[colorValue(gen)];
}

int main([[maybe_unused]] int argc, [[maybe_unused]] char **argv) {
  ushi::u64 nObjects = 1000;
  ushi::f32 downwardAccel = 1e-6f;
  if (argc > 1) {
    nObjects = std::stoi(argv[1]);
  }
  if (argc > 2) {
    downwardAccel = std::stof(argv[2]);
  }

  const f32 screenWidth = 1024.0;
  const f32 screenHeight = 768.0;
  const f32 rectSize = 10.0;

  sf::RenderWindow window(sf::VideoMode(screenWidth, screenHeight),
                          "Falling-down shapes");

  auto pWorld = ushi::World<CustomConfig>::instance();

  pWorld->record<CMotion, CMesh<nVertices>>();

  pWorld->regster<SFallDown, CMotion, CMesh<nVertices>>();
  pWorld->regster<SRender, CMesh<nVertices>>();
  pWorld->regster<SReappear, CMesh<nVertices>>();

  SFallDown::downwardAccel = downwardAccel;
  SRender::pRenderer = &window;
  SReappear::screenHeight = screenHeight;

  auto entities = pWorld->create(nObjects);

  pWorld->addComponents<CMotion>(entities);

  std::srand(static_cast<ushi::u32>(std::time(nullptr)));
  for (auto &entity : entities) {
    f32 randX =
        10.0f + f32(std::rand() / ((RAND_MAX + 1u) / (int)screenWidth - 60));
    f32 randY =
        10.0f + f32(std::rand() / ((RAND_MAX + 1u) / (int)screenHeight - 60));

    CMesh<nVertices> mesh{};
    mesh.position[0] = sf::Vector2f(randX + rectSize, randY + rectSize);
    mesh.position[1] = sf::Vector2f(randX, randY + rectSize);
    mesh.position[2] = sf::Vector2f(randX + rectSize, randY);
    mesh.position[3] = sf::Vector2f(randX, randY);

    auto color = getRandomColor();
    mesh.color[0] = color;
    mesh.color[1] = color;
    mesh.color[2] = color;
    mesh.color[3] = color;

    pWorld->addComponents(entity, std::move(mesh));
  }

  while (window.isOpen()) {
    sf::Event event;
    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed)
        window.close();
    }
    window.clear();

    pWorld->execute<SFallDown>();
    pWorld->execute<SRender>();
    pWorld->execute<SReappear>();

    window.display();
  }
  return 0;
}
