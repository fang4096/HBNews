#include "cFile.h"
#define MAX_LINE_LEN 1024

FILE* CFile::OpenFile(string filename, string mode)
{
	FILE* fp = fopen(filename.c_str(), mode.c_str());
	if(fp == NULL)
		printf("open %s wrong!\n", filename.c_str());
	return fp;
}

void CFile::CloseFile(FILE *fp)
{
	fclose(fp);
}

bool CFile::ReadFileToVector(FILE *fp, vector<string>& lines)
{
	if(fp == NULL)
		return false;
	lines.clear();
	char line[MAX_LINE_LEN];
	while(fgets(line, sizeof(line), fp))
	{
		lines.push_back(string(line));
	}
	return true;
}


