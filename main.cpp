#include "GameEngine.h"
#include "GameScene.h"

int main()
{
    GameEngine game;
    game.changeScene(SceneKey::GAME_SCENE, std::make_shared<GameScene>(game));
    game.run();
}