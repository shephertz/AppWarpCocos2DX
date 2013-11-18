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

		byte* buildWarpRequest(int requestType, std::string payload, int &byteLen)
		{

			byteLen = 16 + payload.length();
			//byte *byteArray = new byte[byteLen];
			byte *byteArray = new byte[byteLen];

			//WarpRequest
			byteArray[0] = (byte)MessageType::request;
			byteArray[1] = (byte)requestType;

			//bytes 2-5 : session id
			byteArray[2] = AppWarpSessionID >> 24;
			byteArray[3] = AppWarpSessionID >> 16;
			byteArray[4] = AppWarpSessionID >> 8;
			byteArray[5] = AppWarpSessionID;

			//bytes 6-9 : request id
			for(int i=6; i<=9; ++i)
				byteArray[i] = 0;

			//byte 10 : reserved
			byteArray[10] = 0;

			//byte 11 : payload type String, Binary, JSON
			if(payload.length() > 0 && requestType != RequestType::update_peers)
				byteArray[11] = (byte)PayLoadType::json;
			else
				byteArray[11] = (byte)PayLoadType::binary;

			//byte 12-15 : payload size
			int payloadSize = payload.length();
			byteArray[12] = payloadSize >> 24;
			byteArray[13] = payloadSize >> 16;
			byteArray[14] = payloadSize >> 8;
			byteArray[15] = payloadSize;

			//bytes 16 onwards : actual payload
			for(unsigned int i=0; i < payload.length(); ++i)
				byteArray[16+i] = payload.at(i);

			return byteArray;
		}

		byte* buildWarpRequest(int requestType, byte *payload, int payload_len, int &byteLen)
		{

			byteLen = 16 + payload_len;
			//byte *byteArray = new byte[byteLen];
			byte* byteArray = new byte[byteLen];

			//WarpRequest
			byteArray[0] = (byte)MessageType::request;
			byteArray[1] = (byte)requestType;
			
			//bytes 2-5 : session id
			byteArray[2] = AppWarpSessionID >> 24;
			byteArray[3] = AppWarpSessionID >> 16;
			byteArray[4] = AppWarpSessionID >> 8;
			byteArray[5] = AppWarpSessionID;

			//bytes 6-9 : request id
			for(int i=6; i<=9; ++i)
				byteArray[i] = 0;

			//byte 10 : reserved
			byteArray[10] = 0;

			//byte 11 : payload type String, Binary, JSON
			if(payload_len > 0 && requestType != RequestType::update_peers)
				byteArray[11] = (byte)PayLoadType::json;
			else
				byteArray[11] = (byte)PayLoadType::binary;

			//byte 12-15 : payload size
			int payloadSize = payload_len;
			byteArray[12] = payloadSize >> 24;
			byteArray[13] = payloadSize >> 16;
			byteArray[14] = payloadSize >> 8;
			byteArray[15] = payloadSize;

			//bytes 16 onwards : actual payload
			for(int i=0; i<payload_len; ++i)
				byteArray[16+i] = payload[i];

			return byteArray;
		}

		byte* buildAuthRequest(std::string username, int &byteLen,std::string APIKEY, std::string SECRETKEY)
		{
			std::string timeStamp = getODataUTCDateFilter();
			std::string params;
			params.append("apiKey");
			params.append(APIKEY);
			params.append("timeStamp");
			params.append(timeStamp);
			params.append("user");
			params.append(username);
			params.append("version0.1");

			unsigned char hmac_digest[20];
			memset(hmac_digest, 0, 20);
			CHMAC_SHA1 hmac_sha1;
			hmac_sha1.HMAC_SHA1((unsigned char *)params.c_str(),params.length(), (unsigned char *)SECRETKEY.c_str(), SECRETKEY.length(),hmac_digest);
			std::string hmac = urlencode(base64_encode(hmac_digest, 20));

			cJSON *payloadJSON;
			payloadJSON = cJSON_CreateObject();
			cJSON_AddStringToObject(payloadJSON,"apiKey", APIKEY.c_str());
			cJSON_AddStringToObject(payloadJSON,"version", "0.1");
			cJSON_AddStringToObject(payloadJSON,"timeStamp", timeStamp.c_str());
			cJSON_AddStringToObject(payloadJSON,"user", username.c_str());
			cJSON_AddStringToObject(payloadJSON,"signature", hmac.c_str());
            cJSON_AddNumberToObject(payloadJSON, "keepalive", WARP_KEEP_ALIVE_TIME_INTERVAL);//recoverytime
            cJSON_AddNumberToObject(payloadJSON, "recoverytime", RECOVERY_ALLOWANCE_TIME);
			char* cRet = cJSON_PrintUnformatted(payloadJSON);
			std::string payload = cRet;
			free(cRet);

			cJSON_Delete(payloadJSON);

			return buildWarpRequest(RequestType::auth, payload, byteLen);
		}

		byte * buildLobbyRequest(int requestType, int &len)
		{
			std::string payload;
			cJSON *payloadJSON;
			payloadJSON = cJSON_CreateObject();
			cJSON_AddTrueToObject(payloadJSON, "isPrimary");
			char* cRet = cJSON_PrintUnformatted(payloadJSON);			
			payload = cRet;
			free(cRet);
			cJSON_Delete(payloadJSON);
			return buildWarpRequest(requestType, payload, len);
		}
    
        byte * buildKeepAliveRequest(int requestType, int &len)
        {
            std::string payload;
            cJSON *payloadJSON;
            payloadJSON = cJSON_CreateObject();
            char* cRet = cJSON_PrintUnformatted(payloadJSON);
            payload = cRet;
            free(cRet);
            cJSON_Delete(payloadJSON);
            return buildWarpRequest(requestType, payload, len);
        }

		byte * buildRoomRequest(int requestType,std::string id, int &len)
		{
			std::string payload;
			cJSON *payloadJSON;
			payloadJSON = cJSON_CreateObject();
			cJSON_AddStringToObject(payloadJSON, "id",id.c_str());
			char* cRet = cJSON_PrintUnformatted(payloadJSON);			
			payload = cRet;
			free(cRet);
			cJSON_Delete(payloadJSON);
			return buildWarpRequest(requestType, payload, len);
		}

		byte * buildCreateRoomRequest(std::string name,std::string owner,int max, int &len)
		{
			std::string payload;
			cJSON *payloadJSON;
			payloadJSON = cJSON_CreateObject();
			cJSON_AddStringToObject(payloadJSON, "name",name.c_str());
			cJSON_AddStringToObject(payloadJSON, "owner",owner.c_str());
			cJSON_AddNumberToObject(payloadJSON, "maxUsers", max);
			char* cRet = cJSON_PrintUnformatted(payloadJSON);			
			payload = cRet;
			free(cRet);
			cJSON_Delete(payloadJSON);
			return buildWarpRequest(RequestType::create_room, payload, len);
		}
		
		byte * buildCreateRoomRequest(std::string name,std::string owner,int max, std::string properties, int time, int &len)
		{
			std::string payload;
			cJSON *payloadJSON;
			payloadJSON = cJSON_CreateObject();
			cJSON_AddStringToObject(payloadJSON, "name",name.c_str());
			cJSON_AddStringToObject(payloadJSON, "owner",owner.c_str());
			cJSON_AddNumberToObject(payloadJSON, "maxUsers", max);
			cJSON_AddStringToObject(payloadJSON, "properties", properties.c_str());
            cJSON_AddNumberToObject(payloadJSON, "turnTime", time);
            cJSON_AddTrueToObject(payloadJSON, "inox");
			char* cRet = cJSON_PrintUnformatted(payloadJSON);			
			payload = cRet;
			free(cRet);
			cJSON_Delete(payloadJSON);
			return buildWarpRequest(RequestType::create_room, payload, len);
		}

		response * buildResponse(char *data, int index)
		{
			response *res = new response;
			res->messageType = data[index+0];
			res->requestType = data[index+1];
			res->resultCode = data[index+2];
			res->reserved = data[index+3];
			res->payLoadType = data[index+4];
			res->payLoadSize = bytesToInteger(data,index+5);
			res->payLoad = new byte[res->payLoadSize];
			for(int i=0; i<res->payLoadSize; ++i)
				res->payLoad[i] = data[9+index+i];

			return res;
		}

		notify* buildNotify(char *data, int index)
		{
			notify *res = new notify;
			res->messageType = data[index+0];
			res->updateType = data[index+1];
			res->reserved = data[index+2];
			res->payLoadType = data[index+3];
			res->payLoadSize = bytesToInteger(data,index+4);
			res->payLoad = new byte[res->payLoadSize];
			for(int i=0; i<res->payLoadSize; ++i)
				res->payLoad[i] = data[8+index+i];

			return res;
		}
}