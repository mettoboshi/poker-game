//
//  Hands.cpp
//  PokerGame
//

#include "Hands.hpp"

USING_NS_CC;

Hands::Hands()
{}

Hands::~Hands()
{
    for(auto card : this->cards)
    {
        CC_SAFE_RELEASE_NULL(card.second->card);
        CC_SAFE_RELEASE_NULL(card.second);
    }
}

bool Hands::init()
{
    return true;
}

// 手札のn番目にカードをセット
void Hands::setCard(int n, Card* card)
{
    CardState* cardState { CardState::create(card, false) };
    
    this->cards.insert(n, cardState);
}

// 手札のn番目のカードを取得
Card* Hands::getCard(int n) const
{
    return this->cards.at(n)->card;
}

// HOLD状態を切り替える
bool Hands::toggleHold(int n)
{
    bool hold { false };
    this->cards.at(n)->hold?hold = false:hold = true;

    return this->cards.at(n)->hold = hold;
}

// HOLD状態を取得
bool Hands::isHold(int n) const
{
    return this->cards.at(n)->hold;
}
