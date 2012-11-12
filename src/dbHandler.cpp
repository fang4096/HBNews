#include "dbHandler.h"
#include "logger.h"
#include "utility.h"
#include "cFile.h"
#include "timeUtils.h"
DB::DB(string newFile, string oldFile, int dayback)
{
    newFileName = newFile;
	oldFileName	= oldFile;
	dayBack		= dayback;
}

bool DB::GetMessage(string channel)
{
	string message;
	FILE *fp_new = CFile::OpenFile(newFileName, "wb");
	FILE *fp_old = CFile::OpenFile(oldFileName, "wb");
	if(fp_new == NULL || fp_old == NULL) return false;
	unsigned int newTimeStamp = TimeUtils::getTimeStamp(dayBack);
	unsigned int oldTimeStamp = TimeUtils::getTimeStamp(dayBack*2);
	
	BSONObj cond = BSON("mt"<<1);
    BSONObj newCond = BSON("pt" << GT << newTimeStamp << "ch" << channel);
    BSONObj oldCond = BSON("pt" << LTE << newTimeStamp << GT << oldTimeStamp << "ch" << channel);
    
	auto_ptr<DBClientCursor> cursor = con.query("common.weiboMsg", Query(newCond), 0, 0, &cond);
	if(!cursor->more())	return false;
	while(cursor->more())
	{
		BSONObj p = cursor->next();
		message   = p.getStringField("mt");
		fprintf(fp_new, "%s\n", message.c_str());
	}
	
	cursor = con.query("common.weiboMsg", Query(oldCond), 0, 0, &cond);
	if(!cursor->more())	return false;
	while(cursor->more())
	{
		BSONObj p = cursor->next();
		message   = p.getStringField("mt");
		fprintf(fp_old, "%s\n", message.c_str());
	}
	
	fclose(fp_new);
	fclose(fp_old);
	
	return true;
}

bool DB::GetMessageForStop(string channel)
{
	string message;
	auto_ptr<DBClientCursor> cursor;
	while(dayBack>0)
	{
        FILE *fp = CFile::OpenFile(newFileName+(dayBack-1), "wb");
        if(fp == NULL) return false;
        unsigned int newTimeStamp = TimeUtils::getTimeStamp(dayBack-1);
        unsigned int oldTimeStamp = TimeUtils::getTimeStamp(dayBack);
        
        BSONObj cond = BSON("mt"<<1);
        BSONObj timeCond = BSON("pt" << LTE << newTimeStamp << GT << oldTimeStamp << "ch" << channel);
        
        cursor = con.query("common.weiboMsg", Query(timeCond), 0, 0, &cond);
        if(!cursor->more())	return false;
        while(cursor->more())
        {
        	BSONObj p = cursor->next();
        	message   = p.getStringField("mt");
        	fprintf(fp, "%s\n", message.c_str());
        }
        dayBack--;
        fclose(fp);
    }	
	return true;
}
/*
bool DB::InsertHotspot(string id, vector<string> &hotspot)
{
	BSONArrayBuilder bsonArray;
	for(int i=0; i<hotspot.size(); i++)
		bsonArray.append(hotspot[i]);
	BSONArray hot = bsonArray.arr();
	con.update("common.hotspot", QUERY("_id"<<id), BSON("$set"<<BSON("wd"<<hot)), false, true);
	return true;
}
*/






















