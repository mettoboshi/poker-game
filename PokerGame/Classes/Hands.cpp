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

    // カードの数字を変数にいれる
    std::vector<int> numbers {};
    for (int i { 0 }; i < HANDS_MAX; i++) {
        numbers.push_back(this->getCard(i)->getNumber());
    }
    // ソート
    std::sort(numbers.begin(), numbers.end());
    // DEBUG：数字を出す
    for (int i { 0 }; i < HANDS_MAX; i++) {
        CCLOG("%d:数字  %d", i, numbers.at(i));
    }
    
    // ロイヤル・ストレート
    bool isRoyal = false;
    if (numbers.at(0) == 1  &&
        numbers.at(1) == 10 &&
        numbers.at(2) == 11 &&
        numbers.at(3) == 12 &&
        numbers.at(4) == 13)
    {
        isRoyal = true;
    }
    
    // ストレート
    bool isStraight = false;
    int firstNumber { numbers.at(0) };
    if (numbers.at(1) == (firstNumber + 1) &&
        numbers.at(2) == (firstNumber + 2) &&
        numbers.at(3) == (firstNumber + 3) &&
        numbers.at(4) == (firstNumber + 4))
    {
        isStraight = true;
    }

    // 判定
    this->hand = Hand::NOTHING;
    if(isRoyal && isFlush)
    {
        this->hand = Hand::ROYAL_STRAIGHT_FLUSH;
    }
    else if(isStraight && isFlush)
    {
        this->hand = Hand::STRAIGHT_FLUSH;
    }
    else if(isFlush)
    {
        this->hand = Hand::FLUSH;
    }
    else if(isRoyal || isStraight)
    {
        this->hand = Hand::STRAIGHT;
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
