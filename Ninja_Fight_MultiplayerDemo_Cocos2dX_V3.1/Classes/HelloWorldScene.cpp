#include "HelloWorldScene.h"

USING_NS_CC;

Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if( !LayerColor::initWithColor(Color4B(255, 255, 255, 255))  )
    {
        return false;
    }
    
    isFirstLaunch = true;
    setTouchEnabled(true);
    showStartGameLayer();
    return true;
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Point origin = Director::getInstance()->getVisibleOrigin();
    
    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.
    
    // add a "close" icon to exit the progress. it's an autorelease object
    auto closeItem = MenuItemImage::create(
                                           "CloseNormal.png",
                                           "CloseSelected.png",
                                           CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));
    
	closeItem->setPosition(Point(origin.x + visibleSize.width - closeItem->getContentSize().width/2 ,
                                 origin.y + closeItem->getContentSize().height/2));
    
    // create menu, it's an autorelease object
    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Point::ZERO);
    this->addChild(menu, 1);
    
    /////////////////////////////
    // 3. add your codes below...
    
    // add a label shows "Hello World"
    // create and initialize a label
    
    auto label = LabelTTF::create("Hello World", "Arial", 24);
    
    // position the label on the center of the screen
    label->setPosition(Point(origin.x + visibleSize.width/2,
                             origin.y + visibleSize.height - label->getContentSize().height));
    
    // add the label as a child to this layer
    this->addChild(label, 1);
    
    // add "HelloWorld" splash screen"
    auto sprite = Sprite::create("HelloWorld.png");
    
    // position the sprite on the center of the screen
    sprite->setPosition(Point(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));
    
    // add the sprite as a child to this layer
    this->addChild(sprite, 0);
    
    return true;
}


void HelloWorld::menuCloseCallback(Ref* pSender)
{
    Director::getInstance()->end();
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}

void HelloWorld::showStartGameLayer()
{
    
    // Get the dimensions of the window for calculation purposes
    Size winSize = Director::getInstance()->getWinSize();
    
    startGameLayer = StartGameLayer::create();
    addChild(startGameLayer);
    
    LabelTTF *buttonTitle = LabelTTF::create("Start Game", "Marker Felt", 30);
    buttonTitle->setColor(Color3B::BLACK);
    
    MenuItemLabel *startGameButton = MenuItemLabel::create(buttonTitle, CC_CALLBACK_1(HelloWorld::connectToAppWarp, this));
    startGameButton->setPosition(Point(winSize.width/2,winSize.height/2));
    Menu *pMenu = Menu::create(startGameButton,NULL);
    pMenu->setPosition(Point::ZERO);
    startGameLayer->addChild(pMenu, 1);
}

void HelloWorld::removeStartGameLayer()
{
    removeChild(startGameLayer,true);
}

void HelloWorld::startGame()
{
    // Initialize arrays
    _targets = Array::create();;//new Array();
    _projectiles = Array::create();
    _projectiles->retain();
    _targets->retain();
    // Get the dimensions of the window for calculation purposes
    Size winSize = Director::getInstance()->getWinSize();
    
    player = (Player*)Sprite::create("Player.png");
    player->setPosition(Point(player->getContentSize().width/2, winSize.height/2));
    player->isEnemy = false;
    addChild(player);
    
    enemy = (Player*)CCSprite::create("Enemy.png");
    enemy->setPosition(Point(winSize.width-enemy->getContentSize().width/2, winSize.height/2));
    enemy->isEnemy = true;
    enemy->setOpacity(100);
    addChild(enemy);
    isConnected = true;
    if (isFirstLaunch)
    {
        scheduleUpdate();
    }
    
    
}

void HelloWorld::update(float time)
{
    
    if (!isEnemyAdded)
    {
        return;
    }
    
    
	Array *projectilesToDelete = Array::create();
    if (_projectiles->count())
    {
        
        Ref *pObj = NULL;
        CCARRAY_FOREACH(_projectiles, pObj)
        {
            Sprite *projectile = (Sprite*)pObj;
            Rect projectileRect = Rect(projectile->getPosition().x - (projectile->getContentSize().width/2),
                                       projectile->getPosition().y - (projectile->getContentSize().height/2),
                                       projectile->getContentSize().width,
                                       projectile->getContentSize().height);
            
            Array *targetsToDelete = Array::create();
            
            Ref *tObj = NULL;
            CCARRAY_FOREACH(_targets, tObj)
            {
                Sprite *target = (Sprite*)tObj;
                Rect targetRect = Rect(target->getPosition().x - (target->getContentSize().width/2),
                                       target->getPosition().y - (target->getContentSize().height/2),
                                       target->getContentSize().width,
                                       target->getContentSize().height);
                
                if (projectileRect.intersectsRect(targetRect))
                {
                    targetsToDelete->addObject(target);
                }
                else if(!targetsToDelete->containsObject(target) && player->boundingBox().intersectsRect(targetRect))
                {
                    targetsToDelete->addObject(target);
                }
                
            }
            
            if (!projectilesToDelete->containsObject(projectile) && projectileRect.intersectsRect(enemy->boundingBox()))
            {
                projectilesToDelete->addObject(projectile);
            }
            tObj=NULL;
            CCARRAY_FOREACH(targetsToDelete, tObj)
            {
                Sprite *target = (Sprite*)tObj;
                _targets->removeObject(target);
                removeChild(target, true);
                _projectilesDestroyed++;
                
            }
            
            if (!projectilesToDelete->containsObject(projectile) && targetsToDelete->count() > 0)
            {
                projectilesToDelete->addObject(projectile);
            }
        }
        
        pObj=NULL;
        CCARRAY_FOREACH(projectilesToDelete, pObj)
        {
            Sprite *projectile = (Sprite*)pObj;
            _projectiles->removeObject(projectile);
            removeChild(projectile, true);
            
        }
    }
    else
    {
        Array *targetsToDelete = Array::create();
        Ref *tObj=NULL;
        CCARRAY_FOREACH(_targets, tObj)
        {
            Sprite *target = (Sprite*)tObj;
            Rect targetRect = Rect(target->getPosition().x - (target->getContentSize().width/2),
                                   target->getPosition().y - (target->getContentSize().height/2),
                                   target->getContentSize().width,
                                   target->getContentSize().height);
            
            if (!targetsToDelete->containsObject(target) && player->boundingBox().intersectsRect(targetRect))
            {
                targetsToDelete->addObject(target);
            }
            
        }
        
        tObj=NULL;
        CCARRAY_FOREACH(targetsToDelete, tObj)
        {
            Sprite *target = (Sprite*)tObj;
            _targets->removeObject(target);
            removeChild(target, true);
            _projectilesDestroyed++;
        }
    }
    
}

void HelloWorld::pauseGame()
{
    
}

void HelloWorld::updateEnemyStatus(Point destination,float actualDuration)
{
    enemy->setOpacity(255);
    isEnemyAdded = true;
	Sprite *target = Sprite::create("Bullet-blue.png");
	target->setPosition(Point(enemy->getPosition().x-enemy->getContentSize().width/2, enemy->getPosition().y));
	addChild(target,10);
	
    // Move projectile to actual endpoint
    ActionInterval* move = CCMoveTo::create(actualDuration, destination);
    CallFuncN* moveFinished = CallFuncN::create(CC_CALLBACK_1(HelloWorld::spriteMoveFinished, this));
    //create(this, callfuncN_selector(HelloWorld::spriteMoveFinished));
    Sequence* seq = CCSequence::create(move,moveFinished,  NULL);
    target->runAction(seq);
    
	// Add to targets array
	target->setTag(3);
	_targets->addObject(target);
}

void HelloWorld::onTouchesEnded(const std::vector<cocos2d::Touch *> &touches, cocos2d::Event *event)
{
    if (!isConnected)
    {
        return;
    }
    
    Touch* touch = (Touch*)(touches[0]);
    Point location = touch->getLocation();
    
    // Set up initial location of projectile
	Size winSize = Director::getInstance()->getWinSize();
	Sprite *projectile = Sprite::create("Bullet-red.png");
	projectile->setPosition(Point(player->getPosition().x+player->getContentSize().width/2, player->getPosition().y));
	
    Point projectilePos = projectile->getPosition();
	// Determine offset of location to projectile
	int offX = location.x - projectilePos.x;
	int offY = location.y - projectilePos.y;
	
	// Bail out if we are shooting down or backwards
	if (offX <= 0) return;
    
    // Ok to add now - we've double checked position
    addChild(projectile,10);
    
	// Play a sound!
    // SimpleAudioEngine::sharedEngine()->playEffect("pew-pew-lei.caf");
	
	
	// Determine where we wish to shoot the projectile to
	int realX = winSize.width + (projectile->getContentSize().width/2);
	float ratio = (float) offY / (float) offX;
	int realY = (realX * ratio) + projectilePos.y;
	Point realDest = Point(realX, realY);
	
	// Determine the length of how far we're shooting
	int offRealX = realX - projectilePos.x;
	int offRealY = realY - projectilePos.y;
	float length = sqrtf((offRealX*offRealX)+(offRealY*offRealY));
	float velocity = 480/1; // 480pixels/1sec
	float realMoveDuration = length/velocity;
	
    //CCPoint destination = CCPointMake(winSize.width-realDest.x, realDest.y);
    
    sendData(winSize.width-realDest.x, realDest.y, realMoveDuration);
    
	// Move projectile to actual endpoint
    ActionInterval* move = MoveTo::create(realMoveDuration, realDest);
    FiniteTimeAction* moveFinished = CallFuncN::create(CC_CALLBACK_1(HelloWorld::spriteMoveFinished, this));
    Sequence* seq = Sequence::create(move,moveFinished,  NULL);
    projectile->runAction(seq);
	// Add to projectiles array
	projectile->setTag(2);
    _projectiles->addObject(projectile);
	
}



void HelloWorld::spriteMoveFinished(Node* pSender)
{
    Sprite *sprite = (Sprite *)pSender;
	removeChild(sprite, true);
	
	if (sprite->getTag() == 3)
    { // target
		_targets->removeObject(sprite);
		
	}
    else if (sprite->getTag() == 2)
    { // projectile
		_projectiles->removeObject(sprite);
	}
}


std::string genRandom()
{
	std::string charStr;
	srand (time(NULL));
    
	for (int i = 0; i < 10; ++i) {
		charStr += (char)(65+(rand() % (26)));
	}
    
	return charStr;
}


/***
 * AppWarp Helper Methods
 */

void HelloWorld::connectToAppWarp(Ref* pSender)
{
    isConnected = false;
    AppWarp::Client *warpClientRef;
    if (isFirstLaunch)
    {
        isFirstLaunch = !isFirstLaunch;
        AppWarp::Client::initialize(APPWARP_APP_KEY,APPWARP_SECRET_KEY);
        warpClientRef = AppWarp::Client::getInstance();
        warpClientRef->setRecoveryAllowance(60);
        warpClientRef->setConnectionRequestListener(this);
        warpClientRef->setNotificationListener(this);
        warpClientRef->setRoomRequestListener(this);
        warpClientRef->setZoneRequestListener(this);
        userName = genRandom();
        warpClientRef->connect(userName);
    }
    else
    {
        AppWarp::Client::getInstance()->connect(userName);
    }
}


void HelloWorld::onConnectDone(int res)
{
    if (res==AppWarp::ResultCode::success)
    {
        unscheduleRecover();
        printf("\nonConnectDone .. SUCCESS..session=%d\n",AppWarp::AppWarpSessionID);
        AppWarp::Client *warpClientRef;
        warpClientRef = AppWarp::Client::getInstance();
        warpClientRef->joinRoom(ROOM_ID);
    }
    else if (res==AppWarp::ResultCode::success_recovered)
    {
        unscheduleRecover();
        removeStartGameLayer();
        printf("\nonConnectDone .. SUCCESS with success_recovered..session=%d\n",AppWarp::AppWarpSessionID);
    }
    else if (res==AppWarp::ResultCode::connection_error_recoverable)
    {
        scheduleRecover();
        printf("\nonConnectDone .. FAILED..connection_error_recoverable..session=%d\n",AppWarp::AppWarpSessionID);
    }
    else if (res==AppWarp::ResultCode::bad_request)
    {
        unscheduleRecover();
        printf("\nonConnectDone .. FAILED with bad request..session=%d\n",AppWarp::AppWarpSessionID);
    }
    else
    {
        unscheduleRecover();
        printf("\nonConnectDone .. FAILED with unknown reason..session=%d\n",AppWarp::AppWarpSessionID);
    }
}

void HelloWorld::scheduleRecover()
{
    printf("\nHelloWorld::scheduleRecover");
    this->schedule(schedule_selector(HelloWorld::recover), 5.0f);
    
    showReconnectingLayer("Reconnecting ...");
}

void HelloWorld::unscheduleRecover()
{
    printf("\nHelloWorld::unscheduleRecover");
    unschedule(schedule_selector(HelloWorld::recover));
}

void HelloWorld::recover(float dt)
{
    printf("\nHelloWorld::recover");
    AppWarp::Client::getInstance()->recoverConnection();
}

void HelloWorld::showReconnectingLayer(std::string message)
{
    
    // Get the dimensions of the window for calculation purposes
    Size winSize = Director::getInstance()->getWinSize();
    
    startGameLayer = StartGameLayer::create();
    startGameLayer->setColor(Color3B(0, 0, 0));
    startGameLayer->setOpacity(50);
    addChild(startGameLayer);
    
    LabelTTF *buttonTitle = LabelTTF::create(message.c_str(), "Marker Felt", 30);
    buttonTitle->setColor(Color3B::BLACK);
    startGameLayer->addChild(buttonTitle);
    buttonTitle->setPosition(Point(winSize.width/2,winSize.height/2));
    
}

void HelloWorld::onJoinRoomDone(AppWarp::room revent)
{
    if (revent.result==0)
    {
        printf("\nonJoinRoomDone .. SUCCESS\n");
        AppWarp::Client *warpClientRef;
        warpClientRef = AppWarp::Client::getInstance();
        warpClientRef->subscribeRoom(ROOM_ID);
        startGame();
        removeStartGameLayer();
    }
    else
        printf("\nonJoinRoomDone .. FAILED\n");
}

void HelloWorld::onSubscribeRoomDone(AppWarp::room revent)
{
    if (revent.result==0)
    {
        printf("\nonSubscribeRoomDone .. SUCCESS\n");
    }
    else
        printf("\nonSubscribeRoomDone .. FAILED\n");
}


void HelloWorld::sendData(float x, float y, float duration)
{
    AppWarp::Client *warpClientRef;
	warpClientRef = AppWarp::Client::getInstance();
    
    std::stringstream str;
    str <<x << "x" <<y << "d" << duration;
    warpClientRef->sendChat(str.str());
}



void HelloWorld::onChatReceived(AppWarp::chat chatevent)
{
    printf("onChatReceived..");
    if(chatevent.sender != userName)
	{
		std::size_t loc = chatevent.chat.find('x');
		std::string str1 = chatevent.chat.substr(0,loc);
		std::string str2 = chatevent.chat.substr(loc+1);
        loc = chatevent.chat.find('d');
        std::string str3 = chatevent.chat.substr(loc+1);
		float x = (float)std::atof (str1.c_str());
		float y = (float)std::atof(str2.c_str());
        float dest = (float)std::atof(str3.c_str());
        updateEnemyStatus(Point(x,y), dest);
    }
}


void HelloWorld::onUserPaused(std::string user,std::string locId,bool isLobby)
{
    //    printf("\nonUserPaused...user=%s",user.c_str());
    //    printf("\nonUserPaused...locId=%s",locId.c_str());
    std::string message = "Waiting for "+user+" response...";
    showReconnectingLayer(message);
}

void HelloWorld::onUserResumed(std::string user,std::string locId,bool isLobby)
{
    //    printf("\nonUserResumed...user=%s",user.c_str());
    //    printf("\nonUserResumed...locId=%s",locId.c_str());
    removeStartGameLayer();
}

