#pragma once

#include "Scene.h"
#include "Physics.h"

class GameScene : public Scene
{
public:

    GameScene(GameEngine& game) :
        Scene(game)
    {}

    void init() override;
    void runSystems() override;
    void sRender() override;
    void sDoAction(const Action& action) override;

    void sMovement();
    void sAnimate();
    void sPlayerState();
    void sPlayerInput();
    void sLifespan();
    void sCollision();
    void sProjectileCollision();
    void sPlayerCollision();
    void sCoolDown();

    void spawnPlayer();
    void shootProjectile();
    void spawnPlatforms();
    void respawnPlayer();
    void animatePlayer();

    int m_placeCooldown = 0;

    ~GameScene() {};
};