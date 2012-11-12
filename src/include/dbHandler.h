#ifndef __DBHANDLER__
#define __DBHANDLER_
#include <stdio.h>
#include <vector>
#include <map>
#include <string>

using namespace std;

class DB
{
	public:
		DB():newFileName("./new.txt"),oldFileName("./old.txt"),dayBack(1){}
		DB(string newFile, string oldFile, int dayback);
		bool GetMessage(string channel);
		//for computing the weibo stop words table
		//can not use the default constructor function
		bool GetMessageForStop(string channel); 
		
	public:
		string			newFileName;
		string          oldFileName;
		int 			dayBack;
};

#endif
