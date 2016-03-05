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

// 役の判定
void Hands::dicisionHand()
{
    // フラッシュ
    bool isFlush = false;
    
    Suit firstSuit { this->getCard(0)->getSuit() };
    if (firstSuit == this->getCard(1)->getSuit() &&
        firstSuit == this->getCard(2)->getSuit() &&
        firstSuit == this->getCard(3)->getSuit() &&
        firstSuit == this->getCard(4)->getSuit())
    {
        isFlush = true;
    }
    
    // 判定
    this->hand = Hand::NOTHING;
    if(isFlush){
        this->hand = Hand::FLUSH;
    }
}

// 役の取得
Hand Hands::getHand() const
{
    return this->hand;
}

// 配当の取得
int Hands::getRate() const
{
    return Rate.at(this->hand);
}
