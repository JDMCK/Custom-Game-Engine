#include "GameEngine.h"
#include "Scene.h"

GameEngine::GameEngine() :
    m_assets("assets_config.txt")
{
    m_window.create(sf::VideoMode(1280, 720), "Game");
    m_window.setKeyRepeatEnabled(false);
    m_window.setFramerateLimit(60);

    m_windowWidth = 1280;
    m_windowHeight = 720;
}

void GameEngine::update()
{
    m_window.clear();
    userInput();
    m_currentScene->update();
    m_window.display();
}

void GameEngine::run()
{
    m_running = true;
    while (m_window.isOpen())
    {
        update();
    }
}

void GameEngine::userInput()
{
    sf::Event event;
    while (m_window.pollEvent(event))
    {
        switch (event.type)
        {
            case sf::Event::Closed:
                m_window.close();
                break;

            case sf::Event::KeyPressed:
            case sf::Event::KeyReleased:
            {
                // If the current scene does not have a registered action with that input, skip this event.
                if (m_currentScene->getActionMap().find(event.key.code) == m_currentScene->getActionMap().end()) continue;
                bool isStartType = (event.type == sf::Event::KeyPressed);
                m_currentScene->sDoAction(Action(m_currentScene->getActionMap().at(event.key.code), isStartType));
                break;
            }
            case sf::Event::MouseButtonPressed:
            case sf::Event::MouseButtonReleased:
            {
                // If the current scene does not have a registered action with that input, skip this event.
                if (m_currentScene->getActionMap().find(event.mouseButton.button) == m_currentScene->getActionMap().end()) continue;
                bool isStartType = (event.type == sf::Event::MouseButtonPressed);
                // If it is a mouse input, value will always be +100 to distinguish from key inputs.
                m_currentScene->sDoAction(Action(m_currentScene->getActionMap().at(event.mouseButton.button + 100), isStartType));
                break;
            }
        }
    }
}

void GameEngine::changeScene(SceneKey sceneKey, std::shared_ptr<Scene> scene, bool endCurrentScene)
{
    // Ends current scene
    if (endCurrentScene)
    {
        // Setting key's value to a nullptr kills Scene (because of smart ptr).
        m_scenes[m_currentSceneKey] = nullptr;
    }

    // Add scene to map if it doesn't exist, else switch to it.
    m_currentScene = m_scenes[sceneKey];
    if (!m_currentScene)
    {
        m_scenes[sceneKey] = scene;
        m_currentScene = scene;
        m_currentScene->init();
    }
    m_currentSceneKey = sceneKey;
}

void GameEngine::quit()
{
    m_scenes.~map();
    m_window.close();
}

const bool GameEngine::isRunning() const
{
    return m_running;
}

const std::shared_ptr<Scene> GameEngine::getCurrentScene() const
{
    return m_currentScene;
}

Assets& GameEngine::getAssets()
{
    return m_assets;
}

sf::RenderWindow& GameEngine::getWindow()
{
    return m_window;
}