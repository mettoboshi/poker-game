//
//  TitleScene.hpp
//  PokerGame
//

#ifndef __PokerGame__TitleScene__
#define __PokerGame__TitleScene__

#include "cocos2d.h"

#include "ui/CocosGUI.h"

class TitleScene : public cocos2d::Layer
{
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();
    
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    
    // ボタンのタッチイベント
    void touchEvent(Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
    
    // implement the "static create()" method manually
    CREATE_FUNC(TitleScene);
};

#endif /* defined(__PokerGame__TitleScene__) */
