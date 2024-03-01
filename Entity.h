#pragma once

#include "Components.h"
#include <memory>
#include <tuple>

enum Tag
{
    PLAYER,
    TILE,
    PROJECTILE,
    PLATFORM,
    DEFAULT
};

class Entity
{
    const size_t m_id;
    const Tag    m_tag   = DEFAULT;
    bool         m_alive = true;

    std::tuple<CTransform,
               CSprite,
               CAnimation,
               CState,
               CGravity,
               CRectShape,
               CInput,
               CLifespan,
               CRectCollider,
               CHealth,
               CCoolDown> components;

    friend class EntityManager;

    Entity(const Tag tag, size_t id) :
        m_tag(tag),
        m_id(id)
    {}

public:

    const size_t id() const;
    const Tag tag() const;
    void destroy();
    bool isAlive() const;

    template <typename T>
    T& getComponent()
    {
        return std::get<T>(components);
    }

    template <typename T, typename... TArgs>
    T& addComponent(TArgs&&... mArgs)
    {
        auto& component = getComponent<T>();
        component = T(std::forward<TArgs>(mArgs)...);
        component.has = true;
        return component;
    }

    template <typename T>
    void removeComponent()
    {
        getComponent<T>() = T();
    }

    template <typename T>
    bool hasComponent()
    {
        return std::get<T>(components).has;
    }
};