#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"
#include "appwarp.h"

using namespace cocos2d;
using namespace CocosDenshion;

CCScene* HelloWorld::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    HelloWorld *layer = HelloWorld::create();

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
    if ( !CCLayerColor::initWithColor(ccc4(255, 255, 255, 255)) )
    {
        return false;
    }
    isFirstLaunch = true;
    setTouchEnabled(true);
    showStartGameLayer();
    return true;
}

void HelloWorld::showStartGameLayer()
{
    // Get the dimensions of the window for calculation purposes
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    int y_offset = 60;
    startGameLayer = StartGameLayer::create();
    addChild(startGameLayer);
    
    CCLabelTTF *buttonTitle = CCLabelTTF::create("Connect", "Marker Felt", 30);
    buttonTitle->setColor(ccBLACK);
    
    CCMenuItemLabel *startGameButton = CCMenuItemLabel::create(buttonTitle, this,menu_selector(HelloWorld::connectToAppWarp));
    startGameButton->setPosition(ccp(winSize.width/2,winSize.height-y_offset));
    
    y_offset+=60;
    
    CCLabelTTF *lobbyTitle = CCLabelTTF::create("JoinLobby", "Marker Felt", 30);
    lobbyTitle->setColor(ccBLACK);
    
    CCMenuItemLabel *joinLobbyButton = CCMenuItemLabel::create(lobbyTitle, this,menu_selector(HelloWorld::joinLobby));
    joinLobbyButton->setPosition(ccp(winSize.width/2,winSize.height-y_offset));
    
    y_offset+=60;
    
    CCLabelTTF *leaveLobbyTitle = CCLabelTTF::create("LeaveLobby", "Marker Felt", 30);
    leaveLobbyTitle->setColor(ccBLACK);
    
    CCMenuItemLabel *leaveLobbyButton = CCMenuItemLabel::create(leaveLobbyTitle, this,menu_selector(HelloWorld::leaveLobby));
    leaveLobbyButton->setPosition(ccp(winSize.width/2,winSize.height-y_offset));
    
    y_offset+=60;
    
    CCLabelTTF *updateButtonTitle = CCLabelTTF::create("Create Room With Prop", "Marker Felt", 30);
    updateButtonTitle->setColor(ccBLACK);
    
    CCMenuItemLabel *updateRoomButton = CCMenuItemLabel::create(updateButtonTitle, this,menu_selector(HelloWorld::createRooms));
    updateRoomButton->setPosition(ccp(winSize.width/2,winSize.height-y_offset));
     y_offset+=60;
    
    CCLabelTTF *getRoomButtonTitle = CCLabelTTF::create("GetRoomProperties", "Marker Felt", 30);
    getRoomButtonTitle->setColor(ccBLACK);
    
    CCMenuItemLabel *getRoomButton = CCMenuItemLabel::create(getRoomButtonTitle, this,menu_selector(HelloWorld::getRoomProperties));
    getRoomButton->setPosition(ccp(winSize.width/2,winSize.height-y_offset));
    
     y_offset+=60;
    CCLabelTTF *disconnectTitle = CCLabelTTF::create("Disconnect", "Marker Felt", 30);
    disconnectTitle->setColor(ccBLACK);
    
    CCMenuItemLabel *disconnectButton = CCMenuItemLabel::create(disconnectTitle, this,menu_selector(HelloWorld::disconnect));
    disconnectButton->setPosition(ccp(winSize.width/2,winSize.height-y_offset));

    //printf("\nshowStartGameLayer = (%f,%f)",winSize.width/2,winSize.height/2);
    CCMenu *pMenu = CCMenu::create(startGameButton,joinLobbyButton,leaveLobbyButton,updateRoomButton,getRoomButton,disconnectButton,NULL);
    pMenu->setPosition(CCPointZero);
    startGameLayer->addChild(pMenu, 1);
}


void HelloWorld::createRooms()
{
    printf("createRooms");
    
    std::map<std::string, std::string> properties;
    properties["a"] = "a";
    properties["b"] = "b";
    properties["c"] = "c";
    
    AppWarp::Client::getInstance()->createRoom("r1", "rajeev", 4, properties);
//    for (int i=3; i<8; i++)
//    {
//        std::string roomName = "r";
//        
//        AppWarp::Client::getInstance()->createRoom("r", "Raj", i);
//    }
}

void HelloWorld::joinLobby()
{
    AppWarp::Client::getInstance()->joinLobby();
}

void HelloWorld::subscribeLobby()
{
    AppWarp::Client::getInstance()->subscribeLobby();
}

void HelloWorld::leaveLobby()
{
    AppWarp::Client::getInstance()->leaveLobby();
}

void HelloWorld::unsubscribeLobby()
{
    AppWarp::Client::getInstance()->unsubscribeLobby();
}

void HelloWorld::getRooms()
{
    AppWarp::Client::getInstance()->getAllRooms();
}

void HelloWorld::updateRoomProperties()
{
    
}

void HelloWorld::getRoomProperties()
{
    std::map<std::string, std::string> properties;
    properties["a"] = "a";
    AppWarp::Client::getInstance()->getRoomWithProperties(properties);
}

void HelloWorld::removeStartGameLayer()
{
    removeChild(startGameLayer,true);
}

void HelloWorld::startGame()
{
    // Initialize arrays
    _targets = new CCArray();
    _projectiles = new CCArray();
    
    // Get the dimensions of the window for calculation purposes
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    
    player = (Player*)CCSprite::create("Player.png");
    player->setPosition(ccp(player->getContentSize().width/2, winSize.height/2));
    player->isEnemy = false;
    addChild(player);
    
    enemy = (Player*)CCSprite::create("Enemy.png");
    enemy->setPosition(ccp(winSize.width-enemy->getContentSize().width/2, winSize.height/2));
    enemy->isEnemy = true;
    enemy->setOpacity(100);
    addChild(enemy);
    isConnected = true;
    if (isFirstLaunch)
    {
        scheduleUpdate();
    }
    
    CCLabelTTF *buttonTitle = CCLabelTTF::create("Disconnect", "Marker Felt", 30);
    buttonTitle->setColor(ccBLACK);
    
    CCMenuItemLabel *startGameButton = CCMenuItemLabel::create(buttonTitle, this,menu_selector(HelloWorld::disconnect));
    startGameButton->setPosition(ccp(winSize.width/2,winSize.height/4));
    //printf("\nshowStartGameLayer = (%f,%f)",winSize.width/2,winSize.height/2);
    CCMenu *pMenu = CCMenu::create(startGameButton,NULL);
    pMenu->setPosition(CCPointZero);
    this->addChild(pMenu, 1);

}

void HelloWorld::update(float time)
{
    
    if (!isEnemyAdded)
    {
        return;
    }
    
	CCArray *projectilesToDelete = CCArray::create();
    if (_projectiles->count())
    {
        CCObject *pObj = NULL;
        CCARRAY_FOREACH(_projectiles, pObj)
        {
            CCSprite *projectile = (CCSprite*)pObj;
            CCRect projectileRect = CCRectMake(projectile->getPosition().x - (projectile->getContentSize().width/2),
                                              projectile->getPosition().y - (projectile->getContentSize().height/2),
                                              projectile->getContentSize().width,
                                              projectile->getContentSize().height);
            
            CCArray *targetsToDelete = CCArray::create();
            
            CCObject *tObj = NULL;
            CCARRAY_FOREACH(_targets, tObj)
            {
                CCSprite *target = (CCSprite*)tObj;
                CCRect targetRect = CCRectMake(target->getPosition().x - (target->getContentSize().width/2),
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
                CCSprite *target = (CCSprite*)tObj;
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
            CCSprite *projectile = (CCSprite*)pObj;
            _projectiles->removeObject(projectile);
            removeChild(projectile, true);

        }
    }
    else
    {
        CCArray *targetsToDelete = CCArray::create();
        CCObject *tObj=NULL;
        CCARRAY_FOREACH(_targets, tObj)
        {
            CCSprite *target = (CCSprite*)tObj;
            CCRect targetRect = CCRectMake(target->getPosition().x - (target->getContentSize().width/2),
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
            CCSprite *target = (CCSprite*)tObj;
            _targets->removeObject(target);
            removeChild(target, true);
            _projectilesDestroyed++;
        }
    }
    	
}

void HelloWorld::pauseGame()
{
    
}

void HelloWorld::updateEnemyStatus(CCPoint destination,float actualDuration)
{
    enemy->setOpacity(255);
    isEnemyAdded = true;
	CCSprite *target = CCSprite::create("Bullet-blue.png");
	target->setPosition(ccp(enemy->getPosition().x-enemy->getContentSize().width/2, enemy->getPosition().y));
	addChild(target,10);
	
    // Move projectile to actual endpoint
    CCActionInterval* move = CCMoveTo::create(actualDuration, destination);
    CCCallFuncN* moveFinished = CCCallFuncN::create(this, callfuncN_selector(HelloWorld::spriteMoveFinished));
    CCSequence* seq = CCSequence::create(move,moveFinished,  NULL);
    target->runAction(seq);

	// Add to targets array
	target->setTag(3);
	_targets->addObject(target);
}

void HelloWorld::ccTouchesEnded(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent)
{
    if (!isConnected)
    {
        return;
    }
    CCSetIterator it = pTouches->begin();
    CCTouch* touch = (CCTouch*)(*it);
    CCPoint location = touch->getLocation();

    // Set up initial location of projectile
	CCSize winSize = CCDirector::sharedDirector()->getWinSize();
	CCSprite *projectile = CCSprite::create("Bullet-red.png");
	projectile->setPosition(ccp(player->getPosition().x+player->getContentSize().width/2, player->getPosition().y));
	
    CCPoint projectilePos = projectile->getPosition();
	// Determine offset of location to projectile
	int offX = location.x - projectilePos.x;
	int offY = location.y - projectilePos.y;
	
	// Bail out if we are shooting down or backwards
	if (offX <= 0) return;
    
    // Ok to add now - we've double checked position
    addChild(projectile,10);
    
	// Play a sound!
    SimpleAudioEngine::sharedEngine()->playEffect("pew-pew-lei.caf");
	
	
	// Determine where we wish to shoot the projectile to
	int realX = winSize.width + (projectile->getContentSize().width/2);
	float ratio = (float) offY / (float) offX;
	int realY = (realX * ratio) + projectilePos.y;
	CCPoint realDest = ccp(realX, realY);
	
	// Determine the length of how far we're shooting
	int offRealX = realX - projectilePos.x;
	int offRealY = realY - projectilePos.y;
	float length = sqrtf((offRealX*offRealX)+(offRealY*offRealY));
	float velocity = 480/1; // 480pixels/1sec
	float realMoveDuration = length/velocity;
	
    //CCPoint destination = CCPointMake(winSize.width-realDest.x, realDest.y);
    
    sendData(winSize.width-realDest.x, realDest.y, realMoveDuration);
       
	// Move projectile to actual endpoint
    CCActionInterval* move = CCMoveTo::create(realMoveDuration, realDest);
    CCCallFuncN* moveFinished = CCCallFuncN::create(this, callfuncN_selector(HelloWorld::spriteMoveFinished));
    CCSequence* seq = CCSequence::create(move,moveFinished,  NULL);
    projectile->runAction(seq);
	// Add to projectiles array
	projectile->setTag(2);
    _projectiles->addObject(projectile);
	
}



void HelloWorld::spriteMoveFinished(CCSprite* pSender)
{
    CCSprite *sprite = (CCSprite *)pSender;
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

void HelloWorld::menuCloseCallback(CCObject* pSender)
{
    CCDirector::sharedDirector()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
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

void HelloWorld::disconnect()
{
    AppWarp::Client::getInstance()->disconnect();
}

void HelloWorld::connectToAppWarp()
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
        warpClientRef->setLobbyRequestListener(this);
        userName = genRandom();
        //warpClientRef->setGeo("us");
        warpClientRef->setGeo("eu");
        //warpClientRef->setGeo("japan");

        warpClientRef->connect(userName);
    }
    else
    {
        AppWarp::Client::getInstance()->connect(userName);
    }
}

void HelloWorld::onDisconnectDone(int res)
{
    showStartGameLayer();
}

void HelloWorld::onConnectDone(int res)
{
    if (res==AppWarp::ResultCode::success)
    {
        unscheduleRecover();
        printf("\nonConnectDone .. SUCCESS..session=%d\n",AppWarp::AppWarpSessionID);
        AppWarp::Client *warpClientRef;
        warpClientRef = AppWarp::Client::getInstance();
        //warpClientRef->joinRoom(ROOM_ID);
    }
    else if (res==AppWarp::ResultCode::success_recovered)
    {
        unscheduleRecover();
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
    schedule(schedule_selector(HelloWorld::recover), 5.0f);
    showReconnectingLayer("Reconnecting ...");
}

void HelloWorld::unscheduleRecover()
{
    unschedule(schedule_selector(HelloWorld::recover));
}

void HelloWorld::recover()
{
    printf("\nHelloWorld::recover");
    AppWarp::Client::getInstance()->recoverConnection();
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
        AppWarp::Client *warpClientRef;
        warpClientRef = AppWarp::Client::getInstance();
        std::map<std::string, std::string> properties;
        properties["1"] = "Owner";
        properties["2"] = "Manager";
        properties["3"] = "Team Lead";
        
      //  properties.insert(std::pair<std::string, std::string>("1","Owner"));
        
        std::vector<std::string> removeArray;
        warpClientRef->updateRoomProperties(ROOM_ID,properties, removeArray);
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

void HelloWorld::onUpdatePropertyDone(AppWarp::liveroom revent)
{
    if (revent.result==0)
    {
        printf("onUpdatePropertyDone....Success\n");
        std::map<std::string, std::string> properties = revent.properties;
        std::map<std::string,std::string>::iterator it;
        for(it = properties.begin(); it != properties.end(); ++it)
		{
			//cJSON_AddStringToObject(propJSON, it->first.c_str(),it->second.c_str());
            printf("key= %s...value= %s\n",it->first.c_str(),it->second.c_str());
		}


    }
    else
    {
        printf("\nonUpdatePropertyDone....Failed");
    }
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
        updateEnemyStatus(ccp(x,y), dest);
    }
}

void HelloWorld::showReconnectingLayer(std::string message)
{
    
    // Get the dimensions of the window for calculation purposes
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    
    startGameLayer = StartGameLayer::create();
    startGameLayer->setColor(ccc3(0, 0, 0));
    startGameLayer->setOpacity(50);
    addChild(startGameLayer);
    
    CCLabelTTF *buttonTitle = CCLabelTTF::create(message.c_str(), "Marker Felt", 30);
    buttonTitle->setColor(ccBLACK);
    startGameLayer->addChild(buttonTitle);
    buttonTitle->setPosition(ccp(winSize.width/2,winSize.height/2));
    
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

void HelloWorld::onCreateRoomDone(AppWarp::room revent)
{
    if (revent.result==0)
    {
        printf("\nonCreateRoomDone...success\n");

        std::map<std::string, std::string> properties;
        properties["abcd"] = "abcd";
        
        std::vector<std::string> removeProperties;
        AppWarp::Client::getInstance()->updateRoomProperties(revent.roomId, properties, removeProperties);
    }
}

void HelloWorld::onGetAllRoomsDone(AppWarp::liveresult res)
{
    if (res.result == 0)
    {
        std::vector<std::string>::iterator it;
        for(it = res.list.begin(); it != res.list.end(); ++it)
		{
            printf("key= %s\n",it->c_str());
		}

    }
}
void HelloWorld::onGetMatchedRoomsDone(AppWarp::matchedroom mevent)
{
    if (mevent.result == 0)
    {
        std::vector<AppWarp::room>::iterator it;
        for(it = mevent.roomData.begin(); it != mevent.roomData.end(); ++it)
		{
            printf("\nonGetMatchedRoomsDone..roomId= %s\n",it->roomId.c_str());
		}
        
    }
}

void HelloWorld::onLeaveLobbyDone(AppWarp::lobby levent)
{
    if (levent.result == 0)
    {
        printf("onLeaveLobbyDone .... success");
    }
    else
    {
        printf("onLeaveLobbyDone .... Failed");
    }
}

void HelloWorld::onJoinLobbyDone(AppWarp::lobby levent)
{
    if (levent.result == 0)
    {
        printf("onJoinLobbyDone .... success");
    }
    else
    {
        printf("onJoinLobbyDone .... Failed");

    }
}

void HelloWorld::onSubscribeLobbyDone(AppWarp::lobby levent)
{
    if (levent.result == 0)
    {
        printf("onSubscribeLobbyDone .... success");
    }
    else
    {
        printf("onSubscribeLobbyDone .... Failed");

    }
}

void HelloWorld::onUnsubscribeLobbyDone(AppWarp::lobby levent)
{
    if (levent.result == 0)
    {
        printf("onUnsubscribeLobbyDone .... success");
    }
    else
    {
        printf("onUnsubscribeLobbyDone .... Failed");

    }
}