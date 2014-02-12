#ifndef CATEYES_H
#define CATEYES_H
#include "StdAfx.h"
#include <time.h>
#include <windows.h>
#include <string.h>
#include <fstream>
#include <cstring>
#include <TlHelp32.h>
#include <Psapi.h>

#using "System.dll"
using namespace System;  
using namespace System::Diagnostics;  
#pragma comment (lib,"Psapi.lib")
using namespace std;
#include "TimeRecord.h"
#include "RecordManager.h"
#include "RulesManager.h"
static char * GetStoreFileName(tm t){
	char *file_name=new char[100];
	sprintf(file_name,"%03d%02d%02d.txt",t.tm_year,t.tm_mon,t.tm_mday);
	return file_name;
}
#endif