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

	// �رհ�ť
	int nStartButton = rcClient.right - 45;
	rcTemp.SetRect(nStartButton, 0, nStartButton + 45, 29);
	pControlBase = new CImageButton(GetSafeHwnd(), this, BT_CLOSE, rcTemp);
	((CImageButton *)pControlBase)->SetBitmap(IDB_BT_CLOSE);
	m_vecControl.push_back(pControlBase);
	// ˢ�°�ť
	nStartButton -= 31;
	rcTemp.SetRect(nStartButton, 0, nStartButton + 31, 29);
	pControlBase = new CImageButton(GetSafeHwnd(), this, BT_RES, rcTemp);
	((CImageButton *)pControlBase)->SetBitmap(IDB_PNG_BT_RES);
	m_vecControl.push_back(pControlBase);

	// ������
	int nXStartButton = 10;
	int nYStartButton = 0;
	rcTemp.SetRect(nXStartButton, nYStartButton, rcClient.right - 1, nYStartButton + 30);
	pControlBase = new CStaticText(GetSafeHwnd(), this, TEXT_SERVER_TITLE, rcTemp, TEXT("�������ڵ�ѡ��"), Color(254, 255, 255, 255), TEXT("΢���ź�"), 14, FontStyleBold);
	((CStaticText *)pControlBase)->SetAlignment(DT_LEFT, DT_VCENTER);
	m_vecControl.push_back(pControlBase);

	// ����4����ť
	CString strList[4] = { L"ȫ������" ,L"��������", L"���ϵ���", L"���ϵ���" };
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
	// �б�ͷ����������/
	/*
	nXStartButton = 25;
	nYStartButton = 90;
	rcTemp.SetRect(nXStartButton, nYStartButton, rcClient.right - 25, nYStartButton + 30);
	pControlBase = new CPicture(GetSafeHwnd(), this, PIC_SERVER_HEADER, rcTemp);
	((CPicture *)pControlBase)->SetBitmap(IDB_PNG_SERVER_HEADER);
	m_vecControl.push_back(pControlBase);
	*/

	// �������б�
	nXStartButton = 25;
	nYStartButton = 120;
	rcTemp.SetRect(nXStartButton, nYStartButton, rcClient.right - 25, nYStartButton + 300);
	pControlBase = new CListGrid(GetSafeHwnd(), this, LIST_SERVER, rcTemp);
	((CListGrid *)pControlBase)->AddItem(L"��ר�߽ڵ�-L2TPģʽ-B1                              32ms");
	((CListGrid *)pControlBase)->AddItem(L"��ר�߽ڵ�-L2TPģʽ-B1                              32ms");
	m_vecControl.push_back(pControlBase);
	
}

// ��Ϣ��Ӧ
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

// ��Ϣ��Ӧ
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
			((CListGrid *)pControlBase)->AddItem(L"��ר�߽ڵ�-L2TPģʽ-B1                              32ms");
			((CListGrid *)pControlBase)->AddItem(L"��ר�߽ڵ�-L2TPģʽ-B1                              32ms");
		}
	}
}