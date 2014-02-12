
// WatchingCatDlg.h : 头文件
#include "CatEyes.h"
//

#pragma once


// CWatchingCatDlg 对话框
class CWatchingCatDlg : public CDialogEx
{
// 构造
public:
	CRecordManager *record_manager;
	CRulesManager *rules_manager;
	CWatchingCatDlg(CWnd* pParent = NULL);	// 标准构造函数

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
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLbnSelchangeList1();
	afx_msg void OnStnClickedMfcpropertygrid1();
};
