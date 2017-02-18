
// CrosshairsDlg.h : 头文件
//

#pragma once
#include "afxwin.h"
#include "afxcmn.h"


// CCrosshairsDlg 对话框
class CCrosshairsDlg : public CDialogEx
{
// 构造
public:
	CCrosshairsDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_CROSSHAIRS_DIALOG };

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
	afx_msg void OnBnClickedButtonTest();
	afx_msg void OnBnClickedButtonStart();
	CComboBox m_crosstype;
//	afx_msg void OnNMCustomdrawSlider1(NMHDR *pNMHDR, LRESULT *pResult);
//	afx_msg void OnCbnSelchangeCombo1();
	afx_msg void OnCbnSelendokComboType();
	afx_msg void OnBnClickedButtonExit();
	CSliderCtrl m_csliderR;
	CSliderCtrl m_csliderG;
	CSliderCtrl m_csliderB;
	CSliderCtrl m_csliderSIZE;
	CSliderCtrl m_csliderWEIGHT;
//	afx_msg void OnTRBNThumbPosChangingSlider2(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMReleasedcaptureSlider2(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMReleasedcaptureSliderg(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMReleasedcaptureSliderb(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMReleasedcaptureSliderSize(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMReleasedcaptureSliderWeight(NMHDR *pNMHDR, LRESULT *pResult);
};
