#include "Entity.h"

const size_t Entity::id() const
{
    return m_id;
}

const Tag Entity::tag() const
{
    return m_tag;
}

void Entity::destroy()
{
    m_alive = false;
}

bool Entity::isAlive() const
{
    return m_alive;
}