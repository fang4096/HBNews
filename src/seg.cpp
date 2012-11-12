#include "seg.h"
#include <stdlib.h>
#include <cstring>
#include <algorithm>
#include <math.h>
#include "utility.h"
#define MAX_SENTENCE_LEN 4096
#define MAX_WORD_LEN 1024
#define TOPIC_WORD_NUM 10

/*
struct stTopicWord{
	string	word;
	double	tf_idf;
	bool operator > (const stTopicWord& rhs) const
	{
		return tf_idf > rhs.tf_idf;
	}
};
*/

ParserFactory* Seg::parserFactory	= ParserFactory::get_instance();

Seg::Seg(string in)
{
	inFileName	= in;
}

void Seg::Clean()
{
	m_words.clear();
}

FILE* Seg::OpenFile(string fileName, string mode)
{
	FILE *fp = fopen(fileName.c_str(), mode.c_str());
	if( fp == NULL )
		printf("open file:%s wrong!\n", fileName.c_str());
	return fp;
}

void Seg::SegMessage(FILE *in)
{
	Parser *parser	= parserFactory->create("crf_seg", NULL, false);
	char lineIn[MAX_SENTENCE_LEN];
	char lineOut[2*MAX_SENTENCE_LEN];
	char word[MAX_WORD_LEN];
	while(fgets(lineIn, sizeof(lineIn), in))
	{
		parser->setopt(BAMBOO_OPTION_TEXT, lineIn);
		vector< Token * > parserResult;
		parser->parse(parserResult);
		vector< Token * >::iterator it = parserResult.begin();
		for(it; it != parserResult.end(); it++)
		{
			strcpy(word, (*it)->get_orig_token());
			string sWord(word);
			if(m_words.count(sWord))
				m_words[sWord] += 1;
			else
				m_words.insert(make_pair(sWord, 1));
			delete *it;
		}
		parserResult.clear();
	}
}

void Seg::Filter(FILE *out)
{
    int threshold=10;
    map< string, int >::iterator it;
	for(it=m_words.begin(); it!=m_words.end(); it++)
	{
        if(it->second<=threshold) continue;
        if(IsJapanese((it->first).c_str())) continue;
        if(!IsChinese((it->first).c_str())) continue;
        
        fprintf(out, "%s\t%d\n", (it->first).c_str(), it->second);
    }
}

bool Seg::PreProcess()
{
	FILE *in	= OpenFile(inFileName, string("rb"));
	FILE *out   = OpenFile(inFileName+".words", string("wb"));
	if( in == NULL || out == NULL )
		return false;
	SegMessage(in); Filter(out);	
	fclose(in); fclose(out);
	return true;
}
/*
bool Seg::FindHotSpot()
{
    if(!PreProcess())
    {
        log(INFO, "ProProcess wrong!\n");
        return false;
    }
    ComputeTFIDF();
    SortForResult();
    return true;
}
*/
/*
void Seg::SortForResult()
{
    int i;
    map< string, double > tempMap;
    map< string, double >::iterator it;
    for(i=0; i<msgForSearch.size(); i++)
    {
        it = msgForSearch[i].begin();
        for(it; it!=msgForSearch[i].end(); it++)
        {
            if(tempMap.count(it->first))
                tempMap[it->first] += it->second;
            else
                tempMap.insert(make_pair(it->first, it->second));
        }
    }
    vector<stTopicWord> tempVector;
    for(it=tempMap.begin(); it!=tempMap.end(); it++)
    {
        stTopicWord topicWord;
        topicWord.word=it->first;
        topicWord.tf_idf=it->second;
    }
    sort(tempVector.begin(), tempVector.end(), greater<stTopicWord>());
	int k, wordNumber = TOPIC_WORD_NUM;
	for(k=0; k<wordNumber && k<tempVector.size(); k++)
	{
		printf("word:%s\ttf_idf:%.2f\n",tempVector[k].word.c_str(),tempVector[k].tf_idf);
		result.push_back(tempVector[k].word);
	}
}
*/
    			
//bool Seg::Search()
//{
//	if( !flag )
//	{
//		flag	= true;
//		int i	= 0;
//		for(i; i<msgForSearch.size(); i++)
//			index.push_back(i);
//	}
//	//testing
////	for(int i=0; i<index.size(); i++)
////		printf("index:%d\n", index[i]);
//	
//	int i, j, times = index.size();
//	vector< int >::iterator itIndex = index.begin();
//	for( i = 0; i < times; i++)
//	{
//		bool isAnswer                        = false;
//		vector< string >::iterator itKeyword = keyword.begin();
//		for(itKeyword; itKeyword != keyword.end(); itKeyword++)
//		{
//			if(msgForSearch[*itIndex].count(*itKeyword))
//			{
//				isAnswer = true;
//				break;
//			}
//		}
//		if(!isAnswer && itIndex != index.end())
//			itIndex = index.erase(itIndex);
//		else
//			itIndex++;
//	}
//	return true;
//}

//bool Seg::DoFilter(vector< string > &sKeyword)
//{
//	if( !flag )
//	{
//		printf("The keyword input wrong!\n");
//		return false;
//	}
//	int i, j, times = index.size();
//	vector< int >::iterator itIndex = index.begin();
//	for( i = 0; i < times; i++)
//	{
//		bool isAnswer                        = true;
//		vector< string >::iterator itKeyword = (sKeyword.begin())++;
//		for(itKeyword; itKeyword != sKeyword.end(); itKeyword++)
//		{
//			if(msgForSearch[*itIndex].count(*itKeyword))
//			{
//				isAnswer = false;
//				break;
//			}
//		}
//		if(!isAnswer && itIndex != index.end())
//		{
//			printf("delete index %d\n", *itIndex);
//			itIndex = index.erase(itIndex);
//		}
//		else
//			itIndex++;
//	}
//	return true;
//}

//bool Seg::DoSearch(vector< string > &sKeyword)
//{
//	PassKeyword(sKeyword);
//	Search();
//	for(int i=0; i<keyword.size(); i++)
//		printf("keyword:%s ",keyword[i].c_str());
////	for(int i=0; i<index.size(); i++)
//	printf("index number:%d ", index.size());
//	printf("\n");
//	return true;
//}

//bool Seg::FindTopicWord()
//{
//	map< string, int > tempResult;
//	vector< int >::iterator itIndex = index.begin();
//	for(itIndex; itIndex!=index.end(); itIndex++)
//	{
//		map< string, int >::iterator itMsg = msgForSearch[*itIndex].begin();
//		for(itMsg; itMsg!=msgForSearch[*itIndex].end(); itMsg++)
//		{
//			if(tempResult.count(itMsg->first))
//				tempResult[itMsg->first] += itMsg->second;
//			else
//				tempResult.insert(make_pair(itMsg->first, itMsg->second));
//		}
//	}
//	vector< struct stTopicWord > answer;
//	map< string, int >::iterator itTemp = tempResult.begin();
//	for(itTemp; itTemp!=tempResult.end(); itTemp++)
//	{
//		if(!filter.count(itTemp->first))
//		{
//			struct stTopicWord temp;
//			temp.word	= itTemp->first;
//			if(!globalIdf.count(itTemp->first))
//				continue;
//			temp.tf_idf	= ((double)itTemp->second)/globalIdf[itTemp->first];
//			answer.push_back(temp);
//		}
//	}
//	sort(answer.begin(), answer.end(), greater<stTopicWord>());
//	int k, wordNumber = TOPIC_WORD_NUM;
//	for(k=0; k<wordNumber && k<answer.size(); k++)
//	{
//		printf("word:%s\ttf_idf:%.2f\n",answer[k].word.c_str(),answer[k].tf_idf);
//		result.push_back(answer[k].word);
//	}
//	return true;
//}
