#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "Player.h"
#include "StartGameLayer.h"
#include "appwarp.h"

#define APPWARP_APP_KEY     "cad2bfab6310acd9696187b98682925125e469ab0d0d585db0b00609f461b791"
#define APPWARP_SECRET_KEY  "55811709916e7ce4405cde0cdc5a254cf4b506fbafdae05760a73100b8080b67"
#define GAME_NAME           "NinjaFight"
#define ROOM_ID             "152328576"
#define ROOM_NAME           "R1"
#define ROOM_OWNER          "Rajeev"
#define MAX_PLAYER          2



class HelloWorld : public cocos2d::CCLayerColor, public AppWarp::ConnectionRequestListener,public AppWarp::RoomRequestListener,public AppWarp::NotificationListener,public AppWarp::ZoneRequestListener,public AppWarp::LobbyRequestListener
{
    
public:
    
    
    cocos2d::CCArray *_targets;
    cocos2d::CCArray *_projectiles;
    int _projectilesDestroyed;
    Player *player;
    Player *enemy;
    int score;
    bool isEnemyAdded;
    bool isConnected;
    std::string userName;
    bool isFirstLaunch;
    StartGameLayer *startGameLayer;
    
    // Method 'init' in cocos2d-x returns bool, instead of 'id' in cocos2d-iphone (an object pointer)
    virtual bool init();

    // there's no 'id' in cpp, so we recommend to return the class instance pointer
    static cocos2d::CCScene* scene();
    void showStartGameLayer();
    void updateRoomProperties();
    void getRoomProperties();
    void createRooms();
    void getRooms();
    void removeStartGameLayer();
    void connectToAppWarp();
    // a selector callback
    void menuCloseCallback(CCObject* pSender);
    void startGame();
    void pauseGame();
    void updateEnemyStatus(cocos2d::CCPoint pos,float duration);
    void spriteMoveFinished(cocos2d::CCSprite* pSender);
    void sendData(float x, float y, float duration);
    virtual void ccTouchesEnded(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent);
    virtual void update(float time);
    
    void joinLobby();
    void subscribeLobby();
    void leaveLobby();
    void unsubscribeLobby();
    // preprocessor macro for "static create()" constructor ( node() deprecated )
    CREATE_FUNC(HelloWorld);
    
    void onConnectDone(int res);
    void onDisconnectDone(int res);
    void onJoinRoomDone(AppWarp::room revent);
    void onSubscribeRoomDone(AppWarp::room revent);
    void onChatReceived(AppWarp::chat chatevent);
    void onUserPaused(std::string user,std::string locId,bool isLobby);
    void onUserResumed(std::string user,std::string locId,bool isLobby);
    void showReconnectingLayer(std::string message);
    void onUpdatePropertyDone(AppWarp::liveroom revent);
    void onGetAllRoomsDone(AppWarp::liveresult res);
    void onCreateRoomDone(AppWarp::room revent);
    void onGetMatchedRoomsDone(AppWarp::matchedroom mevent);
    void onLeaveLobbyDone(AppWarp::lobby levent);
    void onJoinLobbyDone(AppWarp::lobby levent);
    void onSubscribeLobbyDone(AppWarp::lobby levent);
    void onUnsubscribeLobbyDone(AppWarp::lobby levent);
    
    void scheduleRecover();
    void unscheduleRecover();
    void recover();
    void disconnect();
};



#endif // __HELLOWORLD_SCENE_H__
