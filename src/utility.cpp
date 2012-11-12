#include "utility.h"
#include <iostream>
//#include <client/dbclient.h>
//#include <bson/bson.h>
#include <ctime>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <sys/stat.h>
#include "logger.h"
#define CHARLEN 1024
//using namespace mongo;
DBClientConnection con;

static char mongodb_host[30], mongodb_port[20];
const int time_len = 20;
static char time_str[time_len];
static const int file_name_len = 256;
static int command_len = 256;
static const char *prog_name = "topic";
const static char *log_file_name_prefix = "log/topic_";
int each_time_num = 20000;

static void read_config_file()
{
	FILE *fp;
	char temp[256];
	char *p, *q;

	fp = fopen("config.ini", "rb");
	if (!fp)
	{
		log(SEVERE, "No config file config.ini found, please check!\n");
		abort();
	}
	while(fgets(temp, 255, fp) != NULL)
	{
		if((p = strstr(temp, "mongodb:")) != NULL)
		{
			p += strlen("mongodb:");

			while (*p)
			{
				if (*p == ' ' || *p == '\t' || *p == '\n')
					++p;
				else
					break;
			}

			q = mongodb_host;
			while (*p && *p != ' ' && *p != '\t' && *p != '\n')
				*q++ = *p++;
			*q = '\0';

			while (*p)
			{
				if (*p == ' ' || *p == '\t' || *p == '\n')
					++p;
				else
					break;
			}

			q = mongodb_port;
			while (*p && *p != ' ' && *p != '\t' && *p != '\n')
				*q++ = *p++;
			*q = '\0';
		}
	}
	fclose(fp);
}

void initialize () // do some initialization, like connecting to database etc
{
	char connect_str[30];

	strcpy(mongodb_host, "localhost");
	strcpy(mongodb_port, "27017");
	init_log_file(log_file_name_prefix);
	log(INFO, "* * * * * * * * * * * * * %s started * * * * * * * * * * * * *\n", prog_name);
	read_config_file();
	sprintf(connect_str, "%s:%s", mongodb_host, mongodb_port);
	log(INFO, "finished reading config file, connect string -- %s\n", connect_str);
	try 
	{
		con.connect(connect_str);
		log(INFO, "mongodb connect succeed!\n");
	}
	catch (DBException e)
	{
		log (SEVERE, "DataBase connect error: %s\n", e.what());
		abort ();
	}
}

static void replace_str(char *str, char old_ch, char new_ch)
{
	if (str == NULL)
		return;

	while (*str)
	{
		if (*str == old_ch)
			*str = new_ch;
		++str;
	}
}

static time_t getTime(int year, int month, int day)
{
	time_t now;

	struct tm *time_info;
	time(&now);
	time_info = localtime(&now);
	time_info->tm_year = year;
	time_info->tm_mon = month;
	time_info->tm_mday = day;
	time_info->tm_sec = 0;
	time_info->tm_min = 0;
	time_info->tm_hour = 0;

	return mktime(time_info);
}

static void get_time_str(time_t *tm, char splt_char, char *time_str)
{
	struct tm *time_info;
	time_info = localtime(tm);
	strftime (time_str, time_len, "%Y:%m:%d:%H:%M:%S", time_info);

	if (splt_char != ':')
		replace_str(time_str, ':', splt_char);
}

// get the yyyy_mm_dd_hh_MM_ss string and store in the time_str
static void get_time_str(char *time_str)
{
	time_t now;
	time(&now);
	get_time_str(&now, '_', time_str);
	cout << time_str << endl;
}


static void proceed_text(const char *src, char *dst)
{
	if (src == NULL || dst == NULL)
		return;

	do
	{
		if (*src == '\n' || *src == '\r')
		{
			*dst++ = ' ';
		}
		else
			*dst++ = *src;
	} 
	while(*src++);
}

//获取时间
unsigned int GetTime()
{
	time_t t;
	time(&t);
	return (unsigned int)t;
}

bool WriteToFile(string sFileName, string sMode, string sComment)
{
	FILE *fp = fopen(sFileName.c_str(), sMode.c_str());
	if(fp == NULL)
		return false;
	fprintf(fp, "%s", sComment.c_str());
	fclose(fp);
	return true;
}

string VectorToString(vector< string > &vstring)
{
	string result = "";
	vector< string >::iterator it = vstring.begin();
	for(it; it != vstring.end(); it++)
	{
		result += *it + " ";
	}
	return result;
}

//fang:
/*!
* \fn int fn_iIsJapaneseCode(const char* pcSrcStr)
* \brief 是否为日文编码，包含日文编码就认定为日文编码	
* \param[in]	pcSrcStr	要替换的子串
* \return 1 true，0 false
*  \author jade
*  \date [3/27/2008]
*/
int GetUTF8ByteNumForWord(unsigned char firstCh)
{
	unsigned char temp = 0x80;
	int num = 0;
	while(temp & firstCh)
	{
		num++;
		temp = (temp >> 1);
	}
	//printf("The number is:%d\n", num);
	return num;
}

bool IsJapanese(const char* utf8)
{
	int i = 0;
	int iCount = 0;
	int byteCount = 0;
	while(i < strlen(utf8))
	{
		switch(GetUTF8ByteNumForWord((unsigned char)utf8[i]))
		{
		case 0:
			byteCount = 1;
			break;
		case 2:
			byteCount = 2;
			break;
		case 3:
			if((unsigned char)utf8[i] == 0xE3)
			{
				if((unsigned char)utf8[i+1] == 0x81)
				{
					if((unsigned char)utf8[i+2] >= 0x80)
					{
						iCount++;
					}
					
				}
				if((unsigned char)utf8[i+1] == 0x82)
				{
						iCount++;
				}
				if((unsigned char)utf8[i+1] == 0x83)
				{
					if((unsigned char)utf8[i+2] <= 0xBF)
					{
						iCount++;
					}
				}
				if((unsigned char)utf8[i+1] == 0x87)
				{
					if((unsigned char)utf8[i+2] >= 0xB0 && (unsigned char)utf8[i+2] <= 0xBF)
					{
						iCount++;
					}
				}
			}
			byteCount = 3;
			break;
		case 4:
			byteCount = 4;
			break;
		case 5:
			byteCount = 5;
			break;
		case 6:
			byteCount = 6;
			break;
		default:
			printf("The length is more than 6\n");
			byteCount = 1;
			break;
		}
		i+=byteCount;
		if(iCount >= 1)
			return true;
	}
	return false;	
}

bool IsChinese(const char* utf8)
{
	int i = 0;
	int iCount = 0;
	int byteCount = 0;
	while(i < strlen(utf8))
	{
		switch(GetUTF8ByteNumForWord((unsigned char)utf8[i]))
		{
		case 0:
			byteCount = 1;
			break;
		case 2:
			byteCount = 2;
			break;
		case 3:
			if((unsigned char)utf8[i] >= 0xE4 && (unsigned char)utf8[i] <= 0xE9)
			{
				if((unsigned char)utf8[i] == 0xE4)
				{
					if((unsigned char)utf8[i+1] == 0xB8)
					{
						if((unsigned char)utf8[i+2] >=0x80)
						{
							iCount++;
						}
					}
					if((unsigned char)utf8[i+1] > 0xB8)
					{
						iCount++;
					}
				}
				if((unsigned char)utf8[i] == 0xE9)
				{
					if((unsigned char)utf8[i+1] == 0xBF)
					{
						if((unsigned char)utf8[i+2] <= 0xBF)
						{
							iCount++;
						}
					}
					if((unsigned char)utf8[i+1] < 0xBF)
					{
						iCount++;
					}
				}
				if((unsigned char)utf8[i] > 0xE4 && (unsigned char)utf8[i] < 0xE9)
				{
					iCount++;
				}			
			}
			
			byteCount = 3;
			break;
		case 4:
			byteCount = 4;
			break;
		case 5:
			byteCount = 5;
			break;
		case 6:
			byteCount = 6;
			break;
		default:
			printf("The length is more than 6\n");
			byteCount = 1;
			break;
		}
		i+=byteCount;
		if(iCount > 1)
			return true;
	}
	return false;	
}


