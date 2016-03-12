//
//  GameScene.cpp
//  PokerGame
//

#include "GameScene.hpp"

#include "Common.h"
#include "TitleScene.hpp"
#include "audio/include/AudioEngine.h"

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
    
    // BGMを鳴らす
    cocos2d::experimental::AudioEngine::play2d("game.wav", true);
    
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
    this->addChild(betButton, 50);
    betButton->addTouchEventListener(CC_CALLBACK_2(GameScene::onBetButtonTouched, this));
    
    // DEALボタン
    dealButton = ui::Button::create("DEAL.png", "DEAL_on.png", "DEAL_off.png");
    dealButton->setPosition(Vec2(1022.0f, 76.0f));

    // ボタンの表示を非活性にする
    dealButton->setBright(false);

    // タッチ無効化する
    dealButton->setTouchEnabled(false);

    this->addChild(dealButton, 50);
    dealButton->addTouchEventListener(CC_CALLBACK_2(GameScene::onDealButtonTouched, this));
    
    // winラベル
    winLabel = Label::createWithSystemFont(StringUtils::format("Win : $%d", win), "ariel", 38);
    winLabel->setAnchorPoint(Vec2(0, 0));
    winLabel->setPosition(Vec2(34.0f, 85.0f));
    winLabel->setColor(Color3B(255, 0, 0));
    this->addChild(winLabel, 50);
    
    // creditラベル
    creditLabel = Label::createWithSystemFont(StringUtils::format("Credit : $%d", credit), "ariel", 38);
    creditLabel->setAnchorPoint(Vec2(0, 0));
    creditLabel->setPosition(Vec2(34.0f, 40.0f));
    creditLabel->setColor(Color3B(255, 255, 255));
    this->addChild(creditLabel, 50);
    
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

    // 役名の背景画像を読み込み
    textureCache->addImage("text_bg_1.png");
    textureCache->addImage("text_bg_2.png");
    
    // 役名の画像を読み込み
    textureCache->addImage("3_OF_A_KIND.png");
    textureCache->addImage("4_OF_A_KIND.png");
    textureCache->addImage("FLUSH.png");
    textureCache->addImage("FULL_HOUSE.png");
    textureCache->addImage("GAME_OVER.png");
    textureCache->addImage("JACKS_OR_BETTER.png");
    textureCache->addImage("ROYAL_FLUSH.png");
    textureCache->addImage("STRAIGHT_FLUSH.png");
    textureCache->addImage("STRAIGHT.png");
    textureCache->addImage("TRY_AGAIN.png");
    textureCache->addImage("TWO_PAIR.png");
    
    // 役名の背景のスプライト
    rateBg = Sprite::create("text_bg_1.png");
    rateBg->setPosition(visibleSize / 2);
    rateBg->setVisible(false);
    this->addChild(rateBg, 6);
    
    // 役名のスプライト
    rateText = Sprite::create("TRY_AGAIN.png");
    rateText->setPosition(Vec2(visibleSize.width / 2, 540.0f));
    rateText->setVisible(false);
    this->addChild(rateText, 7);
    
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
            // 役表示：そのまま
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
            // 役表示：非表示
            rateBg->setVisible(false);
            rateText->setVisible(false);

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
                this->holdSprites.at(i)->setVisible(this->hands->isHold(i));
            }
            break;
        }
        case GameStatus::HOLD:
        {
            // 役表示：非表示
            rateBg->setVisible(false);
            rateText->setVisible(false);

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
            // 役表示：非表示
            rateBg->setVisible(false);
            rateText->setVisible(false);

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
            // Creditを更新する
            this->updateCredit();

            // 役表示：非表示
            this->setHandSprite();
            
            rateBg->setVisible(true);
            rateText->setVisible(true);

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

// Creditを更新する
void GameScene::updateCredit()
{
    Hand hand { this->hands->getHand() };
    
    if(hand == Hand::NOTHING) return;
    
    this->win = Rate.at(hand);
    this->winLabel->setString(StringUtils::format("Credit : $%d", this->win));

    if(this->win == 0)
    {
        creditLabel->setString(StringUtils::format("Credit : $%d", credit));
    }
    else
    {
        creditLabel->setString(StringUtils::format("Credit : $%d + $%d", credit, win));
    }
    this->credit += this->win;
}

// 役に応じた画像を設定する
void GameScene::setHandSprite()
{
    Hand hand { this->hands->getHand() };

    // 役に合わせた画像を設定
    Texture2D* texture { Director::getInstance()->getTextureCache()->getTextureForKey(HandFileName.at(hand))};
    
    if (hand != Hand::NOTHING)
    {
        // SEを鳴らす
        cocos2d::experimental::AudioEngine::play2d("se_get.wav", false);
        
        // 役が揃っている時の背景
        Texture2D* textureBg { Director::getInstance()->getTextureCache()->getTextureForKey("text_bg_2.png") };
        
        rateBg->setTexture(textureBg);
        rateBg->setTextureRect(Rect(0, 0, textureBg->getContentSize().width, textureBg->getContentSize().height));

        rateBg->setPosition(Vec2(568.0f, 320.0f));
        
        // テキスト画像の位置
        rateText->setPosition(Vec2(568.0f, 540.0f));
    }
    else
    {
        // Creditが0以下の場合はゲームオーバー
        if( this->credit <= 0)
        {
            // 音楽を止める
            cocos2d::experimental::AudioEngine::stopAll();

            // SEを鳴らす
            cocos2d::experimental::AudioEngine::play2d("over.wav", false);
            
            texture = Director::getInstance()->getTextureCache()->getTextureForKey("GAME_OVER.png");
        }
        else
        {
            // SEを鳴らす
            cocos2d::experimental::AudioEngine::play2d("se_hold.wav", false);
        }

        // 役が揃っていない時の背景
        Texture2D* textureBg { Director::getInstance()->getTextureCache()->getTextureForKey("text_bg_1.png") };
        
        rateBg->setTexture(textureBg);
        rateBg->setTextureRect(Rect(0, 0, textureBg->getContentSize().width, textureBg->getContentSize().height));

        rateBg->setPosition(Vec2(568.0f, 288.0f));

        // テキスト画像の位置
        rateText->setPosition(Vec2(568.0f, 288.0f));
    }
    
    rateText->setTexture(texture);
    rateText->setTextureRect(Rect(0, 0, texture->getContentSize().width, texture->getContentSize().height));
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

    // 配る用のSEを鳴らす
    betActions.pushBack(CallFunc::create([](){
        cocos2d::experimental::AudioEngine::play2d("se_draw.wav", false);
    }));
    
    betActions.pushBack(moveAction);

    // カードをめくる用のSEを鳴らす
    betActions.pushBack(CallFunc::create([](){
        cocos2d::experimental::AudioEngine::play2d("se_reverce.wav", false);
    }));
    
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

            // 手役の確定
            this->hands->dicisionHand();

            // ステータスに応じた画面制御
            this->changeScreen();
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
            // ゲームオーバー判定
            if (this->credit <= 0)
            {
                // タイトル画面に戻る
                auto nextScene = TitleScene::createScene();
                Scene* transition = TransitionFade::create(1.0f, nextScene);
                Director::getInstance()->replaceScene(transition);
                return;
            }
            
            // SEを鳴らす
            cocos2d::experimental::AudioEngine::play2d("se_coin.wav", false);
            
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

//            // DEBUG:つみこみ
//            hands->setCard(0, Card::create(Suit::SPADE, 1));
//            hands->setCard(1, Card::create(Suit::SPADE, 3));
//            hands->setCard(2, Card::create(Suit::SPADE, 5));
//            hands->setCard(3, Card::create(Suit::SPADE, 7));
//            hands->setCard(4, Card::create(Suit::SPADE, 9));
            
            // ステータスをBET_WAITに設定
            this->gameStatus = GameStatus::BET_WAIT;
            
            // 手役の確定
            this->hands->dicisionHand();
            
            Hand hand { this->hands->getHand() };
            
            // 4枚役以上が確定していた場合全てのカードをHOLDする
            if(hand == Hand::ROYAL_STRAIGHT_FLUSH ||
               hand == Hand::STRAIGHT_FLUSH ||
               hand == Hand::FOUR_OF_A_KIND ||
               hand == Hand::FULL_HOUSE ||
               hand == Hand::FLUSH ||
               hand == Hand::STRAIGHT)
            {
                for (int i { 0 }; i < HANDS_MAX; i++){
                    this->hands->toggleHold(i);
                }
            }

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
    Vector<Sprite*> changedCards { this->getTurnCards(false) };
    
    for(int i { 0 }; i < changedCards.size(); i++)
    {
        this->cardAction(changedCards.at(i), GameStatus::HOLD, i == HANDS_MAX - 1 ? true : false, true);
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
            // SEを鳴らす
            cocos2d::experimental::AudioEngine::play2d("se_coin.wav", false);

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
    Vector<Sprite*> changedCards { this->getTurnCards(false) };
    
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
        this->cardAction(changedCards.at(i), GameStatus::OVER, i == (changedCards.size() - 1) ? true : false, false);
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
            // SEを鳴らす
            cocos2d::experimental::AudioEngine::play2d("se_hold.wav", false);

            int cardNumber { sprite->getTag() };

            // HOLD状態を変更する
            this->holdSprites.at(cardNumber)->setVisible(this->hands->toggleHold(cardNumber));
            
            break;
        }
        default:
            break;
    }
}

// カードのアクションを実行する
void GameScene::cardAction(Sprite* sprite, GameStatus nextStatus, bool isLast, bool isBet)
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

    // 配る用のSEを鳴らす
    betActions.pushBack(CallFunc::create([](){
        cocos2d::experimental::AudioEngine::play2d("se_draw.wav", false);
    }));

    betActions.pushBack(moveAction);

    // 一番最後のカードの時はカードをめくる関数を呼ぶ
    betActions.pushBack(CallFunc::create([this, nextStatus, isLast, isBet](){
        if(isLast) {
            // カードをめくる
            this->cardTurn(nextStatus, isBet);
        }
    }));

    Sequence* betAction { Sequence::create(betActions) };
    
    // アクションを実行
    sprite->runAction(betAction);
}

// めくるカードを取得する
Vector<Sprite*> GameScene::getTurnCards(bool isBet)
{
    Vector<Sprite*> turnCards {};
    
    if(isBet)
    {
        // BETの場合全てのカードをめくる
        for (int i { 0 }; i < HANDS_MAX; i++)
        {
            turnCards.pushBack(this->cardSprites.at(i));
        }
    }
    else
    {
        // HOLDされていないカードを探す
        Vector<Sprite*> changedCards {};
        for (int i { 0 }; i < HANDS_MAX; i++){
            Sprite* cardSprite { this->cardSprites.at(i) };
            
            // ホールドされている場合は配り直さない
            if(this->hands->isHold(cardSprite->getTag())) continue;
            
            turnCards.pushBack(cardSprites.at(i));
        }
    }
    return turnCards;
}

// カードをめくる
void GameScene::cardTurn(GameStatus nextstatus, bool isBet)
{
    Vector<Sprite*> changedCards { this->getTurnCards(isBet) };
    
    // 配るアクションを実行する
    for(int i { 0 }; i < changedCards.size(); i++)
    {
        GameStatus nextStatus = GameStatus::HOLD;

        if(!isBet) {
            nextStatus = GameStatus::OVER;
        }
        
        this->turnAction(changedCards.at(i), nextstatus, i == (changedCards.size() - 1) ? true : false);
    }
}

// 表にめくるアクション
void GameScene::turnAction(Sprite* sprite, GameStatus nextStatus, bool isLast)
{
    // アクションをまとめる
    Vector<FiniteTimeAction *> actions;

    // ディレイ
    actions.pushBack(DelayTime::create(0.2f * sprite->getTag()));
    
    // カードをめくる用のSEを鳴らす
    actions.pushBack(CallFunc::create([](){
        cocos2d::experimental::AudioEngine::play2d("se_reverce.wav", false);
    }));
    
    // 表にめくるアクション
    actions.pushBack(ScaleTo::create(0.3f, 0.0f, 1.0f));
    actions.pushBack(CallFunc::create([this, sprite](){
        // テクスチャを変更
        Card *card { this->hands->getCard(sprite->getTag()) };
        Texture2D* cardTexture = Director::getInstance()->getTextureCache()->getTextureForKey(card->getFileName());
        sprite->setTexture(cardTexture);
    }));
    actions.pushBack(ScaleTo::create(0.3f, 1, 1));
    
    // ステータスを変更し、ボタンの制御、手役の決定をおこなう
    actions.pushBack(CallFunc::create([this, nextStatus, isLast](){
        if(isLast) {
            // スプライトのタグがHAND_MAX - 1のとき、ステータスを変更する
            this->gameStatus = nextStatus;

            // 手役の確定
            this->hands->dicisionHand();

            // ステータスに応じた画面制御
            this->changeScreen();
        }
    }));

    // めくるアクション
    Sequence* turnAction { Sequence::create(actions) };

    // アクションを実行
    sprite->runAction(turnAction);
}
