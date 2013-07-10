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
		virtual void onConnectDone(int) {};
		virtual void onDisconnectDone(int) {};
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
		virtual void onUpdatePeersReceived(AppWarp::byte update[], int len) {}
		virtual void onUserChangeRoomProperty(AppWarp::room rData, std::string user,std::map<std::string, std::string> properties){}
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
	};

	class UpdateRequestListener
	{
	public:
		virtual void onSendUpdateDone(int res){}
	};
}

#endif