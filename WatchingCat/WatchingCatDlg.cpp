
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
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CWatchingCatDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_CHECK1, Auto_Run);
}

BEGIN_MESSAGE_MAP(CWatchingCatDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_CHECK1, &CWatchingCatDlg::OnBnClickedCheck1)
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
	record_manager=new CRecordManager();
	rules_manager=new CRulesManager();
	CListCtrl *list=(CListCtrl*)GetDlgItem(IDC_LIST6);
	list->ModifyStyle(0,LVS_REPORT );
	list->SetExtendedStyle(list->GetExtendedStyle() | LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);  
	list->InsertColumn(0,"事件");
	list->InsertColumn(1,"描述");
	list->InsertColumn(2,"时间");
	CRect rect;  
	list->GetClientRect(rect); //获得当前客户区信息  
	list->SetColumnWidth(0, rect.Width() / 3); //设置列的宽度。  
	list->SetColumnWidth(1, rect.Width() / 3);  
	list->SetColumnWidth(2, rect.Width() / 3);  
	SetTimer(TIMER1,1000,0);
	Auto_Run=record_manager->CheckAutoRun();
	UpdateData(FALSE);

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
			CTimeRecord * record=record_manager->MakeRecord();
			if(record!=NULL){
				int a=rules_manager->GetRuleIDForRecord(record);
				ofstream outfile("Rules",ios::trunc);
				rules_manager->Write(outfile);

				CListCtrl *list=(CListCtrl*)GetDlgItem(IDC_LIST6);
				for(unsigned int i=0;i<record_manager->RecordNumber;i++){
					record=record_manager->RecordList[i];
					if(list->GetItemCount()<=i){
						list->InsertItem(i,"");

						list->SetItemText(i,0, record->content);  
						list->SetItemText(i, 1, record->description);
						list->EnsureVisible(i, FALSE);
					}
					char * describe=record->DescribeTime();
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
