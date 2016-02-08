//
//  Deck.cpp
//  PokerGame
//

#include "Deck.hpp"

Deck::Deck()
{};

Deck::~Deck()
{}

bool Deck::init()
{
    this->setAllCard();
    
    return true;
}

// デッキにカードをセット
void Deck::setAllCard() {

    // デッキの中身を空にする
    this->deckData.clear();
    this->deckData.shrinkToFit();
    
    // デッキにカードをセットする
    for (int i { 1 }; i <= 13; i++) {
        this->deckData.pushBack(Card::create(Suit::SPADE, i));
        this->deckData.pushBack(Card::create(Suit::HEART, i));
        this->deckData.pushBack(Card::create(Suit::DIAMOND, i));
        this->deckData.pushBack(Card::create(Suit::CLUB, i));
    }
}

// カードを１枚配る
Card* Deck::dealCard() {

    // 先頭のカードを取得
    Card* card = this->deckData.front();
    CC_SAFE_RETAIN(card);

    // デッキからカードを削除
    this->deckData.eraseObject(card);
    
    return card;
}

// シャッフル
void Deck::shuffle() {
    std::random_device rd;
    std::mt19937 engine(rd());
    
    std::shuffle(this->deckData.begin(), this->deckData.end(), engine);

    return;
}
