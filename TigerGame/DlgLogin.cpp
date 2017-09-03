
// IceMusic.cpp : ʵ���ļ�
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


// CDlgLogin ��Ϣ�������

BOOL CDlgLogin::OnInitDialog()
{
	CDlgBase::OnInitDialog();

	SetIcon(m_hIcon, TRUE);
	SetIcon(m_hIcon, FALSE);

	SetWindowText(TEXT("��Ա��¼"));

	return TRUE;
}

inline void CDlgLogin::InitUI(CRect rcClient)
{
	CRect rcTemp;
	int nStartX = 0;
	int nStartY = 0;
	CControlBase * pControlBase = NULL;

	// ���ر���
	pControlBase = new CPicture(GetSafeHwnd(), this, FRAME, rcClient);
	((CPicture *)pControlBase)->SetBitmap(IDB_DLGCHILD_BKG);
	((CPicture *)pControlBase)->SetShowMode(enSMFrame, 3);
	m_vecControl.push_back(pControlBase);

	// ����
	nStartX = 10;
	nStartY = 0;
	rcTemp.SetRect(nStartX, nStartY, rcClient.right - 1, nStartY + 30);
	pControlBase = new CStaticText(GetSafeHwnd(), this, TEXT_LOGIN_TITLE, rcTemp, TEXT("�û���¼"), Color(254, 255, 255, 255), TEXT("Tahoma"), 14, FontStyleBold);
	((CStaticText *)pControlBase)->SetAlignment(DT_LEFT, DT_VCENTER);
	m_vecControl.push_back(pControlBase);

	// �˺�����
	nStartX = 30;
	nStartY = 100;
	rcTemp.SetRect(nStartX, nStartY, nStartX + 200, nStartY + 28);
	pControlBase = new CEditEx(GetSafeHwnd(), this, EDIT_ACCOUNT, rcTemp,TEXT(""));
	((CEditEx *)pControlBase)->SetBitmap(IDB_PNG_EDIT);
	((CEditEx *)pControlBase)->SetSmallBitmap(IDB_BT_DROP_DOWN);
	((CEditEx *)pControlBase)->SetRect(rcTemp);
	m_vecControl.push_back(pControlBase);
	// ��������
	nStartX = 30;
	nStartY = 140;
	rcTemp.SetRect(nStartX, nStartY, nStartX + 200, nStartY + 28);
	pControlBase = new CEditEx(GetSafeHwnd(), this, EDIT_PASSWORD, rcTemp, TEXT(""), TRUE);
	((CEditEx *)pControlBase)->SetBitmap(IDB_PNG_EDIT);
	((CEditEx *)pControlBase)->SetSmallBitmap(IDB_BT_KEY_BOARD);
	((CEditEx *)pControlBase)->SetRect(rcTemp);
	m_vecControl.push_back(pControlBase);
	// ��ס����
	nStartX = 30;
	nStartY = 180;
	rcTemp.SetRect(nStartX, nStartY, nStartX + 100, nStartY + 25);
	pControlBase = new CCheckButton(GetSafeHwnd(),this, BT_CHECK_PASSWORD, rcTemp, TEXT("��ס����"));
	((CCheckButton *)pControlBase)->SetBitmap(IDB_PNG_LONG_CHECK_BOX);
	m_vecControl.push_back(pControlBase);
	// ��������
	nStartX = 200;
	nStartY = 180;
	rcTemp.SetRect(nStartX, nStartY, nStartX + 256, nStartY + 16);
	pControlBase = new CLinkButton(GetSafeHwnd(), this, BT_FORGET_PASSWORD, rcTemp, TEXT("��������"), TEXT("http://www.baidu.com"));
	m_vecControl.push_back(pControlBase);

	// ��¼��ť
	nStartX = 30;
	nStartY = 220;
	rcTemp.SetRect(nStartX, nStartY, nStartX + 220, nStartY + 40);
	pControlBase = new CImageButton(GetSafeHwnd(), this, BT_LOGIN, rcTemp);
	((CImageButton *)pControlBase)->SetBitmap(IDB_PNG_BT_LOGIN);
	m_vecControl.push_back(pControlBase);

	// ע�ᰴť
}

void CDlgLogin::OnSize(UINT nType, CRect rcClient)
{
}

// ���ÿؼ�
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
			//AfxMessageBox(TEXT("��ʼ��¼"));
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
