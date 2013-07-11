//
//  StartGameLayer.cpp
//  AppWarpCocos2dxSample
//
//  Created by shephertz technologies on 11/07/13.
//
//

#include "StartGameLayer.h"

// on "init" you need to initialize your instance
bool StartGameLayer::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !CCLayerColor::initWithColor(cocos2d::ccc4(255, 255, 255, 255)) )
    {
        return false;
    }
    
    setTouchEnabled(true);
    
    return true;
}
