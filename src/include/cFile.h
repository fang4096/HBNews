#ifndef __CFILE_
#define __CFILE_
#include<stdio.h>
#include<vector>
#include<map>
#include<string>
using namespace std;
class CFile
{
	public:
		static FILE* OpenFile(string fileName, string mode);
		static void  CloseFile(FILE *fp);
		static bool  ReadFileToVector(FILE *fp, vector<string>& lines);
		//more functions to be continued
};
#endif
