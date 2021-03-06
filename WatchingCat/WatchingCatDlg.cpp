﻿
// WatchingCatDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "WatchingCat.h"
#include "WatchingCatDlg.h"
#include "CatEyes.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CWatchingCatDlg 对话框




CWatchingCatDlg::CWatchingCatDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CWatchingCatDlg::IDD, pParent)
	, Auto_Run(FALSE)
	, SelectedDate(COleDateTime::GetCurrentTime())
	, ReadCurrentDate(false)
	, Name_UserEvent(_T(""))
	, Description_UserEvent(_T(""))
	, event_locked(FALSE)
	, keyboard_record_enabled(FALSE)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CWatchingCatDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_CHECK1, Auto_Run);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER1, SelectedDate);
	DDX_Text(pDX, IDC_EDIT1, Name_UserEvent);
	DDX_Text(pDX, IDC_EDIT2, Description_UserEvent);
	DDX_Check(pDX, IDC_CHECK_EVENT_LOCK, event_locked);
	DDX_Check(pDX, IDC_CHECK_KEYBOARD_RECORD, keyboard_record_enabled);
}

BEGIN_MESSAGE_MAP(CWatchingCatDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_CHECK1, &CWatchingCatDlg::OnBnClickedCheck1)
	ON_MESSAGE(WM_SHOWTASK,OnShowTask)
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_NOTIFY(DTN_DATETIMECHANGE, IDC_DATETIMEPICKER1, &CWatchingCatDlg::OnDtnDatetimechangeDatetimepicker1)
	ON_BN_CLICKED(IDC_BUTTON_ADDEVENT, &CWatchingCatDlg::OnBnClickedButtonAddEvent)
	ON_BN_CLICKED(IDC_BUTTON_NOEVENT, &CWatchingCatDlg::OnBnClickedButtonNoEvent)
	ON_BN_CLICKED(IDC_CHECK_EVENT_LOCK, &CWatchingCatDlg::OnBnClickedCheckEventLock)
	ON_BN_CLICKED(IDC_CHECK_KEYBOARD_RECORD, &CWatchingCatDlg::OnBnClickedCheckKeyboardRecord)
END_MESSAGE_MAP()


// CWatchingCatDlg 消息处理程序

BOOL CWatchingCatDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);
	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	ShowWindow(SW_HIDE);
	record_manager=new CRecordManager();
	selected_time=record_manager->current_time;
	tmp_record_manager=NULL;
	rules_manager=new CRulesManager();
	CListCtrl *list=(CListCtrl*)GetDlgItem(IDC_LIST6);
	list->ModifyStyle(0,LVS_REPORT );
	list->SetExtendedStyle(list->GetExtendedStyle() | LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);  
	list->InsertColumn(0,"事件");
	list->InsertColumn(1,"描述");
	list->InsertColumn(2,"时间");
	CRect rect;  
	list->GetClientRect(rect); //获得当前客户区信息  
	list->SetColumnWidth(0, rect.Width()/3); //设置列的宽度。  
	list->SetColumnWidth(1, rect.Width() /2);  
	list->SetColumnWidth(2, rect.Width() / 6);  
	SetTimer(TIMER1,1,0);
	Auto_Run=record_manager->CheckAutoRun();
	First_Hide=false;
	ReadCurrentDate=true;
	dummy_event_enabled=false;
	temp_lock_count=0;
	UpdateData(FALSE);
	//显示托盘图标

	_fnToTray();

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CWatchingCatDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CWatchingCatDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}
void CWatchingCatDlg::OnTimer(UINT_PTR nIDEvent)
{
	switch(nIDEvent)
	{
	case TIMER1:
		{ 

			if(!First_Hide){
				First_Hide=1;
				SetTimer(TIMER1,1000,0);
				ShowWindow(SW_HIDE);
				showToolTip("本喵咪将时刻注视着主人的工作！喵~~~","Watching Cat已就位");
			}


			CRecordManager * ptr_record_manager;
			//判断用户选择的显示日期是否是当前日期。
			//如果是，就显示当前record
			if((selected_time.tm_yday==record_manager->current_time.tm_yday)&&
				(selected_time.tm_year==record_manager->current_time.tm_year)){
					ptr_record_manager=record_manager;
					//否则就显示tmp_record中的内容，这部分内容在修改时间的消息响应函数中被获取。
			}else{

				ptr_record_manager=tmp_record_manager;
			}

			CTimeRecord * record=record_manager->MakeRecord();
			if(record!=NULL){
				int a=rules_manager->GetRuleIDForRecord(record);
				ofstream outfile("Rules",ios::trunc);
				rules_manager->Write(outfile);

				if(!this->IsWindowVisible()){
					return;
				}
				CListCtrl *list=(CListCtrl*)GetDlgItem(IDC_LIST6);


				unsigned int i=list->GetItemCount();
				int item_diff=i-ptr_record_manager->RecordNumber;
				if(item_diff){
					while(item_diff>0){
							list->DeleteItem(0);
							item_diff--;
					}
					while(item_diff<0){
							list->InsertItem(list->GetItemCount(),"");
							list->EnsureVisible(list->GetItemCount()-1, FALSE);
							item_diff++;
					}
					if(ptr_record_manager!=record_manager){
						i=0;
					}
				}
				if(i>0 && ptr_record_manager==record_manager)i--;
				for(;i<ptr_record_manager->RecordNumber;i++){
					record=ptr_record_manager->RecordList[i];

					char * describe=record->DescribeTime();
					list->SetItemText(i,0, record->content);  
					list->SetItemText(i, 1, record->description);
					list->SetItemText(i, 2, describe);
					delete(describe);
				}
			}else{
			}


		}
	default:
		break;
	}
	CDialogEx::OnTimer(nIDEvent);
}
//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CWatchingCatDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CWatchingCatDlg::OnBnClickedCheck1()
{
	UpdateData(TRUE);
	if(Auto_Run!=record_manager->CheckAutoRun()){
		record_manager->SetAutoRun(Auto_Run);
	}
}

void CWatchingCatDlg::_fnToTray()
{

	nid.cbSize  = (DWORD)sizeof(NOTIFYICONDATA);
	nid.hWnd    = this->m_hWnd;
	nid.uID     = IDR_MAINFRAME;
	nid.uFlags  = NIF_ICON | NIF_MESSAGE | NIF_TIP ;
	nid.uCallbackMessage = WM_SHOWTASK;             // 自定义的消息名称
	nid.hIcon   = LoadIcon(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDR_MAINFRAME));
	strcpy(nid.szTip, "WatchingCat");                // 信息提示条
	Shell_NotifyIcon(NIM_ADD, &nid);                // 在托盘区添加图标
}
LRESULT CWatchingCatDlg::OnShowTask(WPARAM wParam, LPARAM lParam)
{
	if(wParam != IDR_MAINFRAME)
		return 1;
	switch(lParam)
	{
	case WM_RBUTTONUP:                                        // 右键起来时弹出菜单
		{

			LPPOINT lpoint = new tagPOINT;
			::GetCursorPos(lpoint);                    // 得到鼠标位置
			CMenu menu;
			menu.CreatePopupMenu();                    // 声明一个弹出式菜单
			menu.AppendMenu(MF_STRING, IDC_BUTTON_NOEVENT, "待机");
			menu.AppendMenu(MF_STRING, WM_DESTROY, "关闭");
			menu.TrackPopupMenu(TPM_LEFTALIGN, lpoint->x ,lpoint->y, this);
			HMENU hmenu = menu.Detach();
			menu.DestroyMenu();
			delete lpoint;
		}
		break;
	case WM_LBUTTONDBLCLK:                                 // 双击左键的处理
		{
			this->ShowWindow(SW_SHOWNORMAL);         // 显示主窗口
		}
		break;
	}
	return 0;
}
void CWatchingCatDlg::OnDestroy()
{
	// TODO: Add your specialized code here and/or call the base class
	// 在托盘区删除图标
	Shell_NotifyIcon(NIM_DELETE, &nid);             
	CDialogEx::OnDestroy();

}


void CWatchingCatDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);
	// TODO: Add your message handler code here
	if(nType == SIZE_MINIMIZED)  
	{  
		ShowWindow(SW_HIDE); // 当最小化市，隐藏主窗口              
	}  
}
BOOL CWatchingCatDlg::showToolTip(LPCTSTR szMsg,LPCTSTR szTitle)  
{  
	nid.uFlags = NIF_INFO;  
	nid.uID=IDR_MAINFRAME;  
	nid.dwInfoFlags=1;  
	strcpy_s(nid.szInfoTitle,64,szTitle);  
	CString str;  
	strcpy_s(nid.szInfo,256,szMsg);  
	return Shell_NotifyIcon(NIM_MODIFY,&nid);  
}


void CWatchingCatDlg::OnDtnDatetimechangeDatetimepicker1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMDATETIMECHANGE pDTChange = reinterpret_cast<LPNMDATETIMECHANGE>(pNMHDR);
	UpdateData(TRUE);
	SYSTEMTIME systime;
	VariantTimeToSystemTime(SelectedDate, &systime);
	CTime t_m(systime);
	time_t t = t_m.GetTime();
	selected_time=*localtime(&t);
	if((selected_time.tm_yday!=record_manager->current_time.tm_yday)||
		(selected_time.tm_year!=record_manager->current_time.tm_year)){
			if(tmp_record_manager!=NULL){
				delete(tmp_record_manager);
			}
			tmp_record_manager=new CRecordManager(selected_time);
	}
	*pResult = 0;

}


void CWatchingCatDlg::OnBnClickedButtonAddEvent()
{
	UpdateData(TRUE);
	strcpy(record_manager->my_name,"WatchingCat");
	record_manager->SetDummyEvent(true,(LPSTR)(LPCTSTR)Name_UserEvent,(LPSTR)(LPCTSTR)Description_UserEvent);


}


void CWatchingCatDlg::OnBnClickedButtonNoEvent()
{
	strcpy(record_manager->my_name,record_manager->RecordList[record_manager->RecordNumber-1]->content);
	record_manager->SetDummyEvent(true,"待机中","猫咪正在等待主人的归来- -");
	record_manager->temp_lock_count=5;
	temp_lock_count=5;

}


void CWatchingCatDlg::OnBnClickedCheckEventLock()
{
	UpdateData(TRUE);
	record_manager->event_locked=event_locked;

}


void CWatchingCatDlg::OnBnClickedCheckKeyboardRecord()
{
	UpdateData(TRUE);
}
