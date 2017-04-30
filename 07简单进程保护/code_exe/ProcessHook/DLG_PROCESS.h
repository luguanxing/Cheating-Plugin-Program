#pragma once
#include "afxcmn.h"


// DLG_PROCESS 对话框

class DLG_PROCESS : public CDialogEx
{
	DECLARE_DYNAMIC(DLG_PROCESS)

public:
	DLG_PROCESS(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~DLG_PROCESS();

// 对话框数据
	enum { IDD = IDD_CHOOSEPROCESS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonSelect();
	afx_msg void OnBnClickedButtonClose();
	virtual BOOL OnInitDialog();
	CListCtrl list_process;
	afx_msg void OnLvnColumnclickListProcess(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMDblclkListProcess(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
};
