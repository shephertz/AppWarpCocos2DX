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

#include "appwarp.h"
#include "socket.h"
#include "curl/curl.h"
#define APPWARPSERVERPORT 12346
#define LOOKUPHOST "http://control.appwarp.shephertz.com/lookup"

namespace AppWarp
{
	int AppWarpSessionID = 0;
    

	Client* Client::_instance = NULL;

	Client::Client()
	{
		APIKEY = "";
		SECRETKEY = "";
		_connectionReqListener = NULL;
		_lobbyListener = NULL;
		_notificationListener = NULL;
		_chatlistener = NULL;
		_roomlistener = NULL;
		_zonelistener = NULL;
		_updatelistener = NULL;
        _socket = NULL;
		userName = "";
		APPWARPSERVERHOST = "";
        _state = ConnectionState::disconnected;
	}

	Client::~Client()
	{
		if(_socket != NULL){
            delete _socket;
        }
	}
    
	void Client::terminate()
	{
		if(_instance != NULL){
			delete Client::_instance;
		}
	}

	void Client::initialize(std::string AKEY, std::string SKEY)
	{
		if(_instance != NULL){
            return;
		}
        _instance = new Client();
		_instance->APIKEY = AKEY;
		_instance->SECRETKEY = SKEY;
        _instance->m_bRunning = true;
        _instance->scheduleUpdate();
        
	}

	Client* Client::getInstance()
	{
		return _instance;
	}

	void Client::setConnectionRequestListener(ConnectionRequestListener *listener)
	{
		_connectionReqListener = listener;
	}

	void Client::setLobbyRequestListener(LobbyRequestListener *listner)
	{
		_lobbyListener = listner;
	}

	void Client::setNotificationListener(NotificationListener *listner)
	{
		_notificationListener = listner;
	}

	void Client::setChatRequestListener(ChatRequestListener *listener)
	{
		_chatlistener = listener;
	}

	void Client::setRoomRequestListener(RoomRequestListener *listener)
	{
		_roomlistener = listener;
	}

	void Client::setZoneRequestListener(ZoneRequestListener *listener)
	{
		_zonelistener = listener;
	}

	void Client::setUpdateRequestListener(UpdateRequestListener *listener)
	{
		_updatelistener = listener;
	}

    size_t Client::hostLookupCallback(void *buffer, size_t size, size_t nmemb, void *userp)
    {
        Client* pWarpClient = (Client*)userp;
        
        cJSON *json;
        json = cJSON_Parse((char*)buffer);
        if(json != NULL && json->child!=NULL){
            json = json->child;
            std::string key = json->string;
            std::string value = json->valuestring;
            if(key.compare("address") == 0)
            {
                pWarpClient->APPWARPSERVERHOST = value;
            }
        }
        return size*nmemb;
    }
    
    void* Client::threadConnect( void *ptr )
    {
        Client* pWarpClient = (Client*)ptr;
        if(pWarpClient->APPWARPSERVERHOST.length() > 0){
            pWarpClient->connectSocket();
        }
		else{
            if(pWarpClient->lookup() == 200){
                pWarpClient->connectSocket();
            }
            else{
                pWarpClient->_state = ConnectionState::stream_failed;
            }
		}
        return NULL;
    }
    
	void Client::connect(std::string user)
	{
		if(user.length() == 0 || _socket!=NULL || _state!=ConnectionState::disconnected)
		{
			if(_connectionReqListener != NULL)
				_connectionReqListener->onConnectDone(ResultCode::bad_request);
            return;
		}
        userName = user;
        _state = ConnectionState::connecting;
        pthread_t threadConnection;
        pthread_create(&threadConnection, NULL, &threadConnect, (void *)this);
	}
    
    int Client::lookup()
    {
        CURL *curlHandle;
        CURLcode res;
        long http_code = 0;
        curlHandle = curl_easy_init ( ) ;
        if(!curlHandle){
            return 500;
        }
        curl_easy_setopt(curlHandle, CURLOPT_WRITEFUNCTION, hostLookupCallback);
        curl_easy_setopt(curlHandle, CURLOPT_WRITEDATA, (void *)this);
        curl_easy_setopt(curlHandle, CURLOPT_FOLLOWLOCATION, 1 );
        
        std::string path = LOOKUPHOST;
        path.append("?api=");
        path.append(this->APIKEY);
        curl_easy_setopt(curlHandle, CURLOPT_URL, path.c_str());
        
        res = curl_easy_perform( curlHandle );
        if(res == CURLE_OK){
            curl_easy_getinfo (curlHandle, CURLINFO_RESPONSE_CODE, &http_code);
        }
        else{
            http_code = 500;
        }
        
        curl_easy_cleanup( curlHandle );
        
        return http_code;
    }
    
    void Client::connectSocket()
    {
        _socket = new Utility::Socket(this);
        int result = _socket->sockConnect(APPWARPSERVERHOST, APPWARPSERVERPORT);
        socketConnectionCallback(result);
    }
    
	void Client::socketConnectionCallback(int res)
	{
        if(res == AppWarp::result_failure){
            _state = ConnectionState::stream_failed;
            delete _socket;
            _socket = NULL;
            return;
        }
        _state = ConnectionState::stream_connected;
		int byteLen;
		byte * authReq = buildAuthRequest(userName, byteLen,this->APIKEY,this->SECRETKEY);
		char *data = new char[byteLen];
		for(int i=0; i< byteLen; ++i)
		{
			data[i] = authReq[i];
		}

		_socket->sockSend(data, byteLen);

		delete[] authReq;
		delete[] data;
	}

    void Client::update(float dt){
        if(_socket != NULL && (_state == ConnectionState::connected || _state == ConnectionState::stream_connected)){
            _socket->checkMessages();
        }
        else if(_state == ConnectionState::stream_failed){
            _state = ConnectionState::disconnected;
            if(_connectionReqListener != NULL){
                _connectionReqListener->onConnectDone(ResultCode::connection_error);
            }
        }
    }
    
	void Client::socketNewMsgCallback(char data[], int len)
	{
		int numRead = len;
		int numDecoded = 0;

		while(numDecoded < numRead)
		{
			if(data[numDecoded] == MessageType::response)
				numDecoded += handleResponse(data, numDecoded);
			else
				numDecoded += handleNotify(data, numDecoded);
		}
	}

	void Client::disconnect()
	{
        if((_socket == NULL) || (_socket->sockDisconnect() == AppWarp::result_failure)){
            if(_connectionReqListener != NULL)
                _connectionReqListener->onDisconnectDone(AppWarp::ResultCode::bad_request);
            return;
        }
        delete _socket;
        _socket = NULL;
        _state = ConnectionState::disconnected;
		if(_connectionReqListener != NULL)
			_connectionReqListener->onDisconnectDone(AppWarp::ResultCode::success);

	}

	void Client::joinLobby()
	{
        if(_state != ConnectionState::connected){
            if(_lobbyListener != NULL)
			{
				lobby _lobby;
				_lobby.result = ResultCode::connection_error;
				_lobbyListener->onJoinLobbyDone(_lobby);
			}
            return;
        }
		int byteLen;
		byte *lobbyReq = buildLobbyRequest(RequestType::join_lobby, byteLen);
		char *data = new char[byteLen];
		for(int i=0; i< byteLen; ++i)
		{
			data[i] = lobbyReq[i];
		}

		_socket->sockSend(data, byteLen);

		delete[] data;
		delete[] lobbyReq;
	}

	void Client::leaveLobby()
	{
        if(_state != ConnectionState::connected){
            if(_lobbyListener != NULL)
			{
				lobby _lobby;
				_lobby.result = ResultCode::connection_error;
				_lobbyListener->onLeaveLobbyDone(_lobby);
			}
            return;
        }
		int byteLen;
		byte *lobbyReq = buildLobbyRequest(RequestType::leave_lobby, byteLen);
		char *data = new char[byteLen];
		for(int i=0; i< byteLen; ++i)
		{
			data[i] = lobbyReq[i];
		}

		_socket->sockSend(data, byteLen);

		delete[] data;
		delete[] lobbyReq;
	}

	void Client::subscribeLobby()
	{
        if(_state != ConnectionState::connected){
            if(_lobbyListener != NULL)
			{
				lobby _lobby;
				_lobby.result = ResultCode::connection_error;
				_lobbyListener->onSubscribeLobbyDone(_lobby);
			}
            return;
        }
		int byteLen;
		byte *lobbyReq = buildLobbyRequest(RequestType::subscribe_lobby, byteLen);
		char *data = new char[byteLen];
		for(int i=0; i< byteLen; ++i)
		{
			data[i] = lobbyReq[i];
		}

		_socket->sockSend(data, byteLen);

		delete[] data;
		delete[] lobbyReq;
	}

	void Client::unsubscribeLobby()
	{
        if(_state != ConnectionState::connected){
            if(_lobbyListener != NULL)
			{
				lobby _lobby;
				_lobby.result = ResultCode::connection_error;
				_lobbyListener->onUnsubscribeLobbyDone(_lobby);
			}
            return;
        }
		int byteLen;
		byte *lobbyReq = buildLobbyRequest(RequestType::unsubscribe_lobby, byteLen);
		char *data = new char[byteLen];
		for(int i=0; i< byteLen; ++i)
		{
			data[i] = lobbyReq[i];
		}

		_socket->sockSend(data, byteLen);

		delete[] data;
		delete[] lobbyReq;
	}

	void Client::joinRoom(std::string roomId)
	{
        if(_state != ConnectionState::connected){
			if(_roomlistener != NULL)
			{
				room _room;
				_room.result = ResultCode::connection_error;
				_roomlistener->onJoinRoomDone(_room);
			}
            return;
        }
		int byteLen;
		byte *roomReq = buildRoomRequest(RequestType::join_room, roomId, byteLen);
		char *data = new char[byteLen];
		for(int i=0; i< byteLen; ++i)
		{
			data[i] = roomReq[i];
		}

		_socket->sockSend(data, byteLen);

		delete[] data;
		delete[] roomReq;
	}

	void Client::leaveRoom(std::string roomId)
	{
        if(_state != ConnectionState::connected){
			if(_roomlistener != NULL)
			{
				room _room;
				_room.result = ResultCode::connection_error;
				_roomlistener->onLeaveRoomDone(_room);
			}
            return;
        }
		int byteLen;
		byte *roomReq = buildRoomRequest(RequestType::leave_room, roomId, byteLen);
		char *data = new char[byteLen];
		for(int i=0; i< byteLen; ++i)
		{
			data[i] = roomReq[i];
		}

		_socket->sockSend(data, byteLen);

		delete[] data;
		delete[] roomReq;
	}

	void Client::subscribeRoom(std::string roomId)
	{
        if(_state != ConnectionState::connected){
			if(_roomlistener != NULL)
			{
				room _room;
				_room.result = ResultCode::connection_error;
				_roomlistener->onSubscribeRoomDone(_room);
			}
            return;
        }
		int byteLen;
		byte *roomReq = buildRoomRequest(RequestType::subscribe_room, roomId, byteLen);
		char *data = new char[byteLen];
		for(int i=0; i< byteLen; ++i)
		{
			data[i] = roomReq[i];
		}

		_socket->sockSend(data, byteLen);

		delete[] data;
		delete[] roomReq;
	}

	void Client::unsubscribeRoom(std::string roomId)
	{
        if(_state != ConnectionState::connected){
			if(_roomlistener != NULL)
			{
				room _room;
				_room.result = ResultCode::connection_error;
				_roomlistener->onUnsubscribeRoomDone(_room);
			}
            return;
        }
		int byteLen;
		byte *roomReq = buildRoomRequest(RequestType::unsubscribe_room, roomId, byteLen);
		char *data = new char[byteLen];
		for(int i=0; i< byteLen; ++i)
		{
			data[i] = roomReq[i];
		}

		_socket->sockSend(data, byteLen);

		delete[] data;
		delete[] roomReq;
	}

	void Client::deleteRoom(std::string roomId)
	{
        if(_state != ConnectionState::connected){
			if(_zonelistener != NULL)
			{
				room _room;
				_room.result = ResultCode::connection_error;
				_zonelistener->onDeleteRoomDone(_room);
			}
            return;
        }
		int byteLen;
		byte *roomReq = buildRoomRequest(RequestType::delete_room, roomId, byteLen);
		char *data = new char[byteLen];
		for(int i=0; i< byteLen; ++i)
		{
			data[i] = roomReq[i];
		}

		_socket->sockSend(data, byteLen);

		delete[] data;
		delete[] roomReq;
	}

	void Client::createRoom(std::string name, std::string owner, int max, std::map<std::string,std::string> properties)
	{
        if(_state != ConnectionState::connected){
			if(_zonelistener != NULL)
			{
				room _room;
				_room.result = ResultCode::connection_error;
				_zonelistener->onCreateRoomDone(_room);
			}
            return;
        }
		std::map<std::string,std::string>::iterator it;
		cJSON *propJSON;
		propJSON = cJSON_CreateObject();
		for(it = properties.begin(); it != properties.end(); ++it)
		{
			cJSON_AddStringToObject(propJSON, it->first.c_str(),it->second.c_str());
		}
		char *cRet = cJSON_PrintUnformatted(propJSON);
		std::string prop = cRet;
		if(prop.length() >= MAX_PROPERTY_SIZE_BYTES)
		{
			room _room;
			_room.result = ResultCode::size_error;
			if(_zonelistener != NULL)
				_zonelistener->onCreateRoomDone(_room);
		}

		int byteLen;
		byte *roomReq = buildCreateRoomRequest(name,owner,max, prop, byteLen);
		char *data = new char[byteLen];
		for(int i=0; i< byteLen; ++i)
		{
			data[i] = roomReq[i];
		}

		_socket->sockSend(data, byteLen);

		delete[] data;
		delete[] roomReq;
		cJSON_Delete(propJSON);
		free(cRet);
	}

	void Client::createRoom(std::string name, std::string owner, int max)
	{
        if(_state != ConnectionState::connected){
			if(_zonelistener != NULL)
			{
				room _room;
				_room.result = ResultCode::connection_error;
				_zonelistener->onCreateRoomDone(_room);
			}
            return;
        }
		int byteLen;
		byte *roomReq = buildCreateRoomRequest(name,owner,max, byteLen);
		char *data = new char[byteLen];
		for(int i=0; i< byteLen; ++i)
		{
			data[i] = roomReq[i];
		}

		_socket->sockSend(data, byteLen);

		delete[] data;
		delete[] roomReq;
	}

	void Client::getLiveRoomInfo(std::string roomId)
	{
        if(_state != ConnectionState::connected){
			if(_roomlistener != NULL)
			{
				liveroom _room;
				_room.result = ResultCode::connection_error;
				_roomlistener->onGetLiveRoomInfoDone(_room);
			}
            return;
        }
		int byteLen;
		byte *roomReq = buildRoomRequest(RequestType::get_room_info, roomId, byteLen);
		char *data = new char[byteLen];
		for(int i=0; i< byteLen; ++i)
		{
			data[i] = roomReq[i];
		}

		_socket->sockSend(data, byteLen);

		delete[] data;
		delete[] roomReq;
	}

	void Client::getLiveLobbyInfo()
	{
        if(_state != ConnectionState::connected){
            if(_lobbyListener != NULL)
            {
                livelobby _lobby;
                _lobby.result = ResultCode::connection_error;
                _lobbyListener->onGetLiveLobbyInfoDone(_lobby);
            }
            return;
        }
		int byteLen;
		byte *lobbyReq = buildLobbyRequest(RequestType::get_lobby_info, byteLen);
		char *data = new char[byteLen];
		for(int i=0; i< byteLen; ++i)
		{
			data[i] = lobbyReq[i];
		}

		_socket->sockSend(data, byteLen);

		delete[] data;
		delete[] lobbyReq;
	}

	void Client::getLiveUserInfo(std::string user)
	{
        if(_state != ConnectionState::connected){
            if(_zonelistener != NULL)
			{
				liveuser _user;
				_user.result = ResultCode::connection_error;
				_zonelistener->onGetLiveUserInfoDone(_user);
			}
            return;
        }
		int byteLen;
		byte *req;

		std::string payload;
		cJSON *payloadJSON;
		payloadJSON = cJSON_CreateObject();
		cJSON_AddStringToObject(payloadJSON, "name",user.c_str());
		char *cRet = cJSON_PrintUnformatted(payloadJSON);
		payload = cRet;

		req = buildWarpRequest(RequestType::get_user_info, payload, byteLen);

		char *data = new char[byteLen];
		for(int i=0; i< byteLen; ++i)
		{
			data[i] = req[i];
		}

		_socket->sockSend(data, byteLen);

		delete[] data;
		delete[] req;
		cJSON_Delete(payloadJSON);
		free(cRet);
	}

	void Client::sendChat(std::string str)
	{
        if(_state != ConnectionState::connected){
            if(_chatlistener != NULL)
			{
				_chatlistener->onSendChatDone(ResultCode::connection_error);
			}
            return;
        }
		if(str.length() >= 512)
		{
			if(_chatlistener != NULL)
				_chatlistener->onSendChatDone(ResultCode::bad_request);

			return;
		}

		std::string payload;
		int len;

		cJSON *payloadJSON;
		payloadJSON = cJSON_CreateObject();
		cJSON_AddStringToObject(payloadJSON, "chat" ,str.c_str());
		char *cRet = cJSON_PrintUnformatted(payloadJSON);
		payload = cRet;

		byte * req = buildWarpRequest(RequestType::chat, payload, len);

		char *data = new char[len];
		for(int i=0; i< len; ++i)
		{
			data[i] = req[i];
		}

		_socket->sockSend(data, len);

		delete[] data;
		delete[] req;
		cJSON_Delete(payloadJSON);
		free(cRet);
	}

    void Client::sendPrivateChat(std::string toUser, std::string message)
	{
        if(_state != ConnectionState::connected){
            if(_chatlistener != NULL)
			{
				_chatlistener->onSendPrivateChatDone(ResultCode::connection_error);
			}
            return;
        }
		if(message.length() >= 512)
		{
			if(_chatlistener != NULL)
				_chatlistener->onSendPrivateChatDone(ResultCode::bad_request);
            
			return;
		}
        
		std::string payload;
		int len;
        
		cJSON *payloadJSON;
		payloadJSON = cJSON_CreateObject();
        cJSON_AddStringToObject(payloadJSON, "to" ,toUser.c_str());
		cJSON_AddStringToObject(payloadJSON, "chat" ,message.c_str());
		char *cRet = cJSON_PrintUnformatted(payloadJSON);
		payload = cRet;
        
		byte * req = buildWarpRequest(RequestType::private_chat, payload, len);
        
		char *data = new char[len];
		for(int i=0; i< len; ++i)
		{
			data[i] = req[i];
		}
        
		_socket->sockSend(data, len);
        
		delete[] data;
		delete[] req;
		cJSON_Delete(payloadJSON);
		free(cRet);
	}
    
	void Client::sendUpdate(byte *update,int data_len)
	{
        if(_state != ConnectionState::connected){
            if(_updatelistener != NULL)
			{
				_updatelistener->onSendUpdateDone(ResultCode::connection_error);
			}
            return;
        }
		if(data_len >= 512)
		{
			if(_updatelistener != NULL)
				_updatelistener->onSendUpdateDone(ResultCode::bad_request);

			return;
		}

		int len;
		byte * req = buildWarpRequest(RequestType::update_peers, update, data_len,len);

		char *data = new char[len];
		for(int i=0; i< len; ++i)
		{
			data[i] = req[i];
		}

		_socket->sockSend(data, len);

		delete[] data;
		delete[] req;
	}

	void Client::setCustomUserData(std::string userName, std::string customData)
	{
        if(_state != ConnectionState::connected){
            if(_zonelistener != NULL)
			{
				liveuser _user;
				_user.result = ResultCode::connection_error;
				_zonelistener->onSetCustomUserInfoDone(_user);
			}
            return;
        }
		int byteLen;
		byte *req;

		std::string payload;
		cJSON *payloadJSON;
		payloadJSON = cJSON_CreateObject();
		cJSON_AddStringToObject(payloadJSON, "name",userName.c_str());
		cJSON_AddStringToObject(payloadJSON, "data",customData.c_str());
		char *cRet = cJSON_PrintUnformatted(payloadJSON);
		payload = cRet;

		req = buildWarpRequest(RequestType::set_custom_user_data, payload, byteLen);

		char *data = new char[byteLen];
		for(int i=0; i< byteLen; ++i)
		{
			data[i] = req[i];
		}

		_socket->sockSend(data, byteLen);

		delete[] data;
		delete[] req;
		cJSON_Delete(payloadJSON);
		free(cRet);
	}

	void Client::setCustomRoomData(std::string roomId, std::string customData)
	{
        if(_state != ConnectionState::connected){
            if(_roomlistener != NULL)
			{
				liveroom _room;
				_room.result = ResultCode::connection_error;
				_roomlistener->onSetCustomRoomDataDone(_room);
			}
            return;
        }
		int byteLen;
		byte *req;

		std::string payload;
		cJSON *payloadJSON;
		payloadJSON = cJSON_CreateObject();
		cJSON_AddStringToObject(payloadJSON, "id",roomId.c_str());
		cJSON_AddStringToObject(payloadJSON, "data",customData.c_str());
		char *cRet =  cJSON_PrintUnformatted(payloadJSON);
		payload = cRet;

		req = buildWarpRequest(RequestType::set_custom_room_data, payload, byteLen);

		char *data = new char[byteLen];
		for(int i=0; i< byteLen; ++i)
		{
			data[i] = req[i];
		}

		_socket->sockSend(data, byteLen);

		delete[] data;
		delete[] req;
		cJSON_Delete(payloadJSON);
		free(cRet);
	}

	void Client::getAllRooms()
	{
        if(_state != ConnectionState::connected){
            if(_zonelistener != NULL)
			{
				liveresult _res;
				_res.result = ResultCode::connection_error;
				_zonelistener->onGetAllRoomsDone(_res);
			}
            return;
        }
		int len;
		byte * req = buildWarpRequest(RequestType::get_rooms, "", len);

		char *data = new char[len];
		for(int i=0; i< len; ++i)
		{
			data[i] = req[i];
		}

		_socket->sockSend(data, len);

		delete[] data;
		delete[] req;
	}

	void Client::getOnlineUsers()
	{
        if(_state != ConnectionState::connected){
            if(_zonelistener != NULL)
			{
				liveresult _res;
				_res.result = ResultCode::connection_error;
				_zonelistener->onGetOnlineUsersDone(_res);
			}
            return;
        }
		int len;
		byte * req = buildWarpRequest(RequestType::get_users, "", len);

		char *data = new char[len];
		for(int i=0; i< len; ++i)
		{
			data[i] = req[i];
		}

		_socket->sockSend(data, len);

		delete[] data;
		delete[] req;
	}

    void Client::lockProperties(std::map<std::string, std::string> properties)
    {
        if(_state != ConnectionState::connected){
            if(_roomlistener != NULL)
			{
				_roomlistener->onLockPropertiesDone(ResultCode::connection_error);
			}
            return;
        }
        int byteLen;
		byte *req;
        
		std::map<std::string,std::string>::iterator it;
        
		std::string payload;
		cJSON *propJSON;
		cJSON *payloadJSON;
		payloadJSON = cJSON_CreateObject();
		propJSON = cJSON_CreateObject();
		for(it = properties.begin(); it != properties.end(); ++it)
		{
			cJSON_AddStringToObject(propJSON, it->first.c_str(),it->second.c_str());
		}
        
		cJSON_AddStringToObject(payloadJSON, "lockProperties", cJSON_PrintUnformatted(propJSON));
        
		char *cRet = cJSON_PrintUnformatted(payloadJSON);
		payload = cRet;
        
		req = buildWarpRequest(RequestType::lock_properties, payload, byteLen);
        
		char *data = new char[byteLen];
		for(int i=0; i< byteLen; ++i)
		{
			data[i] = req[i];
		}
        
		_socket->sockSend(data, byteLen);
        
		delete[] data;
		delete[] req;
		cJSON_Delete(propJSON);
		cJSON_Delete(payloadJSON);
		free(cRet);
    }
    
    void Client::unlockProperties(std::vector<std::string> properties)
    {
        if(_state != ConnectionState::connected){
            if(_roomlistener != NULL)
			{
				_roomlistener->onUnlockPropertiesDone(ResultCode::connection_error);
			}
            return;
        }
        int byteLen;
		byte *req;
        
		
		std::string payload;
		
		cJSON *payloadJSON;
		payloadJSON = cJSON_CreateObject();
        
		std::string unlockArrayStr = "";
		for(unsigned int i=0; i<properties.size(); ++i)
		{
			if(i < properties.size()-1)
				unlockArrayStr += properties[i] + ";";
			else
				unlockArrayStr += properties[i];
		}
        
		cJSON_AddStringToObject(payloadJSON,"unlockProperties",unlockArrayStr.c_str());
        
		char *cRet = cJSON_PrintUnformatted(payloadJSON);
		payload = cRet;
        
		req = buildWarpRequest(RequestType::unlock_properties, payload, byteLen);
        
		char *data = new char[byteLen];
		for(int i=0; i< byteLen; ++i)
		{
			data[i] = req[i];
		}
        
		_socket->sockSend(data, byteLen);
        
		delete[] data;
		delete[] req;
		cJSON_Delete(payloadJSON);
		free(cRet);
    }
    
	void Client::updateRoomProperties(std::string roomID, std::map<std::string,std::string> properties,std::vector<std::string> removeArray)
	{
        if(_state != ConnectionState::connected){
            if(_roomlistener != NULL)
			{
				liveroom _room;
				_room.result = ResultCode::connection_error;
				_roomlistener->onUpdatePropertyDone(_room);
			}
            return;
        }
		int byteLen;
		byte *req;

		std::map<std::string,std::string>::iterator it;

		std::string payload;
		cJSON *propJSON;
		cJSON *payloadJSON;
		payloadJSON = cJSON_CreateObject();
		propJSON = cJSON_CreateObject();
		for(it = properties.begin(); it != properties.end(); ++it)
		{
			cJSON_AddStringToObject(propJSON, it->first.c_str(),it->second.c_str());
		}

		cJSON_AddStringToObject(payloadJSON, "id", roomID.c_str());
		cJSON_AddStringToObject(payloadJSON, "addOrUpdate", cJSON_PrintUnformatted(propJSON));

		std::string removeArrayStr = "";
		for(unsigned int i=0; i<removeArray.size(); ++i)
		{
			if(i < removeArray.size()-1)
				removeArrayStr += removeArray[i] + ";";
			else
				removeArrayStr += removeArray[i];
		}

		cJSON_AddStringToObject(payloadJSON,"remove",removeArrayStr.c_str());

		char *cRet = cJSON_PrintUnformatted(payloadJSON);
		payload = cRet;

		req = buildWarpRequest(RequestType::update_room_property, payload, byteLen);

		char *data = new char[byteLen];
		for(int i=0; i< byteLen; ++i)
		{
			data[i] = req[i];
		}

		_socket->sockSend(data, byteLen);

		delete[] data;
		delete[] req;
		cJSON_Delete(propJSON);
		cJSON_Delete(payloadJSON);
		free(cRet);
	}
    
    void Client::getRoomsInUserRange(int minJoinedUsers, int maxJoinedUsers)
    {
        if(_state != ConnectionState::connected){
            if(_zonelistener != NULL)
			{
				matchedroom _room;
				_room.result = ResultCode::connection_error;
				_zonelistener->onGetMatchedRoomsDone(_room);
			}
            return;
        }
		int byteLen;
		byte *req;
        
		std::string payload;
		cJSON *payloadJSON;
		payloadJSON = cJSON_CreateObject();
		cJSON_AddNumberToObject(payloadJSON, "minUsers", minJoinedUsers);
		cJSON_AddNumberToObject(payloadJSON, "maxUsers", maxJoinedUsers);
		char *cRet =  cJSON_PrintUnformatted(payloadJSON);
		payload = cRet;
        
		req = buildWarpRequest(RequestType::get_room_range, payload, byteLen);
        
		char *data = new char[byteLen];
		for(int i=0; i< byteLen; ++i)
		{
			data[i] = req[i];
		}
        
		_socket->sockSend(data, byteLen);
        
		delete[] data;
		delete[] req;
		cJSON_Delete(payloadJSON);
		free(cRet);
    }
    
    void Client::joinRoomInUserRange(int minJoinedUsers, int maxJoinedUsers, bool maxPreferred)
    {
        if(_state != ConnectionState::connected){
            if(_roomlistener != NULL)
			{
				room _room;
				_room.result = ResultCode::connection_error;
				_roomlistener->onJoinRoomDone(_room);
			}
            return;
        }
        int byteLen;
		byte *req;
        
		std::string payload;
		cJSON *payloadJSON;
		payloadJSON = cJSON_CreateObject();
		cJSON_AddNumberToObject(payloadJSON, "minUsers", minJoinedUsers);
		cJSON_AddNumberToObject(payloadJSON, "maxUsers", maxJoinedUsers);
        maxPreferred ? cJSON_AddTrueToObject(payloadJSON, "maxPreferred") : cJSON_AddFalseToObject(payloadJSON, "maxPreferred");
		char *cRet =  cJSON_PrintUnformatted(payloadJSON);
		payload = cRet;
        
		req = buildWarpRequest(RequestType::join_room_range, payload, byteLen);
        
		char *data = new char[byteLen];
		for(int i=0; i< byteLen; ++i)
		{
			data[i] = req[i];
		}
        
		_socket->sockSend(data, byteLen);
        
		delete[] data;
		delete[] req;
		cJSON_Delete(payloadJSON);
		free(cRet);
    }
    
	extern std::string ItoA(int num);

	void Client::joinRoomWithProperties(std::map<std::string,std::string> properties)
	{
        if(_state != ConnectionState::connected){
            if(_roomlistener != NULL)
			{
				room _room;
				_room.result = ResultCode::connection_error;
				_roomlistener->onJoinRoomDone(_room);
			}
            return;
        }
		int byteLen;
		byte *req;

		std::map<std::string,std::string>::iterator it;

		std::string payload;
		cJSON *propJSON;
		cJSON *payloadJSON;
		payloadJSON = cJSON_CreateObject();
		propJSON = cJSON_CreateObject();
		for(it = properties.begin(); it != properties.end(); ++it)
		{
			cJSON_AddStringToObject(propJSON, it->first.c_str(),it->second.c_str());
		}
		cJSON_AddStringToObject(payloadJSON, "properties", cJSON_PrintUnformatted(propJSON));

		char *cRet = cJSON_PrintUnformatted(payloadJSON);
		payload = cRet;

		req = buildWarpRequest(RequestType::join_room_with_properties, payload, byteLen);

		char *data = new char[byteLen];
		for(int i=0; i< byteLen; ++i)
		{
			data[i] = req[i];
		}

		_socket->sockSend(data, byteLen);

		delete[] data;
		delete[] req;
		cJSON_Delete(propJSON);
		cJSON_Delete(payloadJSON);
		free(cRet);
	}

	void Client::getRoomWithProperties(std::map<std::string,std::string> properties)
	{
        if(_state != ConnectionState::connected){
            if(_zonelistener != NULL)
			{
				matchedroom _room;
				_room.result = ResultCode::connection_error;
				_zonelistener->onGetMatchedRoomsDone(_room);
			}
            return;
        }
		int byteLen;
		byte *req;

		std::map<std::string,std::string>::iterator it;

		std::string payload;
		cJSON *propJSON;
		cJSON *payloadJSON;
		payloadJSON = cJSON_CreateObject();
		propJSON = cJSON_CreateObject();
		for(it = properties.begin(); it != properties.end(); ++it)
		{
			cJSON_AddStringToObject(propJSON, it->first.c_str(),it->second.c_str());
		}
		cJSON_AddItemToObject(payloadJSON, "properties", propJSON);

		char *cRet = cJSON_PrintUnformatted(payloadJSON);
		payload = cRet;

		req = buildWarpRequest(RequestType::get_room_with_properties, payload, byteLen);

		char *data = new char[byteLen];
		for(int i=0; i< byteLen; ++i)
		{
			data[i] = req[i];
		}

		_socket->sockSend(data, byteLen);

		delete[] data;
		delete[] req;
		cJSON_Delete(propJSON);
		cJSON_Delete(payloadJSON);
		free(cRet);
	}
}