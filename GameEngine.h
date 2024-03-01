#pragma once

class Scene;

#include <SFML/Graphics.hpp>
#include "Assets.h"
#include "SceneKey.h"

class GameEngine
{
    sf::RenderWindow m_window;
    int              m_windowWidth;
    int              m_windowHeight;

    std::map<SceneKey, std::shared_ptr<Scene>> m_scenes;
    SceneKey                                   m_currentSceneKey;
    std::shared_ptr<Scene>                     m_currentScene;

    Assets m_assets;
    
    bool m_running = false;

    void userInput();
    void update();


public:

    GameEngine();

    void changeScene(SceneKey sceneKey, std::shared_ptr<Scene> scene, bool endCurrentScene = false);

    void run();
    void quit();

    Assets& getAssets();
    sf::RenderWindow& getWindow();
    const bool isRunning() const;
    const std::shared_ptr<Scene> getCurrentScene() const;
};