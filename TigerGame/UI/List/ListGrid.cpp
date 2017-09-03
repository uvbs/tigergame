#include "StdAfx.h"
#include "ListGrid.h"
#include "..\Other\Scroll.h"
#include "..\Picture\Picture.h"

#define					SCROLL_V_LIST								1
#define					IA_LIST_PLAYING								2
#define					BT_LIST_PLAYING								3
#define					BT_LIST_PLAY								4
#define					AREA_LIST_PLAY								5
#define					AREA_LIST_HOVER								6
#define					AREA_LIST_SELECT							7
#define					AREA_LIST_TITLE								8

CListGrid::CListGrid(HWND hWnd, CMessageInterface* pMessage, UINT uControlID, CRect rc, bool bIsVisible/* = TRUE*/)
	: CControlBaseFont(hWnd, pMessage, uControlID, rc, TEXT(""), bIsVisible, FALSE, true, TEXT("Tahoma"), 11)
{
	m_bLButtonDblClk = true;
	SetVAlignment(DT_VCENTER);
	SetFontColor(Color(254, 0, 0, 0));
	m_nControlHeight = rc.Height();
	m_nCurShowTop = 0;
	m_nItemHeight = 30;
	m_nPlayItem = -1;
	m_nSelectItem = -1;
	m_nHoverItem = -1;
	m_bPlay = false;
	m_nContentHeight = m_nControlHeight - m_nItemHeight;		// 内容区域高度

	LogTrace(L"rc=%d,%d,%d,%d m_nControlHeight=%d rc.height=%d \r\n", rc.left, rc.top, rc.right, rc.bottom, m_nControlHeight, rc.Height());

	CControlBase *pControlBase = NULL;
	CRect rcTemp = rc;

	rcTemp.top += 30;
	rcTemp.left = rcTemp.right - 10;
	pControlBase = new CScrollV(hWnd, this, SCROLL_V_LIST, rcTemp);
	pControlBase->SetComControl(true);
	((CScrollV *)pControlBase)->SetScrollInfo(m_nControlHeight, m_nCurShowTop, rcTemp.Height());
	((CScrollV *)pControlBase)->SetBitmap(IDB_PNG_SCROLL_V, IDB_PNG_SCROLL_UP, IDB_PNG_SCROLL_DOWN);
	m_vecControl.push_back(pControlBase);

	LogTrace(L"rcTemp=%d,%d,%d,%d m_nControlHeight=%d rcTemp.height=%d \r\n", rcTemp.left, rcTemp.top, rcTemp.right, rcTemp.bottom, m_nControlHeight, rcTemp.Height());
	
	rcTemp = rc;
	rcTemp.bottom = rcTemp.top + 30;

	//LogTrace(L"reTemp=%d,%d,%d,%d \r\n", rcTemp.left, rcTemp.top, rcTemp.right, rcTemp.bottom);

	pControlBase = new CPicture(hWnd, this, AREA_LIST_TITLE, rcTemp);
	((CPicture *)pControlBase)->SetBitmap(IDB_LISTGRID_TITLE_BG);
	((CPicture *)pControlBase)->SetShowMode(enSMFrame);
	m_vecControl.push_back(pControlBase);
	
	/*
	rcTemp = rc;
	rcTemp.top = 0;
	rcTemp.left = rc.right - 10 - 28;
	rcTemp.right = rcTemp.left + 18;
	rcTemp.bottom = rcTemp.top + 8;
	pControlBase = new CImageAnimation(hWnd, this, IA_LIST_PLAYING, rcTemp);
	((CImageAnimation *)pControlBase)->SetBitmap(IDB_PNG_IA_PLAYING, 14);
	((CImageAnimation *)pControlBase)->SetRun(true);
	m_vecControl.push_back(pControlBase);
	*/

	rcTemp = rc;
	rcTemp.right = rcTemp.left + 5;
	rcTemp.bottom = rcTemp.top + m_nItemHeight;
	pControlBase = new CArea(hWnd, this, AREA_LIST_PLAY, rcTemp, m_clrBK, 255, 255, FALSE);
	pControlBase->SetControlBackColorUpdate(true);
	m_vecControl.push_back(pControlBase);

	rcTemp = rc;
	rcTemp.right = rcTemp.left + 5;
	rcTemp.bottom = rcTemp.top + m_nItemHeight;
	pControlBase = new CArea(hWnd, this, AREA_LIST_HOVER, rcTemp, m_clrBK, 255, 255, FALSE);
	pControlBase->SetControlBackColorUpdate(true);
	m_vecControl.push_back(pControlBase);

	rcTemp = rc;
	rcTemp.right = rcTemp.left + 5;
	rcTemp.bottom = rcTemp.top + m_nItemHeight;
	pControlBase = new CArea(hWnd, this, AREA_LIST_SELECT, rcTemp, m_clrBK, 255, 255, FALSE);
	pControlBase->SetControlBackColorUpdate(true);
	m_vecControl.push_back(pControlBase);
}

CListGrid::~CListGrid(void)
{
}

bool CListGrid::SetControlRect(CRect rc)
{
	CRect rcOld = m_rc;
	m_rc = rc;
	CRect rcTemp;
	for (size_t i = 0; i < m_vecControl.size(); i++)
	{
		CControlBase * pControlBase = m_vecControl.at(i);
		if (pControlBase)
		{
			UINT uControlID = pControlBase->GetControlID();
			if (SCROLL_V_LIST == uControlID)
			{
				rcTemp = m_rc;
				rcTemp.left = rcTemp.right - 10;
			}
			else if (AREA_LIST_PLAY == uControlID)
			{

				rcTemp.left = m_rc.left;
				rcTemp.right = rcTemp.left + 3;
				rcTemp.top = m_nPlayItem * m_nItemHeight + m_rc.top - m_nCurShowTop;
				rcTemp.bottom = rcTemp.top + m_nItemHeight;
			}
			else
			{
				continue;
			}
			pControlBase->SetRect(rcTemp);			
		}
	}
	int nControlHeight = m_nControlHeight;
	m_nControlHeight = __max(m_rc.Height(), m_nItemHeight * m_vecItemText.size());
	if (m_nControlHeight != nControlHeight)
	{
		CControlBase * pControlBase = GetControl(SCROLL_V_LIST);
		if (pControlBase)
		{
			((CScrollV *)pControlBase)->SetScrollInfo(m_nControlHeight, m_nCurShowTop, m_nItemHeight);
		}

		return true;
	}
	return rcOld.Width() != m_rc.Width() || rcOld.Height() != m_rc.Height();
}

void CListGrid::DrawControl(CDC &dc, CRect rcParent, CRect rcUpdate)
{
	int nWidth = m_rc.Width();
	int nHeight = m_rc.Height();

	if (m_bUpdate)
	{
		int nItemCount = (nHeight + m_nItemHeight - 1) / m_nItemHeight;
		if (m_nCurShowTop % m_nItemHeight != 0)
		{
			nItemCount++;
		}
		// 计算画布高度

		UpdateMemDC(dc, nWidth, nItemCount * m_nItemHeight);

		

		Graphics graphics(m_memDC);

		FontFamily fontFamily(m_strFont);
		Font font(&fontFamily, (REAL)m_nFontWidth, m_fontStyle, UnitPixel);

		SolidBrush solidBrush(m_clrText);
		SolidBrush solidBrushPlay(Color(254, GetRValue(m_clrBK), GetGValue(m_clrBK), GetBValue(m_clrBK)));

		graphics.SetTextRenderingHint(TextRenderingHintAntiAliasGridFit);

		StringFormat strFormat;
		strFormat.SetAlignment(StringAlignmentNear);
		strFormat.SetTrimming(StringTrimmingEllipsisCharacter);
		strFormat.SetFormatFlags(StringFormatFlagsNoWrap | StringFormatFlagsMeasureTrailingSpaces);

		// 画标题栏

		int nXPos = 0;
		int nYPos = 30;

		SolidBrush solidBrush1(Color(200, 255, 255, 255));
		SolidBrush solidBrush2(Color(200, 244, 244, 244));
		int nStartIndex = m_nCurShowTop / m_nItemHeight;
		for (size_t i = nStartIndex; i < m_vecItemText.size() && i < nStartIndex + nItemCount; i++)
		{
			graphics.FillRectangle(i % 2 == 0 ? &solidBrush1 : &solidBrush2, 0, nYPos, nWidth, m_nItemHeight);

			CString strItemText = m_vecItemText[i];
			Size size = GetTextBounds(font, strFormat, strItemText);
			CPoint point = GetOriginPoint(nWidth - nXPos, m_nItemHeight, size.Width, size.Height, m_uAlignment, m_uVAlignment);

			RectF rect(nXPos + point.x + 13, nYPos + point.y, nWidth - nXPos - point.x - 10 - 28, nYPos + size.Height);
			graphics.DrawString(strItemText, (INT)_tcslen(strItemText), &font,
				rect, &strFormat, i == m_nPlayItem ? &solidBrushPlay : &solidBrush);

			nYPos += m_nItemHeight;
		}

		if (nYPos < nHeight)
		{
			graphics.FillRectangle(&solidBrush1, 0, nYPos, nWidth, nHeight - nYPos);
		}
	}

	DrawControlEx(dc, rcParent, rcUpdate, 0, m_nCurShowTop - m_nCurShowTop / m_nItemHeight * m_nItemHeight);
}

void CListGrid::UpdateScroll()
{
	m_nControlHeight = __max(m_rc.Height(), m_nItemHeight * m_vecItemText.size());

	CControlBase * pControlBase = GetControl(SCROLL_V_LIST);
	if (pControlBase)
	{
		((CScrollV *)pControlBase)->SetScrollInfo(m_nControlHeight, m_nCurShowTop, m_nItemHeight);
	}

	UpdateControl(true);
}

int CListGrid::AddItem(CString strItemText)
{
	m_vecItemText.push_back(strItemText);
	UpdateScroll();

	return m_vecItemText.size();
}

bool CListGrid::DeleteItem(UINT uID)
{
	bool bRet = false;
	int i = 0;
	vector<CString>::iterator it;
	for (it = m_vecItemText.begin(); it != m_vecItemText.end(); it++)
	{
		if ( i == uID)
		{
			it = m_vecItemText.erase(it); // 不能写成arr.erase(it);
			bRet = true;
			break;
		}
		else
		{
			++it;
		}
		i++;
	}
	UpdateScroll();
	UpdateControl(true);

	return bRet;
}

void CListGrid::DeleteAllItem()
{
	m_vecItemText.clear();
	UpdateScroll();
	UpdateControl(true);
}

int CListGrid::SetSelect(int nPlayItem)
{
	int nOldnPlayItem = m_nPlayItem;
	if (nPlayItem < m_vecItemText.size())
	{
		m_bPlay = true;
		m_nPlayItem = nPlayItem;
		m_nSelectItem = -1;
		if (nPlayItem == m_nHoverItem)
		{
			m_nHoverItem = -1;
		}

		UpdateControlRect(true);
		if (nOldnPlayItem != m_nPlayItem)
		{
			UpdateControl(true);
		}
	}
	

	return nOldnPlayItem;
}

void CListGrid::SetSelectPlay(bool bPlay)
{
	m_bPlay = bPlay;
	UpdateControlRect(true);
}

int CListGrid::GetSelectID()
{
	return m_nSelectItem;
}

// 消息响应
LRESULT CListGrid::OnControlMessage(UINT uControlID, UINT uMsg, void *pData, size_t lSize)
{
	if (uControlID == SCROLL_V_LIST)
	{
		if (0 == uMsg)
		{
			m_nCurShowTop = *(int *)pData;

			UpdateControlRect(true);
			UpdateControl(true);
		}
	}
	return 0L;
}

bool CListGrid::OnControlMouseMove(UINT nFlags, CPoint point)
{
	if (m_bRButtonDown)
	{
		return 0;
	}
	int nHoverItem = m_nHoverItem;

	if (m_rc.PtInRect(point))
	{
		//LogTrace(L"m_rc.top=%d point.y=%d\r\n", m_rc.top, point.y);
		// 鼠标放再头部标题栏
		if ( point.y < m_rc.top + 30)
		{
			m_nHoverItem = -1;
			UpdateControlRect(true);
			return 0;
		}

		m_nHoverItem = m_nCurShowTop / m_nItemHeight + (point.y - m_rc.top - 30 - (m_nCurShowTop / m_nItemHeight * m_nItemHeight - m_nCurShowTop)) / m_nItemHeight;
		if (m_nHoverItem >= m_vecItemText.size() || m_nPlayItem == m_nHoverItem ||m_nHoverItem == m_nSelectItem)
		{
			m_nHoverItem = -1;
		}
	}
	else
	{ 
		m_nHoverItem = -1;
	}	

	if (nHoverItem != m_nHoverItem)
	{
		UpdateControlRect(true);
	}
	return 0L;
}

bool CListGrid::OnControlLButtonDown(UINT nFlags, CPoint point)
{
	if (m_nHoverItem != -1)
	{
		m_nSelectItem = m_nHoverItem;
		m_nHoverItem = -1;
		UpdateControlRect(true);
		PostMessage(LEST_LBUTTONDOWN, &m_nSelectItem, sizeof(m_nSelectItem));
	}

	return true;
}

bool CListGrid::OnControlLButtonUp(UINT nFlags, CPoint point)
{
	return true;
}

bool CListGrid::OnControlRButtonDown(UINT nFlags, CPoint point)
{
	int nHoverItem = m_nHoverItem;
	if (m_nHoverItem == -1)
	{
		nHoverItem = m_nSelectItem;
	}
	if (nHoverItem == -1)
	{
		nHoverItem = m_nPlayItem;
	}

	PostMessage(LEST_RBUTTONDOWN, &nHoverItem, sizeof(nHoverItem));

	return true;
}

bool CListGrid::OnControlRButtonUp(UINT nFlags, CPoint point)
{
	int nHoverItem = m_nHoverItem;
	if (nHoverItem == -1)
	{
		nHoverItem = m_nSelectItem;
	}
	if (nHoverItem == -1)
	{
		nHoverItem = m_nPlayItem;
	}

	PostMessage(LEST_RBUTTONUP, &nHoverItem, sizeof(nHoverItem));

	return true;
}

bool CListGrid::OnControlLButtonDblClk(UINT nFlags, CPoint point)
{
	if (m_nSelectItem != -1)
	{
		PostMessage(LEST_LBUTTONDBLCLK, &m_nSelectItem, sizeof(m_nSelectItem));
	}
	return true;
}

bool CListGrid::OnControlMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	CControlBase * pControlBase = GetControl(SCROLL_V_LIST);
	if (pControlBase)
	{
		((CScrollV *)pControlBase)->OnControlMouseWheel(nFlags, zDelta, pt);
	}
	return true;
}

void CListGrid::UpdateControlRect(bool bSelect)
{
	//if (bSelect)
	{
		CRect rc;
		CControlBase * pControlBase;
		/*
		CControlBase * pControlBase = GetControl(IA_LIST_PLAYING);
		if (pControlBase)
		{
			pControlBase->SetVisible(m_nPlayItem != -1 && m_bPlay);
			if (m_nPlayItem != -1 && m_bPlay)
			{
				rc = pControlBase->GetRect();
				rc.top = m_nPlayItem * m_nItemHeight + (m_nItemHeight - rc.Height()) / 2 + m_rc.top - m_nCurShowTop;
				rc.bottom = rc.top + pControlBase->GetRect().Height();
				pControlBase->SetRect(rc);
			}
		}
		*/
		pControlBase = GetControl(AREA_LIST_PLAY);
		if (pControlBase)
		{
			pControlBase->SetVisible(m_nPlayItem != -1);
			if (m_nPlayItem != -1)
			{
				rc = pControlBase->GetRect();
				rc.top = m_nPlayItem * m_nItemHeight + m_rc.top - m_nCurShowTop;
				rc.bottom = rc.top + m_nItemHeight;
				pControlBase->SetRect(rc);
			}
		}

		pControlBase = GetControl(AREA_LIST_HOVER);
		if (pControlBase)
		{
			pControlBase->SetVisible(m_nHoverItem != -1);
			if (m_nHoverItem != -1)
			{
				rc = pControlBase->GetRect();
				rc.top = m_nHoverItem * m_nItemHeight + m_rc.top - m_nCurShowTop + 30;
				rc.bottom = rc.top + m_nItemHeight;
				pControlBase->SetRect(rc);
			}
		}

		pControlBase = GetControl(AREA_LIST_SELECT);
		if (pControlBase)
		{
			pControlBase->SetVisible(m_nSelectItem != -1);
			if (m_nSelectItem != -1)
			{
				rc = pControlBase->GetRect();
				rc.top = m_nSelectItem * m_nItemHeight + m_rc.top - m_nCurShowTop + 30;
				rc.bottom = rc.top + m_nItemHeight;
				pControlBase->SetRect(rc);
			}
		}
	}
}