#include "StdAfx.h"
#include "CheckButton.h"

CCheckButton::CCheckButton(HWND hWnd, CMessageInterface* pMessage, UINT uControlID, CRect rc, CString strTitle/*= TEXT("")*/, bool bIsVisible/* = TRUE*/, 
						   bool bIsDisable/* = FALSE*/ ,bool bIsPressDown/* = FALSE*/)
						   : CControlBaseFont(hWnd, pMessage, uControlID, rc, strTitle, bIsVisible, bIsDisable)
{
	m_enButtonState = enBSNormal;
	m_bDown = false;
	m_bMouseDown = false;
	m_uVAlignment = DT_VCENTER;

}

CCheckButton::~CCheckButton(void)
{

}

bool CCheckButton::SetBitmap(UINT nResourceID, CString strType)
{
	if (m_Image.LoadImage(m_hWnd, nResourceID, strType, 6))
	{
		UpdateControl(true);
		return true;
	}
	return false;
}

bool CCheckButton::SetBitmap(CString strImage)
{
	if (m_Image.LoadImage(m_hWnd, strImage, 6))
	{
		UpdateControl(true);
		return true;
	}
	return false;
}

bool CCheckButton::GetCheck()
{
	return m_bDown;
}

bool CCheckButton::SetCheck(bool bCheck)
{
	if (m_bIsDisable)
	{
		return m_bDown;
	}

	bool bDown = m_bDown;
	m_bDown = bCheck;
	if(m_bDown)
	{
		m_enButtonState = enBSDown;
	}
	else
	{
		 m_enButtonState = enBSNormal;
	}
	if(m_bDown != bDown)
	{
		UpdateControl(m_bUpdate);
	}
	return m_bDown;
}

bool CCheckButton::OnControlMouseMove(UINT nFlags, CPoint point)
{
	enumButtonState buttonState = m_enButtonState;
	if (!m_bIsDisable && !m_bMouseDown)
	{
		if(m_rc.PtInRect(point))
		{
			if(m_bDown)
			{
				m_enButtonState = enBSHoverDown;
			}
			else
			{
				m_enButtonState = enBSHover;
			}
		}
		else
		{
			if(m_bDown)
			{
				m_enButtonState = enBSDown;
			}
			else
			{
				m_enButtonState = enBSNormal;
			}
		}
	}
	
	if(buttonState != m_enButtonState)
	{
		UpdateControl(m_bUpdate);
		return true;
	}
	return false;
}

bool CCheckButton::OnControlLButtonDown(UINT nFlags, CPoint point)
{
	enumButtonState buttonState = m_enButtonState;
	if (!m_bIsDisable)
	{
		if(m_rc.PtInRect(point))
		{
			m_bMouseDown = true;
			if(m_bDown)
			{
				m_enButtonState = enBSHoverDown;
			}
			else
			{
				m_enButtonState = enBSHover;
			}
		}
	}
	
	if(buttonState != m_enButtonState)
	{
		UpdateControl(m_bUpdate);
		return true;
	}
	return false;
}

bool CCheckButton::OnControlLButtonUp(UINT nFlags, CPoint point)
{
	enumButtonState buttonState = m_enButtonState;
	if (!m_bIsDisable)
	{
		if(m_rc.PtInRect(point))
		{
			if(m_bMouseDown)
			{
				m_bDown = !m_bDown;
				PostMessage(BUTTOM_UP, 0, 0);
			}
			if(m_bDown)
			{
				m_enButtonState = enBSHoverDown;
			}
			else
			{
				m_enButtonState = enBSHover;
			}			
		}
		else
		{
			if(m_bDown)
			{
				m_enButtonState = enBSDown;
			}
			else
			{
				m_enButtonState = enBSNormal;
			}			
		}
	}
	m_bMouseDown = false;
	
	if(buttonState != m_enButtonState)
	{
		UpdateControl(m_bUpdate);
		return true;
	}
	return false;
}

void  CCheckButton::SetControlDisable(bool bIsDisable)
{
	if(m_bIsDisable != bIsDisable)
	{
		m_bIsDisable = bIsDisable;
		if(bIsDisable)
		{
			if(m_bDown)
			{
				m_enButtonState = enBSDisableDown;
			}
			else
			{
				m_enButtonState = enBSDisable;
			}
		}
		else
		{
			if(m_bDown)
			{
				m_enButtonState = enBSDown;
			}
			else
			{
				m_enButtonState = enBSNormal;
			}
		}
	}
}

void CCheckButton::DrawControl(CDC &dc, CRect rcParent, CRect rcUpdate)
{
	int nWidth = m_rc.Width();
	int nHeight = m_rc.Height();

	if(m_bUpdate)
	{
		UpdateMemDC(dc, nWidth * 6, nHeight);

		Graphics graphics(m_memDC);
		CRect  rcTemp(0, 0, nWidth, nHeight);
		
		Image *pImage = m_Image.GetImage();
		CSize sizeImage = m_Image.GetImageSize();
		// »­³öcheckboxÍ¼
		for(int i = 0; i < 6; i++)
		{
			graphics.DrawImage(pImage, Rect(rcTemp.left, rcTemp.top + (nHeight - sizeImage.cy) / 2,   sizeImage.cx, sizeImage.cy),
				i * sizeImage.cx, 0, sizeImage.cx, sizeImage.cy, UnitPixel);

			rcTemp.OffsetRect(nWidth, 0);
		}
		// Ð´ÎÄ×Ö
		if(!m_strTitle.IsEmpty())
		{
			rcTemp.SetRect(0, 0, nWidth, nHeight);

			FontFamily fontFamily(m_strFont);
			Font font(&fontFamily, (REAL)m_nFontWidth, m_fontStyle, UnitPixel);
			
			graphics.SetTextRenderingHint( TextRenderingHintAntiAliasGridFit );

			StringFormat strFormat;
			strFormat.SetAlignment(StringAlignmentNear);
			strFormat.SetTrimming(StringTrimmingEllipsisCharacter);
			strFormat.SetFormatFlags( StringFormatFlagsNoWrap | StringFormatFlagsMeasureTrailingSpaces);
			Size size = GetTextBounds(font, strFormat, m_strTitle);
			CPoint point = GetOriginPoint(nWidth - sizeImage.cx - 3, nHeight, size.Width, size.Height, m_uAlignment, m_uVAlignment);

			for(int i = 0; i < 6; i++)
			{
				SolidBrush solidBrush(enBSDisable == i ? Color(128, 128, 128, 128) : Color(254, 0, 0, 0));
				
				RectF rect(sizeImage.cx + 3 + point.x + i * nWidth, point.y + 1, nWidth - sizeImage.cx - 3 - point.x, size.Height);

				graphics.DrawString(m_strTitle, (INT)_tcslen(m_strTitle), &font, 
					rect, &strFormat, &solidBrush);				
			}
		}
		
	}

	DrawControlEx(dc, rcParent, rcUpdate, m_enButtonState * nWidth, 0);
}