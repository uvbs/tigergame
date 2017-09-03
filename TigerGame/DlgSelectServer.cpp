#include "StdAfx.h"
#include "TigerGame.h"
#include "DlgSelectServer.h"


CDlgSelectServer::CDlgSelectServer(void)
{
	m_bImage = true;
	m_bAutoClose = false;
	m_size.cx = 300;
	m_size.cy = 400;
}

CDlgSelectServer::~CDlgSelectServer(void)
{
}

void CDlgSelectServer::DrawWindowEx(CDC &dc, CRect rcClient)
{

}
void CDlgSelectServer::InitUI(CRect rcClient)
{
	CRect rcTemp;
	CControlBase * pControlBase = NULL;

	// 关闭按钮
	int nStartButton = rcClient.right - 45;
	rcTemp.SetRect(nStartButton, 0, nStartButton + 45, 29);
	pControlBase = new CImageButton(GetSafeHwnd(), this, BT_CLOSE, rcTemp);
	((CImageButton *)pControlBase)->SetBitmap(IDB_BT_CLOSE);
	m_vecControl.push_back(pControlBase);
	// 刷新按钮
	nStartButton -= 31;
	rcTemp.SetRect(nStartButton, 0, nStartButton + 31, 29);
	pControlBase = new CImageButton(GetSafeHwnd(), this, BT_RES, rcTemp);
	((CImageButton *)pControlBase)->SetBitmap(IDB_PNG_BT_RES);
	m_vecControl.push_back(pControlBase);

	// 标题栏
	int nXStartButton = 10;
	int nYStartButton = 0;
	rcTemp.SetRect(nXStartButton, nYStartButton, rcClient.right - 1, nYStartButton + 30);
	pControlBase = new CStaticText(GetSafeHwnd(), this, TEXT_SERVER_TITLE, rcTemp, TEXT("服务器节点选择"), Color(254, 255, 255, 255), TEXT("微软雅黑"), 14, FontStyleBold);
	((CStaticText *)pControlBase)->SetAlignment(DT_LEFT, DT_VCENTER);
	m_vecControl.push_back(pControlBase);

	// 区域4个按钮
	CString strList[4] = { L"全国适用" ,L"华北地区", L"华南地区", L"西南地区" };
	nXStartButton = 10;
	nYStartButton = 50;
	for (int i = 0; i < 4; i++)
	{
		nXStartButton = 25 + i * 78;
		rcTemp.SetRect(nXStartButton, nYStartButton, nXStartButton + 78, nYStartButton + 30);

		pControlBase = new CTextButton(GetSafeHwnd(), this, BT_AREA_0 + i, rcTemp, strList[i], Color(254, 0, 0, 0), Color(254, 0, 0, 0), Color(254, 255, 255, 255));
		((CTextButton *)pControlBase)->SetAlignment(StringAlignmentCenter);
		((CTextButton *)pControlBase)->SetVAlignment(DT_VCENTER);
		((CTextButton *)pControlBase)->SetControlBkg();
		m_vecControl.push_back(pControlBase);
	}
	// 列表头部文字描述/
	/*
	nXStartButton = 25;
	nYStartButton = 90;
	rcTemp.SetRect(nXStartButton, nYStartButton, rcClient.right - 25, nYStartButton + 30);
	pControlBase = new CPicture(GetSafeHwnd(), this, PIC_SERVER_HEADER, rcTemp);
	((CPicture *)pControlBase)->SetBitmap(IDB_PNG_SERVER_HEADER);
	m_vecControl.push_back(pControlBase);
	*/

	// 服务器列表
	nXStartButton = 25;
	nYStartButton = 120;
	rcTemp.SetRect(nXStartButton, nYStartButton, rcClient.right - 25, nYStartButton + 300);
	pControlBase = new CListGrid(GetSafeHwnd(), this, LIST_SERVER, rcTemp);
	((CListGrid *)pControlBase)->AddItem(L"云专线节点-L2TP模式-B1                              32ms");
	((CListGrid *)pControlBase)->AddItem(L"云专线节点-L2TP模式-B1                              32ms");
	m_vecControl.push_back(pControlBase);
	
}

// 消息响应
LRESULT CDlgSelectServer::OnControlMessage(UINT uControlID, UINT uMsg, void *pData, size_t lSize)
{
	if (uControlID == BT_CLOSE)
	{
		if (uMsg == BUTTOM_UP)
		{
			ShowWindow(SW_HIDE);
		}
	}
	else if (uControlID == BT_AREA_0)
	{
		if (uMsg == BUTTOM_UP)
		{
			UpdateAreaBtnStatus(BT_AREA_0);
			UpdateAreaServerList(BT_AREA_0);
		}
	}
	else if (uControlID == BT_AREA_1)
	{
		if (uMsg == BUTTOM_UP)
		{
			UpdateAreaBtnStatus(BT_AREA_1);
			UpdateAreaServerList(BT_AREA_1);
		}
	}
	else if (uControlID == BT_AREA_2)
	{
		if (uMsg == BUTTOM_UP)
		{
			UpdateAreaBtnStatus(BT_AREA_2);
			UpdateAreaServerList(BT_AREA_2);
		}
	}
	else if (uControlID == BT_AREA_3)
	{
		if (uMsg == BUTTOM_UP)
		{
			UpdateAreaBtnStatus(BT_AREA_3);
			UpdateAreaServerList(BT_AREA_3);
		}
	}
	else if (uControlID == BT_RES)
	{
		if (uMsg == BUTTOM_UP)
		{
			CControlBase * pControlBase = GetControl(LIST_SERVER);
			if (pControlBase)
			{
				((CListEx *)pControlBase)->DeleteAllItem();
			}
		}
	}
	return 0;
}

// 消息响应
LRESULT CDlgSelectServer::OnWindowMessage(UINT uControlID, UINT uMsg, void *pData, size_t lSize)
{
	return 0;
}

void CDlgSelectServer::UpdateAreaBtnStatus(UINT uID)
{
	for (size_t i = 0; i < 4; i++)
	{
		UINT uNowId = BT_AREA_0 + i;
		CControlBase * pControlBase = GetControl(uNowId);
		if (pControlBase)
		{
			((CTextButton *)pControlBase)->m_enButtonState = uNowId == uID ? enBSDown : enBSNormal;
		}
	}
}

void CDlgSelectServer::UpdateAreaServerList(UINT uID)
{
	CControlBase * pControlBase = GetControl(LIST_SERVER);
	if (pControlBase)
	{
		for (size_t i = 0; i < 3; i++)
		{
			((CListGrid *)pControlBase)->AddItem(L"云专线节点-L2TP模式-B1                              32ms");
			((CListGrid *)pControlBase)->AddItem(L"云专线节点-L2TP模式-B1                              32ms");
		}
	}
}