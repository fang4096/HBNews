#ifndef __SEG_
#define __SEG_

#include<stdio.h>
#include<vector>
#include<map>
#include<set>
#include<string>
#include"bamboo.hxx"
using namespace std;
using namespace bamboo;
class Seg
{
	public:
		Seg():inFileName("./message.txt"){}
		Seg(std::string in);
		bool PreProcess();
		void Clean();
		~Seg(){}
	private:
		FILE* OpenFile(string fileName, string mode);
		void  SegMessage(FILE *in);
		void  Filter(FILE *out);       //reduce the size of words
//		void  Filter(const set<string> &stopWords);
//		void  SortForResult();
	public:
//		vector< string >             result;
		map< string, int >	         m_words;
	private:
		string                       inFileName;	
		static ParserFactory*        parserFactory;
};
#endif
