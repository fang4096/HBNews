#ifndef UTILITY_H
#define UTILITY_H
#include <client/dbclient.h>
#include <bson/bson.h>
#include <string>
#include <vector>

using namespace mongo;

struct _stLookupstatusInfo
{
	string m_sID;
	unsigned int m_nStatusNum;
};

extern DBClientConnection con;

unsigned int GetTime();
void initialize();
//向文件中写入comment
bool WriteToFile(string sFileName, string sMode, string sComment);
string VectorToString(vector< string > &vstring);

bool IsJapanese(const char* utf8);
bool IsChinese(const char* utf8);

#endif
