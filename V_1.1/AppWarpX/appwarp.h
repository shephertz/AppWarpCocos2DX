/*
 Copyright (c) 2013 Shephertz Technologies Pvt. Ltd.
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in all
 copies or substantial portions of the Software.
 
 The Software shall be used for Good, not Evil.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 SOFTWARE.
 */

#ifndef __APPWARP__
#define __APPWARP__

#include <string>
#include <time.h>
#include <sstream>
#include <map>
#include <vector>

#include "pthread.h"
#include "socket.h"

#include "HMAC_SHA1.h"
#include "base64.h"
#include "urlencode.h"
#include "cJSON.h"

#include "defines.h"
#include "utilities.h"
#include "requests.h"
#include "listener.h"
#include "cocos2d.h"

namespace AppWarp
{
	extern int AppWarpSessionID;

	class Client : public cocos2d::CCNode
	{
	public:
		~Client();
		static Client* getInstance();
		static void initialize(std::string, std::string);
        static void terminate();
        
        int _state;
		void setConnectionRequestListener(ConnectionRequestListener *);
		void setLobbyRequestListener(LobbyRequestListener *);
		void setNotificationListener(NotificationListener *);
		void setChatRequestListener(ChatRequestListener *);
		void setRoomRequestListener(RoomRequestListener *);
		void setZoneRequestListener(ZoneRequestListener *);
		void setUpdateRequestListener(UpdateRequestListener *);
		
		void connect(std::string);
		void disconnect();
		void joinLobby();
		void leaveLobby();
		void subscribeLobby();
		void unsubscribeLobby();
		void joinRoom(std::string);
		void subscribeRoom(std::string);
		void leaveRoom(std::string);
		void unsubscribeRoom(std::string);
		void createRoom(std::string,std::string,int);
		void createRoom(std::string,std::string,int,std::map<std::string,std::string>);
		void deleteRoom(std::string);
		void getLiveRoomInfo(std::string);
		void getLiveLobbyInfo();
		void getLiveUserInfo(std::string);
		void sendChat(std::string);
		void sendUpdate(byte*,int);
		void setCustomUserData(std::string, std::string);
		void setCustomRoomData(std::string, std::string);
		void getAllRooms();
		void getOnlineUsers();
		void updateRoomProperties(std::string, std::map<std::string,std::string>,std::vector<std::string>);
		void updateRoomProperties(std::string, std::map<std::string,std::string>);
		void joinRoomWithNUser(int);
		void joinRoomWithProperties(std::map<std::string,std::string>);
		void getRoomWithNUser(int);
		void getRoomWithProperties(std::map<std::string,std::string>);


		void socketConnectionCallback(int);
        void socketNewMsgCallback(char[], int len);        
        virtual void update(float dt);
        
    private:
		std::string APIKEY;
		std::string SECRETKEY;
		std::string APPWARPSERVERHOST;
		
        Utility::Socket* _socket;
		static Client* _instance;
		Client();
		ConnectionRequestListener * _connectionReqListener;
		LobbyRequestListener *_lobbyListener;
		NotificationListener *_notificationListener;
		ChatRequestListener *_chatlistener;
		RoomRequestListener *_roomlistener;
		ZoneRequestListener *_zonelistener;
		UpdateRequestListener *_updatelistener;
        
		int handleResponse(char *, int);
		int handleNotify(char *, int);
		void handleAuthResponse(response *);
		void handleLobbyResponse(int,response *);
		void handleRoomResponse(int, response *);
		void handleZoneResponse(int, response *);
        void connectSocket();
        int lookup();
        static size_t hostLookupCallback(void *buffer, size_t size, size_t nmemb, void *userp);
        static void* threadConnect( void *ptr );
		std::string userName;
	};
}

#endif