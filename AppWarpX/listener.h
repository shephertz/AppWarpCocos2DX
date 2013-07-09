
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