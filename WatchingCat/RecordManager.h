#ifndef RECORDMANAGER_H
#define RECORDMANAGER_H
#include "CatEyes.h"
#include "TimeRecord.h"
#define MAX_RECORD 65535
class CRecordManager{
private:


public:
	CTimeRecord * RecordList[MAX_RECORD];
	unsigned int RecordNumber;
	tm current_time;
	BOOL PromotePrivilege(BOOL bEnable);
	CRecordManager();
	void ReadADay(tm t);
	CTimeRecord * GetLatestRecord();
	CTimeRecord *  ReceiveNewRecord(CTimeRecord * record);
	CTimeRecord * MakeRecord();
	bool CheckAutoRun();
	bool SetAutoRun(bool enabled);
};
#endif