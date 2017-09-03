
// IceMusic.cpp : 实现文件
//

#include "stdafx.h"
#include "DlgLogin.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


CDlgLogin::CDlgLogin(CWnd* pParent /*=NULL*/)
	: CDlgBase(CDlgLogin::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	m_bOverRegio = false;
	m_bChangeSize = true;
	m_bShowTaskbar = true;

	m_MinSize.SetSize(360, 300);
	m_MaxSize.SetSize(360, 300);
	m_bMinBotton = true;
}

void CDlgLogin::DoDataExchange(CDataExchange* pDX)
{
	CDlgBase::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CDlgLogin, CDlgBase)
	ON_WM_QUERYDRAGICON()

	//}}AFX_MSG_MAP
	ON_WM_DROPFILES()
END_MESSAGE_MAP()


// CDlgLogin 消息处理程序

BOOL CDlgLogin::OnInitDialog()
{
	CDlgBase::OnInitDialog();

	SetIcon(m_hIcon, TRUE);
	SetIcon(m_hIcon, FALSE);

	SetWindowText(TEXT("会员登录"));

	return TRUE;
}

inline void CDlgLogin::InitUI(CRect rcClient)
{
	CRect rcTemp;
	int nStartX = 0;
	int nStartY = 0;
	CControlBase * pControlBase = NULL;

	// 加载背景
	pControlBase = new CPicture(GetSafeHwnd(), this, FRAME, rcClient);
	((CPicture *)pControlBase)->SetBitmap(IDB_DLGCHILD_BKG);
	((CPicture *)pControlBase)->SetShowMode(enSMFrame, 3);
	m_vecControl.push_back(pControlBase);

	// 标题
	nStartX = 10;
	nStartY = 0;
	rcTemp.SetRect(nStartX, nStartY, rcClient.right - 1, nStartY + 30);
	pControlBase = new CStaticText(GetSafeHwnd(), this, TEXT_LOGIN_TITLE, rcTemp, TEXT("用户登录"), Color(254, 255, 255, 255), TEXT("Tahoma"), 14, FontStyleBold);
	((CStaticText *)pControlBase)->SetAlignment(DT_LEFT, DT_VCENTER);
	m_vecControl.push_back(pControlBase);

	// 账号输入
	nStartX = 30;
	nStartY = 100;
	rcTemp.SetRect(nStartX, nStartY, nStartX + 200, nStartY + 28);
	pControlBase = new CEditEx(GetSafeHwnd(), this, EDIT_ACCOUNT, rcTemp,TEXT(""));
	((CEditEx *)pControlBase)->SetBitmap(IDB_PNG_EDIT);
	((CEditEx *)pControlBase)->SetSmallBitmap(IDB_BT_DROP_DOWN);
	((CEditEx *)pControlBase)->SetRect(rcTemp);
	m_vecControl.push_back(pControlBase);
	// 密码输入
	nStartX = 30;
	nStartY = 140;
	rcTemp.SetRect(nStartX, nStartY, nStartX + 200, nStartY + 28);
	pControlBase = new CEditEx(GetSafeHwnd(), this, EDIT_PASSWORD, rcTemp, TEXT(""), TRUE);
	((CEditEx *)pControlBase)->SetBitmap(IDB_PNG_EDIT);
	((CEditEx *)pControlBase)->SetSmallBitmap(IDB_BT_KEY_BOARD);
	((CEditEx *)pControlBase)->SetRect(rcTemp);
	m_vecControl.push_back(pControlBase);
	// 记住密码
	nStartX = 30;
	nStartY = 180;
	rcTemp.SetRect(nStartX, nStartY, nStartX + 100, nStartY + 25);
	pControlBase = new CCheckButton(GetSafeHwnd(),this, BT_CHECK_PASSWORD, rcTemp, TEXT("记住密码"));
	((CCheckButton *)pControlBase)->SetBitmap(IDB_PNG_LONG_CHECK_BOX);
	m_vecControl.push_back(pControlBase);
	// 忘记密码
	nStartX = 200;
	nStartY = 180;
	rcTemp.SetRect(nStartX, nStartY, nStartX + 256, nStartY + 16);
	pControlBase = new CLinkButton(GetSafeHwnd(), this, BT_FORGET_PASSWORD, rcTemp, TEXT("忘记密码"), TEXT("http://www.baidu.com"));
	m_vecControl.push_back(pControlBase);

	// 登录按钮
	nStartX = 30;
	nStartY = 220;
	rcTemp.SetRect(nStartX, nStartY, nStartX + 220, nStartY + 40);
	pControlBase = new CImageButton(GetSafeHwnd(), this, BT_LOGIN, rcTemp);
	((CImageButton *)pControlBase)->SetBitmap(IDB_PNG_BT_LOGIN);
	m_vecControl.push_back(pControlBase);

	// 注册按钮
}

void CDlgLogin::OnSize(UINT nType, CRect rcClient)
{
}

// 重置控件
void CDlgLogin::ResetControlBackHSL(COLOR_HSL colorHSL, COLOR_HSL OldColorHSL)
{
	if (colorHSL.luminance > 65 && OldColorHSL.luminance <= 65 || colorHSL.luminance <= 65 && OldColorHSL.luminance > 65)
	{
	}
}

HCURSOR CDlgLogin::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

LRESULT CDlgLogin::OnControlMessage(UINT uControlID, UINT uMsg, void *pData, size_t lSize)
{
	if (uControlID == EDIT_ACCOUNT)
	{
		if (uMsg == BUTTOM_UP)
		{
			//AfxMessageBox(TEXT("11111"));
		}
	}
	else if (uControlID == BT_LOGIN)
	{
		if (uMsg == BUTTOM_UP)
		{
			//AfxMessageBox(TEXT("开始登录"));
			CDlgBase::OnOK();
		}
	}
	return 0;
}

LRESULT CDlgLogin::OnWindowMessage(UINT uControlID, UINT uMsg, void *pData, size_t lSize)
{

	return 0;
}

void CDlgLogin::OnWindowMouseLeave()
{

}

void CDlgLogin::OnWindowMouseHover()
{

}


void CDlgLogin::OnTimer(UINT uTimerID)
{
	CDlgBase::OnTimer(uTimerID);
}
