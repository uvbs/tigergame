#include "StdAfx.h"
#include "MenuItem.h"

CMenuItem::CMenuItem(HWND hWnd, CMessageInterface* pMessage, UINT uControlID, CRect rc, CString strTitle/*= TEXT("")*/, int nLeft/* = 30*/, bool bSelect/* = false*/,
					 bool bIsVisible/* = TRUE*/,  bool bIsDisable/* = FALSE*/ ,bool bIsPressDown/* = FALSE*/)
						   : CControlBaseFont(hWnd, pMessage, uControlID, rc, strTitle, bIsVisible, bIsDisable)
{
	m_enButtonState = enBSNormal;
	m_bDown = false;
	m_bMouseDown = false;
	m_uVAlignment = DT_VCENTER;
	m_bSelect = bSelect;

	m_nLeft = nLeft;
}

CMenuItem::~CMenuItem(void)
{
}

bool CMenuItem::SetBitmap(UINT nResourceID, CString strType)
{
	if (m_Image.LoadImage(m_hWnd, nResourceID, strType, (m_bSelect ? 6 : 4)))
	{
		UpdateControl(true);
		return true;
	}
	return false;
}

bool CMenuItem::SetBitmap(CString strImage)
{
	if (m_Image.LoadImage(m_hWnd, strImage, (m_bSelect ? 6 : 4)))
	{
		UpdateControl(true);
		return true;
	}
	return false;
}

bool CMenuItem::GetCheck()
{
	return m_bDown;
}

bool CMenuItem::SetCheck(bool bCheck)
{
	if(!m_bSelect) return false;

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

bool CMenuItem::OnControlMouseMove(UINT nFlags, CPoint point)
{
	enumButtonState buttonState = m_enButtonState;
	if (!m_bIsDisable && !m_bMouseDown)
	{
		if(m_rc.PtInRect(point))
		{
			if(m_bSelect)
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
				m_enButtonState = enBSHover;
			}
		}
		else
		{
			if(m_bSelect)
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

bool CMenuItem::OnControlLButtonDown(UINT nFlags, CPoint point)
{
	enumButtonState buttonState = m_enButtonState;
	if (!m_bIsDisable)
	{
		if(m_rc.PtInRect(point))
		{
			m_bMouseDown = true;
			if(m_bSelect)
			{
				bool bDown = m_bDown;
				if(m_bDown)
				{
					m_enButtonState = enBSHoverDown;					
				}
				else
				{
					m_enButtonState = enBSHover;
				}	

				tagMenuItemInfo menuItemInfo;
				menuItemInfo.bDown = bDown;
				menuItemInfo.bSelect = false;
				PostMessage(BUTTOM_DOWN, &menuItemInfo, sizeof(menuItemInfo));
			}
			else
			{
				m_enButtonState = enBSDown;

				tagMenuItemInfo menuItemInfo;
				menuItemInfo.bDown = false;
				menuItemInfo.bSelect = false;
				PostMessage(BUTTOM_DOWN, &menuItemInfo, sizeof(menuItemInfo));
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

bool CMenuItem::OnControlLButtonUp(UINT nFlags, CPoint point)
{
	bool bSend = false;
	bool bbDown = false;
	bool bSelect = false;
	enumButtonState buttonState = m_enButtonState;
	if (!m_bIsDisable)
	{
		if(m_rc.PtInRect(point))
		{
			if(m_bSelect)
			{
				if(m_bDown)
				{
					m_enButtonState = enBSHoverDown;
				}
				else
				{
					m_enButtonState = enBSHover;
				}
				if(m_bMouseDown)
				{
					m_bDown = !m_bDown;
					bSend = false;
					bbDown = m_bDown;
					bSelect = true;
				}				
			}
			else
			{
				m_enButtonState = enBSHover;
				bSend = true;
				
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

		if(bSend)
		{
			tagMenuItemInfo menuItemInfo;
			menuItemInfo.bDown = bbDown;
			menuItemInfo.bSelect = bSelect;
			PostMessage(BUTTOM_UP, &menuItemInfo, sizeof(menuItemInfo));
		}
		return true;
	}
	return false;
}

void  CMenuItem::SetControlDisable(bool bIsDisable)
{
	if(m_bIsDisable != bIsDisable)
	{
		m_bIsDisable = bIsDisable;
		if(bIsDisable)
		{
			if(m_bDown && m_bSelect)
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
		::InvalidateRect(m_hWnd, &m_rc, true);
	}
}

void CMenuItem::DrawControl(CDC &dc, CRect rcParent, CRect rcUpdate)
{
	int nWidth = m_rc.Width();
	int nHeight = m_rc.Height();

	if(m_bUpdate)
	{
		int nImageCount = m_bSelect ? 6 : 4;
		UpdateMemDC(dc, nWidth * nImageCount, nHeight);

		Graphics graphics(m_memDC);
		Image *pImage = m_Image.GetImage();
		CSize sizeImage = m_Image.GetImageSize();

		CRect  rcTemp(0, 0, nWidth, nHeight);

		for(int i = 0; i < nImageCount; i++)
		{
			if(enBSHover == i || (enBSDown == i && !m_bSelect) || enBSHoverDown == i)
			{
				SolidBrush brush(Color(254, 71, 156, 235));
				graphics.FillRectangle(&brush, i * nWidth, 0, nWidth, nHeight);
			}

			if(pImage != NULL)
			{
				graphics.DrawImage(pImage, Rect(rcTemp.left + (m_nLeft - sizeImage.cx) / 2, rcTemp.top + (nHeight - sizeImage.cy) / 2,   sizeImage.cx, sizeImage.cy),
					i * sizeImage.cx, 0, sizeImage.cx, sizeImage.cy, UnitPixel);
			}

			rcTemp.OffsetRect(nWidth, 0);
		}
		
		if(!m_strTitle.IsEmpty())
		{
			rcTemp.SetRect(0, 0, nWidth, nHeight);

			FontFamily fontFamily(m_strFont);
			Font font(&fontFamily, (REAL)m_nFontWidth, m_fontStyle, UnitPixel);
			graphics.SetTextRenderingHint( TextRenderingHintAntiAliasGridFit );

			StringFormat strFormat;
			strFormat.SetAlignment(StringAlignmentNear);
			strFormat.SetFormatFlags( StringFormatFlagsNoWrap | StringFormatFlagsMeasureTrailingSpaces);
			Size size = GetTextBounds(font, strFormat, m_strTitle);
			CPoint point = GetOriginPoint(nWidth - m_nLeft, nHeight, size.Width, size.Height, m_uAlignment, m_uVAlignment);


			for(int i = 0; i < nImageCount; i++)
			{
				SolidBrush solidBrush(enBSDisable == i ? Color(254, 128, 128, 128) : (enBSHover == i || (enBSDown == i && !m_bSelect) || enBSHoverDown == i ? Color(254, 255, 255, 255) : Color(254, 56, 56, 56)));

				RectF rect(m_nLeft + point.x + i * nWidth, point.y, nWidth - (m_nLeft + point.x), size.Height);
				graphics.DrawString(m_strTitle, (INT)_tcslen(m_strTitle), &font, 
					rect, &strFormat, &solidBrush);				
			}
		}
	}

	DrawControlEx(dc, rcParent, rcUpdate, m_enButtonState * nWidth, 0);
}