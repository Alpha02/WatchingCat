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

		if(keys[idx_rule][0]=='$'){
			char * tmp_key=keys[idx_rule]+1;
			if(strstr(record->description,tmp_key)!=NULL){
				return true;
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
	void CTimeRecord::Continue(){
		time_continue++;
	}
	CTimeRecord::CTimeRecord(){
		content=new char[200];
		description=new char[200];
	}
	CTimeRecord::CTimeRecord(tm t,char *str,char * str2){
		time_record=t;
		time_continue=1;
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
		if (strcmp(content,target->content))return false;
		if (strcmp(description,target->description))return false;
		return true;
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
			<<time_continue<<endl<<content<<endl<<description<<endl;

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
				>>time_record.tm_year
				>>time_continue;
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
	char * CTimeRecord::DescribeTime(){
		char *str=new char[200];
		sprintf(str,"%d:%d:%d(%ds)",time_record.tm_hour,time_record.tm_min,time_record.tm_sec,time_continue);
		return str;
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
		for(unsigned int i=0;i<MAX_RECORD;i++){
			RecordList[i]=NULL;
		}
		time_t t=time(0);
		current_time=*localtime(&t);
		ReadADay(current_time);
	}
	CTimeRecord * CRecordManager::GetLatestRecord(){
		if(RecordNumber>0){
			return RecordList[RecordNumber-1];
		}
		return NULL;
	}
	void CRecordManager::ReadADay(tm t){
		ifstream infile(GetStoreFileName(t));
		RecordNumber=0;
		if(infile){
		while(!infile.eof()&&RecordNumber<MAX_RECORD){
			if(RecordList[RecordNumber]==NULL){
				RecordList[RecordNumber]=new CTimeRecord();
			}
			if(RecordList[RecordNumber]->Read(infile)){
				RecordNumber++;
			}
		}
		}
	}
	CTimeRecord * CRecordManager::ReceiveNewRecord(CTimeRecord * record){
		CTimeRecord * tmp_record=GetLatestRecord();
		if(tmp_record!=NULL){
			if(record->EqualTo(tmp_record)){
				GetLatestRecord()->Continue();
				delete(record);
				record=tmp_record;
			}else{
				tmp_record->WriteToFile();
				RecordList[RecordNumber]=record;
				RecordNumber++;
			}

		}else{
			RecordList[RecordNumber]=record;
			RecordNumber++;
		}
		return record;

	}
	CTimeRecord * CRecordManager::MakeRecord(){

		time_t t=time(0);
		current_time=*localtime(&t);
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
//********************************
		HMODULE hMods[512] = {0};
		DWORD cbNeeded = 0;
		TCHAR szModName[MAX_PATH];
		BOOL Wow64Process;
		HANDLE hProcess = ::OpenProcess(PROCESS_QUERY_INFORMATION|PROCESS_VM_READ|PROCESS_QUERY_LIMITED_INFORMATION, FALSE, pid);
		IsWow64Process(hProcess, &Wow64Process); //判断是32位还是64位进程
		if(EnumProcessModulesEx(hProcess, hMods, sizeof(hMods), &cbNeeded, Wow64Process?LIST_MODULES_32BIT:LIST_MODULES_64BIT)){
		//用对应的方法枚举模块
		GetModuleFileNameEx(hProcess, hMods[0], szModName, _countof(szModName));
		//获取路径
		CloseHandle(hProcess);
		strcpy(str_Path,szModName);
		String^ str1= System::Runtime::InteropServices::Marshal::PtrToStringAnsi((IntPtr)str_Path);
		FileVersionInfo^ myFileVersionInfo = FileVersionInfo::GetVersionInfo(str1);  
		String ^ str2=myFileVersionInfo->FileDescription;
		char *ch2 = (char*)(void*)System::Runtime::InteropServices::Marshal::StringToHGlobalAnsi(str2);
		if(ch2!=NULL){
			strcpy(str_Description,ch2);
		}
		//以上获取应用程序描述
		if(strlen(str_Description)==0){
			strcpy(str_Description,"NULL");
		}

		CTimeRecord *record=new CTimeRecord(current_time,str_Description,str_WindowsText);
		record=ReceiveNewRecord(record);
		return record;
		}else{
			return NULL;
		}
		
		
	}
	bool CRecordManager::CheckAutoRun(){
		HKEY   hKey; 
		char pFileName[MAX_PATH] = {0}; 
		//得到程序自身的全路径 
		DWORD dwRet = GetModuleFileName(NULL,pFileName, MAX_PATH); 
		//找到系统的启动项 
		LPCTSTR lpRun = _T("Software\\Microsoft\\Windows\\CurrentVersion\\Run"); 
		//打开启动项Key 
		long lRet = RegOpenKey(HKEY_LOCAL_MACHINE, lpRun, &hKey); 
		if(lRet== ERROR_SUCCESS ){
			char out_data[MAX_PATH]; 
			DWORD dwType = REG_SZ; 
			DWORD dwSize = MAX_PATH; 
			lRet=RegQueryValueEx(hKey,_T("WatchingCat"),NULL,&dwType,(LPBYTE)out_data,&dwSize);
			RegCloseKey(hKey); 
			if((lRet== ERROR_SUCCESS) && (strcmp(out_data,pFileName)==0)){
				return true;
			}else{
				return false;
			}
		}
		return false;
	}
	bool CRecordManager::SetAutoRun(bool enabled){
		HKEY   hKey; 
		char pFileName[MAX_PATH] = {0}; 
		//得到程序自身的全路径 
		DWORD dwRet = GetModuleFileName(NULL,pFileName, MAX_PATH); 
		//找到系统的启动项 
		LPCTSTR lpRun = _T("Software\\Microsoft\\Windows\\CurrentVersion\\Run"); 
		//打开启动项Key 
		long lRet = RegOpenKey(HKEY_LOCAL_MACHINE, lpRun, &hKey); 
		if((lRet== ERROR_SUCCESS )&& (enabled))
		{
			//添加注册
			char out_data[MAX_PATH]; 
			DWORD dwType = REG_SZ; 
			DWORD dwSize = MAX_PATH; 
			RegSetValueEx(hKey, _T("WatchingCat"), 0,dwType,(BYTE*)pFileName, dwSize);
			RegCloseKey(hKey); 
		}
		if((lRet== ERROR_SUCCESS)&& (!enabled))
		{
			//删除注册
			RegDeleteValue(hKey,_T("WatchingCat"));
			RegCloseKey(hKey); 
		}
		return 0;
	}