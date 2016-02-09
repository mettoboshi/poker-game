//
//  Common.h
//  PokerGame
//

#ifndef PokerGame_Common_h
#define PokerGame_Common_h

constexpr int START_CREDIT { 100 }; // Creditの初期値
constexpr int BET_NUM      { 10 };  // ベット枚数
constexpr int HANDS_MAX    { 5 };   // 手札の最大値

// トランプのマーク
enum class Suit {
    SPADE,
    HEART,
    DIAMOND,
    CLUB,
    
    SIZE,
};

#endif /* PokerGame_Common_h */
