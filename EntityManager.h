#ifndef ENTITY_MANAGER_H
#define ENTITY_MANAGER_H
#include <iostream>
#include <memory>
#include <map>
#include "Entity.h"

typedef std::vector<std::shared_ptr<Entity>> EntityVec;

class EntityManager {
  size_t                           m_lastId = 0;
  EntityVec                        m_entities;
  EntityVec                        m_toAdd;
  std::map<std::string, EntityVec> m_entityMap;

  public:
    EntityManager();

    void init ();

    void update ();

    std::shared_ptr<Entity> addEntity (const std::string& tag);

    EntityVec getEntities();

    EntityVec getEntities(const std::string& tag);
};

#endif