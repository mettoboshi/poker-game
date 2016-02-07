//
//  GameScene.hpp
//  PokerGame
//

#ifndef __PokerGame__GameScene__
#define __PokerGame__GameScene__

#include "cocos2d.h"

USING_NS_CC;

class GameScene : public cocos2d::Layer
{
// ----- クラスメソッド
public:
    static cocos2d::Scene* createScene();

// ----- インスタンスメソッド
private:
    virtual bool init(); // 初期化
    
    CREATE_FUNC(GameScene);
};

#endif /* defined(__PokerGame__GameScene__) */
