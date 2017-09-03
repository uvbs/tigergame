#include "StdAfx.h"
#include "TigerGame.h"
#include "DlgSelectGame.h"


CDlgSelectGame::CDlgSelectGame(void)
{
	m_bImage = true;
	m_bAutoClose = false;
	m_size.cx = 300;
	m_size.cy = 400;
}

CDlgSelectGame::~CDlgSelectGame(void)
{
}

void CDlgSelectGame::DrawWindowEx(CDC &dc, CRect rcClient)
{

}
void CDlgSelectGame::InitUI(CRect rcClient)
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
	pControlBase = new CStaticText(GetSafeHwnd(), this, TEXT_GAME_TITLE, rcTemp, TEXT("选择游戏"), Color(254, 255, 255, 255), TEXT("微软雅黑"), 14, FontStyleBold);
	((CStaticText *)pControlBase)->SetAlignment(DT_LEFT, DT_VCENTER);
	m_vecControl.push_back(pControlBase);


	CString strList[9] = { L"最近玩过" ,L"亚服", L"美服", L"欧服", L"台服", L"东南亚服", L"韩服", L"日服", L"澳服", };

	nXStartButton = 81;
	nYStartButton = 40;

	for (size_t i = 0; i < 9; i++)
	{
		LogTrace(L"%s\r\n", strList[i]);

		int nTempX = nXStartButton + (i-1) * 65;
		int nTempY = nYStartButton;
		if ( i >= 5 )
		{
			nTempX = nXStartButton + (i-5) * 65;
			nTempY = 69;
		}
		rcTemp.SetRect(nTempX, nTempY, nTempX + 64, nTempY + 28);
		if (i == 0)
		{
			rcTemp.SetRect(15, 40, 15 + 65, 40 + 57);
		}
		pControlBase = new CTextButton(GetSafeHwnd(), this, BT_PLAY + i, rcTemp, strList[i], Color(254, 0, 0, 0), Color(254, 0, 0, 0), Color(254, 255, 255, 255));
		((CTextButton *)pControlBase)->SetAlignment(StringAlignmentCenter);
		((CTextButton *)pControlBase)->SetVAlignment(DT_VCENTER);
		((CTextButton *)pControlBase)->SetControlBkg();
		m_vecControl.push_back(pControlBase);
	}

	nXStartButton = 30;
	nYStartButton = 120;
	rcTemp.SetRect(nXStartButton, nYStartButton, nXStartButton + 301, nYStartButton + 201);
	pControlBase = new CSelectBox(GetSafeHwnd(), this, BOX_IMAGE, rcTemp, 3, 2, true, Color(254, 255, 255, 255), Color(128, 0, 0, 0), Color(254, 56, 158, 221));
	((CSelectBox *)pControlBase)->SetBitmap(IDB_GAME_PUBG_AS, Color(254, 224, 107, 87));
	((CSelectBox *)pControlBase)->SetBitmap(IDB_GAME_PUBG_AS, Color(254, 224, 107, 87));
	((CSelectBox *)pControlBase)->SetBitmap(IDB_GAME_PUBG_AS, Color(254, 224, 107, 87));
	((CSelectBox *)pControlBase)->SetBitmap(IDB_GAME_PUBG_AS, Color(254, 224, 107, 87));
	((CSelectBox *)pControlBase)->SetBitmap(IDB_GAME_PUBG_AS, Color(254, 224, 107, 87));
	((CSelectBox *)pControlBase)->SetBitmap(IDB_GAME_PUBG_AS, Color(254, 224, 107, 87));
	m_vecControl.push_back(pControlBase);

}

// 消息响应
LRESULT CDlgSelectGame::OnControlMessage(UINT uControlID, UINT uMsg, void *pData, size_t lSize)
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
LRESULT CDlgSelectGame::OnWindowMessage(UINT uControlID, UINT uMsg, void *pData, size_t lSize)
{
	return 0;
}