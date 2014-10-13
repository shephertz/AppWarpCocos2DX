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
#ifndef defines__h
#define defines__h
#include <vector>
#include <map>
namespace AppWarp
{
	typedef unsigned char byte;
	const int MAX_PROPERTY_SIZE_BYTES = 2048;

	struct response_struct
	{
		int messageType;
		int requestType;
		int resultCode;
		int reserved;
		int payLoadType;
		int payLoadSize;
		byte *payLoad;
	};
	typedef response_struct response; 

	struct notify_struct
	{
		int messageType;
		int updateType;
		int reserved;
		int payLoadType;
		int payLoadSize;
		byte *payLoad;
	};
	typedef notify_struct notify; 

	const int result_success = 0;
	const int result_failure = 1;

	// Warp Response Result Code
	namespace ResultCode
	{
		enum
		{
			success = 0,
			auth_error = 1,
			resource_not_found = 2,
			resource_moved = 3,
			bad_request = 4,	
			connection_error = 5,	
			unknown_error = 6,
			size_error = 7,
            success_recovered = 8,
            connection_error_recoverable = 9,
            user_paused_error = 10
		};
	}

	//Message Type
	namespace MessageType
	{
		enum
		{
			request = 0,
			response = 1,
			update = 2
		};
	}

	//Request Type
	namespace RequestType
	{
		enum
		{
			auth = 1,
			join_lobby = 2,
			subscribe_lobby = 3, 
			unsubscribe_lobby = 4,  
			leave_lobby = 5,   
			create_room = 6,   
			join_room = 7,   
			subscribe_room = 8,   
			unsubscribe_room = 9,   
			leave_room = 10,   
			delete_room = 11,   
			chat = 12,   
			update_peers = 13,   
			signout = 14,
			create_zone = 15,
			delete_zone = 16, 
			get_rooms = 17,
			get_users = 18,
			get_user_info = 19,
			get_room_info = 20,
			set_custom_room_data = 21,
			set_custom_user_data = 22,        
			get_lobby_info = 23,
			//MatchMaking
			join_room_n_user = 24,
			update_room_property = 25,
			join_room_with_properties = 27,
			get_room_with_n_user = 28,
			get_room_with_properties = 29,
            private_chat = 30,
            move = 31,
            private_update = 32,
            lock_properties = 35,
            unlock_properties = 36,
            join_room_range = 37,
            get_room_range = 38,
            keep_alive = 63,
            assoc_port = 64,
            ack_assoc_port = 65,
            start_game = 66,
            stop_game = 67,
            get_move_history = 68,
            set_next_turn = 69
		};
	}

	//Payload Type
	namespace PayLoadType
	{
		enum{
			flat_string = 0, 
			binary = 1,    
			json = 2
		};
	}

	//Update Type
	namespace UpdateType
	{
		enum
		{
			room_created = 1,
			room_deleted = 2,
			user_joined_lobby = 3,
			user_left_lobby = 4,
			user_joined_room = 5,
			user_left_room = 6,
			user_online = 7,
			user_offline = 8,
			chat = 9,
			update_peers = 10,
			//MatchMaking
			room_property_change = 11,
            private_chat = 12,
            move_completed = 13,
            user_paused = 14,
            user_resumed = 15,
            game_started = 16,
            game_stopped = 17,
            private_update = 18,
            next_turn_requested = 19
		};
	}
    
    //Reason Code
    namespace ReasonCode
    {
        enum{
            waiting_for_paused_user = 21,
            invalid_api_key = 22
        };
    }

    namespace ConnectionState
    {
        enum
        {
            connected = 0,
            connecting = 1,
            
            disconnected = 2,
            disconnecting= 3,
            
            recovering = 4,
        };
    }
    
    namespace SocketStream
    {
        enum
        {
            stream_connecting = 1,
            stream_connected = 2,
            stream_failed = 3,
        };
    }
    
	typedef struct lobby_struct
	{
		std::string lobbyid;
		std::string owner;
		int maxUsers;
		std::string name;
		bool isPrimary;
		int result;
	} lobby; 

	typedef struct room_struct
	{
		std::string roomId;
		std::string owner;
		int maxUsers;
		std::string name;
		int result;
	} room; 

	typedef struct livelobby_struct
	{
		room rm;
		std::string customData;
		std::vector<std::string> users;
		int result;
	} livelobby;

	typedef struct liveroom_struct
	{
		room rm;
		std::string customData;
		std::vector<std::string> users;
		std::map<std::string, std::string> properties;
		int result;
	} liveroom;

	typedef struct liveuser_struct
	{
		std::string name;
		std::string locationId;
		std::string customData;
		int result;
	} liveuser;

	typedef struct liveresult_struct
	{
		int result;
		std::vector<std::string> list;
	} liveresult;

	typedef struct matchedroom_struct
	{
		int result;
		std::vector<room> roomData;
	} matchedroom;

	typedef struct chat_struct
	{
		std::string chat;
		std::string sender;
		int locid;
		bool isLocationLobby;
		int result;
	} chat;
    
    typedef struct move_struct
    {
        std::string sender;
        std::string moveData;
        std::string nextTurn;
        std::string roomId;
    } move;
}

#endif