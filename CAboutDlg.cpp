#include "stdafx.h"
#include "CAboutDlg.h"
#include ".\caboutdlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif




CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	ON_NOTIFY(NM_CLICK, IDC_SYSLINK1, &CAboutDlg::OnNMClickSyslink1)
END_MESSAGE_MAP()


void CAboutDlg::OnNMClickSyslink1(NMHDR* pNMHDR, LRESULT* pResult)
{

}