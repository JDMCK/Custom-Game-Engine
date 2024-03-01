#pragma once

#include "Entity.h"

typedef std::shared_ptr<Entity> EntityPtr;

struct Collision
{
    float ox = 0;
    float oy = 0;

    Collision(float ox, float oy) :
        ox(ox),
        oy(oy)
    {}
};

class Physics
{
public:

    static const float GRAVITY;

    static Collision getCollisionAABB(EntityPtr e1, EntityPtr e2, bool prev=false);
    static bool isCollidedAABB(Collision c);
    static void resolveCollisionSimpleAABB(EntityPtr e1, EntityPtr e2, Collision col);
    static void resolveCollisionPhysicsAABB(EntityPtr e1, EntityPtr e2, Collision col);
};