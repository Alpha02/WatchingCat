
// WatchingCat.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CWatchingCatApp:
// �йش����ʵ�֣������ WatchingCat.cpp
//

class CWatchingCatApp : public CWinApp
{
public:
	CWatchingCatApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CWatchingCatApp theApp;