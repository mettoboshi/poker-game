//
//  Hands.cpp
//  PokerGame
//

#include "Hands.hpp"

USING_NS_CC;

Hands::Hands()
{}

Hands::~Hands()
{}

bool Hands::init()
{
    return true;
}

// 手札のn番目にカードをセット
void Hands::setCard(int n, Card* card)
{
    this->cards.insert(n, card);
}

// 手札のn番目のカードを取得
Card* Hands::getCard(int n) const
{
    return this->cards.at(n);
}
