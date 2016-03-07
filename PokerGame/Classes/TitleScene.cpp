//
//  TitleScene.cpp
//  PokerGame
//

#include "TitleScene.hpp"
#include "GameScene.hpp"
#include "audio/include/AudioEngine.h"

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

    // 画面サイズ
    Size visibleSize { Director::getInstance()->getVisibleSize() };
    
    // 背景
    Sprite* bgSprite { Sprite::create("title_bg.png") };
    bgSprite->setPosition(visibleSize / 2);
    this->addChild(bgSprite);

    // ロゴ
    Sprite* logoSprite { Sprite::create("title_logo.png") };
    logoSprite->setPosition(Vec2(568.0f, 400.0f));
    this->addChild(logoSprite);
    
    // スタートボタン
    ui::Button* startButton { ui::Button::create("title_start.png") };
    startButton->setPosition(Vec2(568.0f, 130.0f));
    this->addChild(startButton);
    startButton->addTouchEventListener(CC_CALLBACK_2(TitleScene::touchEvent, this));

    // BGMを鳴らす
    cocos2d::experimental::AudioEngine::play2d("title.wav", true);

    return true;
}

// ボタンのタッチイベント
void TitleScene::touchEvent(Ref *pSender, ui::Widget::TouchEventType type)
{
    switch (type)
    {
        case ui::Widget::TouchEventType::BEGAN:
        {
            // 音楽を止める
            cocos2d::experimental::AudioEngine::stopAll();

            // SEを鳴らす
            cocos2d::experimental::AudioEngine::play2d("se_coin.wav", false);

            Scene* gameScene { GameScene::createScene() };
            TransitionFade* fade = TransitionFade::create(1.0f, gameScene);
            Director::getInstance()->replaceScene(fade);

            break;
        }
        default:
            break;
    }
}

