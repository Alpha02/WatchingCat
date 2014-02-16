
// WatchingCatDlg.h : ͷ�ļ�
#include "CatEyes.h"
#include "atlcomtime.h"
//

#pragma once


// CWatchingCatDlg �Ի���
class CWatchingCatDlg : public CDialogEx
{
// ����
public:
	NOTIFYICONDATA nid;
	CRecordManager *record_manager;
	CRecordManager *tmp_record_manager;
	CRulesManager *rules_manager;
	tm selected_time;
	CWatchingCatDlg(CWnd* pParent = NULL);	// ��׼���캯��
	void _fnToTray();
// �Ի�������
	enum { IDD = IDD_WATCHINGCAT_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
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
	// �Ƿ񿪻�����
	BOOL Auto_Run;
	BOOL First_Hide;
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	// ��ʾ��ǰ��Ҫ��ʾ��¼����
	COleDateTime SelectedDate;
	afx_msg void OnDtnDatetimechangeDatetimepicker1(NMHDR *pNMHDR, LRESULT *pResult);
	// ָʾϵͳ�Ƿ����ڶ�ȡ��ǰ���ڵ����ݵ�״̬
	bool ReadCurrentDate;
	// �û���ӵ��Զ����¼�������
	CString Name_UserEvent;
	// �û��Զ�����ӵ��¼�����
	CString Description_UserEvent;
	afx_msg void OnBnClickedButtonAddEvent();
	afx_msg void OnBnClickedButtonNoEvent();
	afx_msg void OnBnClickedCheckEventLock();
	// �Ƿ�������ǰ�¼����������л����ڽ����ı��¼���
	BOOL event_locked;
	BOOL dummy_event_enabled;
	int temp_lock_count;
	BOOL keyboard_record_enabled;
	afx_msg void OnBnClickedCheckKeyboardRecord();
};
