//
//  Hands.hpp
//  PokerGame
//

#ifndef __PokerGame__Hands__
#define __PokerGame__Hands__

#include "Card.hpp"

#include "Common.h"

class Hands: public cocos2d::Ref
{
// ----- 構造体
private:
    struct CardState : Ref
    {
        Card* card;
        bool  hold;
        
        static CardState* create(Card* card, bool hold)
        {
            CardState* cardState = new CardState();
            CC_SAFE_RETAIN(cardState);
            cardState->card = card;
            CC_SAFE_RETAIN(cardState->card);
            cardState->hold = hold;
            
            return std::move(cardState);
        }
    };
    
// ---------- クラスメソッド
public:
    CREATE_FUNC(Hands);
    
// ---- インスタンス変数
private:
    cocos2d::Map<int, CardState*> cards {}; // 手札のカード
    Hand hand { Hand::NOTHING };            // 役

// ----- インスタンスメソッド
private:
    Hands();     // コンストラクタ
    ~Hands();    // デストラクタ

    bool init(); // 初期化
    
public:
    void setCard(int n, Card* card);  // 手札のn番目にカードをセット
    Card* getCard(int n) const;       // 手札のn番目のカードを取得

    bool toggleHold(int n);           // HOLD状態を切り替える
    bool isHold(int n) const;         // HOLD状態を取得
    
    void dicisionHand();              // 役の確定
    Hand getHand() const;             // 役の取得
    int getRate() const;              // 配当の取得
};

#endif /* defined(__PokerGame__Hands__) */
