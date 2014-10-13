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

#include "udpsocket.h"
#include <fcntl.h>
#include "appwarp.h"

using namespace std;

namespace AppWarp
{
	namespace Utility
	{
        
		UdpSocket::UdpSocket(Client* owner)
		{
            _callBack = owner;
            sockd = -1;
		}
        
		UdpSocket::~UdpSocket()
		{
            
		}
        
        int UdpSocket::connect(string host, short port)
        {
            if ((sockd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
                return AppWarp::result_failure;
            }
            
            fcntl(sockd, F_SETFL, O_NONBLOCK);
            
            memset((char*)&serv_name, 0, sizeof(serv_name));
            
            serv_name.sin_family = AF_INET;
            inet_aton(host.c_str(), &serv_name.sin_addr);
            serv_name.sin_port = htons(port);
            
            return AppWarp::result_success;
        }
        
		int UdpSocket::sockSend(char *messageToSend,int messageLength)
		{
            if (sendto(sockd, messageToSend, messageLength, 0, (struct sockaddr *)&serv_name, sizeof(serv_name)) < 0) {
                return AppWarp::result_failure;
            }
            return AppWarp::result_success;
		}
        
		void UdpSocket::checkMessages()
		{
            if(sockd == -1){
                return;
            }
            unsigned char buf[2048];
            socklen_t addrlen = sizeof(serv_name);
            int ret = recvfrom(sockd, buf, 2048, 0, (struct sockaddr *)&serv_name, &addrlen);
            if(ret > 0){
                if(buf[0] == MessageType::response){
                    response* udpresponse = buildResponse((char*)buf, 0);
                    _callBack->udpresponse(udpresponse);
                    delete udpresponse;
                }
                else if(buf[0] == MessageType::update){
                    notify* notification = buildNotify((char*)buf, 0);
                    _callBack->udpnotify(notification);
                    delete notification;
                }
            }
		}
        
        void UdpSocket::disconnect()
        {
            close(sockd);
        }
        
	}
}