//
//  TitleScene.cpp
//  PokerGame
//

#include "TitleScene.hpp"

USING_NS_CC;

Scene* TitleScene::createScene()
{
    // 'scene' is an autorelease object
    Scene* scene { Scene::create() };
    
    // 'layer' is an autorelease object
    Layer* layer { TitleScene::create() };
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool TitleScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    return true;
}
