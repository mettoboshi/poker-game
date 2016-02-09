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
// ---------- クラスメソッド
public:
    CREATE_FUNC(Hands);
    
// ---- インスタンス変数
private:
    cocos2d::Map<int, Card*> cards {}; // 手札のカード
    
// ----- インスタンスメソッド
private:
    Hands();     // コンストラクタ
    ~Hands();    // デストラクタ

    bool init(); // 初期化
    
public:
    void setCard(int n, Card* card); // 手札のn番目にカードをセット
    Card* getCard(int n) const;      // 手札のn番目のカードを取得
};

#endif /* defined(__PokerGame__Hands__) */
