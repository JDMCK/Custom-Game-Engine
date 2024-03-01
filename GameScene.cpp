#include "GameEngine.h"
#include "GameScene.h"
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <math.h>

void GameScene::sDoAction(const Action& action)
{
    CInput& cInp = m_player->getComponent<CInput>();

    switch (action.getKey())
    {
    case ActionKey::LEFT:
        cInp.left = action.isStartType();;
        break;
    case ActionKey::RIGHT:
        cInp.right = action.isStartType();;
        break;
    case ActionKey::JUMP:
        cInp.jump = action.isStartType();
        break;
    case ActionKey::PRIMARY:
        cInp.primary = action.isStartType();
        break;
    }
}

void GameScene::runSystems()
{
    sLifespan();
    sCoolDown();
    sPlayerInput();
    sPlayerState();
    sMovement();
    sCollision();
    sAnimate();
    sRender();
}

void GameScene::init()
{
    registerAction(sf::Keyboard::W,     ActionKey::UP);
    registerAction(sf::Keyboard::A,     ActionKey::LEFT);
    registerAction(sf::Keyboard::S,     ActionKey::DOWN);
    registerAction(sf::Keyboard::D,     ActionKey::RIGHT);
    registerAction(sf::Keyboard::Space, ActionKey::JUMP);
    registerAction(sf::Keyboard::E,     ActionKey::INTERACT);
    registerAction(sf::Keyboard::P,     ActionKey::PAUSE);
    registerAction(sf::Mouse::Left,     ActionKey::PRIMARY, true);
    spawnPlatforms();
    spawnPlayer();
}

void GameScene::sRender()
{
    bool showBoundingBoxes = false;

    for (auto& e : m_entityManager.getEntities())
    {
        if (e->hasComponent<CRectShape>())
            m_window.draw(e->getComponent<CRectShape>().shape);

        if (e->hasComponent<CSprite>())
            m_window.draw(e->getComponent<CSprite>().sprite);

        if (e->hasComponent<CAnimation>())
        {
            m_window.draw(e->getComponent<CAnimation>().animation.getSprite());
        }

        if (showBoundingBoxes && e->hasComponent<CRectCollider>())
        {
            m_window.draw(e->getComponent<CRectCollider>().shape);
        }
    }
}

void GameScene::sMovement()
{
    for (auto& e : m_entityManager.getEntities())
    {
        if (e->hasComponent<CTransform>())
        {
            auto& cTrans = e->getComponent<CTransform>();
            cTrans.prevPosition = cTrans.position;
            if (e->hasComponent<CRectShape>())
            {
                e->getComponent<CRectShape>().shape.setPosition(cTrans.position.x, cTrans.position.y);
            }

            if (e->hasComponent<CGravity>())
            {
                cTrans.velocity.y = std::min(50, (int) (cTrans.velocity.y + e->getComponent<CGravity>().acceleration));
            }
            cTrans.position += cTrans.velocity;
        }
    }
}

void GameScene::sAnimate()
{
    // Apply animation logic to specific entities first before animation updates are applied.
    animatePlayer();

    for (auto& e : m_entityManager.getEntities())
    {
        if (e->hasComponent<CTransform>())
        {
            auto& cTrans = e->getComponent<CTransform>();
            if (e->hasComponent<CRectShape>())
            {
                CRectShape& cShape = e->getComponent<CRectShape>();
                cShape.shape.setPosition(cTrans.position.x, cTrans.position.y);
                cShape.shape.setRotation(cTrans.rotation);
            }
            if (e->hasComponent<CSprite>())
            {
                CSprite& cSprite = e->getComponent<CSprite>();
                cSprite.sprite.setPosition(cTrans.position.x, cTrans.position.y);
                cSprite.sprite.setRotation(cTrans.rotation);
            }
            if (e->hasComponent<CAnimation>())
            {
                CAnimation& cAnim = e->getComponent<CAnimation>();

                cAnim.animation.update();

                auto& scale = cAnim.animation.getSprite().getScale();
                cAnim.animation.getSprite().setScale((cAnim.isFacingRight ? 1 : -1) * std::abs(scale.x), scale.y);
                cAnim.animation.getSprite().setPosition(cTrans.position.x, cTrans.position.y);
                cAnim.animation.getSprite().setRotation(cTrans.rotation);

                if (!cAnim.animation.isRepeat() && cAnim.animation.hasEnded() && cAnim.animation.isDestroyOnEnd())
                {
                    m_entityManager.removeEntity(e->id());
                }
            }
            if (e->hasComponent<CRectCollider>())
            {
                CRectCollider& cRect = e->getComponent<CRectCollider>();
                cRect.shape.setPosition(cTrans.position.x, cTrans.position.y);
            }
        }
    }
}

void GameScene::animatePlayer()
{
    CAnimation& cAnim = m_player->getComponent<CAnimation>();
    CState& cState = m_player->getComponent<CState>();

    if (cState.state != cState.prevState)
    {
        switch (cState.state)
        {
        case State::RUN_S:
            cAnim.animation = m_game.getAssets().getAnimation("Run");
            break;
        case State::IDLE_S:
            cAnim.animation = m_game.getAssets().getAnimation("Idle");
            break;
        case State::JUMP_S:
            cAnim.animation = m_game.getAssets().getAnimation("Jump");
            break;
        case State::FALL_S:
            cAnim.animation = m_game.getAssets().getAnimation("Fall");
            break;
        }
        cState.prevState = cState.state;
    }
}

void GameScene::sPlayerState()
{
    CState& cState = m_player->getComponent<CState>();
    CAnimation& cAnim = m_player->getComponent<CAnimation>();
    CTransform& cTran = m_player->getComponent<CTransform>();
    CInput& cInp = m_player->getComponent<CInput>();
    CRectCollider& cRect = m_player->getComponent<CRectCollider>();

    if (cInp.left)
    {
        cAnim.isFacingRight = false;
    }
    else if (cInp.right)
    {
        cAnim.isFacingRight = true;
    }
    
    if (cRect.isCollidedBot)
    {
        if (cInp.left)
        {
            cState.state = State::RUN_S;
        }
        else if (cInp.right)
        {
            cState.state = State::RUN_S;
        }
        else {
            cState.state = State::IDLE_S;
        }
    }
    else if (cTran.velocity.y < 0)
    {
        cState.state = State::JUMP_S;
    }
    else if (cTran.velocity.y > 0)
    {
        cState.state = State::FALL_S;
    }
}

void GameScene::sPlayerInput()
{
    const float PLAYER_MOVEMENT_SPEED = 7.f;
    const float PLAYER_JUMP_POWER = 20.f;

    CTransform& cTran = m_player->getComponent<CTransform>();
    CInput& cInp = m_player->getComponent<CInput>();
    CRectCollider& cRect = m_player->getComponent<CRectCollider>();

    cTran.velocity.x = (-cInp.left + cInp.right) * PLAYER_MOVEMENT_SPEED;
    if (cRect.isCollidedBot && cInp.jump)
        cTran.velocity.y = -PLAYER_JUMP_POWER;

    if (cInp.primary)
        shootProjectile();
}

void GameScene::shootProjectile()
{
    const float PROJECTILE_SPEED = 10.f;

    CCoolDown& cCool = m_player->getComponent<CCoolDown>();
    CTransform& cTrans = m_player->getComponent<CTransform>();

    Vec2 velocity = (Vec2(sf::Mouse::getPosition(m_window)) - cTrans.position).normalize() * PROJECTILE_SPEED;
    float rotation = velocity.rotation();

    if (cCool.isReadyPrimary) {
        float scale = 4.f;
        auto projectile = m_entityManager.addEntity(Tag::PROJECTILE);
        projectile->addComponent<CTransform>(cTrans.position, velocity, rotation);
        projectile->addComponent<CAnimation>(m_game.getAssets().getAnimation("Lightning_Ball"));
        projectile->addComponent<CRectCollider>(10 * scale, 10 * scale, 1, true);
        projectile->addComponent<CLifespan>(60 * 5);

        m_player->getComponent<CCoolDown>().primaryCurrent = 0;
    }
}

void GameScene::sLifespan()
{
    for (auto& e : m_entityManager.getEntities())
    {
        if (e->hasComponent<CLifespan>())
        {
            e->getComponent<CLifespan>().remaining--;
            if (e->getComponent<CLifespan>().remaining <= 0) e->destroy();
        }
    }
}

void GameScene::sCoolDown()
{
    for (auto& e : m_entityManager.getEntities())
    {
        if (e->hasComponent<CCoolDown>())
        {
            CCoolDown& cCool = e->getComponent<CCoolDown>();
            cCool.isReadyPrimary = cCool.primaryCurrent >= cCool.primaryDuration;
            cCool.primaryCurrent = std::min(++cCool.primaryCurrent, cCool.primaryDuration);

            cCool.isReadySecondary = cCool.secondaryCurrent >= cCool.secondaryDuration;
            cCool.secondaryCurrent = std::min(++cCool.secondaryCurrent, cCool.secondaryDuration);
        }
    }
}

void GameScene::sCollision()
{
    // Resetting all collisions to false and fixing stationary objects in place.
    for (auto& e : m_entityManager.getEntities())
    {
        if (e->hasComponent<CRectCollider>())
        {
            auto& cRect = e->getComponent<CRectCollider>();
            cRect.isCollidedBot = false;
            cRect.isCollidedTop = false;
            cRect.isCollidedLeft = false;
            cRect.isCollidedRight = false;
            cRect.triggered = false;
        }
    }

    for (int i = 0; i < m_entityManager.getEntities().size() - 1; i++)
    {
        for (int j = i + 1; j < m_entityManager.getEntities().size(); j++)
        {
            std::shared_ptr<Entity> e1 = m_entityManager.getEntities()[i];
            std::shared_ptr<Entity> e2 = m_entityManager.getEntities()[j];

            if (e1->hasComponent<CRectCollider>() && e2->hasComponent<CRectCollider>())
            {
                CRectCollider& cRect1 = e1->getComponent<CRectCollider>();
                CRectCollider& cRect2 = e2->getComponent<CRectCollider>();

                Collision col = Physics::getCollisionAABB(e1, e2);
                if (Physics::isCollidedAABB(col))
                {
                    bool continueFlag = false;
                    if (cRect1.isTrigger && cRect1.collisionPriority > cRect2.collisionPriority)
                    {
                        continueFlag = true;
                        cRect1.triggered = true;
                        //cRect1.triggerer = e2;
                    }
                    if (cRect2.isTrigger && cRect2.collisionPriority > cRect1.collisionPriority)
                    {
                        continueFlag = true;
                        cRect2.triggered = true;
                        //cRect2.triggerer = e1;
                    }
                    if (continueFlag) continue;

                    Physics::resolveCollisionSimpleAABB(e1, e2, col);
                }
            }
        }
    }
    sPlayerCollision();
    sProjectileCollision();
}

void GameScene::sProjectileCollision()
{
    for (auto& p : m_entityManager.getEntitiesByTag(Tag::PROJECTILE))
    {
        CRectCollider& cRect = p->getComponent<CRectCollider>();
        CAnimation& cAnim = p->getComponent<CAnimation>();
        CTransform& cTrans = p->getComponent<CTransform>();
        CLifespan& cLife = p->getComponent<CLifespan>();

        if (cRect.triggered)
        {
            cAnim.animation = m_game.getAssets().getAnimation("Explosion");
            cTrans.velocity = Vec2::ZERO;
            cTrans.rotation = 0.f;
            p->removeComponent<CRectCollider>();
        }
    }
}

void GameScene::sPlayerCollision()
{
    // Bottom of screen collision
    if (m_player->getComponent<CTransform>().position.y + m_player->getComponent<CRectCollider>().halfHeight > height())
    {
        m_player->getComponent<CTransform>().position.y = height() - m_player->getComponent<CRectCollider>().halfHeight;
        m_player->getComponent<CTransform>().velocity.y = 0;
        m_player->getComponent<CRectCollider>().isCollidedBot = true;
    }
}

void GameScene::respawnPlayer()
{
    m_player->getComponent<CTransform>().position = Vec2(width() / 2, height() / 2);
}

void GameScene::spawnPlatforms()
{
    auto p1 = m_entityManager.addEntity(Tag::PLATFORM);
    p1->addComponent<CTransform>(Vec2(width() / 2, height() - 100));
    p1->addComponent<CRectCollider>(200, 30);
    p1->addComponent<CRectShape>(200, 30, sf::Color::Magenta, sf::Color::White, 0);

    auto p2 = m_entityManager.addEntity(Tag::PLATFORM);
    p2->addComponent<CTransform>(Vec2(width() - 200, height() - 100));
    p2->addComponent<CRectCollider>(30, 175);
    p2->addComponent<CRectShape>(30, 175, sf::Color::Cyan, sf::Color::White, 0);

    auto p3 = m_entityManager.addEntity(Tag::PLATFORM);
    p3->addComponent<CTransform>(Vec2(width() - 600, height() - 200));
    p3->addComponent<CRectCollider>(30, 30);
    p3->addComponent<CRectShape>(30, 30, sf::Color::Yellow, sf::Color::White, 0);
}

void GameScene::spawnPlayer()
{
    m_player = m_entityManager.addEntity(Tag::PLAYER);

    float mx = width() / 2.f;
    float my = height() / 2.f;

    float scale = 4.f;

    m_player->addComponent<CTransform>(Vec2(50, 50));
    m_player->addComponent<CAnimation>(m_game.getAssets().getAnimation("Idle"));
    m_player->addComponent<CState>(State::IDLE_S);
    m_player->addComponent<CRectCollider>(11 * scale, 22 * scale , 1);
    m_player->getComponent<CRectCollider>().shape.setPosition(mx, my);
    m_player->addComponent<CGravity>();
    m_player->addComponent<CInput>();
    m_player->addComponent<CCoolDown>(60);
}