#include "stdafx.h"
#include "CatEyes.h"

	void CRulesManager::Write(ofstream &outfile){
		for(int i=0;i<rules_number;i++){
			outfile<<keys[i]<<endl<<values[i]<<endl;
		}
	}
	bool CRulesManager::Read(ifstream &infile){
		while((!infile.eof())&&(rules_number<MAX_RULES)){
			if(!keys[rules_number]){
				keys[rules_number]=new char[KEYS_LEN];
			}
			if(!values[rules_number]){
				values[rules_number]=new char[VALUES_LEN];
			}
			infile.getline(keys[rules_number],KEYS_LEN);
			if(infile.eof())return false;
			infile.getline(values[rules_number],VALUES_LEN);
			rules_number++;

		}
		return true;
	}
	CRulesManager::CRulesManager(){
		rules_number=0;
		for(int i=0;i<MAX_RULES;i++){
			keys[i]=NULL;
			values[i]=NULL;
		}
		ifstream infile("Rules");
		if(infile!=NULL){
			Read(infile);		
			infile.close();
		}
	}
	void CRulesManager::NewRule(char * key,char * value){
		if(rules_number<MAX_RULES){
		if(!keys[rules_number]){
			keys[rules_number]=new char[KEYS_LEN];
		}
		if(!values[rules_number]){
			values[rules_number]=new char[VALUES_LEN];
		}
		strcpy(keys[rules_number],key);
		strcpy(values[rules_number],value);
		rules_number++;
		}
	}
	bool CRulesManager::MatchRule(CTimeRecord * record,int idx_rule){
		if(strcmp(record->content,"NULL")==0){
			if(keys[idx_rule][0]=='$'){
				char * tmp_key=keys[idx_rule]+1;
				if(strstr(record->description,tmp_key)!=NULL){
					return true;
				}
			}
		}else{
			if(strstr(record->content,keys[idx_rule])!=NULL){
				return true;
			}
		}
		return false;
	}
	int CRulesManager::GetRuleIDForRecord(CTimeRecord * record){
		for(int i=0;i<rules_number;i++){
			if(MatchRule(record,i)){
				return i;
			}
		}
		char *tmp_key=new char[200];

		if(strcmp(record->content,"NULL")==0){
			strcpy(tmp_key,"$");
			tmp_key=strcat(tmp_key,record->description);
		}else{
			tmp_key=record->content;
		}
		NewRule(tmp_key,tmp_key);

	}
	CTimeRecord::CTimeRecord(){
		content=new char[200];
		description=new char[200];
	}
	CTimeRecord::CTimeRecord(tm t,char *str,char * str2){
		time_record=t;
		content=new char[200];
		description=new char[200];
		strcpy(content,str);
		strcpy(description,str2);
	}
	CTimeRecord::~CTimeRecord(){
		delete content;
		delete description;
	}
	CString CTimeRecord::GetContent(){
		return content;
	}
	void CTimeRecord::SetContent(char * str){
		strcpy(content,str);
	}
	bool CTimeRecord::EqualTo(CTimeRecord * target){
		return (!strcmp(content,target->content));
	}

	void CTimeRecord::Write(ofstream &outfile){
		outfile<<time_record.tm_hour<<" "
			<<time_record.tm_isdst<<" "
			<<time_record.tm_mday<<" "
			<<time_record.tm_min<<" "
			<<time_record.tm_mon<<" "
			<<time_record.tm_sec<<" "
			<<time_record.tm_wday<<" "
			<<time_record.tm_yday<<" "
			<<time_record.tm_year<<" "
			<<endl<<content<<endl<<description<<endl;

	}
	bool CTimeRecord::Read(ifstream &infile){
		if(!infile.eof()){
			infile>>time_record.tm_hour
				>>time_record.tm_isdst
				>>time_record.tm_mday
				>>time_record.tm_min
				>>time_record.tm_mon
				>>time_record.tm_sec
				>>time_record.tm_wday
				>>time_record.tm_yday
				>>time_record.tm_year;
			if(infile.eof())return 0;
			infile.getline(content,200);
			if(infile.eof())return 0;
			infile.getline(content,200);
			if(infile.eof())return 0;
			infile.getline(description,200);
			return 1;
		}
		return 0;
	}
	void CTimeRecord::WriteToFile(){
		ofstream outfile(GetStoreFileName(time_record),ios::app);
		Write(outfile);
		outfile.close();

	}

	BOOL CRecordManager::PromotePrivilege(BOOL bEnable)  
	{  
		// 附给本进程特权,以便访问系统进程  
		HANDLE hToken;  
		// 打开一个进程的访问令牌  
		if(::OpenProcessToken(::GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES, &hToken))   
		{  
			// 取得特权名称为"SetDebugPrivilege"的LUID  
			LUID uID;  
			if (!::LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &uID))  
			{  
				return FALSE;  
			}  


			// 调整特权级别  
			TOKEN_PRIVILEGES tp;  
			tp.PrivilegeCount = 1;  
			tp.Privileges[0].Luid = uID;  
			tp.Privileges[0].Attributes = bEnable ? SE_PRIVILEGE_ENABLED : 0;  
			if(!::AdjustTokenPrivileges(hToken, FALSE, &tp, sizeof(tp), NULL,NULL))  
			{  
				return FALSE;  
			}  

			// 关闭访问令牌句柄  
			::CloseHandle(hToken);  
			return TRUE;  
		}  
		return FALSE;  
	}  
	CRecordManager::CRecordManager(){
		PromotePrivilege(TRUE);
		RecordNumber=0;
		for(int i=0;i<MAX_RECORD;i++){
			RecordList[i]=NULL;
		}
	}
	void CRecordManager::ReadADay(tm t){
		ifstream infile(GetStoreFileName(t));
		RecordNumber=0;
		while(!infile.eof()&&RecordNumber<MAX_RECORD){
			if(RecordList[RecordNumber]==NULL){
				RecordList[RecordNumber]=new CTimeRecord();
			}
			if(RecordList[RecordNumber]->Read(infile)){
				RecordNumber++;
			}
		}
	}
	CTimeRecord * CRecordManager::MakeRecord(){

		time_t t=time(0);
		tm current_time=*localtime(&t);
		HWND hwnd=GetForegroundWindow();
		char str_Description[200];
		char str_Path[200];
		char str_WindowsText[200];
		GetWindowText(hwnd,str_WindowsText,200);
		if(strlen(str_WindowsText)==0){
			strcpy(str_WindowsText,"NULL");
		}
		DWORD pid=0;
		GetWindowThreadProcessId(hwnd,&pid);
		MODULEENTRY32 me32;
		me32.dwSize=sizeof(me32);
		HANDLE hProcessSnap=CreateToolhelp32Snapshot(TH32CS_SNAPMODULE,pid);
		if(Module32First(hProcessSnap,&me32)){
			strcpy(str_Path,me32.szExePath);
			String^ str1= System::Runtime::InteropServices::Marshal::PtrToStringAnsi((IntPtr)str_Path);
			FileVersionInfo^ myFileVersionInfo = FileVersionInfo::GetVersionInfo(str1);  
			String ^ str2=myFileVersionInfo->FileDescription;
			char *ch2 = (char*)(void*)System::Runtime::InteropServices::Marshal::StringToHGlobalAnsi(str2);
			strcpy(str_Description,ch2);
			if(strlen(str_Description)==0){
				strcpy(str_Description,"NULL");
			}
		}else{
			strcpy(str_Description,"NULL");
		}
		CTimeRecord *record=new CTimeRecord(current_time,str_Description,str_WindowsText);
		record->WriteToFile();

		ReadADay(current_time);
		return record;
	}