#include <ECS/ECS.hpp>

#include <catch2/catch_all.hpp>

#include <iostream>

ECS_SIMPLE_ENTITY_CLASS(EntityA);
ECS_SIMPLE_ENTITY_CLASS(EntityB);
ECS_SIMPLE_ENTITY_CLASS(EntityC);
ECS_SIMPLE_ENTITY_CLASS(EntityD);

std::vector<ecs::u64> testAdd(ecs::EntityManager &entityManager,
                              ecs::u64 nEntities) {
  std::vector<ecs::u64> entityIDs(nEntities);
  for (auto &entityID : entityIDs) {
    entityID = entityManager.add<EntityA>().getID();
  }
  return entityIDs;
}

void testRemove(ecs::EntityManager &entityManager,
                std::vector<ecs::u64> entityIDs) {
  for (auto entityID : entityIDs) {
    entityManager.remove(entityID);
  }
}

TEST_CASE("Benchmark methods", "[ecs::EntityManager]") {
  ecs::EntityManager entityManager;
  std::vector<ecs::u64> entityIDs;
  const ecs::u64 nEntities = 1'000;

  BENCHMARK("EntityManager::add()") {
    entityIDs = testAdd(entityManager, nEntities);
  };
  entityManager.clear();
  entityIDs = testAdd(entityManager, nEntities);
  REQUIRE(entityManager.nEntities() == nEntities);

  BENCHMARK("EntityManager::remove()") {
    return testRemove(entityManager, entityIDs);
  };
  REQUIRE(entityManager.nEntities() == 0);
}