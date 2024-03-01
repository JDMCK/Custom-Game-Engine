#include "EntityManager.h"

void EntityManager::update()
{
    auto entityPredicate = [](const std::shared_ptr<Entity>& e) -> bool { return !e->isAlive(); };
    m_entities.erase(std::remove_if(m_entities.begin(), m_entities.end(), entityPredicate), m_entities.end());
    for (auto& pair : m_entityMap)
    {
        pair.second.erase(std::remove_if(pair.second.begin(), pair.second.end(), entityPredicate), pair.second.end());
    }

    for (std::shared_ptr<Entity> e :  m_toAdd)
    {
        m_entities.push_back(e);
        m_entityMap[e->tag()].push_back(e);
    }
    m_toAdd.clear();
}

std::shared_ptr<Entity> EntityManager::addEntity(const Tag tag)
{
    auto e = std::shared_ptr<Entity>(new Entity(tag, m_totalEntities++));
    m_toAdd.push_back(e);
    return e;
}

void EntityManager::removeEntity(const size_t id)
{
    for (std::shared_ptr<Entity> e : m_entities)
    {
        if (e->id() == id)
        {
            e->destroy();
            return;
        }
    }
}

void EntityManager::removeEntitiesByTag(const Tag tag)
{
    for (auto e : m_entityMap[tag])
    {
        e->destroy();
    }
}

EntityVec& EntityManager::getEntities()
{
    return m_entities;
}

EntityVec& EntityManager::getEntitiesByTag(const Tag tag)
{
    return m_entityMap[tag];
}