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
    CC_SAFE_RELEASE_NULL(this->hands);
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
    
    // トランプ画像の読み込み
    TextureCache* textureCache { Director::getInstance()->getTextureCache() };
    for (int i { 1 } ; i <= 13; ++i){
        textureCache->addImage(StringUtils::format("s%d.png", i));
        textureCache->addImage(StringUtils::format("h%d.png", i));
        textureCache->addImage(StringUtils::format("d%d.png", i));
        textureCache->addImage(StringUtils::format("c%d.png", i));
    }

    // 手札の初期化
    cardSprites.clear();

    // 手札の表示
    for(int i { 0 }; i < HANDS_MAX; ++i) {
        // スプライトを作成して配置
        Sprite* sprite { Sprite::create("card_1.png") };
        sprite->setPosition(Vec2((226.0f * i) + 116.0f, 288.0f));
        this->addChild(sprite, 5, i);

        // スプライトを非表示にする
        sprite->setVisible(false);

        // スプライトを配列に保存
        cardSprites.pushBack(sprite);
    }
    
    // HOLDボタンとHOLD画像
    for(int i = 0; i < HANDS_MAX; ++i) {
        // HOLDボタン
        ui::Button* holdButton { ui::Button::create("card_on.png") };
        holdButton->setPosition(Vec2((226.0f * i) + 116.0f, 288.0f));
        holdButton->addTouchEventListener(CC_CALLBACK_2(GameScene::onHoldButtonTouched, this));
        holdButton->setOpacity(0);
        this->holdButtons.pushBack(holdButton);
        this->addChild(holdButton, 2, i);
        
        // HOLD画像
        Sprite* holdSprite { Sprite::create("hold.png") };
        holdSprite->setPosition(Vec2((226.0f * i) + 116.0f, 288.0f));
        this->holdSprites.pushBack(holdSprite);
        this->addChild(holdSprite, 6, i);
    }

    // ステータスをBETに設定
    this->gameStatus = GameStatus::BET;

    // ステータスに応じた画面制御
    this->changeScreen();
}

// ゲームの初期設定
void GameScene::setupGame()
{
    // クレジット
    this->credit = START_CREDIT;
    
    // デッキ
    this->deck = Deck::create();
    CC_SAFE_RETAIN(this->deck);
    
    // 手札
    this->hands = Hands::create();
    CC_SAFE_RETAIN(this->hands);
}

// ゲームのステータスに応じた画面制御
void GameScene::changeScreen()
{
    switch (gameStatus)
    {
        case GameStatus::BET:
        {
            // BETボタン：押せる
            this->betButton->setBright(true);
            this->betButton->setTouchEnabled(true);

            // DEALボタン：押せない
            this->dealButton->setBright(false);
            this->dealButton->setTouchEnabled(false);

            for(int i = 0; i < HANDS_MAX; ++i) {
                // HOLDボタン: 押せない
                this->holdButtons.at(i)->setVisible(false);

                // HOLD表示：非表示
                this->holdSprites.at(i)->setVisible(false);
            }
            break;
        }
        case GameStatus::BET_WAIT:
        {
            // BETボタン：押せない
            this->betButton->setBright(false);
            this->betButton->setTouchEnabled(false);

            // DEALボタン：押せない
            this->dealButton->setBright(false);
            this->dealButton->setTouchEnabled(false);

            // HOLD
            for(int i = 0; i < HANDS_MAX; ++i) {
                // HOLDボタン：押せない
                this->holdButtons.at(i)->setVisible(false);

                // HOLD表示：非表示
                this->holdSprites.at(i)->setVisible(false);
            }
            break;
        }
        case GameStatus::HOLD:
            break;
            
        case GameStatus::DEAL:
            break;
            
        case GameStatus::OVER:
            break;
            
        default:
            break;
    }
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

            // デッキにカードをセット
            this->deck->setAllCard();

            // デッキをシャッフル
            this->deck->shuffle();

            // 手札を配る
            for (int i { 0 } ; i < HANDS_MAX; ++i)
            {
                Card *card { deck->dealCard() };
                hands->setCard(i, card);
            }

            // ステータスをBET_WAITに設定
            this->gameStatus = GameStatus::BET_WAIT;

            // ステータスに応じた画面制御
            this->changeScreen();
            
            // BETした時のアクション
            this->betAction();

            break;
        }
        default:
            break;
    }
}

// BETした時のアクション
void GameScene::betAction() {

    // 配るアクションを実行する
    for (int i { 0 }; i < HANDS_MAX; i++){
        Sprite* cardSprite { this->cardSprites.at(i) };
        
        // カードを全て裏面にする
        Texture2D* cardTexture = Director::getInstance()->getTextureCache()->getTextureForKey("card_1.png");
        cardSprite->setTexture(cardTexture);

        // カードの表示位置を変更
        cardSprite->setPosition(Vec2(cardSprite->getPositionX(), 480.0f));

        // カードを表示
        cardSprite->setVisible(true);

        // カードを透明にする
        cardSprite->setOpacity(0);
        
        // カードの横幅を元に戻す
        cardSprite->setScaleX(1.0f);

        // アクションを作成
        Vector<FiniteTimeAction *>  moveActions;
        DelayTime::create(i * 0.2f);
        moveActions.pushBack(MoveTo::create(0.4f, Vec2(cardSprite->getPositionX(), 288.0f)));
        moveActions.pushBack(FadeIn::create(0.4f));
        Spawn* moveAction { Spawn::create(moveActions) };
        
        // アクションをまとめる
        Vector<FiniteTimeAction *> betActions;
        betActions.pushBack(DelayTime::create(0.2f * i));
        betActions.pushBack(moveAction);

        // 表にめくるアクション
        betActions.pushBack(ScaleTo::create(0.3f, 0.0f, 1.0f));
        betActions.pushBack(CallFunc::create([this, cardSprite](){
            // テクスチャを変更
            Card *card { this->hands->getCard(cardSprite->getTag()) };
            Texture2D* cardTexture = Director::getInstance()->getTextureCache()->getTextureForKey(card->getFileName());
            cardSprite->setTexture(cardTexture);
        }));
        betActions.pushBack(ScaleTo::create(0.3f, 1, 1));
        
        Sequence* betAction { Sequence::create(betActions) };
        
        // アクションを実行
        cardSprite->runAction(betAction);
    }
    
    return;
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

// HOLDボタンがタッチされたとき
void GameScene::onHoldButtonTouched(Ref *pSender, ui::Widget::TouchEventType type)
{
    Sprite *sprite { static_cast<Sprite*>(pSender) };
    
    switch (type)
    {
        case ui::Widget::TouchEventType::BEGAN:

            // HOLD状態を変更する
            CCLOG("HOLD:%d isHold:%d", sprite->getTag(), this->hands->toggleHold(sprite->getTag()));
            
            break;
        default:
            break;
    }
}
