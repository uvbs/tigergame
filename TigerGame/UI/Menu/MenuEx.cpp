#include "StdAfx.h"
#include "MenuEx.h"
// #include <imm.h>
// #pragma comment ( lib , "imm32.lib" )

CMenuEx::CMenuEx(CString strFont/* = TEXT("Tahoma")*/, int nFontWidth/* = 12*/, FontStyle fontStyle/* = FontStyleRegular*/)
{
	m_strFont = strFont;
	m_nFontWidth = nFontWidth;
	m_fontStyle = fontStyle;
	m_uAlignment = DT_LEFT;
	m_uVAlignment = DT_TOP;

	m_point.SetPoint(0, 0);
	m_nLeft = 30;
	m_nHeight = 30;
	m_nWidth = 113;
	m_nSeparatorHeight = 4;
}

CMenuEx::~CMenuEx(void)
{
}

bool CMenuEx::Create(CWnd *pParent, CPoint point, UINT uControlID, UINT nResourceID, int nFrameSize/* = 4*/, enumBackMode enBackMode/* = enBMFrame*/, int nMinWidth/* = 113*/)
{
	m_point = point;
	CRect rc(point.x - nMinWidth / 2, point.y, point.x + nMinWidth / 2, point.y + nMinWidth);
	return CDlgPopup::Create(pParent, rc, uControlID, nResourceID, enBackMode, nFrameSize);
}

bool CMenuEx::Create(CWnd *pParent, CPoint point, UINT uControlID, CString strImage, int nFrameSize/* = 4*/, enumBackMode enBackMode/* = enBMFrame*/, int nMinWidth/* = 113*/)
{
	m_point = point;
	CRect rc(point.x - nMinWidth / 2, point.y, point.x + nMinWidth / 2, point.y + nMinWidth);
	return CDlgPopup::Create(pParent, rc, uControlID, strImage, enBackMode, nFrameSize);
}

void CMenuEx::DrawWindowEx(CDC &dc, CRect rcClient)
{

}
void CMenuEx::InitUI(CRect rcClient)
{
	//ImmDisableIME(GetCurrentThreadId());
}

int CMenuEx::AddMenu(CString strText, UINT uMenuID, int nResourceID, bool bSelect, int nIndex)
{
	CControlBase * pControlBase = NULL;

	FontFamily fontFamily(m_strFont);
	Font font(&fontFamily, (REAL)m_nFontWidth, m_fontStyle, UnitPixel);;

	StringFormat strFormat;
	strFormat.SetAlignment(StringAlignmentNear);
	strFormat.SetFormatFlags(StringFormatFlagsNoWrap | StringFormatFlagsMeasureTrailingSpaces);
	Size size = GetTextBounds(font, strFormat, strText);

	if (size.Width > m_nWidth - m_nLeft - 4)
	{
		m_nWidth = size.Width + m_nLeft + 4;
	}

	pControlBase = new CMenuItem(GetSafeHwnd(), this, uMenuID, CRect(0, 0, 0, 0), strText, m_nLeft, bSelect);
	((CControlBaseFont *)pControlBase)->SetFont(m_strFont, m_nFontWidth, m_fontStyle);
	if (nResourceID != -1)
	{
		((CMenuItem *)pControlBase)->SetBitmap(nResourceID);
	}

	if (nIndex >= 0 && nIndex < m_vecControl.size())
	{
		m_vecControl.insert(m_vecControl.begin() + nIndex, pControlBase);
	}
	else
	{
		m_vecControl.push_back(pControlBase);
	}

	SetMenuPoint();
	return m_vecControl.size();
}

int CMenuEx::AddSeparator(int nIndex)
{
	CControlBase * pControlBase = new CLine(GetSafeHwnd(), this, -1, CRect(0, 0, 0, 0), Color(254, 227, 229, 230));

	if (nIndex >= 0 && nIndex < m_vecControl.size())
	{
		m_vecControl.insert(m_vecControl.begin() + nIndex, pControlBase);
	}
	else
	{
		m_vecControl.push_back(pControlBase);
	}

	SetMenuPoint();
	return m_vecControl.size();
}

// 设置菜单项位置
void CMenuEx::SetMenuPoint()
{
	int nXPos = 2;
	int nYPos = 2;
	CRect rc;
	for (size_t i = 0; i < m_vecControl.size(); i++)
	{
		CControlBase * pControlBase = m_vecControl[i];
		if (-1 == pControlBase->GetControlID())
		{
			rc.SetRect(m_nLeft + 4, nYPos + 1, m_nWidth - 9, nYPos + 2);
			nYPos += 4;

		}
		else
		{
			rc.SetRect(nXPos, nYPos, m_nWidth - 2, nYPos + m_nHeight);
			nYPos += m_nHeight;
		}
		SetControlRect(pControlBase, rc);
	}
	nYPos += 2;
	
	int nWidth = GetSystemMetrics(SM_CXSCREEN);
	int nHeight = GetSystemMetrics(SM_CYSCREEN);

	int nMenuX = m_point.x;
	int nMenuY = m_point.y;
	if (m_point.x + m_nWidth > nWidth)
	{
		nMenuX = m_point.x - m_nWidth;
	}

	if (m_point.y + nYPos > nHeight)
	{
		nMenuY = m_point.y - nYPos;
	}
	
	SetWindowPos(NULL, nMenuX, nMenuY, m_nWidth, nYPos, SWP_SHOWWINDOW);
	InvalidateRect(NULL);
}

// 消息响应
LRESULT CMenuEx::OnControlMessage(UINT uControlID, UINT uMsg, void *pData, size_t lSize)
{
	tagMenuItemInfo *pMenuItemInfo = (tagMenuItemInfo *)pData;
	tagMenuExInfo menuExInfo;
	menuExInfo.uMenuID = uControlID;
	menuExInfo.bSelect = pMenuItemInfo->bSelect;
	menuExInfo.bDown = pMenuItemInfo->bDown;


	PostMessageEx(uMsg, &menuExInfo, sizeof(menuExInfo));

	if (uMsg == BUTTOM_UP)
	{
		CloseWindow();
	}

	return 0;
}

// 消息响应
LRESULT CMenuEx::OnWindowMessage(UINT uControlID, UINT uMsg, void *pData, size_t lSize)
{
	return 0;
}