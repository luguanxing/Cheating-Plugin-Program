// DLG_PROCESS.cpp : 实现文件
//

#include "stdafx.h"
#include "ProcessHook.h"
#include "DLG_PROCESS.h"
#include "ProcessHookDlg.h"
#include "afxdialogex.h"

///////////////////////////////////////////////////////////////////
#include <tlhelp32.h>
int nameup = -1;
int idup = -1;
CListCtrl *listctrl;
extern DWORD g_processid;
extern CProcessHookDlg *g_dlg;
///////////////////////////////////////////////////////////////////

// DLG_PROCESS 对话框

IMPLEMENT_DYNAMIC(DLG_PROCESS, CDialogEx)

DLG_PROCESS::DLG_PROCESS(CWnd* pParent /*=NULL*/)
	: CDialogEx(DLG_PROCESS::IDD, pParent)
{

}

DLG_PROCESS::~DLG_PROCESS()
{
}

void DLG_PROCESS::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_PROCESS, list_process);
}


BEGIN_MESSAGE_MAP(DLG_PROCESS, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_SELECT, &DLG_PROCESS::OnBnClickedButtonSelect)
	ON_BN_CLICKED(IDC_BUTTON_CLOSE, &DLG_PROCESS::OnBnClickedButtonClose)
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_LIST_PROCESS, &DLG_PROCESS::OnLvnColumnclickListProcess)
//	ON_NOTIFY(NM_RDBLCLK, IDC_LIST_PROCESS, &DLG_PROCESS::OnNMRDblclkListProcess)
ON_NOTIFY(NM_DBLCLK, IDC_LIST_PROCESS, &DLG_PROCESS::OnNMDblclkListProcess)
ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()









 int   CALLBACK   cmp(LPARAM   lParam1,   LPARAM   lParam2,   LPARAM   lParamSort) {
	 if (lParamSort == 0) {		//对进程名称排序
		 if (nameup == -1 || nameup == 1) {
			CString strItem1   =   listctrl->GetItemText(lParam1,   0);  
			CString strItem2   =   listctrl->GetItemText(lParam2,   0); 
			return wcscmp(strItem1, strItem2);
		 } else {
			CString strItem1   =   listctrl->GetItemText(lParam1,   0);  
			CString strItem2   =   listctrl->GetItemText(lParam2,   0); 
			return wcscmp(strItem2, strItem1);
		 }
	 } else {		//对进程ID排序
		 if (idup == -1 || idup == 1) {
			CString strItem1   =   listctrl->GetItemText(lParam1,   1);  
			CString strItem2   =   listctrl->GetItemText(lParam2,   1); 
			return _ttoi(strItem1) > _ttoi(strItem2);
		 } else {
			CString strItem1   =   listctrl->GetItemText(lParam1,   1);  
			CString strItem2   =   listctrl->GetItemText(lParam2,   1); 
			return _ttoi(strItem1) < _ttoi(strItem2);
		 }
	 }
	 return 0;
 }


BOOL DLG_PROCESS::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	return TRUE;
}

void DLG_PROCESS::OnLvnColumnclickListProcess(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	int col = pNMLV->iItem;					//列索引
	int row = pNMLV->iSubItem;				//行索引

	if (col == -1) {
		CHeaderCtrl* pListHeader = list_process.GetHeaderCtrl();
		HDITEM hditem;
		enum   { sizeOfBuffer = 256 };
		TCHAR  lpBuffer[sizeOfBuffer];
		hditem.mask = HDI_TEXT;
		hditem.pszText = lpBuffer;
		hditem.cchTextMax = sizeOfBuffer;
		pListHeader->GetItem(0, &hditem);
		if (row == 0) {				//按进程名称排序
			for(int i=0;i<list_process.GetItemCount();i++)  
				list_process.SetItemData(i,i);
			list_process.SortItems(cmp, 0);

			if (nameup == -1 || nameup == 1) {
				hditem.pszText = _T("进程名称▲");
				pListHeader->SetItem(0, &hditem);
				nameup = 0;
			} else {
				hditem.pszText = _T("进程名称▼");
				pListHeader->SetItem(0, &hditem);
				nameup = 1;
			}
			hditem.pszText = _T("进程id号");
			pListHeader->SetItem(1, &hditem);
		} else if (row == 1) {	//按进程ID号排序
			for(int i=0;i<list_process.GetItemCount();i++)  
				list_process.SetItemData(i,i);
			list_process.SortItems(cmp, 1);

			pListHeader->GetItem(1, &hditem);
			if (idup == -1 || idup == 1) {
				hditem.pszText = _T("进程id号▲");
				pListHeader->SetItem(1, &hditem);
				idup = 0;
			 } else {
				hditem.pszText = _T("进程id号▼");
				pListHeader->SetItem(1, &hditem);
				idup = 1;
			 }
			hditem.pszText = _T("进程名称");
			pListHeader->SetItem(0, &hditem);
		}
	}
	*pResult = 0;
}

void DLG_PROCESS::OnNMDblclkListProcess(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	OnBnClickedButtonSelect();
	*pResult = 0;
}


void DLG_PROCESS::OnBnClickedButtonSelect()
{
	int selectitem = list_process.GetSelectionMark();
	if (selectitem == -1) {
		::MessageBox(NULL, _T("未选择进程"), _T("错误"), MB_ICONERROR);
		return;
	}
	WCHAR cprocessnamebuff[255];
	list_process.GetItemText(list_process.GetSelectionMark(), 0, (LPTSTR)cprocessnamebuff, sizeof(cprocessnamebuff));
	g_dlg->cs_processname = cprocessnamebuff;
	WCHAR buff[250];
	list_process.GetItemText(list_process.GetSelectionMark(), 1, buff, sizeof(buff));
	g_processid = _wtoi(buff);
	g_dlg->UpdateData(false);
	DLG_PROCESS::OnCancel();
}


void DLG_PROCESS::OnBnClickedButtonClose()
{
	// TODO: 在此添加控件通知处理程序代码
	DLG_PROCESS::OnCancel();
}



void DLG_PROCESS::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);
	listctrl = &list_process;
	list_process.InsertColumn(0, _T("进程名称"),  0, 100);
	list_process.InsertColumn(1, _T("进程ID号"),  0, 75);
	list_process.SetExtendedStyle(LVS_EX_FULLROWSELECT);
	list_process.DeleteAllItems();
	PROCESSENTRY32 pe32;
	pe32.dwSize = sizeof(pe32);
	HANDLE hpsnap = ::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);	//当前所有进程快照
	if (hpsnap == INVALID_HANDLE_VALUE)
		::MessageBox(NULL, _T("CreateToolhelp32Snapshot错误"),  _T("失败"), MB_ICONERROR);
	int i = 0;
	BOOL bnext = ::Process32First(hpsnap, &pe32);	//遍历当前进程快照
	while (bnext) {
		list_process.InsertItem(i, pe32.szExeFile);
		CString buff;
		buff.Format(_T("%d"), pe32.th32ProcessID);
		list_process.SetItemText(i, 1, buff);
		bnext = ::Process32Next(hpsnap, &pe32);
	}
	::CloseHandle(hpsnap);
}
