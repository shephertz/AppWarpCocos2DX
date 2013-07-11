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
        /**
         * Returns the singleton instance of WarpClient
         */
		static Client* getInstance();
        
        /**
		 * This should be your first API call to WarpClient. This will instantiate
		 * the WarpClient singleton and set it up to be used with the keys provided
		 * in params. Calling it more than once will return error. This will setup
		 * the client to work with the cloud server appwarp.shephertz.com
		 *
		 * @param apikey
		 * @param secretekey
		 * @return void
		 */
		static void initialize(std::string, std::string);
        
        /**
		 * Terminate the singleton instance and frees the allocated memory
		 *
		 * @return void
		 */
        static void terminate();
        
        /**
		 * The state of the Client. Values are -
         * ConnectionState::disconnected = 0,
         * ConnectionState::connecting = 1,
         * ConnectionState::connected = 2,
		 */
        int _state;
        
        /**
		 * Set your listener object on which callbacks will be invoked when a
		 * response from the server is received for connect, authenticate and
		 * disconnect APIs.
		 *
		 * @param listener
		 * @return void
		 */
		void setConnectionRequestListener(ConnectionRequestListener *);
        
        /**
		 * Set your listener object on which callbacks will be invoked when a
		 * response from the server is received for Lobby requests like
		 * join/leaveLobby, subscribe/unsubscribeLobby and getLiveLobbyInfo
		 *
		 * @param listener
		 * @return void
		 */
		void setLobbyRequestListener(LobbyRequestListener *);
        
        /**
		 * Set your listener object on which callbacks will be invoked when a
		 * notification is received from the server for any resource that has been
		 * subscribed to.
		 *
		 * @param listener
		 * @return void
		 */
		void setNotificationListener(NotificationListener *);
        
        /**
		 * Set your listener object on which callbacks will be invoked when a
		 * response from the server is received for sendChat request.
		 *
		 * @param listener
		 * @return void
		 */
		void setChatRequestListener(ChatRequestListener *);
        
        /**
		 * Set your listener object on which callbacks will be invoked when a
		 * response from the server is received for Room requests like
		 * join/leaveRoom, subscribe/unsubscribeRoom and getLiveRoomInfo
		 *
		 * @param listener
		 * @return void
		 */
		void setRoomRequestListener(RoomRequestListener *);
        
        /**
		 * Set your listener object on which callbacks will be invoked when a
		 * response from the server is received for Zone level requests like
		 * create/deleteRoom, User requests etc. disconnect APIs.
		 *
		 * @param listener
		 * @return void
		 */
		void setZoneRequestListener(ZoneRequestListener *);
        
        /**
		 * Set your listener object on which callbacks will be invoked when a
		 * response from the server is received for sendUpdate request.
		 *
		 * @param listener
		 * @return void
		 */
		void setUpdateRequestListener(UpdateRequestListener *);
		
        /**
		 * Initiates your connection with the WARP server. The result of the
		 * operation is provided in the onConnectDone callback of the
		 * ConnectionListener.
		 *
		 * @param user
		 * @return void
		 */
		void connect(std::string);
        
        /**
		 * Disconnects the connection with the WARP server. The result for this
		 * request will be provided in the onDisconnectDone callback of the
		 * ConnectionListener.
		 *
		 * @return void
		 */
		void disconnect();
        
        /**
		 * sends a join lobby request to the server. Result of the request is
		 * provided in the onJoinLobbyDone callback of the LobbyListener
		 *
		 * @return void
		 */
		void joinLobby();
        
        /**
		 * sends a leave lobby request to the server. Result of the request is
		 * provided in the onLeaveLobbyDone callback of the LobbyListener
		 *
		 * @return void
		 */
		void leaveLobby();
        
        /**
		 * sends a subscribe lobby request to the server. Result of the request is
		 * provided in the onSubscribeLobbyDone callback of the LobbyListener
		 *
		 * @return void
		 */
		void subscribeLobby();
        
        /**
		 * sends a unsubscribe lobby request to the server. Result of the request is
		 * provided in the onUnSubscribeLobbyDone callback of the LobbyListener
		 *
		 * @return void
		 */
		void unsubscribeLobby();
        
        /**
		 * sends a join room request to the server. Result of the request is
		 * provided in the onJoinRoomDone callback of the RoomListener.
		 *
		 * @param roomId
		 * @return void
		 */
		void joinRoom(std::string);
        
        /**
		 * sends a subscribe room request to the server. Result of the request is
		 * provided in the onSubscribeRoomDone callback of the RoomListener.
		 *
		 * @param roomId
		 * @return void
		 */
		void subscribeRoom(std::string);
        /**
		 * sends a leave room request to the server. Result of the request is
		 * provided in the onLeaveRoomDone callback of the RoomListener.
		 *
		 * @param roomId
		 * @return void
		 */
		void leaveRoom(std::string roomId);
        
		/**
		 * sends a unsubscribe room request to the server. Result of the request is
		 * provided in the onUnsubscribeRoomDone callback of the RoomListener.
		 *
		 * @param roomId
		 * @return void
		 */
		void unsubscribeRoom(std::string roomId);
        
		/**
		 * sends a create room request to the server. Result of the request is
		 * provided in the onCreateRoomDone callback of the ZoneRequestListener.
		 *
		 * @param name
		 * @param owner
		 * @param maxUsers
		 * @return void
		 */
		void createRoom(std::string name,std::string owner,int maxUsers);
        
		/**
		 * sends a create room request to the server. Result of the request is
		 * provided in the onCreateRoomDone callback of the ZoneRequestListener.
		 *
		 * @param name
		 * @param owner
		 * @param maxUsers
		 * @param properties
		 * @return void
		 */
		void createRoom(std::string name,std::string owner,int maxUsers,std::map<std::string,std::string> properties);
        
        /**
		 * sends a delete room request to the server. Result of the request is
		 * provided in the onDeleteRoomDone callback of the ZoneListener.
		 *
		 * @param roomId
		 * @return void
		 */
		void deleteRoom(std::string roomId);
        
		/**
		 * Retrieves live information of the room from the server. Result is
		 * provided in the onGetLiveRoomInfo callback of the RoomListener.
		 *
		 * @param roomid
		 * @return void
		 */
		void getLiveRoomInfo(std::string roomid);
        
		/**
		 * Retrieves live information of the lobby from the server. Result is
		 * provided in the onGetLiveLobbyInfo callback of the LobbyListener.
		 *
		 * @return void
		 */
		void getLiveLobbyInfo();
        
		/**
		 * Retrieves live information of the user from the server. Result is
		 * provided in the onGetLiveUserInfo callback of the ZoneListener.
		 *
		 * @param username
		 * @return void
		 */
		void getLiveUserInfo(std::string username);
        
		/**
		 * sends a chat message to room in which the user is currently joined.
		 * Result of the request is provided in the onSendChatDone callback of the
		 * ChatListener.
		 *
		 * @param message
		 * @return void
		 */
		void sendChat(std::string message);
        
        /**
		 * sends a private chat message to a user irrespecive of the room joined. The message
         * is delivered only to the specified user. Result of the request is provided in the 
         * onSendPrivateChatDone callback of the ChatListener.
		 *
         * @param toUser
		 * @param message
		 * @return void
		 */
		void sendPrivateChat(std::string toUser, std::string message);
        
		/**
		 * sends a custom update message to room in which the user is currently
		 * joined. Result of the request is provided in the onSendUpdatePeersDone
		 * callback of the UpdateListener.
		 *
		 * @param update
		 * @param updateLen
		 */
		void sendUpdate(byte* update,int updateLen);
        
		/**
		 * Updates the custom roomData associated with the given user on the server.
		 * Result is provided in the onSetCustomUserDataDone callback of the
		 * ZoneListener.
		 *
		 * @param username
		 * @param roomData
		 * @return void
		 */
		void setCustomUserData(std::string username, std::string roomData);
        
		/**
		 * Updates the custom roomData associated with the given room on the server.
		 * Result is provided in the onSetCustomRoomDataDone callback of the
		 * RoomListener.
		 *
		 * @param roomid
		 * @param roomData
		 * @return void
		 */
		void setCustomRoomData(std::string roomid, std::string roomData);
        
		/**
		 * Retrieves room ids of all the live rooms on the server. Result is
		 * provided in the onGetAllRoomsDone callback of the ZoneListener.
		 *
		 * @return void
		 */
		void getAllRooms();
        
		/**
		 * Retrieves usernames of all the users connected to the server. Result is
		 * provided in the onGetOnlineUsers callback of the ZoneListener.
		 *
		 * return @void
		 */
		void getOnlineUsers();
        
		/**
		 * sends a update room properties request to the server to update the properties of the room
		 * Result of the request is provided
		 * in the onJoinRoomDone callback of the RoomListener.
		 * @param roomId
		 * @param properties
		 * @param removeProperties
		 */
		void updateRoomProperties(std::string roomId, std::map<std::string,std::string> properties,std::vector<std::string> removeProperties);
        
        /**
         * Lock the properties associated with the joined room on the server for requested user. Result is provided 
         * in the onLockPropertyDone callback of the registered RoomRequestListener objects. Lock properties will fail 
         * if any other user has lock on same property, otherwise property will be added in lockTable with owner name.
         * This request (if successful) will also result in an onUserChangeRoomProperty notification on the registered
         * NotifyListener objects to be triggered for all subscribed users of the room.
         *
         * @param properties
         */
        void lockProperties(std::map<std::string, std::string> properties);
        
        /**
         * Unlock the properties associated with the joined room on the server for requested user. Result is provided
         * in the onUnlockPropertyDone callback of the registered RoomRequestListener objects. Unlock properties will fail
         * if any other user has lock on same property. This request (if successful) will also result in an 
         * onUserChangeRoomProperty notification on the registered NotifyListener objects to be triggered
         * for all subscribed users of the room.
         *
         * @param properties
         */
        void unlockProperties(std::vector<std::string> properties);
        
        /**Match making
		 * sends a join room request to the server if user want to join a room with required properties
		 * Result of the request is provided
		 * in the onJoinRoomDone callback of the RoomListener.
		 *
		 * @param properties
		 * return @void
		 */
		void joinRoomWithProperties(std::map<std::string,std::string>);
        
        /**
         * sends a request to the server for all the rooms that currently have
         * joined users in the range given
         * @param minJoinedUsers
         * @param maxJoinedUsers
         */
        void getRoomsInUserRange(int minJoinedUsers, int maxJoinedUsers);
        
        /**
         * sends a join room request to the server for any room that currently has
         * joined users in the range given. maxPreferred indicates the preference in
         * the case that multiple rooms with the desired params are found.
         * @param minJoinedUsers
         * @param maxJoinedUsers
         * @param maxPreferred
         */
        void joinRoomInUserRange(int minJoinedUsers, int maxJoinedUsers, bool maxPreferred);
        /**
		 * Retrieves information of the room that contain specific
		 * properties from the server. Result is
		 * provided in the onGetLiveRoomInfo callback of the RoomListener.
		 *
		 * @param properties
		 * @return void
		 */
		void getRoomWithProperties(std::map<std::string,std::string>);


        /**
         * Methods used for internal AppWarp socket callbacks.
         * Not required to be called in Cocos2DX application code.
         */
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