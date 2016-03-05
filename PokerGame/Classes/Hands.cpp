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

    // ペア系の判定
    // ペアの数のカウント用
    int pearCount = 0;
    // ジャックス・オア・ベター
    bool isJacks = false;
    // スリーカード
    bool is3card = false;
    // フォーカード
    bool is4card = false;
    
    // トランプの数字についてそれぞれ調査する
    for(int no { 1 }; no <= 13; no++){
        // 手札に数字が何枚含まれているかを数える
        int count = 0;
        for(int i { 0 }; i < HANDS_MAX; i++){
            // 調査対象の数字ならカウント
            if(numbers.at(i) == no){
                count++;
            }
        }
        // 枚数に応じて、ペアのカウントを計算
        switch(count)
        {
            case 2:
            {
                // ペアの場合はカウント
                pearCount++;
                if(no > 10 || no == 1){
                    // J Q K A の場合は、ジャックス・オア・ベター
                    isJacks = true;
                }
                break;
            }
            case 3:
            {
                // スリーカード
                is3card = true;
                break;
            }
            case 4:
            {
                // フォーカード
                is4card = true;
                break;
            }
        }
    }

    // ツーペア
    bool is2pear = false;
    if(pearCount == 2){
        is2pear = true;
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
    else if(is4card)
    {
        this->hand = Hand::FOUR_OF_A_KIND;
    }
    else if(is3card && pearCount > 0)
    {
        this->hand = Hand::FULL_HOUSE;
    }
    else if(isFlush)
    {
        this->hand = Hand::FLUSH;
    }
    else if(isRoyal || isStraight)
    {
        this->hand = Hand::STRAIGHT;
    }
    else if(is3card)
    {
        this->hand = Hand::THREE_OF_A_KIND;
    }
    else if(pearCount == 2)
    {
        this->hand = Hand::TWOPAIR;
    }
    else if(isJacks)
    {
        this->hand = Hand::JACKS_OR_BETTER;
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
