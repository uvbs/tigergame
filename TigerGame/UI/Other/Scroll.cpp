#include "StdAfx.h"
#include "Scroll.h"
#include "..\Button\ImageButton.h"

#define					BT_UP								1
#define					BT_DOWN								2

CScrollV::CScrollV(HWND hWnd, CMessageInterface* pMessage, UINT uControlID, CRect rc, bool bIsVisible/* = TRUE*/)
			: CControlBase(hWnd, pMessage, uControlID, rc, bIsVisible)
{
	m_nMaxRange = 1;
	m_nCurrentPos = 0;
	m_nPageRange = 1;
	m_nRowRange = 1;
	m_rcBlock.SetRectEmpty();

	m_bButton = false;
	m_bShowScroll = false;
	m_bHover = false;
	m_enButtonState = enBSNormal;
	m_nDownPoint = -1;
	m_nOffsetBlock = -1;
}

CScrollV::~CScrollV(void)
{
	
}

bool CScrollV::SetBitmap(UINT nRangeResourceID, CString strType)
{
	if (m_Image.LoadImage(m_hWnd, nRangeResourceID, strType, 4, 3))
	{
		UpdateControl(true);
		return true;
	}
	return false;
}

bool CScrollV::SetBitmap(CString strRangeImage)
{ 
	if (m_Image.LoadImage(m_hWnd, strRangeImage, 4, 3))
	{
		UpdateControl(true);
		return true;
	}
	return false;
}

bool CScrollV::SetBitmap(UINT nRangeResourceID, UINT nUpResourceID, UINT nDownResourceID, CString strType/* = TEXT("PNG")*/)
{
	if (m_Image.LoadImage(m_hWnd, nRangeResourceID, strType, 4, 3))
	{
		if (!m_bButton)
		{
			m_bButton = true;
			CRect rcButton = m_rc;
			rcButton.bottom = m_rc.top + m_rc.Width();

			CControlBase * pControlBase = NULL;
			pControlBase = new CImageButton(m_hWnd, this, BT_UP, rcButton, true);	
			pControlBase->SetComControl(true);
			m_vecControl.push_back(pControlBase);

			rcButton = m_rc;
			rcButton.top = m_rc.bottom - m_rc.Width();
			pControlBase = new CImageButton(m_hWnd, this, BT_DOWN, rcButton);
			pControlBase->SetComControl(true);
			m_vecControl.push_back(pControlBase);
		}

		CControlBase * pControlBase = NULL;
		pControlBase = GetControl(BT_UP);
		if (pControlBase)
		{
			((CImageButton *)pControlBase)->SetBitmap(nUpResourceID);
		}
		pControlBase = GetControl(BT_DOWN);
		if (pControlBase)
		{
			((CImageButton *)pControlBase)->SetBitmap(nDownResourceID);
		}

		SetRange(true);
	}

	return false;
}
bool CScrollV::SetBitmap(CString strRangeImage, CString strUpImage, CString strDownImage)
{
	if (m_Image.LoadImage(m_hWnd, strRangeImage, 4, 3))
	{
		if (!m_bButton)
		{
			m_bButton = true;
			CRect rcButton = m_rc;
			rcButton.bottom = m_rc.top + m_rc.Width();

			CControlBase * pControlBase = NULL;
			pControlBase = new CImageButton(m_hWnd, this, BT_UP, rcButton, true);
			pControlBase->SetComControl(true);
			m_vecControl.push_back(pControlBase);

			rcButton = m_rc;
			rcButton.top = m_rc.bottom - m_rc.Width();
			pControlBase = new CImageButton(m_hWnd, this, BT_DOWN, rcButton);
			pControlBase->SetComControl(true);
			m_vecControl.push_back(pControlBase);
		}

		CControlBase * pControlBase = NULL;
		pControlBase = GetControl(BT_UP);
		if (pControlBase)
		{
			((CImageButton *)pControlBase)->SetBitmap(strUpImage);
		}
		pControlBase = GetControl(BT_DOWN);
		if (pControlBase)
		{
			((CImageButton *)pControlBase)->SetBitmap(strDownImage);
		}

		SetRange(true);
	}

	return false;
}

void CScrollV::SetScrollCurrentPos(int nCurrentPos)
{
	m_nCurrentPos = nCurrentPos;
	SetRange();
}

void CScrollV::SetScrollMaxRange(int nMaxRange)
{
	m_nMaxRange = nMaxRange;
	SetRange();
}

void CScrollV::SetScrollRowRange(int nRowRange)
{
	m_nRowRange = nRowRange;
	SetRange();
}

void CScrollV::SetScrollInfo(int nMaxRange, int nCurrentPos, int nRowRange)
{
	m_nMaxRange = nMaxRange;
	m_nCurrentPos = nCurrentPos;
	m_nRowRange = nRowRange;
	SetRange(true);
}

bool CScrollV::OnControlMouseMove(UINT nFlags, CPoint point)
{		
	if(!m_bShowScroll)
	{
		return FALSE;
	}

	enumButtonState buttonState = m_enButtonState;
	bool bHover = m_bHover;
	if (!m_bIsDisable)
	{
		if(m_enButtonState != enBSDown)
		{
			if (!m_bLButtonDown)
			{
				if(m_rc.PtInRect(point))
				{
					m_bHover = true;
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
					m_bHover = false;
					m_enButtonState = enBSNormal;
				}
			}
		}
		else
		{		
			int nPos = point.y;
			if (point.y < m_rc.top + m_nOffsetBlock + (m_bButton ? m_rc.Width() : 0))
			{
				nPos = m_rc.top + m_nOffsetBlock + (m_bButton ? m_rc.Width() : 0);
			}
			if (point.y > m_rc.bottom - (m_rcBlock.Height() - m_nOffsetBlock) - (m_bButton ? m_rc.Width() : 0))
			{
				nPos = m_rc.bottom - (m_rcBlock.Height() - m_nOffsetBlock) - (m_bButton ? m_rc.Width() : 0);
			}
			int nMoveHeight = nPos - m_nDownPoint;
			MoveRange(nMoveHeight);
			m_nDownPoint += nMoveHeight;

			return true;
		}
	}

	if(buttonState != m_enButtonState || bHover != m_bHover)
	{
		UpdateControl(m_bUpdate);
		return true;
	}
	return false;
}

bool CScrollV::OnControlLButtonDown(UINT nFlags, CPoint point)
{
	if(!m_bShowScroll)
	{
		return FALSE;
	}

	enumButtonState buttonState = m_enButtonState;
	if (!m_bIsDisable)
	{
		if(m_rcBlock.PtInRect(point))
		{
			m_enButtonState = enBSDown;
			m_nDownPoint = point.y;
			m_nOffsetBlock = point.y - m_rcBlock.top;
		}
		else
		{
			m_nCurrentPos += point.y < m_rcBlock.top ? -m_nPageRange : m_nPageRange;
			return SetRange(false, true);
		}
	}
	
	if(buttonState != m_enButtonState)
	{
		UpdateControl(m_bUpdate);
		return true;
	}
	return false;
}

bool  CScrollV::OnControlLButtonUp(UINT nFlags, CPoint point)
{	
	if(!m_bShowScroll)
	{
		return FALSE;
	}

	enumButtonState buttonState = m_enButtonState;
	if (!m_bIsDisable)
	{		
		m_nDownPoint = -1;
		m_nOffsetBlock = 0;
		if(m_rcBlock.PtInRect(point))
		{
			m_enButtonState = enBSHover;
		}
		else
		{
			m_enButtonState = enBSNormal;
		}
	}
	
	if(buttonState != m_enButtonState)
	{
		UpdateControl(m_bUpdate);
		return true;
	}
	return false;
}

bool CScrollV::OnControlMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	m_nCurrentPos += -zDelta;
	SetRange(false, true);

	return true;
}

void CScrollV::DrawControl(CDC &dc, CRect rcParent, CRect rcUpdate)
{
	int nWidth = m_rc.Width();
	int nHeight = m_rc.Height();

	if(m_bUpdate)
	{		
		UpdateMemDC(dc, nWidth * 4, nHeight);
		
		if(m_bShowScroll)
		{
			CRect  rcTemp;
			//滚动条背景
			int nButtonHeight = m_bButton ? m_rc.Width() : 0;

			rcTemp.SetRect(0, nButtonHeight, nWidth, nHeight - nButtonHeight);
			DrawRange(m_memDC, rcTemp, 0);

			CSize sizeImage = m_Image.GetImageSize();

			//滚动块
			rcTemp.SetRect(m_rcBlock.left - m_rc.left, 0,m_rcBlock.left - m_rc.left +  m_rcBlock.Width(), m_rcBlock.Height());

			rcTemp.OffsetRect(m_rc.Width(), 0);
			DrawRange(m_memDC, rcTemp, sizeImage.cx);

			rcTemp.OffsetRect(m_rc.Width(), 0);
			DrawRange(m_memDC, rcTemp, sizeImage.cx * 2);
			
			rcTemp.OffsetRect(m_rc.Width(), 0);
			DrawRange(m_memDC, rcTemp, sizeImage.cx * 3);
		}		 		
	}

	if (m_bHover)
	{
		DrawControlEx(dc, rcParent, rcUpdate);
	}

	if (m_enButtonState == enBSHover)
	{
		DrawControlEx(dc, rcParent, rcUpdate, m_rcBlock.left - m_rc.left, m_rcBlock.top - m_rc.top, m_rcBlock.Width(), m_rcBlock.Height(), nWidth * 2 + m_rcBlock.left - m_rc.left, 0);
	}
	else if (m_enButtonState == enBSDown)
	{
		DrawControlEx(dc, rcParent, rcUpdate, m_rcBlock.left - m_rc.left, m_rcBlock.top - m_rc.top, m_rcBlock.Width(), m_rcBlock.Height(), nWidth * 3 + m_rcBlock.left - m_rc.left, 0);
	}
	else
	{
		DrawControlEx(dc, rcParent, rcUpdate, m_rcBlock.left - m_rc.left, m_rcBlock.top - m_rc.top, m_rcBlock.Width(), m_rcBlock.Height(), nWidth * 1 + m_rcBlock.left - m_rc.left, 0);
	}
}

void CScrollV::DrawRange(HDC hDC, CRect rc, int nXPos)
{
	m_nImageFrame = 4;
	Graphics graphics(hDC);

	Image *pImage = m_Image.GetImage();
	CSize sizeImage = m_Image.GetImageSize();
	CRect rcTemp;

	if (rc.Height() < m_nImageFrame * 2)
	{
		rcTemp.SetRect(rc.left, rc.top + sizeImage.cy, rc.right, rc.bottom - sizeImage.cy);
		graphics.DrawImage(pImage, Rect(rcTemp.left, rcTemp.top, rcTemp.Width(), rcTemp.Height()),
			nXPos, sizeImage.cy, sizeImage.cx, sizeImage.cy, UnitPixel);
	}
	else
	{
		rcTemp.SetRect(rc.left, rc.top, rc.right, rc.top + m_nImageFrame);
		graphics.DrawImage(pImage, Rect(rcTemp.left, rcTemp.top, rcTemp.Width(), rcTemp.Height()),
			nXPos, 0, sizeImage.cx, m_nImageFrame, UnitPixel);

		if (rc.Height() != m_nImageFrame * 2)
		{
			rcTemp.SetRect(rc.left, rc.top + m_nImageFrame, rc.right, rc.bottom - m_nImageFrame);
			graphics.DrawImage(pImage, Rect(rcTemp.left, rcTemp.top, rcTemp.Width(), rcTemp.Height()),
				nXPos, sizeImage.cy, sizeImage.cx, sizeImage.cy, UnitPixel);
		}

		rcTemp.SetRect(rc.left, rc.bottom - m_nImageFrame, rc.right, rc.bottom);
		graphics.DrawImage(pImage, Rect(rcTemp.left, rcTemp.top, rcTemp.Width(), rcTemp.Height()),
			nXPos, sizeImage.cy * 3 - m_nImageFrame, sizeImage.cx, m_nImageFrame, UnitPixel);
	}
}

int CScrollV::MoveRange(int nMove)
{
	m_rcBlock.OffsetRect(0, nMove);

	int nButtonHeight = m_bButton ? m_rc.Width() : 0;
	if (m_rcBlock.top < m_rc.top + nButtonHeight)
	{
		m_rcBlock.OffsetRect(0, m_rc.top + nButtonHeight - m_rcBlock.top);
	}

	if (m_rcBlock.bottom > m_rc.bottom - nButtonHeight)
	{
		m_rcBlock.OffsetRect(0, m_rc.bottom - nButtonHeight - m_rcBlock.bottom);
	}

	m_nCurrentPos = (m_rcBlock.top - (m_rc.top + nButtonHeight)) * (m_nMaxRange) / (m_rc.Height() - nButtonHeight * 2);

	if (m_nCurrentPos > (m_nMaxRange - m_nPageRange))
	{
		m_nCurrentPos = (m_nMaxRange - m_nPageRange);
	}

	if (m_nCurrentPos < 0)
	{
		m_nCurrentPos = 0;
	}

	UpdateControl(m_bUpdate);
	PostMessage(0, &m_nCurrentPos, sizeof(m_nCurrentPos));

	return m_nCurrentPos;
}

int CScrollV::SetRange(bool bUpdate/* = false*/, bool bSendMessage/* = false*/)
{
	int nButtonHeight = m_bButton ? m_rc.Width() : 0;

	bool bShowButton = m_bButton;
	if (m_rc.Height() - 3 < nButtonHeight * 2)
	{
		nButtonHeight = 0;
		bShowButton = false;
	}

	m_nPageRange = m_rc.Height();

	if (m_nCurrentPos > (m_nMaxRange - m_nPageRange))
	{
		m_nCurrentPos = (m_nMaxRange - m_nPageRange);		
	}
	if (m_nCurrentPos < 0)
	{
		m_nCurrentPos = 0;
	}
	CRect rcOldBlock = m_rcBlock;
	bool bShowScroll = m_bShowScroll;
	if (m_nPageRange >= m_nMaxRange)
	{
		m_bShowScroll = false;
	}
	else
	{
		m_bShowScroll = true;
		int nRangeHeight = m_rc.Height() - nButtonHeight * 2;
		int nBlockHeight = (int)__max(m_rc.Height() > 5 ? 2 : 1, ((double)(m_nPageRange * nRangeHeight)) / (m_nMaxRange));
		int nRangeTop = (int)__min(m_rc.bottom - nButtonHeight - nBlockHeight, __max(m_rc.top + nButtonHeight, m_rc.top + nButtonHeight + m_nCurrentPos  * nRangeHeight / (m_nMaxRange)));


		m_rcBlock.SetRect(m_rc.left, nRangeTop, m_rc.right, nRangeTop + nBlockHeight);
		if (m_rcBlock.top < m_rc.top)
		{
			m_rcBlock.OffsetRect(0, m_rc.top - m_rcBlock.top);
		}

		if (m_rcBlock.bottom > m_rc.bottom)
		{
			m_rcBlock.OffsetRect(0, m_rc.bottom - m_rcBlock.bottom);
		}
	}

	m_nDownPoint += m_rcBlock.top - rcOldBlock.top;

	for (size_t i = 0; i < m_vecControl.size(); i++)
	{
		CControlBase * pControlBase = m_vecControl.at(i);
		if (pControlBase)
		{
			pControlBase->SetVisible(m_bShowScroll && bShowButton);
		}
	}

	UpdateControl(bUpdate || m_bUpdate);

	if (bSendMessage)
	{
		PostMessage(0, &m_nCurrentPos, sizeof(m_nCurrentPos));
	}

	return m_nCurrentPos;
}

void CScrollV::SetCurrentPos(int nCurrentPos)
{
	m_nCurrentPos = nCurrentPos;
	SetRange(false, true);
}

bool CScrollV::SetControlRect(CRect rc)
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
	CRect rcTemp;
	int nButtonHeight = m_bButton ? m_rc.Width() : 0;
	for (size_t i = 0; i < m_vecControl.size(); i++)
	{
		CControlBase * pControlBase = m_vecControl.at(i);
		if (pControlBase)
		{
			UINT uControlID = pControlBase->GetControlID();
			if (BT_UP == uControlID)
			{
				rcTemp = m_rc;
				rcTemp.bottom = m_rc.top + nButtonHeight;
			}
			else if(BT_DOWN == uControlID)
			{
				rcTemp = m_rc;
				rcTemp.top = m_rc.bottom - nButtonHeight;
			}
			else
			{
				continue;
			}
			pControlBase->SetRect(rcTemp);			
		}
	}

	return m_rc.Height() != rcOld.Height() || m_rc.Width() != rcOld.Width();
}

// 消息响应
LRESULT CScrollV::OnControlMessage(UINT uControlID, UINT uMsg, void *pData, size_t lSize)
{
	if(uControlID == BT_UP)
	{
		if(BUTTOM_UP == uMsg)
		{
			m_nCurrentPos += -m_nRowRange;
			SetRange(false, true);
		}
	}
	else if (uControlID == BT_DOWN)
	{
		if(BUTTOM_UP == uMsg)
		{
			m_nCurrentPos += m_nRowRange;
			SetRange(false, true);
		}
	}
	return 0L; 
}