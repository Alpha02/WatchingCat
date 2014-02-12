#ifndef RULESMANAGER_H
#define RULESMANAGER_H
#include "TimeRecord.h"
#define KEYS_LEN 100
#define VALUES_LEN 200
#define MAX_RULES 200
class CRulesManager{
private:

	char * keys[MAX_RULES];
	char * values[MAX_RULES];
	int rules_number;
public:

	void Write(ofstream &outfile);
	bool Read(ifstream &infile);
	CRulesManager();
	void NewRule(char * key,char * value);
	bool MatchRule(CTimeRecord * record,int idx_rule);
	int GetRuleIDForRecord(CTimeRecord * record);
};
#endif