#include "StdAfx.h"
#include "DlgBubble.h"


CDlgBubble::CDlgBubble(void)
{
	m_nArrowHeigth = 5;
	m_nArrowPos = 10;
	m_clrFrame = Color(255, 0, 0, 0);
	m_clrFill = Color(255, 255, 255, 255);
}

CDlgBubble::~CDlgBubble(void)
{
}

void CDlgBubble::SetArrowInfo(int nArrowPos, int nArrowHeigth)
{
	m_nArrowHeigth = nArrowHeigth;
	m_nArrowPos = nArrowPos;
}

void CDlgBubble::SetColor(Color clrFrame, Color clrFill)
{
	m_clrFrame = clrFrame;
	m_clrFill = clrFill;
}

void CDlgBubble::DrawWindowEx(CDC &dc, CRect rcClient)
{
	CRect rc(0, 0, m_rc.Width(), m_rc.Height());
	Graphics graphics(dc);

	CRgn rgnTemp;
	CRgn rgnNew;
	CPoint point[3];

	point[0].SetPoint(rc.left + m_nArrowPos, rc.top);
	point[1].SetPoint(rc.left + m_nArrowPos + m_nArrowHeigth - 1, rc.top + m_nArrowHeigth);
	point[2].SetPoint(rc.left + m_nArrowPos - m_nArrowHeigth + 1, rc.top + m_nArrowHeigth);

	rgnNew.CreatePolygonRgn(point, 3, 2);
	rgnTemp.CreateRoundRectRgn(rc.left, rc.top + m_nArrowHeigth, rc.right + 1, rc.bottom + 1, 3, 3);
	rgnNew.CombineRgn(&rgnTemp, &rgnNew, RGN_OR);

	Region gern(rgnNew);
	SolidBrush brush(m_clrFrame);
	graphics.FillRegion(&brush, &gern);

	rgnNew.DeleteObject();
	rgnTemp.DeleteObject();

	point[0].SetPoint(rc.left + m_nArrowPos, rc.top + 2);
	point[1].SetPoint(rc.left + m_nArrowPos + m_nArrowHeigth - 1, rc.top + m_nArrowHeigth + 2);
	point[2].SetPoint(rc.left + m_nArrowPos - m_nArrowHeigth + 1, rc.top + m_nArrowHeigth + 2);

	rgnNew.CreatePolygonRgn(point, 3, 2);
	rgnTemp.CreateRoundRectRgn(rc.left + 1, rc.top + m_nArrowHeigth + 1, rc.right, rc.bottom, 3, 3);
	rgnNew.CombineRgn(&rgnTemp, &rgnNew, RGN_OR);

	Region gernNew(rgnNew);
	SolidBrush brushNew(m_clrFill);
	graphics.FillRegion(&brushNew, &gernNew);

	rgnNew.DeleteObject();
	rgnTemp.DeleteObject();
}

void CDlgBubble::InitUI(CRect rcClient)
{

}

// 消息响应
LRESULT CDlgBubble::OnControlMessage(UINT uControlID, UINT uMsg, void *pData, size_t lSize)
{
	return 0;
}

// 消息响应
LRESULT CDlgBubble::OnWindowMessage(UINT uControlID, UINT uMsg, void *pData, size_t lSize)
{
	return 0;
}