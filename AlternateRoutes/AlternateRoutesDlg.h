
// AlternateRoutesDlg.h : header file
//

#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include "graph.h"
#define WM_MY_THREAD_MESSAGE    WM_APP+100

// CAlternateRoutesDlg dialog
class CAlternateRoutesDlg : public CDialog
{
// Construction
public:
	CAlternateRoutesDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ALTERNATEROUTES_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;
	CString algorithm_select;
	
	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()


public:
	CSpinButtonCtrl num_paths_spin;
	CSpinButtonCtrl m_efficiency_spin;
	Graph graph;
	afx_msg void OnDeltaposSpin1(NMHDR *pNMHDR, LRESULT *pResult);
	CEdit m_num_paths_edit;
	afx_msg void OnNMReleasedcaptureSlider1(NMHDR *pNMHDR, LRESULT *pResult);
	CEdit m_efficiency_edit;
	afx_msg void OnNMCustomdrawSlider1(NMHDR *pNMHDR, LRESULT *pResult);
	CSliderCtrl m_efficiency_slider;
	afx_msg void OnDeltaposSpin2(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedGraphupload();
	afx_msg void OnEnChangeEdit1();
	afx_msg void StatusUpdate(CString msg);
	void OnBnClickedKshortest2();
	CEdit m_status;
	CTabCtrl m_ctrlTAB;
	CButton m_go;
	CButton m_heuristic;
	CButton m_shortest;
	CButton m_shortest_lap;
	afx_msg void OnBnClickedHeuristic();
	afx_msg void OnBnClickedKshortest1();
	void disableInputs();
	CButton m_abort;
	afx_msg void OnBnClickedGo();
	afx_msg void OnBnClickedAbort();
	LRESULT OnThreadMessage(WPARAM wParam, LPARAM);
};
