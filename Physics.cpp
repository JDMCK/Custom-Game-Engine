#include "Physics.h"

const float GRAVITY = 1.0f;

Collision Physics::getCollisionAABB(EntityPtr e1, EntityPtr e2, bool prev)
{       
    auto& cTrans1 = e1->getComponent<CTransform>();
    auto& cTrans2 = e2->getComponent<CTransform>();
    auto& cRect1 = e1->getComponent<CRectCollider>();
    auto& cRect2 = e2->getComponent<CRectCollider>();

    float dx = 0.f;
    float dy = 0.f;

    if (prev)
    {
        dx = abs(cTrans1.prevPosition.x - cTrans2.prevPosition.x);
        dy = abs(cTrans1.prevPosition.y - cTrans2.prevPosition.y);
    }
    else
    {
        dx = abs(cTrans1.position.x - cTrans2.position.x);
        dy = abs(cTrans1.position.y - cTrans2.position.y);
    }
    float ox = cRect1.halfWidth + cRect2.halfWidth - dx;
    float oy = cRect1.halfHeight + cRect2.halfHeight - dy;

    return Collision(ox, oy);
}

bool Physics::isCollidedAABB(Collision col)
{
    return (col.ox > 0 && col.oy > 0);
}

/*
Resolves collision by moving entity with lower collision priority out of entity with higher collision priority.
*/
void Physics::resolveCollisionSimpleAABB(EntityPtr e1, EntityPtr e2, Collision col)
{
    // Not resolving collisions with equal collision priority.
    if (e1->getComponent<CRectCollider>().collisionPriority == e2->getComponent<CRectCollider>().collisionPriority) return;

    // Ensures e1 has higher collision priority (0 being the highest).
    if (e2->getComponent<CRectCollider>().collisionPriority < e1->getComponent<CRectCollider>().collisionPriority)
    {
        std::shared_ptr<Entity> temp = e1;
        e1 = e2;
        e2 = temp;
    }

    auto& cRect1 = e1->getComponent<CRectCollider>();
    auto& cRect2 = e2->getComponent<CRectCollider>();
    auto& cTrans1 = e1->getComponent<CTransform>();
    auto& cTrans2 = e2->getComponent<CTransform>();

    Collision prevCol = getCollisionAABB(e1, e2, true);

    if (prevCol.ox > 0 && cTrans2.position.y < cTrans1.position.y) // top
    {
        cTrans2.position.y -= col.oy;
        cTrans2.velocity.y = 0;
        cRect2.isCollidedBot = true;
        cRect1.isCollidedTop = true;
    }
    else if (prevCol.ox > 0 && cTrans2.position.y > cTrans1.position.y) // bot
    {
        cTrans2.position.y += col.oy;
        cTrans2.velocity.y = 0;
        cRect2.isCollidedTop = true;
        cRect1.isCollidedBot = true;
    }
    else if (prevCol.oy > 0 && cTrans2.position.x < cTrans1.position.x) // left
    {
        cTrans2.position.x -= col.ox;
        //cTrans2.velocity.x = 0;
        cRect2.isCollidedRight = true;
        cRect1.isCollidedLeft = true;
    }
    else if (prevCol.oy > 0 && cTrans2.position.x > cTrans1.position.x) // right
    {
        cTrans2.position.x += col.ox;
        //cTrans2.velocity.x = 0;
        cRect2.isCollidedLeft = true;
        cRect1.isCollidedRight = true;
    } 
    else if (cTrans2.position.x > cTrans1.position.x) // diagonal cases
    {
        cTrans2.position.x += col.ox;
        cTrans2.velocity.x = 0;
        cRect2.isCollidedLeft = true;
        cRect1.isCollidedRight = true;
    }
    else
    {
        cTrans2.position.x -= col.ox;
        cTrans2.velocity.x = 0;
        cRect2.isCollidedRight = true;
        cRect1.isCollidedLeft = true;
    }
}
