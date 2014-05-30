//
//  StartGameLayer.h
//  AppWarpCocos2dxSample
//
//  Created by shephertz technologies on 11/07/13.
//
//

#ifndef __AppWarpCocos2dxSample__StartGameLayer__
#define __AppWarpCocos2dxSample__StartGameLayer__

#include <iostream>
#include "cocos2d.h"

class StartGameLayer : public cocos2d::LayerColor
{
protected:
    
public:
    // Method 'init' in cocos2d-x returns bool, instead of 'id' in cocos2d-iphone (an object pointer)
    virtual bool init();
        
    // preprocessor macro for "static create()" constructor ( node() deprecated )
    CREATE_FUNC(StartGameLayer);
};



#endif /* defined(__AppWarpCocos2dxSample__StartGameLayer__) */
