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
#ifndef __APPWARP_LISTENER__
#define __APPWARP_LISTENER__

#include <vector>

namespace AppWarp
{
	class ConnectionRequestListener
	{
	public:
		virtual void onConnectDone(int, int) {};
		virtual void onDisconnectDone(int) {};
        virtual void onInitUDPDone(int) {};
	};

	class LobbyRequestListener
	{
	public:
		virtual void onJoinLobbyDone(AppWarp::lobby levent) {};
		virtual void onLeaveLobbyDone(AppWarp::lobby levent) {};
		virtual void onSubscribeLobbyDone(AppWarp::lobby levent) {};
		virtual void onUnsubscribeLobbyDone(AppWarp::lobby levent) {};
		virtual void onGetLiveLobbyInfoDone(AppWarp::livelobby levent) {};
	};

	class NotificationListener
	{
	public:
		virtual void onRoomCreated(AppWarp::room rData) {}
		virtual void onRoomDestroyed(AppWarp::room rData) {}
		virtual void onUserLeftRoom(AppWarp::room rData, std::string user) {}
		virtual void onUserJoinedRoom(AppWarp::room rData, std::string user) {}
		virtual void onUserLeftLobby(AppWarp::lobby ldata, std::string user) {}
		virtual void onUserJoinedLobby(AppWarp::lobby ldata, std::string user) {}
		virtual void onChatReceived(AppWarp::chat chatevent) {}
        virtual void onPrivateChatReceived(std::string sender, std::string message) {}
		virtual void onUpdatePeersReceived(AppWarp::byte update[], int len, bool isUDP) {}
        virtual void onPrivateUpdateReceived(std::string sender, AppWarp::byte update[], int len, bool isUDP) {}
		virtual void onUserChangeRoomProperty(AppWarp::room rData, std::string user,std::map<std::string, std::string> properties, std::map<std::string, std::string>lockTable){}
        virtual void onUserPaused(std::string user,std::string locId,bool isLobby){}
        virtual void onUserResumed(std::string user,std::string locId,bool isLobby){}
        
        virtual void onGameStarted(std::string sender, std::string id, std::string nextTurn){}
        virtual void onGameStopped(std::string sender, std::string id){}
        virtual void onMoveCompleted(move event){}
        virtual void onNextTurnRequest(std::string lastTurn){}
	};

	class RoomRequestListener
	{
	public:
		virtual void onSubscribeRoomDone(AppWarp::room revent){}
		virtual void onUnsubscribeRoomDone(AppWarp::room revent){}
		virtual void onJoinRoomDone(AppWarp::room revent){}
		virtual void onLeaveRoomDone (AppWarp::room revent){}
		virtual void onGetLiveRoomInfoDone(AppWarp::liveroom revent){}
		virtual void onSetCustomRoomDataDone (AppWarp::liveroom revent){}
		virtual void onUpdatePropertyDone(AppWarp::liveroom revent){}
        virtual void onLockPropertiesDone(int result){}
        virtual void onUnlockPropertiesDone(int result){}
	};

    class TurnBasedRoomRequestListener
    {
    public:
        virtual void onStartGameDone(int res){}
        virtual void onStopGameDone(int res){}
        virtual void onSendMoveDone(int res){}
        virtual void onGetMoveHistoryDone(int res, std::vector<move> history){}
        virtual void onSetNextTurnDone(int res){}
        virtual void onGetGameStatusDone(int result,bool gameStatus){}
    };
    
	class ZoneRequestListener
	{
	public:
		virtual void onCreateRoomDone (AppWarp::room revent){}
		virtual void onDeleteRoomDone (AppWarp::room revent){} 
		virtual void onGetAllRoomsDone (AppWarp::liveresult res){}
		virtual void onGetOnlineUsersDone (AppWarp::liveresult res){}
		virtual void onGetLiveUserInfoDone (AppWarp::liveuser uevent){}
		virtual void onSetCustomUserInfoDone (AppWarp::liveuser uevent){}
		virtual void onGetMatchedRoomsDone(AppWarp::matchedroom mevent){}
	};

	class ChatRequestListener
	{
	public:
		virtual void onSendChatDone(int res){}
        virtual void onSendPrivateChatDone(int res){}
	};

	class UpdateRequestListener
	{
	public:
		virtual void onSendUpdateDone(int res){}
        virtual void onSendPrivateUpdateDone(int res){}
	};
}

#endif