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
		std::string ItoA(int num)
		{
			std::stringstream str;
			str << num;
			std::string res(str.str());
			return res;
		}

		int bytesToInteger(char data[], int offset)
		{
			int value = 0;
			for(int i=0; i<4; ++i)
			{
				value = (value << 8) + (data[offset + i] & 0xff);
			}

			return value;
		}

		std::string getJSONString(const char* name,byte *data,int len)
		{
			std::string user;

			char *str = new char[len];
			for(int i=0; i<len; ++i)
			{
				str[i] = (char)data[i];
			}

			cJSON *json, *begPtr;
			json = cJSON_Parse(str);
			begPtr = json;
			json = json->child;
			while(json != NULL)
			{
				if(strcmp(json->string,name) ==  0)
				{
					user = json->valuestring;
					break;
				}
				json = json->next;
			}
			cJSON_Delete(begPtr);
			delete[] str;
			return user;
		}

		int getJSONInt(const char* name,byte *data,int len)
		{
			int num = -1;

			char *str = new char[len];
			for(int i=0; i<len; ++i)
			{
				str[i] = (char)data[i];
			}

			cJSON *json, *begPtr;
			json = cJSON_Parse(str);
			begPtr = json;
			json = json->child;
			while(json != NULL)
			{
				if(strcmp(json->string,name) ==  0)
				{
					num = json->valueint;
					break;
				}
				json = json->next;
			}
			cJSON_Delete(begPtr);
			delete[] str;
			return num;
		}

		bool getJSONBool(const char* name,byte *data,int len)
		{
			bool ret = false;

			char *str = new char[len];
			for(int i=0; i<len; ++i)
			{
				str[i] = (char)data[i];
			}

			cJSON *json, *begPtr;
			json = cJSON_Parse(str);
			begPtr = json;
			json = json->child;
			while(json != NULL)
			{
				if(strcmp(json->string,name) ==  0)
				{
					if(json->type == cJSON_True)
						ret = true;
					else
						ret = false;
					break;
				}
				json = json->next;
			}
			cJSON_Delete(begPtr);
			delete[] str;
			return ret;
		}

		std::string getODataUTCDateFilter()
		{
			time_t localTime;
			struct tm *utcTime_tm;

			localTime = time(NULL);
			utcTime_tm = gmtime(&localTime);
			
			std::string date(ItoA(1900+utcTime_tm->tm_year));
			date.append("-");
			if((utcTime_tm->tm_mon+1) < 10)
			{
				date.append("0");
				date.append(ItoA(utcTime_tm->tm_mon+1));
			}
			else
				date.append(ItoA(utcTime_tm->tm_mon+1));
			date.append("-");
			if((utcTime_tm->tm_mday) < 10)
			{
				date.append("0");
				date.append(ItoA(utcTime_tm->tm_mday));
			}
			else
				date.append(ItoA(utcTime_tm->tm_mday));
			date.append("T");
			date.append(ItoA(utcTime_tm->tm_hour));
			date.append(":");
			date.append(ItoA(utcTime_tm->tm_min));
			date.append(":");
			date.append(ItoA(utcTime_tm->tm_sec));
			date.append(".000Z");

			return date;
		}
}