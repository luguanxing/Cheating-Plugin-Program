
// ProcessHookDlg.h : 头文件
//

#pragma once
#include "afxwin.h"


// CProcessHookDlg 对话框
class CProcessHookDlg : public CDialogEx
{
// 构造
public:
	CProcessHookDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_PROCESSHOOK_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonSelect();
	CString cs_processname;
	afx_msg void OnBnClickedButtonStart();
	afx_msg void OnBnClickedButtonClose();
	bool LoadDriver(void);
	bool UnloadDriver(void);
	CString m_drivername;
	CString m_driverpath;
	CButton btn_start;
	CButton btn_close;
};
