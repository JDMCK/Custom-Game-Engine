#pragma once

#include <SFML/Graphics.hpp>
#include "Vec2.h"
#include "Animation.h"
#include "Entity.h"

class Component
{
public:
    bool has = false;
};

class CTransform : public Component
{
public:

    Vec2  position     = Vec2::ZERO;
    Vec2  prevPosition = Vec2::ZERO;
    Vec2  velocity     = Vec2::ZERO;
    float rotation     = 0.f;

    CTransform() {};
    CTransform(Vec2 position, Vec2 velocity, float rotation) :
        position(position),
        velocity(velocity),
        rotation(rotation)
    {}
    CTransform(Vec2 position, Vec2 velocity) :
        position(position),
        velocity(velocity)
    {}
    CTransform(Vec2 position) :
        position(position)
    {}
};

class CSprite : public Component
{
public:

    sf::Sprite sprite;
    bool       isFacingRight = true;

    float offsetX = 0.f;
    float offsetY = 0.f;

    CSprite() {}
    
    CSprite(float width, float height, sf::Texture& texture, bool isFacingRight, float scale, float offsetX = 0, float offsetY = 0) :
        sprite(texture),
        isFacingRight(isFacingRight),
        offsetX(offsetX),
        offsetY(offsetY)
    {
        sprite.setTextureRect(sf::IntRect(0, 0, width, height));
        sprite.setOrigin((width + offsetX) / 2, (height + offsetY) / 2);
        sprite.setScale(scale, scale);
    }
};

class CAnimation : public Component
{
public:

    Animation animation;
    bool      isFacingRight = true;
    bool      lockAnimation = false;

    CAnimation() {}

    CAnimation(Animation animation, bool isFacingRight = true, bool lockAnimation = false) :
        animation(animation),
        isFacingRight(isFacingRight),
        lockAnimation(lockAnimation)
    {}
};

enum State
{
    DEFAULT_S,
    RUN_S,
    IDLE_S,
    JUMP_S,
    FALL_S,
    HURT_S,
    DEAD_S
};

class CState : public Component
{
public:

    State state = DEFAULT_S;
    State prevState = DEFAULT_S;

    CState() {}

    CState(State state) :
        state(state),
        prevState(state)
    {}
};

class CGravity : public Component
{
public:

    float acceleration = 1.0f;

    CGravity() {};

    CGravity(float acceleration) :
        acceleration(acceleration)
    {}
};

class CRectShape : public Component
{
public:

    sf::RectangleShape shape;

    CRectShape() {};

    CRectShape(float width, float height, const sf::Color& fill,
           const sf::Color& outline, float outlineThickness) :
        shape(sf::Vector2f(width, height))
    {
        shape.setFillColor(fill);
        shape.setOutlineColor(outline);
        shape.setOutlineThickness(outlineThickness);
        shape.setOrigin(sf::Vector2f(width / 2, height / 2));
    }
};

class CInput : public Component
{
public:

    bool isLocked  = false;

    bool up        = false;
    bool down      = false;
    bool left      = false;
    bool right     = false;
    bool jump      = false;
    bool crouch    = false;
    bool primary   = false;
    bool secondary = false;

    CInput() {};
};

class CLifespan : public Component
{
public:

    float total = 0.0f;
    float remaining = 0.0f;

    CLifespan() {};

    CLifespan(float total) :
        total(total),
        remaining(total)
    {}
};

class CRectCollider : public Component
{
public:

    float width  = 0.0f;
    float height = 0.0f;

    float halfWidth  = 0.0f;
    float halfHeight = 0.0f;

    bool isTrigger = false;
    bool triggered = false;
    //std::shared_ptr<Entity> triggerer;

    int collisionPriority = 0;

    bool isCollidedTop   = false;
    bool isCollidedBot   = false;
    bool isCollidedLeft  = false;
    bool isCollidedRight = false;

    sf::RectangleShape shape;
    bool showCollider = false;

    CRectCollider() {};

    CRectCollider(float width, float height, int collisionPriority = 0, bool isTrigger = false) :
        width(width),
        height(height),
        halfWidth(width / 2),
        halfHeight(height / 2),
        collisionPriority(collisionPriority),
        isTrigger(isTrigger)
    {
        shape.setSize(sf::Vector2f(width - 1, height - 1));
        shape.setFillColor(sf::Color::Transparent);
        shape.setOutlineColor(sf::Color::Green);
        shape.setOutlineThickness(1);
        shape.setOrigin(sf::Vector2f(width / 2, height / 2));
    }
};

class CHealth : public Component
{
public:
    
    float health = 100;

    bool isInvulnerable = false;

    CHealth() {}
};

class CCoolDown : public Component
{
public:

    size_t primaryDuration = 0;
    size_t primaryCurrent = 0;
    bool   isReadyPrimary = false;

    size_t secondaryDuration = 0;
    size_t secondaryCurrent = 0;
    bool   isReadySecondary = false;

    CCoolDown() {}

    CCoolDown(size_t primaryDuration, size_t primaryCurrent = 0, bool startReadyPrimary = false,
              size_t secondaryDuration = 0, size_t secondaryCurrent = 0, bool startReadySecondary = false) :
        primaryDuration(primaryDuration),
        primaryCurrent(primaryCurrent),
        isReadyPrimary(startReadyPrimary),
        secondaryDuration(secondaryDuration),
        secondaryCurrent(secondaryCurrent),
        isReadySecondary(startReadySecondary)
    {}
};