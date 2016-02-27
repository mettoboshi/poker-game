//
//  Card.hpp
//  PokerGame
//

#ifndef __PokerGame__Card__
#define __PokerGame__Card__

#include "Common.h"
#include "cocos2d.h"

class Card : public cocos2d::Ref
{
// ---------- クラスメソッド
public:
    static Card* create(Suit suit, int number); // create関数
    
// ---- インスタンス変数
private:
    Suit suit;  // カードのマーク
    int number; // カードの数字

// ----- インスタンスメソッド
private:
    Card();  // コンストラクタ
    ~Card(); // デストラクタ

    bool init(Suit suit, int number); // 初期化

public:
    Suit getSuit() const;  // マークを取得
    int getNumber() const; // 数字を取得
    std::string getFileName() const; // ファイル名を取得
};

#endif /* __PokerGame__Card__ */
