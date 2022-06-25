
// FrameClientDlg.h : header file
//

#pragma once
#include "AlgoTSEng.h"
#include "Resource.h"
#include "AlgoManager.h"
// CFrameClientDlg dialog
class CFrameClientDlg : public CDialogEx
{
// Construction
public:
	CFrameClientDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_FRAMECLIENT_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCheck1();
//	afx_msg void OnEnChangeEdit3();
	void upStatusMsg(bool bflag){
		if(bflag){
		    ((CEdit *)this->GetDlgItem(IDC_EDIT1))->SetWindowTextW(CString("Service!!!"));
		}
		else{
		    ((CEdit *)this->GetDlgItem(IDC_EDIT1))->SetWindowTextW(CString("UnService!!!"));
		}
    }

	void addTipsMsg(string msg);

private:
	//AlgoTSEng ats;
	AlgoManager* am;
	bool bStart;
public:

};
