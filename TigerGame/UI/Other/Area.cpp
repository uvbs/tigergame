#include "StdAfx.h"
#include "Area.h"

CArea::CArea(HWND hWnd, CMessageInterface* pMessage, UINT uControlID, CRect rc, Color clr,
	int nBeginTransparent/* = 128*/,int nEndTransparent/* = 128*/,  bool bIsVisible/* = TRUE*/)
			: CControlBase(hWnd, pMessage, uControlID, rc, bIsVisible, FALSE, FALSE)
{
	m_clr = clr;
	m_nBeginTransparent = nBeginTransparent;
	m_nEndTransparent = nEndTransparent;
}

CArea::~CArea(void)
{
}

void CArea::DrawControl(CDC &dc, CRect rcParent, CRect rcUpdate)
{
	int nWidth = m_rc.Width();
	int nHeight = m_rc.Height();

	if(m_bUpdate)
	{
		UpdateMemDC(dc, nWidth, nHeight);
		if (m_bBackColorUpdate)
		{
			m_clr.SetFromCOLORREF(m_clrBK);
		}
		Graphics graphics(m_memDC);
		LinearGradientBrush linGrBrush(Point(0, 0), Point(0, m_rc.Height()),
			Color(m_nBeginTransparent, m_clr.GetR(), m_clr.GetG(), m_clr.GetB()), Color(m_nEndTransparent, m_clr.GetR(), m_clr.GetG(), m_clr.GetB()));
		graphics.FillRectangle(&linGrBrush, 0, 0, m_rc.Width(), m_rc.Height());
	}

	DrawControlEx(dc, rcParent, rcUpdate);
}

void  CArea::SetColor(Color clr)
{
	m_clr = clr;
	UpdateControl(true);
}