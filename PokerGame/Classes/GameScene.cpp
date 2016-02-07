//
//  GameScene.cpp
//  PokerGame
//

#include "GameScene.hpp"

Scene* GameScene::createScene()
{
    // シーンの作成
    Scene* scene { Scene::create() };
    
    // レイヤーの作成
    Layer* layer { GameScene::create() };
    scene->addChild(layer);

    return scene;
}

// 初期化
bool GameScene::init()
{
    // レイヤーの作成
    if(!Layer::init())
    {
        return false;
    }
    
    return true;
}
