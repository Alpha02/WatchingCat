#ifndef RECORDMANAGER_H
#define RECORDMANAGER_H
#include "CatEyes.h"
#include "TimeRecord.h"
#define MAX_RECORD 65535
class CRecordManager{
private:

	char name_dummy_event[100];
	char description_dummy_event[200];

	bool dummy_event_enable;

public:
	char my_name[100];
	CTimeRecord * RecordList[MAX_RECORD];
	unsigned int RecordNumber;
	tm current_time;
	bool event_locked;	
	BOOL PromotePrivilege(BOOL bEnable);
	CRecordManager();
	~CRecordManager();
	void Init();
	CRecordManager(tm record_date);
	void ReadADay(tm t);
	CTimeRecord * GetLatestRecord();
	CTimeRecord *  ReceiveNewRecord(CTimeRecord * record);
	CTimeRecord * MakeRecord();
	bool CheckAutoRun();
	bool SetAutoRun(bool enabled);
	void SetDummyEvent(bool enable,char * name,char * description);
	unsigned int temp_lock_count;
};
#endif