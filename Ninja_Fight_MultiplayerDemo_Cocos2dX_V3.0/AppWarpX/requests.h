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

#ifndef requests__h
#define requests__h

namespace AppWarp
{
		byte* buildWarpRequest(int, std::string, int &, byte reserved=0);
		byte* buildWarpRequest(int , byte *, int , int &, byte reserved = 0);
		byte* buildAuthRequest(std::string, int &,std::string, std::string);
		byte* buildLobbyRequest(int,int &);
		byte *buildRoomRequest(int,std::string,int &);
		byte *buildCreateRoomRequest(std::string,std::string,int,int &);
		byte *buildCreateRoomRequest(std::string,std::string,int,std::string,int,int &);
        byte *buildKeepAliveRequest(int requestType, int &len);
        byte * buildSignOutRequest(int requestType, int &len);
		response *buildResponse(char *, int);
		notify *buildNotify(char *, int);
}

#endif