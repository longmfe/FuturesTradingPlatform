
// FrameClientDlg.cpp : implementation file
//

#include "stdafx.h"
#include "FrameClient.h"
#include "FrameClientDlg.h"
#include "afxdialogex.h"
#include "readMe.h"
#include <list>
#include "UtilFun.h"
//#include "LiuTestAlgoTSEng.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//extern LiuTestStrategy* testS;
//for ui ctrl

CFrameClientDlg* pUIDlg;

// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
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


// CFrameClientDlg dialog




CFrameClientDlg::CFrameClientDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CFrameClientDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	pUIDlg = this;
}

void CFrameClientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CFrameClientDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CFrameClientDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_CHECK1, &CFrameClientDlg::OnBnClickedCheck1)
//	ON_EN_CHANGE(IDC_EDIT3, &CFrameClientDlg::OnEnChangeEdit3)
END_MESSAGE_MAP()


// CFrameClientDlg message handlers

BOOL CFrameClientDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
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

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	readMe rm;
	list<string> contents = rm.getContents();
	CString ss("");
	for(list<string>::iterator it = contents.begin(); it != contents.end(); it++){
		ss += (*it).c_str();
		ss += "\r\n";
	}
	((CEdit *)this->GetDlgItem(IDC_EDIT4))->SetWindowTextW(ss);
	((CButton *)this->GetDlgItem(IDOK))->EnableWindow(false);
	bStart = false;

	((CEdit *)this->GetDlgItem(IDC_EDIT1))->SetWindowTextW(CString("UnService!"));

	//ats.initService();

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CFrameClientDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CFrameClientDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CFrameClientDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CFrameClientDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	//CDialogEx::OnOK();
	((CButton *)this->GetDlgItem(IDOK))->EnableWindow(false);
	bStart = true;
	UtilFun uf;
	bool bRealM = ((CButton *)this->GetDlgItem(IDC_CHECK2))->GetCheck();
	bool bRealT = ((CButton *)this->GetDlgItem(IDC_CHECK3))->GetCheck();
	if(bRealM && bRealT)
		MessageBox(_T("真实行情，真实交易？"),_T("确认"), MB_OKCANCEL );
	else if(bRealM && !bRealT)
		MessageBox(_T("真实行情，模拟撮合？"),_T("确认"), MB_OKCANCEL );
	else if(!bRealM && bRealT)
		MessageBox(_T("ES行情，真实交易？——此选项错误"),_T("确认"), MB_OKCANCEL );
	else if(!bRealM && !bRealT)
		MessageBox(_T("ES行情，模拟撮合？"),_T("确认"), MB_OKCANCEL );
	am = new AlgoManager(bRealM, bRealT); //bool bRealMarket , bool bRealTrade
	addTipsMsg(uf.getCHCurDate() + string("启动程序！"));
	((CEdit *)this->GetDlgItem(IDC_EDIT1))->SetWindowTextW(CString("Service!"));
	//ats.initService();
}


void CFrameClientDlg::OnBnClickedCheck1()
{
	// TODO: Add your control notification handler code here
	if(!bStart &&  ((CButton *)this->GetDlgItem(IDC_CHECK1))->GetCheck() == 1){
	    ((CButton *)this->GetDlgItem(IDOK))->EnableWindow(true);
	}
	else{
		((CButton *)this->GetDlgItem(IDOK))->EnableWindow(false);
	}
}


void CFrameClientDlg::addTipsMsg(string msg){
	CString source;
	((CEdit *)this->GetDlgItem(IDC_EDIT4))->GetWindowTextW(source);
	source += msg.c_str();
	source += "\r\n";
	((CEdit *)this->GetDlgItem(IDC_EDIT4))->SetWindowTextW(source);
}
