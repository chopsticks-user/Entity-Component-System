#include <Ushi/Ushi.hpp>

#include <array>
#include <cstdlib>
#include <cxxabi.h>
#include <iostream>
#include <memory>
#include <string>

using ushi::f64;
using ushi::u64;

struct Color : public ushi::Component {
  std::array<u64, 4> data;

  constexpr Color() noexcept = default;
  constexpr Color(u64 r, u64 g, u64 b, u64 a) noexcept : data{r, g, b, a} {}
};

struct Position : public ushi::Component {
  std::array<f64, 3> data;

  constexpr Position() noexcept = default;
  constexpr Position(f64 x, f64 y, f64 z) noexcept : data{x, y, z} {}
};

struct ScalarAdd {
  static u64 scalar;

  static void function(Color &c, Position &p) {
    for (auto &value : c.data) {
      value += scalar;
    }
    for (auto &value : p.data) {
      value += static_cast<f64>(scalar);
    }
  }
};
u64 ScalarAdd::scalar = 0;

struct ScalarMul {
  static u64 scalar;

  static void function(Color &c, Position &p) {
    for (auto &value : c.data) {
      value *= static_cast<u64>(scalar);
    }
    for (auto &value : p.data) {
      value *= static_cast<f64>(scalar);
    }
  }
};
u64 ScalarMul::scalar = 1;

struct NegatePosition {
  static void function(Position &p) {
    for (auto &value : p.data) {
      value = -value;
    }
  }
};

struct PrintInfo {
  static void function(const Color &c, const Position &p) {
    std::cout << "\t[ Color=( ";
    for (const auto &value : c.data) {
      std::cout << value << ' ';
    }
    std::cout << "), Position=( ";
    for (const auto &value : p.data) {
      std::cout << value << ' ';
    }
    std::cout << ") ]\n";
  }
};

struct CustomConfig {
  using SignatureType = std::bitset<2>; // 2 components at most
  using EIDGeneratorType = ushi::DefaultConfig::EIDGeneratorType;
};

int main([[maybe_unused]] int argc, [[maybe_unused]] char **argv) {
  auto pWorld = ushi::World<CustomConfig>::instance();

  // Record the two Component types
  pWorld->record<Color, Position>();

  // Ensure that we can't record more components
  if (!pWorld->componentRecordFull()) {
    std::cerr << "Internal error occurred. Exiting.\n";
  }

  // Create an entity containing zero components
  auto entity1 = pWorld->create();

  // Create 5 entities, each of which containing a Color of (255,255,255,0)
  auto entities = pWorld->create<Color>(5, {255, 255, 255, 0});

  // We can verify that
  if (!pWorld->hasComponents<Color>(entities)) {
    std::cerr << "Internal error occurred. Exiting.\n";
    return EXIT_FAILURE;
  }

  // Add a Position to each of the 5 entities
  pWorld->addComponents(entities, Position{0.0, 0.0, 0.0});

  // Make sure they each contain a Color component and a Position component
  if (!pWorld->hasComponents<Color, Position>(entities)) {
    std::cerr << "Internal error occurred. Exiting.\n";
    return EXIT_FAILURE;
  }

  // Add Color and Position the single entity
  pWorld->addComponents(entity1, Color{}, Position{});

  // Resigter all defined systems along with their required components
  pWorld->regster<ScalarAdd, Color, Position>();
  pWorld->regster<ScalarMul, Color, Position>();
  pWorld->regster<NegatePosition, Position>();
  pWorld->regster<PrintInfo, Color, Position>();

  // Display the original data of each entity
  std::cout << "Original state: \n";
  pWorld->execute<PrintInfo>();
  std::cout << '\n';

  // For each entity E, perform (E+200)*3 then negate E.position
  ScalarAdd::scalar = 200;
  pWorld->execute<ScalarAdd>();

  ScalarMul::scalar = 3;
  pWorld->execute<ScalarMul>();

  pWorld->execute<NegatePosition>();

  // Display the final result
  std::cout << "Final state: For each entity E, perform (E+200)*3 then "
               "negate E.position\n";
  pWorld->execute<PrintInfo>();
  std::cout << '\n';

  return 0;
}