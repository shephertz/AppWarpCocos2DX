//
//  Player.h
//  AppWarpCocos2dxSample
//
//  Created by shephertz technologies on 11/07/13.
//
//

#ifndef __AppWarpCocos2dxSample__Player__
#define __AppWarpCocos2dxSample__Player__

#include <iostream>


#include "cocos2d.h"

class Player : public cocos2d::CCSprite
{
protected:
    
public:
    // Method 'init' in cocos2d-x returns bool, instead of 'id' in cocos2d-iphone (an object pointer)
    virtual bool init();
    bool isEnemy;
    
    /** sets the isEneny value */
    inline void setIsEnemy(bool bValue) { isEnemy = bValue; }
    /** get the isEneny value */
    inline bool getIsEnemy(void) { return isEnemy; }
    
    // preprocessor macro for "static create()" constructor ( node() deprecated )
    CREATE_FUNC(Player);
};


#endif /* defined(__AppWarpCocos2dxSample__Player__) */
