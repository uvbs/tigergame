#include "StdAfx.h"
#include "Line.h"

CLine::CLine(HWND hWnd, CMessageInterface* pMessage, UINT uControlID, CRect rc, 
	Color clr/* = Color(255, 255, 255, 255)*/, bool bIsVisible/* = TRUE*/)
			: CControlBase(hWnd, pMessage, uControlID, rc, bIsVisible, FALSE, FALSE)
{
	m_clr = clr;
}

CLine::~CLine(void)
{
}

void CLine::DrawControl(CDC &dc, CRect rcParent, CRect rcUpdate)
{
	int nWidth = m_rc.Width();
	int nHeight = m_rc.Height();

	if(m_bUpdate)
	{
		UpdateMemDC(dc, nWidth, nHeight);
		
		Graphics graphics(m_memDC);
		SolidBrush brush(m_clr);
		graphics.FillRectangle(&brush, 0, 0, nWidth, nHeight);
	}

	DrawControlEx(dc, rcParent, rcUpdate);
}