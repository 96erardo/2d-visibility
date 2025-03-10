#include <iostream>
#include <map>
#include "EntityManager.h"
#include "Entity.h"

EntityManager::EntityManager() {}

void EntityManager::init () {

}

void EntityManager::update () {
  m_entities.erase(
    std::remove_if(m_entities.begin(), m_entities.end(), [](std::shared_ptr<Entity> e) {
      return e->isActive() == false;
    }),
    m_entities.end()
  );

  for (auto& [tag, entities] : m_entityMap) {
    entities.erase(
      std::remove_if(entities.begin(), entities.end(), [](std::shared_ptr<Entity> e) { 
        return e->isActive() == false; 
      }),
      entities.end()
    );
  }

  for (const auto add : m_toAdd) {
    m_entities.push_back(add);
    m_entityMap[add->tag()].push_back(add);
  }

  m_toAdd.clear();
}

std::shared_ptr<Entity> EntityManager::addEntity (const std::string& tag) {
  std::shared_ptr<Entity> entity = std::make_shared<Entity>(m_lastId++, tag);

  m_toAdd.push_back(entity);

  return entity;
}

EntityVec EntityManager::getEntities() {
  return m_entities;
}

EntityVec EntityManager::getEntities(const std::string& tag) {
  return m_entityMap[tag];
}