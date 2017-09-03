
// IceMusic.cpp : 实现文件
//

#include "stdafx.h"
#include "DlgAbout.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


CDlgAbout::CDlgAbout(CWnd* pParent /*=NULL*/)
	: CDlgBase(CDlgAbout::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	m_bOverRegio = false;
	m_bChangeSize = true;
	m_bShowTaskbar = true;

	m_MinSize.SetSize(384, 280);
	m_MaxSize.SetSize(384, 280);
	m_bMinBotton = false;
}

void CDlgAbout::DoDataExchange(CDataExchange* pDX)
{
	CDlgBase::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CDlgAbout, CDlgBase)
	ON_WM_QUERYDRAGICON()

	//}}AFX_MSG_MAP
	ON_WM_DROPFILES()
END_MESSAGE_MAP()


// CDlgAbout 消息处理程序

BOOL CDlgAbout::OnInitDialog()
{
	CDlgBase::OnInitDialog();

	SetIcon(m_hIcon, TRUE);
	SetIcon(m_hIcon, FALSE);

	SetWindowText(TEXT("关于我们"));

	return TRUE;
}

inline void CDlgAbout::InitUI(CRect rcClient)
{
	CRect rcTemp;
	int nStartX = 0;
	int nStartY = 0;
	CControlBase * pControlBase = NULL;

	int nXStartButton = 1;
	int nYStartButton = 30;

	nXStartButton = 10;
	nYStartButton = 0;
	rcTemp.SetRect(nXStartButton, nYStartButton, rcClient.right - 1, nYStartButton + 30);
	pControlBase = new CStaticText(GetSafeHwnd(), this, TEXT_ABOUT_TITLE, rcTemp, TEXT("关于我们"), Color(254, 255, 255, 255), TEXT("Tahoma"), 14, FontStyleBold);
	((CStaticText *)pControlBase)->SetAlignment(DT_LEFT, DT_VCENTER);
	m_vecArea.push_back(pControlBase);

}

void CDlgAbout::OnSize(UINT nType, CRect rcClient)
{
}

// 重置控件
void CDlgAbout::ResetControlBackHSL(COLOR_HSL colorHSL, COLOR_HSL OldColorHSL)
{
	if (colorHSL.luminance > 65 && OldColorHSL.luminance <= 65 || colorHSL.luminance <= 65 && OldColorHSL.luminance > 65)
	{
	}
}

HCURSOR CDlgAbout::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

LRESULT CDlgAbout::OnControlMessage(UINT uControlID, UINT uMsg, void *pData, size_t lSize)
{
	return 0;
}

LRESULT CDlgAbout::OnWindowMessage(UINT uControlID, UINT uMsg, void *pData, size_t lSize)
{

	return 0;
}

void CDlgAbout::OnWindowMouseLeave()
{

}

void CDlgAbout::OnWindowMouseHover()
{

}


void CDlgAbout::OnTimer(UINT uTimerID)
{
	CDlgBase::OnTimer(uTimerID);
}
