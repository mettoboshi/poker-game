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
        {
            // BETボタン：押せない
            this->betButton->setBright(false);
            this->betButton->setTouchEnabled(false);

            // DEALボタン：押せる
            this->dealButton->setBright(true);
            this->dealButton->setTouchEnabled(true);

            // HOLD
            for(int i = 0; i < HANDS_MAX; ++i) {
                // HOLDボタン：押せる
                this->holdButtons.at(i)->setVisible(true);

                // HOLD表示：非表示
                holdSprites.at(i)->setVisible(false);
            }
            break;
        }
        case GameStatus::DEAL:
        {
            // BETボタン：押せない
            this->betButton->setBright(false);
            this->betButton->setTouchEnabled(false);

            // DEALボタン：押せない
            this->dealButton->setBright(false);
            this->dealButton->setTouchEnabled(false);

            // HOLD
            for(int i { 0 }; i < HANDS_MAX; i++) {
                // HOLDボタン：押せない
                holdButtons.at(i)->setVisible(false);

                // HOLD表示：そのまま
            }
            break;
        }
        case GameStatus::OVER:
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
        default:
            break;
    }
}

// カードのアクションを実行する
void GameScene::cardAction(Sprite* sprite, GameStatus nextStatus, bool isLast)
{
    // カードを裏面にする
    Texture2D* cardTexture = Director::getInstance()->getTextureCache()->getTextureForKey("card_1.png");
    sprite->setTexture(cardTexture);
    
    // カードの表示位置を変更
    sprite->setPosition(Vec2(sprite->getPositionX(), 480.0f));
    
    // カードを表示
    sprite->setVisible(true);
    
    // カードを透明にする
    sprite->setOpacity(0);
    
    // カードの横幅を元に戻す
    sprite->setScaleX(1.0f);
    
    // アクションを作成
    Vector<FiniteTimeAction *>  moveActions;
    DelayTime::create(sprite->getTag() * 0.2f);
    moveActions.pushBack(MoveTo::create(0.4f, Vec2(sprite->getPositionX(), 288.0f)));
    moveActions.pushBack(FadeIn::create(0.4f));
    Spawn* moveAction { Spawn::create(moveActions) };
    
    // アクションをまとめる
    Vector<FiniteTimeAction *> betActions;
    betActions.pushBack(DelayTime::create(0.2f * sprite->getTag()));
    betActions.pushBack(moveAction);
    
    // 表にめくるアクション
    betActions.pushBack(ScaleTo::create(0.3f, 0.0f, 1.0f));
    betActions.pushBack(CallFunc::create([this, sprite](){
        // テクスチャを変更
        Card *card { this->hands->getCard(sprite->getTag()) };
        Texture2D* cardTexture = Director::getInstance()->getTextureCache()->getTextureForKey(card->getFileName());
        sprite->setTexture(cardTexture);
    }));
    betActions.pushBack(ScaleTo::create(0.3f, 1, 1));
    
    // ステータスを変更し、ボタンの制御、手役の決定をおこなう
    betActions.pushBack(CallFunc::create([this, nextStatus, isLast](){
        if(isLast) {
            // スプライトのタグがHAND_MAX - 1のとき、ステータスを変更する
            this->gameStatus = nextStatus;

            // ステータスに応じた画面制御
            this->changeScreen();
            
            // 手役の確定
            this->hands->dicisionHand();          
        }
    }));
    
    Sequence* betAction { Sequence::create(betActions) };
    
    // アクションを実行
    sprite->runAction(betAction);
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

            // DEBUG:つみこみ
            hands->setCard(0, Card::create(Suit::SPADE, 1));
            hands->setCard(1, Card::create(Suit::SPADE, 3));
            hands->setCard(2, Card::create(Suit::SPADE, 5));
            hands->setCard(3, Card::create(Suit::SPADE, 7));
            hands->setCard(4, Card::create(Suit::SPADE, 9));
            
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

        // アクションを実行
        this->cardAction(cardSprite, GameStatus::HOLD, i == HANDS_MAX - 1 ? true : false);
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
            // ステータスをDEALに設定
            this->gameStatus = GameStatus::DEAL;

            // ステータスに応じた画面制御
            this->changeScreen();
            
            // 手札を配る
            for (int i { 0 }; i < HANDS_MAX; i++){
                if (this->hands->isHold(i) == false) {
                    hands->setCard(i, deck->dealCard());
                }
            }

            // アクションを実行
            this->dealAction();

            break;
        }
        default:
            break;
    }
}

// DEAL時のアクション
void GameScene::dealAction()
{
    // HOLDされていないカードを探す
    Vector<Sprite*> changedCards {};
    for (int i { 0 }; i < HANDS_MAX; i++){
        Sprite* cardSprite { this->cardSprites.at(i) };

        // ホールドされている場合は配り直さない
        if(this->hands->isHold(cardSprite->getTag())) continue;
        
        changedCards.pushBack(cardSprites.at(i));
    }

    // 全てがHOLDされている場合はカードアクションは実行せず結果を表示する
    if(changedCards.empty())
    {
        // ステータスをOVERに変更
        this->gameStatus = GameStatus::OVER;
        
        // ステータスに応じた画面制御
        this->changeScreen();
    }
    
    // 配るアクションを実行する
    for(int i { 0 }; i < changedCards.size(); i++)
    {
        this->cardAction(changedCards.at(i), GameStatus::OVER, i == (changedCards.size() - 1) ? true : false);
    }
}

// HOLDボタンがタッチされたとき
void GameScene::onHoldButtonTouched(Ref *pSender, ui::Widget::TouchEventType type)
{
    Sprite *sprite { static_cast<Sprite*>(pSender) };
    
    switch (type)
    {
        case ui::Widget::TouchEventType::BEGAN:
        {
            int cardNumber { sprite->getTag() };

            // HOLD状態を変更する
            this->holdSprites.at(cardNumber)->setVisible(this->hands->toggleHold(cardNumber));
            
            break;
        }
        default:
            break;
    }
}
