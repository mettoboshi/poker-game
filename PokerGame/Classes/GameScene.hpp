//
//  GameScene.hpp
//  PokerGame
//

#ifndef __PokerGame__GameScene__
#define __PokerGame__GameScene__

#include "cocos2d.h"

#include "ui/CocosGUI.h"
#include "Deck.hpp"
#include "Hands.hpp"

USING_NS_CC;

class GameScene : public cocos2d::Layer
{
// ----- クラスメソッド
public:
    static cocos2d::Scene* createScene();

// ----- インスタンス変数
private:
    GameStatus gameStatus; // ゲームのステータス

    int credit { 0 }; // クレジット数
    int win    { 0 }; // Win数

    Deck* deck { nullptr }; // デッキ
    Hands* hands { nullptr }; // 手札
    
    Label* winLabel    { nullptr }; // WINラベル
    Label* creditLabel { nullptr }; // Creditラベル

    Sprite* rateText { nullptr }; // 役名
    Sprite* rateBg   { nullptr }; // 役名の背景
    
    Vector<Sprite*> cardSprites {}; // 手札のスプライト
    
    ui::Button* betButton  { nullptr }; // BETボタン
    ui::Button* dealButton { nullptr }; // DEALボタン
    Vector<ui::Button*> holdButtons {}; // HOLDボタン

    Vector<Sprite*> holdSprites; // HOLD画像

// ----- インスタンスメソッド
private:
    GameScene();  // コンストラクタ
    ~GameScene(); // デストラクタ
    
    virtual bool init(); // 初期化
    
    void setupScreen();  // 画面の設定
    void setupGame();    // ゲームの初期設定

    void changeScreen(); // ゲームのステータスに応じた画面制御
    void updateCredit();  // Creditを更新する
    void setHandSprite(); // 役に応じた画像を設定する

    void cardAction(Sprite* sprite, GameStatus nextStatus, bool isLast); // カードのアクションを実行する
    
    void onBetButtonTouched(Ref *pSender, ui::Widget::TouchEventType type);  // BETボタンがタッチされたとき
    void betAction();    // BETした時のアクション

    void onDealButtonTouched(Ref *pSender, ui::Widget::TouchEventType type); // DEALボタンがタッチされたとき
    void dealAction();   // DEAL時のアクション

    void onHoldButtonTouched(Ref *pSender, ui::Widget::TouchEventType type); // HOLDボタンがタッチされたとき
    void cardAction(Sprite* sprite, GameStatus nextStatus, bool isLast, bool isBet); // カードのアクションを実行する
    Vector<Sprite*> getTurnCards(bool isBet); // めくるカードを取得する
    void cardTurn(GameStatus nextStatus, bool isBet); // カードをめくる
    void turnAction(Sprite* sprite, GameStatus nextStatus, bool isLast); // 表にめくるアクション
    
    CREATE_FUNC(GameScene);
};

#endif /* defined(__PokerGame__GameScene__) */
