//
//  Common.h
//  PokerGame
//

#ifndef PokerGame_Common_h
#define PokerGame_Common_h

#include <map>

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

// ゲームのステータス
enum class GameStatus {
    BET,
    BET_WAIT,
    HOLD,
    DEAL,
    OVER
};

// 役
enum class Hand
{
    ROYAL_STRAIGHT_FLUSH,
    STRAIGHT_FLUSH,
    FOUR_OF_A_KIND,
    FULL_HOUSE,
    FLUSH,
    STRAIGHT,
    THREE_OF_A_KIND,
    TWOPAIR,
    JACKS_OR_BETTER,
    NOTHING
};

// 配当
const std::map<Hand, int> Rate
{
    { Hand::ROYAL_STRAIGHT_FLUSH, 5000 },
    { Hand::STRAIGHT_FLUSH      , 1500 },
    { Hand::FOUR_OF_A_KIND      , 1000 },
    { Hand::FULL_HOUSE          , 250  },
    { Hand::FLUSH               , 150  },
    { Hand::STRAIGHT            , 100  },
    { Hand::THREE_OF_A_KIND     , 50   },
    { Hand::TWOPAIR             , 20   },
    { Hand::JACKS_OR_BETTER     , 10   },
    { Hand::NOTHING             , 0    },
};

#endif /* PokerGame_Common_h */
