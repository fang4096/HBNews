#include "seg.h"
#include "cFile.h"
#include "dbHandler.h"
#include "utility.h"
using namespace std;
bool test()
{
	initialize();
    string fileName("message");	
    int dayBack = 10;
	DB db(fileName, fileName, dayBack);
	if(!db.GetMessageForStop("twitt"))
	{
		printf("GetMessage Wrong!\n");
		return false;
	}
	while(dayBack>0)
	{
        Seg seg(fileName+(dayBack--));
        if(!seg.PreProcess())
    	{
        	printf("Seg Error!\n");
        	return false;
        }
    }
//	Seg segNew("./new.txt"), segOld("./old.txt");
//	if(!segNew.PreProcess() || !segOld.PreProcess())
//	{
//        	printf("Seg Error!\n");
//        	return false;
//    }
	return true;
/*
	Seg segMessage(filename);
    if(!segMessage.FindHotSpot())
    {
        log(INFO, "hot error\n");
        return false;
    }
	segMessage.Clean();
*/
}
int main(int argc, const char* argv[])
{
    test();
/*    
	if(argc != 4)
	{
		printf("Please enter 4 parameters!\n");
		return -1;
	}
	if(atoi(argv[3]) > 10)
	{
		printf("Want to produce too much subTopics!\n");
		return -1;
	}
	vector<string> vsChannel;
	vsChannel.push_back("twitt");
	vsChannel.push_back("tsohu");
	vsChannel.push_back("tsina");
	vsChannel.push_back("ttenc");
	vsChannel.push_back("tnete");
	if(!test(string(argv[1]), atoi(argv[2]), atoi(argv[3]), vsChannel)) return -1;
	return 0;
*/
}


