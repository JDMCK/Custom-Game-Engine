#include "Scene.h"
#include "GameEngine.h"

Scene::Scene(GameEngine& game) :
    m_game(game),
    m_window(game.getWindow())
{}

void Scene::update()
{
    m_entityManager.update();
    runSystems();
    if (m_hasSceneChanged)
    {
        m_game.changeScene(m_nextSceneKey, m_nextScene, m_hasSceneEnded);
    }
}

void Scene::registerAction(int inputKey, ActionKey actionKey, bool isMouse)
{
    // If it is a mouse input, value will always be +100 to distinguish from key inputs.
    m_actionMap[inputKey + (isMouse ? 100 : 0)] = actionKey;
}

size_t Scene::width() const
{
    return m_window.getSize().x;
}

size_t Scene::height() const
{
    return m_window.getSize().y;
}

bool Scene::hasSceneEnded() const
{
    return m_hasSceneEnded;
}

bool Scene::hasSceneChanged() const
{
    return m_hasSceneChanged;
}

void Scene::initSceneChange(SceneKey sceneKey, std::shared_ptr<Scene> scene, bool endCurrentScene)
{
    m_hasSceneChanged = true;
    m_hasSceneEnded = endCurrentScene;
    m_nextSceneKey = sceneKey;
    m_nextScene = scene;
}

const std::map<int, ActionKey>& Scene::getActionMap() const
{
    return m_actionMap;
}

void Scene::setPaused(bool paused)
{
    m_paused = paused;
}