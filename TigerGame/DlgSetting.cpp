#include "StdAfx.h"
#include "TigerGame.h"
#include "DlgSetting.h"


CDlgSetting::CDlgSetting(void)
{
	m_bImage = true;
	m_bAutoClose = false;
	m_size.cx = 300;
	m_size.cy = 400;
}

CDlgSetting::~CDlgSetting(void)
{
}

void CDlgSetting::DrawWindowEx(CDC &dc, CRect rcClient)
{

}
void CDlgSetting::InitUI(CRect rcClient)
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
	pControlBase = new CStaticText(GetSafeHwnd(), this, TEXT_SETTING_TITLE, rcTemp, TEXT("高级设置"), Color(254, 255, 255, 255), TEXT("微软雅黑"), 14, FontStyleBold);
	((CStaticText *)pControlBase)->SetAlignment(DT_LEFT, DT_VCENTER);
	m_vecControl.push_back(pControlBase);

}

// 消息响应
LRESULT CDlgSetting::OnControlMessage(UINT uControlID, UINT uMsg, void *pData, size_t lSize)
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
LRESULT CDlgSetting::OnWindowMessage(UINT uControlID, UINT uMsg, void *pData, size_t lSize)
{
	return 0;
}