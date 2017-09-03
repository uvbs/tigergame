#include "StdAfx.h"
#include "Slider.h"

CSlider::CSlider(HWND hWnd, CMessageInterface* pMessage, UINT uControlID, CRect rc, CSize sizeBlock, bool bIsVisible/* = TRUE*/)
			: CControlBase(hWnd, pMessage, uControlID, rc, bIsVisible)
{
	m_sizeBlock = sizeBlock;
	m_nMaxRange = 100;
	m_nCurrentPos = 0;
	m_rcBlock.SetRectEmpty();

	m_bHoverControl = false;
	m_enButtonState = enBSNormal;
	m_nDownPoint = -1;
	m_nOffsetBlock = -1;
}

CSlider::~CSlider(void)
{
	
}

bool CSlider::SetBitmap(UINT nRangeResourceID, CString strType)
{
	if (m_ImageBack.LoadImage(m_hWnd, nRangeResourceID, strType, 2, 1))
	{
		UpdateControl(true);
		return true;
	}
	return false;
}

bool CSlider::SetBitmap(CString strRangeImage)
{ 
	if (m_ImageBack.LoadImage(m_hWnd, strRangeImage, 2, 1))
	{
		UpdateControl(true);
		return true;
	}
	return false;
}

bool CSlider::SetBitmap(UINT nRangeResourceID, UINT nBlockResourceID, CString strType/* = TEXT("PNG")*/)
{
	if (m_ImageBack.LoadImage(m_hWnd, nRangeResourceID, strType, 2, 1))
	{
		m_ImageBlock.LoadImage(m_hWnd, nBlockResourceID, strType, 4, 1);
		SetRange(true);
	}

	return false;
}
bool CSlider::SetBitmap(CString strRangeImage, CString strBlockImage)
{
	if (m_ImageBack.LoadImage(m_hWnd, strRangeImage, 2, 1))
	{
		m_ImageBlock.LoadImage(m_hWnd, strRangeImage, 4, 1);
		SetRange(true);
	}

	return false;
}

void CSlider::SetCurrentPos(int nCurrentPos)
{
	m_nCurrentPos = nCurrentPos;
	SetRange();
}

void CSlider::SetSliderMaxRange(int nMaxRange)
{
	m_nMaxRange = nMaxRange;
	SetRange();
}

void CSlider::SetSliderInfo(int nMaxRange, int nCurrentPos)
{
	m_nMaxRange = nMaxRange;
	m_nCurrentPos = nCurrentPos;
	SetRange();
}

bool CSlider::OnControlMouseMove(UINT nFlags, CPoint point)
{		
	enumButtonState buttonState = m_enButtonState;
	bool bHoverControl = m_bHoverControl;
	if (!m_bIsDisable)
	{
		if(m_enButtonState != enBSDown)
		{
			if (!m_bLButtonDown)
			{
				if(m_rc.PtInRect(point))
				{
					m_bHoverControl = true;
					if(m_rcBlock.PtInRect(point))
					{
						m_enButtonState = enBSHover;
					}
					else
					{
						m_enButtonState = enBSNormal;
					}
				}
				else
				{
					m_bHoverControl = false;
					m_enButtonState = enBSNormal;
				}
			}
		}
		else
		{	
			int nPos = point.x;
			if (point.x < m_rc.left + m_nOffsetBlock)
			{
				nPos = m_rc.left + m_nOffsetBlock;
			}
			if (point.x > m_rc.right - (m_rcBlock.Width() - m_nOffsetBlock))
			{
				nPos = m_rc.right - (m_rcBlock.Width() - m_nOffsetBlock);
			}
			
			int nMoveSize = nPos - m_nDownPoint;
			MoveRange(nMoveSize);
			m_nDownPoint += nMoveSize;
			return true;
		}
	}

	if (buttonState != m_enButtonState || bHoverControl != m_bHoverControl)
	{
		UpdateControl(m_bUpdate);
		return true;
	}
	return false;
}

bool CSlider::OnControlLButtonDown(UINT nFlags, CPoint point)
{
	enumButtonState buttonState = m_enButtonState;
	if (!m_bIsDisable)
	{
		if(m_rcBlock.PtInRect(point))
		{
			m_enButtonState = enBSDown;
			m_nDownPoint = point.x;
			m_nOffsetBlock = point.x - m_rcBlock.left;
		}
		else
		{
			int nDrawPos = point.x - m_rc.left;
			m_nCurrentPos = nDrawPos * m_nMaxRange / (m_rc.Width() - m_sizeBlock.cx);
			return SetRange(false, true);
		}
	}
	
	if (buttonState != m_enButtonState)
	{
		UpdateControl(m_bUpdate);
		return true;
	}
	return false;
}

bool  CSlider::OnControlLButtonUp(UINT nFlags, CPoint point)
{	
	enumButtonState buttonState = m_enButtonState;
	bool bHoverControl = m_bHoverControl;
	if (!m_bIsDisable)
	{		
		m_nDownPoint = -1;
		if (m_rc.PtInRect(point))
		{
			if (m_rcBlock.PtInRect(point))
			{
				m_enButtonState = enBSHover;
			}
			else
			{
				m_enButtonState = enBSNormal;
			}
		}
		else
		{
			m_bHoverControl = false;
			m_enButtonState = enBSNormal;
		}
	}
	
	if (buttonState != m_enButtonState || bHoverControl != m_bHoverControl)
	{
		UpdateControl(m_bUpdate);
		return true;
	}
	return false;
}

bool CSlider::OnControlMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	m_nCurrentPos += zDelta < 0 ? 2 : -2;
	SetRange(false, true);

	return true;
}

void CSlider::DrawControl(CDC &dc, CRect rcParent, CRect rcUpdate)
{
	int nWidth = m_rc.Width();
	int nHeight = m_rc.Height();

	if(m_bUpdate)
	{		
		UpdateMemDC(dc, nWidth * 2 + m_sizeBlock.cx * 4, nHeight);

		CRect  rcTemp;
		rcTemp.SetRect(0, 0, nWidth, nHeight);

		CSize sizeImage = m_ImageBack.GetImageSize();
		Image *pImage = m_ImageBack.GetImage();

		Graphics graphics(m_memDC);

		graphics.DrawImage(pImage, Rect(rcTemp.left, rcTemp.top, rcTemp.Width(), rcTemp.Height()),
			0, 0, sizeImage.cx, sizeImage.cy, UnitPixel);
		rcTemp.OffsetRect(nWidth, 0);
		graphics.DrawImage(pImage, Rect(rcTemp.left, rcTemp.top, rcTemp.Width(), rcTemp.Height()),
			sizeImage.cx, 0, sizeImage.cx, sizeImage.cy, UnitPixel);

		sizeImage = m_ImageBlock.GetImageSize();
		pImage = m_ImageBlock.GetImage();
		rcTemp.OffsetRect(nWidth, 0);
		graphics.DrawImage(pImage, Rect(rcTemp.left, rcTemp.top, m_sizeBlock.cx * 4, m_sizeBlock.cy),
				0, 0, sizeImage.cx * 4, sizeImage.cy, UnitPixel);
	}

	int nDrawPos = m_nCurrentPos * (nWidth/* - m_sizeBlock.cx*/) / m_nMaxRange;
	DrawControlEx(dc, rcParent, rcUpdate, 0, 0, nDrawPos, nHeight);
	DrawControlEx(dc, rcParent, rcUpdate, nDrawPos, 0, nWidth - nDrawPos, nHeight, nWidth + nDrawPos, 0);

	if (m_bHoverControl)
	{
		DrawControlEx(dc, rcParent, rcUpdate, m_rcBlock.left - m_rc.left, m_rcBlock.top - m_rc.top, m_sizeBlock.cx, m_sizeBlock.cy, 
			nWidth * 2 + m_enButtonState * m_sizeBlock.cx, 0);
	}
}

int CSlider::MoveRange(int nMove)
{
	m_rcBlock.OffsetRect(nMove, 0);
	if (m_rcBlock.left < m_rc.left)
	{
		m_rcBlock.OffsetRect(m_rc.left - m_rcBlock.left, 0);
	}

	if (m_rcBlock.right > m_rc.right)
	{
		m_rcBlock.OffsetRect(m_rc.right - m_rcBlock.right, 0);
	}
	m_nCurrentPos = (m_rcBlock.left - m_rc.left) * m_nMaxRange / (m_rc.Width() - m_sizeBlock.cx);
	if (m_nCurrentPos >m_nMaxRange)
	{
		m_nCurrentPos = m_nMaxRange;
	}

	if (m_nCurrentPos < 0)
	{
		m_nCurrentPos = 0;
	}
	UpdateControl(m_bUpdate);
	PostMessage(0, &m_nCurrentPos, sizeof(m_nCurrentPos));

	return m_nCurrentPos;
}

int CSlider::SetRange(bool bUpdate/* = false*/, bool bSendMessage/* = false*/)
{	
	int n = m_nCurrentPos * (m_rc.Width() - m_sizeBlock.cx) / m_nMaxRange;
	int nRangeX = m_rc.left +  m_nCurrentPos * (m_rc.Width() - m_sizeBlock.cx) / m_nMaxRange;
	int nRangeY = m_rc.top + (m_rc.Height() - m_sizeBlock.cy) / 2;

	CRect rcOldBlock = m_rcBlock;
	m_rcBlock.SetRect(nRangeX, nRangeY, nRangeX + m_sizeBlock.cx, nRangeY + m_sizeBlock.cy);
	if (m_rcBlock.left < m_rc.left)
	{
		m_rcBlock.OffsetRect(m_rc.left - m_rcBlock.left, 0);
	}

	if (m_rcBlock.right > m_rc.right)
	{
		m_rcBlock.OffsetRect(m_rc.right - m_rcBlock.right, 0);
	}

	if (m_nCurrentPos > (m_nMaxRange))
	{
		m_nCurrentPos = (m_nMaxRange);
	}
	if (m_nCurrentPos < 0)
	{
		m_nCurrentPos = 0;
	}

	m_nDownPoint += m_rcBlock.left - rcOldBlock.left;

	UpdateControl(bUpdate || m_bUpdate);

	if (bSendMessage)
	{
		PostMessage(0, &m_nCurrentPos, sizeof(m_nCurrentPos));
	}

	return m_nCurrentPos;
}

bool CSlider::SetControlRect(CRect rc)
{
	CRect rcOld = m_rc;
	m_rc = rc;
	if (m_rc.Height() != rcOld.Height() || m_rc.Width() != rcOld.Width())
	{
		SetRange(true);
	}
	else
	{
		m_rcBlock.OffsetRect(m_rc.left - rcOld.left, m_rc.top - rcOld.top);
	}

	return m_rc.Height() != rcOld.Height() || m_rc.Width() != rcOld.Width();
}

// ÏûÏ¢ÏìÓ¦
LRESULT CSlider::OnControlMessage(UINT uControlID, UINT uMsg, void *pData, size_t lSize)
{
	return 0L; 
}