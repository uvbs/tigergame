#include "StdAfx.h"
#include "ImageButton.h"

CImageButton::CImageButton(HWND hWnd, CMessageInterface* pMessage, UINT uControlID, CRect rc, bool bAnimation/* = true*/, CString strTitle/*= TEXT("")*/, bool bIsVisible/* = TRUE*/, 
						   bool bIsDisable/* = FALSE*/ ,bool bIsPressDown/* = FALSE*/)
	: CControlBaseFont(hWnd, pMessage, uControlID, rc, strTitle, bIsVisible, bIsDisable)
{
	m_enButtonState = enBSNormal;

	m_bTimer = bAnimation;
	m_nIndex = 0;
	m_nMaxIndex = 8;
}

CImageButton::~CImageButton(void)
{
}

bool CImageButton::SetBitmap(UINT nResourceID, CString strType)
{
	if (m_Image.LoadImage(m_hWnd, nResourceID, strType, 4))
	{
		UpdateControl(true);
		return true;
	}
	return false;
}

bool CImageButton::SetBitmap(CString strImage)
{
	if (m_Image.LoadImage(m_hWnd, strImage, 4))
	{
		UpdateControl(true);
		return true;
	}
	return false;
}

bool CImageButton::OnControlMouseMove(UINT nFlags, CPoint point)
{
	enumButtonState buttonState = m_enButtonState;
	if (!m_bIsDisable && m_enButtonState != enBSDown)
	{
		if(m_rc.PtInRect(point))
		{
			m_enButtonState = enBSHover;
		}
		else
		{
			m_enButtonState = enBSNormal;
		}
		if (buttonState != m_enButtonState)
		{
			if(m_bTimer && !m_bIsDisable && m_bIsVisible && (0 == m_nIndex || m_nMaxIndex == m_nIndex) && (enBSNormal == buttonState || enBSHover == buttonState))
			{
				if(0 == m_nIndex)
				{
					m_nIndex = m_nMaxIndex / 2;
				}
				//Æô¶¯¹ý¶É			
				m_bRunTime = true;
			}

			UpdateControl(m_bUpdate);
			return true;
		}
	}

	if(buttonState != m_enButtonState)
	{
		UpdateControl(m_bUpdate);
		return true;
	}
	return false;
}

bool CImageButton::OnControlLButtonDown(UINT nFlags, CPoint point)
{	
	enumButtonState buttonState = m_enButtonState;
	if (!m_bIsDisable)
	{
		if(m_rc.PtInRect(point))
		{
			m_enButtonState = enBSDown;
			if(m_bTimer)
			{
				m_bRunTime = false;
				m_nIndex = 8;
			}
			PostMessage(BUTTOM_DOWN, 0, 0);
		}
	}
	if(buttonState != m_enButtonState)
	{
		UpdateControl(m_bUpdate);
		return true;
	}

	return false;
}

bool CImageButton::OnControlLButtonUp(UINT nFlags, CPoint point)
{
	bool bSend = false;
	enumButtonState buttonState = m_enButtonState;
	if (!m_bIsDisable)
	{
		if(m_rc.PtInRect(point))
		{
			if(m_enButtonState == enBSDown)
			{
				bSend = true;
			}
			m_enButtonState = enBSHover;
			
		}
		else
		{
			m_enButtonState = enBSNormal;
			if(m_bTimer)
			{
				m_bRunTime = true;
				m_nIndex = 8;
			}
		}
	}

	if(buttonState != m_enButtonState)
	{
		UpdateControl(m_bUpdate);

		if(bSend)
		{
			PostMessage(BUTTOM_UP, 0, 0);
		}
		return true;
	}
	return false;
}

void  CImageButton::SetControlDisable(bool bIsDisable)
{
	if(m_bIsDisable != bIsDisable)
	{
		m_bIsDisable = bIsDisable;
		if(bIsDisable)
		{
			m_enButtonState = enBSDisable;
		}
		else
		{
			m_enButtonState = enBSNormal;
		}
		
		UpdateControl(true);
	}
}

bool CImageButton::OnControlTimer()
{
	if(!m_bRunTime)
	{
		return FALSE;
	}

	if(enBSNormal == m_enButtonState)
	{
		m_nIndex--;
		if(m_nIndex < 0)
		{
			m_nIndex = 0;
		}
	}
	else if(enBSHover == m_enButtonState)
	{
		m_nIndex++;
		if(m_nIndex > m_nMaxIndex)
		{
			m_nIndex = m_nMaxIndex;
		}
	}
	if(0 == m_nIndex || m_nIndex == m_nMaxIndex)
	{
		m_bRunTime = false;
	}

	UpdateControl(m_bUpdate);

	return true;
}

void CImageButton::DrawControl(CDC &dc, CRect rcParent, CRect rcUpdate)
{
	int nWidth = m_rc.Width();
	int nHeight = m_rc.Height();

	if(m_bUpdate)
	{
		UpdateMemDC(dc, nWidth * (4 + m_nMaxIndex), nHeight);
		
		ColorMatrix matrix;
		ImageAttributes imageAttr;

		ZeroMemory(&matrix, sizeof(matrix));
		for(int i = 0; i < 5; i++)
		{
			matrix.m[i][i] = 1.0f;
		}		

		Graphics graphics(m_memDC);
		CRect  rcTemp(0, 0, nWidth, nHeight);

		Image *pImage = m_Image.GetImage();
		CSize sizeImage = m_Image.GetImageSize();

		for(int i = 0; i < 4; i++)
		{
			graphics.DrawImage(pImage, Rect(rcTemp.left, rcTemp.top, rcTemp.Width(), rcTemp.Height()),
				i * sizeImage.cx, 0, sizeImage.cx, sizeImage.cy, UnitPixel);

			rcTemp.OffsetRect(nWidth, 0);
		}

		for (int i = 4; i < 4 + m_nMaxIndex; i++)
		{
			matrix.m[3][3] = 1 - ((float)(i - 4 + 1)) / m_nMaxIndex;
			imageAttr.SetColorMatrix(&matrix);
			graphics.DrawImage(pImage, Rect(rcTemp.left, rcTemp.top, rcTemp.Width(), rcTemp.Height()),
				sizeImage.cx * enBSNormal, 0, sizeImage.cx, sizeImage.cy, UnitPixel ,&imageAttr);

			matrix.m[3][3] = ((float)(i - 4 + 1)) / m_nMaxIndex;
			imageAttr.SetColorMatrix(&matrix);
			graphics.DrawImage(pImage, Rect(rcTemp.left, rcTemp.top, rcTemp.Width(), rcTemp.Height()),
				sizeImage.cx * enBSHover, 0, sizeImage.cx, sizeImage.cy, UnitPixel ,&imageAttr);

			rcTemp.OffsetRect(nWidth, 0);
		}
	}

	if(m_nIndex != 0 && m_bRunTime && m_bTimer)
	{
		DrawControlEx(dc, rcParent, rcUpdate, (4 + m_nIndex - 1) * nWidth, 0);
	}
	else
	{
		DrawControlEx(dc, rcParent, rcUpdate, m_enButtonState * nWidth, 0);
	}
}