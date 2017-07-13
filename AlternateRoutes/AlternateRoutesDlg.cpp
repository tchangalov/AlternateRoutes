
// AlternateRoutesDlg.cpp : implementation file
//

#include "stdafx.h"
#include "AlternateRoutes.h"
#include "AlternateRoutesDlg.h"
#include "afxdialogex.h"
#include <iostream>
#include <fstream>
#include <string>
#include "json.hpp"

using namespace std;

using json = nlohmann::json;
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CAboutDlg dialog used for App About
CWinThread* m_hThread;
HANDLE m_hKillEvent;
UINT HeuristicAnalysis(void*);
UINT ShortestPathLowOverlapAnalysis(void*);
UINT ShortestPathAnalysis(void*);

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

	// Dialog Data
	#ifdef AFX_DESIGN_TIME
		enum { IDD = IDD_ABOUTBOX };
	#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CAlternateRoutesDlg dialog

CAlternateRoutesDlg::CAlternateRoutesDlg(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_ALTERNATEROUTES_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CAlternateRoutesDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_num_paths_edit);
	DDX_Control(pDX, IDC_EDIT2, m_efficiency_edit);
	DDX_Control(pDX, IDC_SLIDER1, m_efficiency_slider);
	DDX_Control(pDX, IDC_EDIT4, m_status);
	DDX_Control(pDX, IDC_GO, m_go);
	DDX_Control(pDX, IDC_HEURISTIC, m_heuristic);
	DDX_Control(pDX, IDC_KSHORTEST1, m_shortest);
	DDX_Control(pDX, IDC_KSHORTEST2, m_shortest_lap);
	DDX_Control(pDX, IDC_ABORT, m_abort);
}

BEGIN_MESSAGE_MAP(CAlternateRoutesDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER1, &CAlternateRoutesDlg::OnNMReleasedcaptureSlider1)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER1, &CAlternateRoutesDlg::OnNMCustomdrawSlider1)
	ON_EN_CHANGE(IDC_EDIT1, &CAlternateRoutesDlg::OnEnChangeEdit1)
	ON_BN_CLICKED(IDC_KSHORTEST2, &CAlternateRoutesDlg::OnBnClickedKshortest2)
	ON_BN_CLICKED(IDC_HEURISTIC, &CAlternateRoutesDlg::OnBnClickedHeuristic)
	ON_BN_CLICKED(IDC_KSHORTEST1, &CAlternateRoutesDlg::OnBnClickedKshortest1)
	ON_BN_CLICKED(IDC_GO, &CAlternateRoutesDlg::OnBnClickedGo)
	ON_BN_CLICKED(IDC_ABORT, &CAlternateRoutesDlg::OnBnClickedAbort)
	ON_MESSAGE(WM_MY_THREAD_MESSAGE, OnThreadMessage)
END_MESSAGE_MAP()


// CAlternateRoutesDlg message handlers

BOOL CAlternateRoutesDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

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
									// Slider Control
	m_efficiency_slider.SetPos(85);

	// Edit Control Settings
	CFont *myFont = new CFont();
	myFont->CreateFont(24, 0, 0, 0, FW_HEAVY, true, false,
		0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
		FIXED_PITCH | FF_MODERN, _T("Times New ROman"));

	m_num_paths_edit.SetFont(myFont);
	m_efficiency_edit.SetFont(myFont);
	m_num_paths_edit.SetWindowTextW(_T("500"));

	disableInputs();

	MessageBox(_T("Welcome to Alternate Routes, please upload a graph to begin."), MB_OK);

	CFileDialog dlgFile(TRUE);
	CString fileName;
	const int c_cMaxFiles = 100;
	const int c_cbBuffSize = (c_cMaxFiles * (MAX_PATH + 1)) + 1;
	dlgFile.GetOFN().lpstrFile = fileName.GetBuffer(c_cbBuffSize);
	dlgFile.GetOFN().nMaxFile = c_cbBuffSize;
	dlgFile.DoModal();
	fileName.ReleaseBuffer();

	CString a;
	a.Format(_T("File '%s' uploaded successfully"), PathFindFileName(fileName));
	StatusUpdate(a);

	algorithm_select = "none";
	json json_data;
	std::ifstream graph_file(fileName, std::ifstream::binary);
	graph_file >> json_data;
	graph = Graph::Graph(json_data);
	int size = graph.getSize();
	graph.shortestPath(json_data["starting"]);
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CAlternateRoutesDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CAlternateRoutesDlg::OnPaint()
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
		CDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CAlternateRoutesDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CAlternateRoutesDlg::OnDeltaposSpin1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;
}


void CAlternateRoutesDlg::OnNMReleasedcaptureSlider1(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: Add your control notification handler code here

	*pResult = 0;
	CString a;
	a.Format(_T("Efficiency changed to %d%%"), m_efficiency_slider.GetPos());

}

void CAlternateRoutesDlg::OnNMCustomdrawSlider1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;
	
	CString a;
	a.Format(_T("%d %%"), m_efficiency_slider.GetPos());
	m_efficiency_edit.SetWindowTextW(a);
}


void CAlternateRoutesDlg::OnDeltaposSpin2(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;
}


void CAlternateRoutesDlg::OnBnClickedGraphupload()
{
	CFileDialog dlgFile(TRUE);
	CString fileName;
	const int c_cMaxFiles = 100;
	const int c_cbBuffSize = (c_cMaxFiles * (MAX_PATH + 1)) + 1;
	dlgFile.GetOFN().lpstrFile = fileName.GetBuffer(c_cbBuffSize);
	dlgFile.GetOFN().nMaxFile = c_cbBuffSize;

	dlgFile.DoModal();
	fileName.ReleaseBuffer();
	CString a;
	a.Format(_T("File '%s' uploaded successfully"), PathFindFileName(fileName));
}
	// TODO: Add your control notification handler code here


void CAlternateRoutesDlg::OnEnChangeEdit1()
{
	CString sWindowText;
	CString a;

	m_num_paths_edit.GetWindowText(sWindowText);
	a.Format(_T("Number of paths changed to %d"), _ttoi(sWindowText));

	StatusUpdate(a);

}

void CAlternateRoutesDlg::StatusUpdate(CString msg)
{
	m_status.SetWindowTextW(msg);
}

void CAlternateRoutesDlg::OnBnClickedHeuristic()
{
	// Disable all inputs and enable the ones that are necessary for Heuristic 
	// Here we need the number of paths as well as the efficiency threshold (See HeuristicAnalysis)
	disableInputs();
	m_efficiency_slider.EnableWindow(TRUE);
	m_efficiency_edit.EnableWindow(TRUE);
	m_num_paths_edit.EnableWindow(TRUE);
	m_go.EnableWindow(TRUE);

	// Disable selected algorithm button and enable the other two if user changes their mind
	m_heuristic.EnableWindow(FALSE);
	m_shortest.EnableWindow(TRUE);
	m_shortest_lap.EnableWindow(TRUE);

	// Populate algorithm selection variable
	algorithm_select = "heuristic";
	StatusUpdate(_T("Selection: Heuristic Algorithm"));
}


void CAlternateRoutesDlg::OnBnClickedKshortest1()
{
	// Disable all inputs and enable the ones that are necessary for k-ShortestPath algorithm
	// Here we only need the number of paths desired (See ShortestPathAnalysis)
	disableInputs();
	m_num_paths_edit.EnableWindow(TRUE);
	m_go.EnableWindow(TRUE);

	// Disable selected algorithm button and enable the other two if user changes their mind
	m_shortest.EnableWindow(FALSE);
	m_heuristic.EnableWindow(TRUE);
	m_shortest_lap.EnableWindow(TRUE);

	// Populate algorithm selection variable
	algorithm_select = "shortest";
	StatusUpdate(_T("Selection: k-Shortest Path Algorithm"));
}

void CAlternateRoutesDlg::OnBnClickedKshortest2()
{
	// Disable all inputs and enable the ones that are necessary for k-ShortestPath algorithm
	// Here we only need the number of paths desired (See ShortestPathLowOverlapAnalysis)
	disableInputs();
	m_num_paths_edit.EnableWindow(TRUE);
	m_go.EnableWindow(TRUE);
	
	// Disable selected algorithm button and enable the other two if user changes their mind
	m_shortest_lap.EnableWindow(FALSE);
	m_heuristic.EnableWindow(TRUE);
	m_shortest.EnableWindow(TRUE);

	// Populate algorithm selection variable
	algorithm_select = "shortest_lap";
	StatusUpdate(_T("Selection: k-Shortest Path with Limited Overlap Algorithm"));
	}

void CAlternateRoutesDlg::disableInputs()
{
	m_efficiency_slider.EnableWindow(FALSE);
	m_efficiency_edit.EnableWindow(FALSE);
	m_num_paths_edit.EnableWindow(FALSE);
	m_abort.EnableWindow(FALSE);
	m_go.EnableWindow(FALSE);
}


void CAlternateRoutesDlg::OnBnClickedGo()
{
	disableInputs();
	m_abort.EnableWindow(TRUE);
	m_heuristic.EnableWindow(FALSE);
	m_shortest.EnableWindow(FALSE);
	m_shortest_lap.EnableWindow(FALSE);
	
	if (algorithm_select == "heuristic") {
		m_hThread = AfxBeginThread(HeuristicAnalysis, this);
	} else if (algorithm_select == "shortest") {
		m_hThread = AfxBeginThread(ShortestPathAnalysis, this);
	} else if (algorithm_select == "shortest_lap") {
		m_hThread = AfxBeginThread(ShortestPathLowOverlapAnalysis, this);
	}
	else {
		StatusUpdate(_T("Unexpected Error, try again"));
		disableInputs();
		m_heuristic.EnableWindow(TRUE);
		m_shortest.EnableWindow(TRUE);
		m_shortest_lap.EnableWindow(TRUE);
	}
	
	// TODO: Add your control notification handler code here
}


UINT HeuristicAnalysis(void *pParam)
{
	CAlternateRoutesDlg* pThis = (CAlternateRoutesDlg*)pParam;

	pThis->SendMessage(WM_MY_THREAD_MESSAGE, 0);

	return 0;
}

UINT ShortestPathAnalysis(void *pParam)
{
	CAlternateRoutesDlg* pThis = (CAlternateRoutesDlg*)pParam;

	pThis->SendMessage(WM_MY_THREAD_MESSAGE, 1);

	return 0;
}

UINT ShortestPathLowOverlapAnalysis(void *pParam)
{
	CAlternateRoutesDlg* pThis = (CAlternateRoutesDlg*)pParam;

	pThis->SendMessage(WM_MY_THREAD_MESSAGE, 2);

	return 0;
}

void CAlternateRoutesDlg::OnBnClickedAbort()
{
	StatusUpdate(_T("Please select an algorithm to restart"));
	// Disable input parameters and enable the algorithm selection
	// Effectively resets the program
	disableInputs();
	m_heuristic.EnableWindow(TRUE);
	m_shortest.EnableWindow(TRUE);
	m_shortest_lap.EnableWindow(TRUE);

	// Wait till target thread responds, and exists
	WaitForSingleObject(m_hThread->m_hThread, INFINITE);

	// TODO: Add your control notification handler code here
}


LRESULT CAlternateRoutesDlg::OnThreadMessage(WPARAM wParam, LPARAM)
{
	switch (wParam) {
		case 0:
			StatusUpdate(_T("aye"));
			break;
		case 1:
			StatusUpdate(_T("shortest"));
			break;
		case 2:
			StatusUpdate(_T("shortest lap"));
			break;
		default:
			StatusUpdate(_T("no aye"));
			break;
	}
	return 0;
}