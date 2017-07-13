// main.cpp : implementation file
//

#include "stdafx.h"
#include "AlternateRoutes.h"
#include "main.h"
#include "afxdialogex.h"


// main dialog

IMPLEMENT_DYNAMIC(main, CDialogEx)

main::main(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_ALTERNATEROUTESMAIN, pParent)
{

}

main::~main()
{
}

void main::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT4, m_status);
	DDX_Control(pDX, IDC_EDIT1, m_status2);
}


BEGIN_MESSAGE_MAP(main, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &main::OnBnClickedButton1)
END_MESSAGE_MAP()


// main message handlers


void main::OnBnClickedButton1()
{
	m_status2.SetWindowTextW(_T("HI"));
	// TODO: Add your control notification handler code here
}
