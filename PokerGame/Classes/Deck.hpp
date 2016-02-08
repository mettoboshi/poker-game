//
//  Deck.hpp
//  PokerGame
//

#ifndef __PokerGame__Deck__
#define __PokerGame__Deck__

#include "Common.h"
#include "Card.hpp"

class Deck : public cocos2d::Ref
{
// ---------- クラスメソッド
public:
    CREATE_FUNC(Deck);

// ---- インスタンス変数
private:
    cocos2d::Vector<Card*> deckData;

// ----- インスタンスメソッド
private:
    Deck();  // コンストラクタ
    ~Deck(); // デストラクタ

    bool init(); // 初期化

public:
    void setAllCard(); // デッキにカードをセット
    Card* dealCard();  // カードを１枚配る
    void shuffle();    // シャッフル
    
};

#endif /* defined(__PokerGame__Deck__) */
