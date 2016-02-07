//
//  GameScene.cpp
//  PokerGame
//

#include "GameScene.hpp"

#include "Common.h"

Scene* GameScene::createScene()
{
    // シーンの作成
    Scene* scene { Scene::create() };
    
    // レイヤーの作成
    Layer* layer { GameScene::create() };
    scene->addChild(layer);

    return scene;
}

// コンストラクタ
GameScene::GameScene()
{}

// デストラクタ
GameScene::~GameScene()
{
    CC_SAFE_RELEASE_NULL(this->deck);
}

// 初期化
bool GameScene::init()
{
    // レイヤーの作成
    if(!Layer::init())
    {
        return false;
    }

    // ゲームの初期設定
    this->setupGame();

    // 画面表示の設定
    this->setupScreen();
    
    return true;
}

// 画面の設定
void GameScene::setupScreen()
{
    // 画面サイズ
    Size visibleSize = Director::getInstance()->getVisibleSize();
    
    // 背景画像
    Sprite* bg { Sprite::create("bg.png") };
    bg->setPosition(visibleSize / 2);
    this->addChild(bg, 1);
   
    // カードを置くエリア
    Sprite* bgCardArea { Sprite::create("bg_card_area.png") };
    bgCardArea->setPosition(Vec2(568.0f, 288.0f));
    this->addChild(bgCardArea, 2);
    
    // 得点表
    Sprite* bgScoreMenu { Sprite::create("bg_score_menu.png") };
    bgScoreMenu->setPosition(Vec2(772.0f, 544.0f));
    this->addChild(bgScoreMenu, 3);
    
    // ロゴ
    Sprite* bgLogo { Sprite::create("bg_logo.png") };
    bgLogo->setPosition(Vec2(204.0f, 544.0f));
    this->addChild(bgLogo, 4);
    
    // BETボタン
    betButton = ui::Button::create("BET.png", "BET_on.png", "BET_off.png");
    betButton->setPosition(Vec2(795.0f, 76.0f));
    this->addChild(betButton, 5);
    betButton->addTouchEventListener(CC_CALLBACK_2(GameScene::onBetButtonTouched, this));
    
    // DEALボタン
    dealButton = ui::Button::create("DEAL.png", "DEAL_on.png", "DEAL_off.png");
    dealButton->setPosition(Vec2(1022.0f, 76.0f));

    // ボタンの表示を非活性にする
    dealButton->setBright(false);

    // タッチ無効化する
    dealButton->setTouchEnabled(false);

    this->addChild(dealButton, 5);
    dealButton->addTouchEventListener(CC_CALLBACK_2(GameScene::onDealButtonTouched, this));
    
    // winラベル
    winLabel = Label::createWithSystemFont(StringUtils::format("Win : $%d", win), "ariel", 28);
    winLabel->setAnchorPoint(Vec2(0, 0));
    winLabel->setPosition(Vec2(34.0f, 108.0f));
    winLabel->setColor(Color3B(255, 0, 0));
    this->addChild(winLabel, 5);
    
    // creditラベル
    creditLabel = Label::createWithSystemFont(StringUtils::format("Credit : $%d", credit), "ariel", 28);
    creditLabel->setAnchorPoint(Vec2(0, 0));
    creditLabel->setPosition(Vec2(34.0f, 76.0f));
    creditLabel->setColor(Color3B(255, 255, 255));
    this->addChild(creditLabel, 5);
}

// ゲームの初期設定
void GameScene::setupGame()
{
    // クレジット
    this->credit = START_CREDIT;
    
    // デッキ
    this->deck = Deck::create();
    CC_SAFE_RETAIN(this->deck);
}

// BETボタンがタッチされたとき
void GameScene::onBetButtonTouched(Ref *pSender, ui::Widget::TouchEventType type)
{
    switch (type)
    {
        case ui::Widget::TouchEventType::BEGAN:
        {
            // Win数の初期化
            this->win = 0;

            // クレジットからBET枚数を引く
            this->credit -= BET_NUM;

            // ラベルを更新
            this->winLabel->setString(StringUtils::format("Win : $%d", win));
            this->creditLabel->setString(StringUtils::format("Credit : $%d", credit));

            break;
        }
        default:
            break;
    }
}

// DEALボタンがタッチされたとき
void GameScene::onDealButtonTouched(Ref *pSender, ui::Widget::TouchEventType type)
{
    switch (type)
    {
        case ui::Widget::TouchEventType::BEGAN:
        {
            CCLOG("DEAL");
            break;
        }
        default:
            break;
    }
}
