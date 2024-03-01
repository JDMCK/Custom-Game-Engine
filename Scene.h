#pragma once

class GameEngine;

#include "EntityManager.h"
#include "SceneKey.h"
#include "Action.h"

class Scene
{

protected:
    GameEngine&              m_game;
    sf::RenderWindow&        m_window;
    EntityManager            m_entityManager;
    std::map<int, ActionKey> m_actionMap;
    bool                     m_paused = false;

    bool                     m_hasSceneEnded = false;
    bool                     m_hasSceneChanged = false;
    SceneKey                 m_nextSceneKey;
    std::shared_ptr<Scene>   m_nextScene;

    size_t                   m_currentFrame = 0;

    std::shared_ptr<Entity>  m_player;

    void setPaused(bool paused);

public:

    Scene(GameEngine& game);

    void update();
    virtual void init() = 0;
    virtual void runSystems() = 0;
    virtual void sRender() = 0;
    virtual void sDoAction(const Action& action) = 0;

    void registerAction(int inputKey, ActionKey actionKey, bool isMouse = false);

    size_t width() const;
    size_t height() const;

    bool hasSceneEnded() const;
    bool hasSceneChanged() const;
    void initSceneChange(SceneKey sceneKey, std::shared_ptr<Scene>, bool endCurrentScene);
    const std::map<int, ActionKey>& getActionMap() const;

    ~Scene() {}
};