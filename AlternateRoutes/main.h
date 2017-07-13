#pragma once
#include "afxwin.h"


// main dialog

class main : public CDialogEx
{
	DECLARE_DYNAMIC(main)

public:
	main(CWnd* pParent = NULL);   // standard constructor
	virtual ~main();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ALTERNATEROUTESMAIN };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	void StatusUpdate(CString msg);

	DECLARE_MESSAGE_MAP()
public:
	CEdit m_status;
	afx_msg void OnBnClickedButton1();
	CEdit m_status2;
};
