#ifndef RECORDMANAGER_H
#define RECORDMANAGER_H
#include "CatEyes.h"
#include "TimeRecord.h"
#define MAX_RECORD 1440
class CRecordManager{
private:


public:
	CTimeRecord * RecordList[MAX_RECORD];
	int RecordNumber;
	BOOL PromotePrivilege(BOOL bEnable);
	CRecordManager();
	void ReadADay(tm t);
	CTimeRecord * MakeRecord();
};
#endif