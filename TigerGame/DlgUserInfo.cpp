#include "StdAfx.h"
#include "TigerGame.h"
#include "DlgUserInfo.h"


CDlgUserInfo::CDlgUserInfo(void)
{
	m_bImage = true;
	m_bAutoClose = false;
	m_size.cx = 300;
	m_size.cy = 400;
}

CDlgUserInfo::~CDlgUserInfo(void)
{
}

void CDlgUserInfo::DrawWindowEx(CDC &dc, CRect rcClient)
{

}
void CDlgUserInfo::InitUI(CRect rcClient)
{
	CRect rcTemp;
	CControlBase * pControlBase = NULL;

	// 关闭按钮
	int nStartButton = rcClient.right - 45;
	rcTemp.SetRect(nStartButton, 0, nStartButton + 45, 29);
	pControlBase = new CImageButton(GetSafeHwnd(), this, BT_CLOSE, rcTemp);
	((CImageButton *)pControlBase)->SetBitmap(IDB_BT_CLOSE);
	m_vecControl.push_back(pControlBase);

	int nXStartButton = 10;
	int nYStartButton = 0;
	rcTemp.SetRect(nXStartButton, nYStartButton, rcClient.right - 1, nYStartButton + 30);
	pControlBase = new CStaticText(GetSafeHwnd(), this, TEXT_GAME_TITLE, rcTemp, TEXT("用户信息"), Color(254, 255, 255, 255), TEXT("微软雅黑"), 14, FontStyleBold);
	((CStaticText *)pControlBase)->SetAlignment(DT_LEFT, DT_VCENTER);
	m_vecControl.push_back(pControlBase);

}

// 消息响应
LRESULT CDlgUserInfo::OnControlMessage(UINT uControlID, UINT uMsg, void *pData, size_t lSize)
{
	if (uControlID == BT_CLOSE)
	{
		if (uMsg == BUTTOM_UP)
		{
			ShowWindow(SW_HIDE);
		}
	}
	return 0;
}

// 消息响应
LRESULT CDlgUserInfo::OnWindowMessage(UINT uControlID, UINT uMsg, void *pData, size_t lSize)
{
	return 0;
}