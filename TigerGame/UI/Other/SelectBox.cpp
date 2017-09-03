#include "StdAfx.h"
#include "SelectBox.h"

CSelectBox::CSelectBox(HWND hWnd, CMessageInterface* pMessage, UINT uControlID, CRect rc, int nXCount, int nYCount, bool bImage,
					   Color clrFrame/* = Color(254, 0, 0, 0)*/, Color clrHover/* = Color(64, 128, 128, 128)*/, Color clrSelect/* = Color(254, 255, 255, 255)*/, bool bIsVisible/* = TRUE*/)
			: CControlBase(hWnd, pMessage, uControlID, rc, bIsVisible)
{
	m_nXCount = nXCount;
	m_nYCount = nYCount;
	m_clrFrame = clrFrame;
	m_clrHover = clrHover;
	m_clrSelect = clrSelect;

	m_nXHover = -1;
	m_nYHover = -1;
	m_nXSelect = -1;
	m_nYSelect = -1;

	m_bImage = bImage;
}

CSelectBox::~CSelectBox(void)
{
	for (size_t i = 0; i < m_vecImage.size(); i++)
	{
		delete m_vecImage[i];
	}
	m_vecImage.clear();
	m_vecclr.clear();
}

bool CSelectBox::SetBitmap(UINT nResourceID, Color clr, int nIndex, CString strType)
{
	if(nIndex >= 0 && nIndex < m_vecImage.size())
	{
		IceImage *pImage = m_vecImage[nIndex];
		m_vecclr[nIndex] = clr;
		if (pImage->LoadImage(m_hWnd, nResourceID, strType))
		{
			UpdateControl(true);
			return true;
		}
	}
	else if(nIndex == -1 && m_vecImage.size() < m_nXCount * m_nYCount)
	{
		IceImage *pImage = new IceImage;
		if (pImage->LoadImage(m_hWnd, nResourceID, strType))
		{
			m_vecImage.push_back(pImage);
			m_vecclr.push_back(clr);

			UpdateControl(true);
			return true;
		}
	}

	return false;
}

bool CSelectBox::SetBitmap(CString strImage, Color clr, int nIndex)
{
	if(nIndex >= 0 && nIndex < m_vecImage.size())
	{
		IceImage *pImage = m_vecImage[nIndex];
		m_vecclr[nIndex] = clr;

		if (pImage->LoadImage(m_hWnd, strImage))
		{
			UpdateControl(true);
			return true;
		}
	}
	else if(nIndex < -1 && m_vecImage.size() < m_nXCount * m_nYCount)
	{
		IceImage *pImage = new IceImage;
		if (pImage->LoadImage(m_hWnd, strImage))
		{
			m_vecImage.push_back(pImage);
			m_vecclr.push_back(clr);

			UpdateControl(true);
			return true;
		}
	}

	return false;
}

bool CSelectBox::SetColor(Color clr, int nIndex/* = -1*/)
{
	if(nIndex >= 0 && nIndex < m_vecclr.size())
	{
		m_vecclr[nIndex] = clr;
		UpdateControl(true);
		return true;
	}
	else if(nIndex < -1 && m_vecclr.size() < m_nXCount * m_nYCount)
	{
		m_vecclr.push_back(clr);
		UpdateControl(true);
		return true;
	}

	return false;
}

bool CSelectBox::SetColor(Color clr[], int nColorCount)
{
	m_vecclr.clear();
	for(int i = 0; i < nColorCount; i++)
	{
		if(i >= m_nXCount * m_nYCount) break;

		m_vecclr.push_back(clr[i]);
	}
	
	UpdateControl(true);
	return true;
}

void CSelectBox::SetControlVisible(bool bIsVisible)
{
	m_bIsVisible = bIsVisible; 
	m_nXHover = -1;
	m_nYHover = -1;
	m_nXSelect = -1;
	m_nYSelect = -1;
}

void CSelectBox::DrawControl(CDC &dc, CRect rcParent, CRect rcUpdate)
{
	int nWidth = m_rc.Width();
	int nHeight = m_rc.Height();

	int nItemWidth = nWidth / m_nXCount;
	int nItemHeight = nHeight / m_nYCount;
	int nXPos = (nWidth - nItemWidth * m_nXCount) / 2;		
	int nYPos = (nHeight - nItemHeight * m_nYCount) / 2;

	if(m_bUpdate)
	{
		UpdateMemDC(dc, nWidth, nHeight * 3);
		
		int nXPosTemp = nXPos;
		int nYPosTemp = nYPos;

		Graphics graphics(m_memDC);
		Pen pen(m_clrFrame, 1);

 		for(int i = 0; i <= m_nYCount; i++)
 		{
 			graphics.DrawLine(&pen, nXPos, nYPosTemp, nXPos + nItemWidth * m_nXCount, nYPosTemp);
 			nYPosTemp += nItemHeight;
 		}
 
 		for(int i = 0; i <= m_nXCount; i++)
 		{
 			graphics.DrawLine(&pen, nXPosTemp, nYPos, nXPosTemp, nYPos + nItemHeight * m_nYCount);
 			nXPosTemp += nItemWidth;
 		}

		if(m_bImage)
		{
			for(size_t i = 0; i < m_vecImage.size(); i++)
			{
				Image *pImage = m_vecImage[i]->GetImage();				
				if (pImage != NULL)
				{
					CSize sizeImage = m_vecImage[i]->GetImageSize();
					graphics.DrawImage(pImage, Rect(nXPos + nItemWidth * (i % m_nXCount) + 1, nYPos + nItemHeight * (i / m_nXCount) + 1, nItemWidth - 1, nItemHeight - 1),
						0, 0, sizeImage.cx, sizeImage.cy, UnitPixel);
				}
			}
		}
		else
		{
			for(size_t i = 0; i < m_vecclr.size(); i++)
			{
				SolidBrush brush(m_vecclr[i]);
				graphics.FillRectangle(&brush, nXPos + nItemWidth * (i % m_nXCount) + 1, nYPos + nItemHeight * (i / m_nXCount) + 1, nItemWidth - 1, nItemHeight - 1);
			}
		}

		//选择
		SolidBrush brush(m_clrHover);
		nYPosTemp = nYPos + nHeight;
		if(m_bImage)
		{
			for(size_t i = 0; i < m_vecImage.size(); i++)
			{			
				graphics.FillRectangle(&brush, nXPos + nItemWidth * (i % m_nXCount) + 1, nYPosTemp + nItemHeight * (i / m_nXCount) + 1, nItemWidth - 1, nItemHeight - 1);
			}
		}
		else
		{
			for(size_t i = 0; i < m_vecclr.size(); i++)
			{			
				graphics.FillRectangle(&brush, nXPos + nItemWidth * (i % m_nXCount) + 1, nYPosTemp + nItemHeight * (i / m_nXCount) + 1, nItemWidth - 1, nItemHeight - 1);
			}
		}

		int nLineWidth = m_bImage ? 2 : 1;
		//选中
		pen.SetColor(m_clrSelect);
		pen.SetWidth(nLineWidth);

		nYPosTemp = nYPos + nHeight * 2;
		for(int i = 0; i < m_nYCount; i++)
		{
			nXPosTemp = nXPos;
			for(int j = 0; j < m_nXCount; j++)
			{				
				Rect rect(nXPosTemp + nLineWidth, nYPosTemp + nLineWidth, nItemWidth - 1 - nLineWidth, nItemHeight - 1 - nLineWidth);

				//绘制矩形
				graphics.DrawRectangles(&pen, &rect, 1);
				nXPosTemp += nItemWidth;
			}
			nYPosTemp += nItemHeight;
		}
	}

	DrawControlEx(dc, rcParent, rcUpdate);

	if(m_nXSelect != -1 && m_nYSelect != -1)
	{
		DrawControlEx(dc, rcParent, rcUpdate, nXPos + m_nXSelect * nItemWidth, nYPos + m_nYSelect * nItemHeight, nItemWidth, nItemHeight, m_nXSelect * nItemWidth, nHeight * 2 + m_nYSelect * nItemHeight);
	}

	if(m_nXHover != -1 && m_nYHover != -1)
	{
		DrawControlEx(dc, rcParent, rcUpdate, nXPos + m_nXHover * nItemWidth, nYPos + m_nYHover * nItemHeight, nItemWidth, nItemHeight, m_nXHover * nItemWidth, nHeight + m_nYHover * nItemHeight);
	}
}

bool CSelectBox::OnControlMouseMove(UINT nFlags, CPoint point)
{
	int nOldXHover = m_nXHover;
	int nOldYHover = m_nYHover;

	if (!m_bIsDisable)
	{
		int nWidth = m_rc.Width();
		int nHeight = m_rc.Height();
		int nItemWidth = nWidth / m_nXCount;
		int nItemHeight = nHeight / m_nYCount;
		int nXPos = (nWidth - nItemWidth * m_nXCount) / 2;		
		int nYPos = (nHeight - nItemHeight * m_nYCount) / 2;
		
		CRect rc = m_rc;
		rc.left += nXPos;
		rc.top += nYPos;
		rc.right = rc.left + nItemWidth * m_nXCount;
		rc.bottom = rc.top + nItemHeight * m_nYCount;

		if(rc.PtInRect(point))
		{
			m_nXHover = (point.x - rc.left) / nItemWidth;
			m_nYHover = (point.y - rc.top) / nItemHeight;

			if(m_nXHover == m_nXSelect && m_nYHover == m_nYSelect)
			{
				m_nXHover = -1;
				m_nYHover = -1;
			}
		}
		else
		{
			m_nXHover = -1;
			m_nYHover = -1;
		}
	}	

	if(nOldXHover != m_nXHover || nOldYHover != m_nYHover)
	{
		UpdateControl(m_bUpdate);
		return true;
	}
	return false;
}

bool CSelectBox::OnControlLButtonDown(UINT nFlags, CPoint point)
{	
	int nOldXHover = m_nXHover;
	int nOldYHover = m_nYHover;
	int nOldXSelect = m_nXSelect;
	int nOldYSelect = m_nYSelect;

	if (!m_bIsDisable)
	{
		if(m_rc.PtInRect(point))
		{
			if(m_nXHover != -1 && m_nYHover != -1)
			{
				m_nXSelect = m_nXHover;
				m_nYSelect = m_nYHover;
				m_nXHover = -1;
				m_nYHover = -1;
			}

			if(nOldXSelect != m_nXSelect || m_nYSelect != nOldYSelect)
			{
				tagSelectBox selectBox;
				if(m_bImage)
				{
					selectBox.nType = 0;
					selectBox.clr = m_vecclr[m_nXCount * m_nYSelect + m_nXSelect];
					selectBox.uIndex = m_nXCount * m_nYSelect + m_nXSelect;
				}
				else
				{
					selectBox.nType = 1;
					selectBox.clr = m_vecclr[m_nXCount * m_nYSelect + m_nXSelect];
				}
				PostMessage(BUTTOM_DOWN, &selectBox, sizeof(selectBox));
			}
		}
	}

	if(nOldXHover != m_nXHover || nOldYHover != m_nYHover || nOldXSelect != m_nXSelect ||  nOldYSelect != m_nYSelect)
	{
		UpdateControl(m_bUpdate);
		return true;
	}
	return false;
}