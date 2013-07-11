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


namespace AppWarp
{

		void buildLobbyData(lobby &_lobby, byte *data,int len)
		{

			_lobby.lobbyid = getJSONString("id",data, len);	
			_lobby.isPrimary = getJSONBool("isPrimary",data, len);
			_lobby.maxUsers = getJSONInt("maxUsers",data, len);
			_lobby.name.append(getJSONString("name",data, len));
			_lobby.owner.append(getJSONString("owner",data, len));
		}

		void buildRoomData(room &_room, byte *data,int len)
		{
			_room.roomId = getJSONString("id",data,len);
			_room.owner = getJSONString("owner",data,len);
			_room.name = getJSONString("name",data,len);
			_room.maxUsers = getJSONInt("maxUsers",data,len);
		}

		int Client::handleResponse(char *data, int index)
		{
			response *res;
			res = buildResponse(data, index);
			
			if(res->requestType == RequestType::auth)
				handleAuthResponse(res);
			else if(res->requestType == RequestType::join_lobby)
				handleLobbyResponse(RequestType::join_lobby,res);
			else if(res->requestType == RequestType::subscribe_lobby)
				handleLobbyResponse(RequestType::subscribe_lobby,res);
			else if(res->requestType == RequestType::unsubscribe_lobby)
				handleLobbyResponse(RequestType::unsubscribe_lobby,res);
			else if(res->requestType == RequestType::leave_lobby)
				handleLobbyResponse(RequestType::leave_lobby,res);
			else if(res->requestType == RequestType::get_lobby_info)
				handleLobbyResponse(RequestType::get_lobby_info,res);
			else if(res->requestType == RequestType::chat)
			{
				if(_chatlistener != NULL)
					_chatlistener->onSendChatDone(res->resultCode);
			}
            else if(res->requestType == RequestType::private_chat)
			{
				if(_chatlistener != NULL)
					_chatlistener->onSendPrivateChatDone(res->resultCode);
			}
			else if(res->requestType == RequestType::update_peers)
			{
				if(_updatelistener != NULL)
					_updatelistener->onSendUpdateDone(res->resultCode);
			}
            else if(res->requestType == RequestType::lock_properties)
			{
				if(_roomlistener != NULL)
					_roomlistener->onLockPropertiesDone(res->resultCode);
			}
            else if(res->requestType == RequestType::unlock_properties)
			{
				if(_roomlistener != NULL)
					_roomlistener->onUnlockPropertiesDone(res->resultCode);
			}
			else if(res->requestType == RequestType::join_room || res->requestType == RequestType::join_room_n_user || res->requestType == RequestType::join_room_with_properties || res->requestType == RequestType::join_room_range)
				handleRoomResponse(RequestType::join_room, res);
			else if(res->requestType == RequestType::leave_room)
				handleRoomResponse(RequestType::leave_room, res);
			else if(res->requestType == RequestType::subscribe_room)
				handleRoomResponse(RequestType::subscribe_room, res);
			else if(res->requestType == RequestType::unsubscribe_room)
				handleRoomResponse(RequestType::unsubscribe_room, res);
			else if(res->requestType == RequestType::delete_room)
				handleZoneResponse(RequestType::delete_room, res);
			else if(res->requestType == RequestType::create_room)
				handleZoneResponse(RequestType::create_room, res);
			else if(res->requestType == RequestType::get_room_info)
				handleRoomResponse(RequestType::get_room_info, res);
			else if(res->requestType == RequestType::get_user_info)
				handleZoneResponse(RequestType::get_user_info,res);
			else if(res->requestType == RequestType::set_custom_user_data)
				handleZoneResponse(RequestType::set_custom_user_data, res);
			else if(res->requestType == RequestType::set_custom_room_data)
				handleRoomResponse(RequestType::set_custom_room_data, res);
			else if(res->requestType == RequestType::get_rooms)
				handleZoneResponse(RequestType::get_rooms, res);
			else if(res->requestType == RequestType::get_users)
				handleZoneResponse(RequestType::get_users, res);
			else if(res->requestType == RequestType::update_room_property)
				handleRoomResponse(RequestType::update_room_property, res);
			else if(res->requestType == RequestType::get_room_with_n_user || res->requestType == RequestType::get_room_with_properties || res->requestType == RequestType::get_room_range)
				handleZoneResponse(res->requestType, res);

			char *log=new char[res->payLoadSize+1];
			for(int i=0; i<res->payLoadSize;i++)
				log[i] = res->payLoad[i];
			log[res->payLoadSize] = '\0';
			delete[] log;

			int ret = res->payLoadSize+9;
			delete[] res->payLoad;
			delete res;
			return ret;
		}

		int Client::handleNotify(char *data, int index)
		{
			notify *res = buildNotify(data, index);
            if(_notificationListener != NULL){
                if(res->updateType == UpdateType::user_joined_lobby)
                {
                    lobby _lobby;
                    buildLobbyData(_lobby,res->payLoad,res->payLoadSize);
                    std::string user = getJSONString("user",res->payLoad,res->payLoadSize);
                    _notificationListener->onUserJoinedLobby(_lobby,user);
                }
                else if(res->updateType == UpdateType::user_left_lobby)
                {
                    lobby _lobby;
                    buildLobbyData(_lobby,res->payLoad,res->payLoadSize);
                    std::string user = getJSONString("user",res->payLoad,res->payLoadSize);
                    _notificationListener->onUserLeftLobby(_lobby,user);
                }
                else if(res->updateType == UpdateType::chat)
                {
                    chat cht;
                    cht.chat = getJSONString("chat",res->payLoad, res->payLoadSize);
                    cht.sender = getJSONString("sender",res->payLoad, res->payLoadSize);
                    cht.locid = getJSONInt("locid",res->payLoad, res->payLoadSize);
                    cht.isLocationLobby = getJSONBool("isLocationLobby",res->payLoad, res->payLoadSize);
                    _notificationListener->onChatReceived(cht);
                }
                else if(res->updateType == UpdateType::private_chat)
                {
                    std::string sender = getJSONString("sender",res->payLoad, res->payLoadSize);
                    std::string message = getJSONString("chat", res->payLoad, res->payLoadSize);
                    _notificationListener->onPrivateChatReceived(sender, message);
                }
                else if(res->updateType == UpdateType::update_peers)
                {
                    byte *update = new byte[res->payLoadSize];
                    for(int i=0; i<res->payLoadSize; ++i)
                        update[i] = res->payLoad[i];
                    _notificationListener->onUpdatePeersReceived(update, res->payLoadSize);
                    
                    delete[] update;
                }
                else if(res->updateType == UpdateType::room_created)
                {
                    room rm;
                    buildRoomData(rm, res->payLoad, res->payLoadSize);
                    _notificationListener->onRoomCreated(rm);
                }
                else if(res->updateType == UpdateType::room_deleted)
                {
                    room rm;
                    buildRoomData(rm, res->payLoad, res->payLoadSize);
                    _notificationListener->onRoomDestroyed(rm);
                }
                else if(res->updateType == UpdateType::user_joined_room)
                {
                    room rm;
                    buildRoomData(rm, res->payLoad, res->payLoadSize);
                    std::string user = getJSONString("user",res->payLoad,res->payLoadSize);
                    _notificationListener->onUserJoinedRoom(rm, user);
                }
                else if(res->updateType == UpdateType::user_left_room)
                {
                    room rm;
                    buildRoomData(rm, res->payLoad, res->payLoadSize);
                    std::string user = getJSONString("user",res->payLoad,res->payLoadSize);
                    _notificationListener->onUserLeftRoom(rm, user);
                }
                else if(res->updateType == UpdateType::room_property_change)
                {
                    std::map<std::string, std::string> properties;
                    std::map<std::string, std::string> lockTable;
                    room rm;
                    buildRoomData(rm, res->payLoad, res->payLoadSize);
                    std::string user = getJSONString("sender",res->payLoad,res->payLoadSize);
                    
                    std::string properties_str = getJSONString("properties",res->payLoad,res->payLoadSize);
                    cJSON *json;
                    json = cJSON_Parse(properties_str.c_str());
                    if(json != NULL)
                    {
                        json = json->child;
                        while(json != NULL)
                        {
                            properties.insert(std::pair<std::string,std::string>(json->string,json->valuestring));
                            json = json->next;
                        }
                    }
                    
                    std::string locktable_str = getJSONString("lockProperties",res->payLoad,res->payLoadSize);
                    cJSON *jsonLocktable;
                    jsonLocktable = cJSON_Parse(locktable_str.c_str());
                    if(jsonLocktable != NULL)
                    {
                        jsonLocktable = jsonLocktable->child;
                        while(jsonLocktable != NULL)
                        {
                            lockTable.insert(std::pair<std::string,std::string>(jsonLocktable->string,jsonLocktable->valuestring));
                            jsonLocktable = jsonLocktable->next;
                        }
                    }
                    
                    _notificationListener->onUserChangeRoomProperty(rm, user,properties, lockTable);
                    
                    cJSON_Delete(json);
                    cJSON_Delete(jsonLocktable);
                }
            }

			int ret = res->payLoadSize+8;
			delete[] res->payLoad;
			delete res;
			return ret;
		}

		void Client::handleAuthResponse(response *res)
		{
			if(res->resultCode == ResultCode::success)
			{
				char *str = new char[res->payLoadSize];
				for(int i=0; i<res->payLoadSize; ++i)
				{
					str[i] = (char)res->payLoad[i];
				}
				std::string sessionId_str = getJSONString("sessionid",res->payLoad, res->payLoadSize);
				int sessionId = atoi(sessionId_str.c_str());
				AppWarpSessionID = sessionId;
                _state = ConnectionState::connected;
				delete[] str;
			}
            else {
                _state = ConnectionState::disconnected;
            }
			if(_connectionReqListener != NULL)
				_connectionReqListener->onConnectDone(res->resultCode);
		}

		void Client::handleLobbyResponse(int reqType,response *res)
		{
			lobby _lobby;
			
			buildLobbyData(_lobby,res->payLoad, res->payLoadSize);
			_lobby.result = res->resultCode;

			if(reqType == RequestType::join_lobby)
			{
				if(_lobbyListener != NULL)
					_lobbyListener->onJoinLobbyDone(_lobby);
			}
			else if(reqType == RequestType::subscribe_lobby)
			{
				if(_lobbyListener != NULL)
					_lobbyListener->onSubscribeLobbyDone(_lobby);
			}
			else if(reqType == RequestType::unsubscribe_lobby)
			{
				if(_lobbyListener != NULL)
					_lobbyListener->onUnsubscribeLobbyDone(_lobby);
			}
			else if(reqType == RequestType::leave_lobby)
			{
				if(_lobbyListener != NULL)
					_lobbyListener->onLeaveLobbyDone(_lobby);
			}
			else if(reqType == RequestType::get_lobby_info)
			{
				livelobby ll;
				ll.result = res->resultCode;
				buildRoomData(ll.rm,res->payLoad,res->payLoadSize);
				ll.customData = getJSONString("data",res->payLoad,res->payLoadSize);
				std::string users = getJSONString("usernames",res->payLoad,res->payLoadSize);
				std::string name = "";
				for(unsigned int i=0; i<users.length(); ++i)
				{
					if(users[i] == ';')
					{
						ll.users.push_back(name);
						name = "";
					}
					else
					{
						name.append(users.substr(i,1));
					}
				}

				if(_lobbyListener != NULL)
					_lobbyListener->onGetLiveLobbyInfoDone(ll);
			}
		}

		void Client::handleRoomResponse(int reqType,response *res)
		{
			room _room;
			
			buildRoomData(_room,res->payLoad, res->payLoadSize);
			_room.result = res->resultCode;

			if(reqType == RequestType::join_room)
			{
				if(_roomlistener != NULL)
					_roomlistener->onJoinRoomDone(_room);
			}
			else if(reqType == RequestType::leave_room)
			{
				if(_roomlistener != NULL)
					_roomlistener->onLeaveRoomDone(_room);
			}
			else if(reqType == RequestType::subscribe_room)
			{
				if(_roomlistener != NULL)
					_roomlistener->onSubscribeRoomDone(_room);
			}
			else if(reqType == RequestType::unsubscribe_room)
			{
				if(_roomlistener != NULL)
					_roomlistener->onUnsubscribeRoomDone(_room);
			}
			else if(reqType == RequestType::get_room_info)
			{
				liveroom lr;
				lr.result = res->resultCode;

				buildRoomData(lr.rm,res->payLoad,res->payLoadSize);
				lr.customData = getJSONString("data",res->payLoad,res->payLoadSize);
				std::string users = getJSONString("usernames",res->payLoad,res->payLoadSize);
				std::string name = "";
				for(unsigned int i=0; i<users.length(); ++i)
				{
					if(users[i] == ';')
					{
						lr.users.push_back(name);
						name = "";
					}
					else
					{
						name.append(users.substr(i,1));
					}
				}

				std::string properties = getJSONString("properties",res->payLoad,res->payLoadSize);

				cJSON *json;
				json = cJSON_Parse(properties.c_str());
				if(json != NULL)
				{
					json = json->child;
					while(json != NULL)
					{
						lr.properties.insert(std::pair<std::string,std::string>(json->string,json->valuestring));
						json = json->next;
					}
				}

				if(_roomlistener != NULL)
					_roomlistener->onGetLiveRoomInfoDone(lr);

				cJSON_Delete(json);
			}
			else if(reqType == RequestType::set_custom_room_data)
			{
				liveroom lr;
				lr.result = res->resultCode;

				buildRoomData(lr.rm,res->payLoad,res->payLoadSize);
				lr.customData = getJSONString("data",res->payLoad,res->payLoadSize);
				std::string users = getJSONString("usernames",res->payLoad,res->payLoadSize);
				std::string name = "";
				for(unsigned int i=0; i<users.length(); ++i)
				{
					if(users[i] == ';')
					{
						lr.users.push_back(name);
						name = "";
					}
					else
					{
						name.append(users.substr(i,1));
					}
				}

				std::string properties = getJSONString("properties",res->payLoad,res->payLoadSize);

				cJSON *json;
				json = cJSON_Parse(properties.c_str());
				if(json != NULL)
				{
					json = json->child;
					while(json != NULL)
					{
						lr.properties.insert(std::pair<std::string,std::string>(json->string,json->valuestring));
						json = json->next;
					}
				}

				if(_roomlistener != NULL)
					_roomlistener->onSetCustomRoomDataDone(lr);

				cJSON_Delete(json);
			}
			else if(reqType == RequestType::update_room_property)
			{
				liveroom lr;
				lr.result = res->resultCode;

				buildRoomData(lr.rm,res->payLoad,res->payLoadSize);
				lr.customData = getJSONString("data",res->payLoad,res->payLoadSize);
				std::string users = getJSONString("usernames",res->payLoad,res->payLoadSize);
				std::string name = "";
				for(unsigned int i=0; i<users.length(); ++i)
				{
					if(users[i] == ';')
					{
						lr.users.push_back(name);
						name = "";
					}
					else
					{
						name.append(users.substr(i,1));
					}
				}

				std::string properties = getJSONString("properties",res->payLoad,res->payLoadSize);

				cJSON *json;
				json = cJSON_Parse(properties.c_str());
				json = json->child;
				while(json != NULL)
				{
					lr.properties.insert(std::pair<std::string,std::string>(json->string,json->valuestring));
					json = json->next;
				}

				if(_roomlistener != NULL)
					_roomlistener->onUpdatePropertyDone(lr);

				cJSON_Delete(json);
			}
		}

		void Client::handleZoneResponse(int reqType,response *res)
		{
			room _room;
			
			buildRoomData(_room,res->payLoad, res->payLoadSize);
			_room.result = res->resultCode;

			if(reqType == RequestType::delete_room)
			{
				if(_zonelistener != NULL)
					_zonelistener->onDeleteRoomDone(_room);
			}
			else if(reqType == RequestType::create_room)
			{
				if(_zonelistener != NULL)
					_zonelistener->onCreateRoomDone(_room);
			}
			else if(reqType == RequestType::get_user_info)
			{
				liveuser lu;
				lu.result = res->resultCode;

				lu.name = getJSONString("name",res->payLoad,res->payLoadSize);
				lu.customData = getJSONString("custom",res->payLoad,res->payLoadSize);
				lu.locationId = getJSONString("locationId",res->payLoad,res->payLoadSize);

				if(_zonelistener != NULL)
					_zonelistener->onGetLiveUserInfoDone(lu);
			}
			else if(reqType == RequestType::set_custom_user_data)
			{
				liveuser lu;
				lu.result = res->resultCode;

				lu.name = getJSONString("name",res->payLoad,res->payLoadSize);
				lu.customData = getJSONString("custom",res->payLoad,res->payLoadSize);
				lu.locationId = getJSONString("locationId",res->payLoad,res->payLoadSize);

				if(_zonelistener != NULL)
					_zonelistener->onSetCustomUserInfoDone(lu);
			}
			else if(reqType == RequestType::get_rooms)
			{
				liveresult lr;
				lr.result = res->resultCode;

				std::string ids = getJSONString("ids",res->payLoad,res->payLoadSize);
				std::string id = "";
				for(unsigned int i=0; i<ids.length(); ++i)
				{
					if(ids[i] == ';')
					{
						lr.list.push_back(id);
						id = "";
					}
					else
					{
						id.append(ids.substr(i,1));
					}
				}

				if(_zonelistener != NULL)
					_zonelistener->onGetAllRoomsDone(lr);
			}
			else if(reqType == RequestType::get_users)
			{
				liveresult lr;
				lr.result = res->resultCode;

				std::string users = getJSONString("names",res->payLoad,res->payLoadSize);
				std::string name = "";
				for(unsigned int i=0; i<users.length(); ++i)
				{
					if(users[i] == ';')
					{
						lr.list.push_back(name);
						name = "";
					}
					else
					{
						name.append(users.substr(i,1));
					}
				}

				if(_zonelistener != NULL)
					_zonelistener->onGetOnlineUsersDone(lr);
			}
			else if(reqType == RequestType::get_room_with_n_user ||
                    reqType == RequestType::get_room_with_properties ||
                    reqType == RequestType::get_room_range)
			{
				matchedroom mr;
				mr.result = res->resultCode;

				char *str = new char[res->payLoadSize];
				for(int i=0; i<res->payLoadSize; ++i)
				{
					str[i] = (char)res->payLoad[i];
				}

				cJSON *json,*json2, *begPtr;
				json = cJSON_Parse(str);
				begPtr = json;
				json = json->child;
				while(json != NULL)
				{
					room rm;
					json2 = json->child;
					while(json2 != NULL)
					{
						if(strcmp(std::string(json2->string).c_str(),"name") == 0)
							rm.name = json2->valuestring;
						if(strcmp(std::string(json2->string).c_str(),"owner") == 0)
							rm.owner = json2->valuestring;
						if(strcmp(std::string(json2->string).c_str(),"maxUsers") == 0)
							rm.maxUsers = json2->valueint;

						rm.roomId = json->string;

						json2 = json2->next;
					}
					mr.roomData.push_back(rm);
					json = json->next;
				}

				if(_zonelistener != NULL)
					_zonelistener->onGetMatchedRoomsDone(mr);

				cJSON_Delete(begPtr);
				delete[] str;
			}
		}
}