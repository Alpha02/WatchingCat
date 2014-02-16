
// WatchingCatDlg.h : 头文件
#include "CatEyes.h"
#include "atlcomtime.h"
//

#pragma once


// CWatchingCatDlg 对话框
class CWatchingCatDlg : public CDialogEx
{
// 构造
public:
	NOTIFYICONDATA nid;
	CRecordManager *record_manager;
	CRecordManager *tmp_record_manager;
	CRulesManager *rules_manager;
	tm selected_time;
	CWatchingCatDlg(CWnd* pParent = NULL);	// 标准构造函数
	void _fnToTray();
// 对话框数据
	enum { IDD = IDD_WATCHINGCAT_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLbnSelchangeList1();
	afx_msg void OnStnClickedMfcpropertygrid1();
	afx_msg void OnBnClickedCheck1();
	afx_msg LRESULT OnShowTask(WPARAM wParam, LPARAM lParam);
	BOOL showToolTip(LPCTSTR szMsg,LPCTSTR szTitle); 
	// 是否开机启动
	BOOL Auto_Run;
	BOOL First_Hide;
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	// 表示当前需要显示记录日期
	COleDateTime SelectedDate;
	afx_msg void OnDtnDatetimechangeDatetimepicker1(NMHDR *pNMHDR, LRESULT *pResult);
	// 指示系统是否工作在读取当前日期的数据的状态
	bool ReadCurrentDate;
	// 用户添加的自定义事件的名称
	CString Name_UserEvent;
	// 用户自定义添加的事件描述
	CString Description_UserEvent;
	afx_msg void OnBnClickedButtonAddEvent();
	afx_msg void OnBnClickedButtonNoEvent();
	afx_msg void OnBnClickedCheckEventLock();
	// 是否锁定当前事件，锁定后切换窗口将不改变事件。
	BOOL event_locked;
	BOOL dummy_event_enabled;
	int temp_lock_count;
	BOOL keyboard_record_enabled;
	afx_msg void OnBnClickedCheckKeyboardRecord();
};
