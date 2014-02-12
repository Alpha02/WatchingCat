#ifndef TIMERECORD_H
#define TIMERECORD_H
#include "CatEyes.h"
class CTimeRecord{
private:
public:
	tm time_record;
	char *content;
	char *description;
	CTimeRecord();
	CTimeRecord(tm t,char *str,char * str2);
	~CTimeRecord();
	CString GetContent();
	void SetContent(char * str);
	bool EqualTo(CTimeRecord * target);

	void Write(ofstream &outfile);
	bool Read(ifstream &infile);
	void WriteToFile();
};
#endif