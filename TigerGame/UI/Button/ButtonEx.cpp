#include "StdAfx.h"
#include "ButtonEx.h"

CButtonEx::CButtonEx(HWND hWnd, CMessageInterface* pMessage, UINT uControlID, CRect rc, CString strTitle/*= TEXT("")*/, bool bIsVisible/* = TRUE*/, 
						   bool bIsDisable/* = FALSE*/ ,bool bIsPressDown/* = FALSE*/)
						   : CControlBaseFont(hWnd, pMessage, uControlID, rc, strTitle, bIsVisible, bIsDisable)
{
	m_enButtonState = enBSNormal;
	m_uAlignment = DT_CENTER;
	m_uVAlignment = DT_VCENTER;
}

CButtonEx::~CButtonEx(void)
{

}

bool CButtonEx::SetBitmap(UINT nResourceID, CString strType)
{
	if (m_Image.LoadImage(m_hWnd, nResourceID, strType, 4))
	{
		UpdateControl(true);
		return true;
	}
	return false;
}

bool CButtonEx::SetBitmap(CString strImage)
{
	if (m_Image.LoadImage(m_hWnd, strImage, 4))
	{
		UpdateControl(true);
		return true;
	}
	return false;
}

bool CButtonEx::OnControlMouseMove(UINT nFlags, CPoint point)
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
	}

	if(buttonState != m_enButtonState)
	{
		UpdateControl(m_bUpdate);
		return true;
	}
	return false;
}

bool CButtonEx::OnControlLButtonDown(UINT nFlags, CPoint point)
{	
	enumButtonState buttonState = m_enButtonState;
	if (!m_bIsDisable)
	{
		if(m_rc.PtInRect(point))
		{
			m_enButtonState = enBSDown;
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

bool CButtonEx::OnControlLButtonUp(UINT nFlags, CPoint point)
{	
	enumButtonState buttonState = m_enButtonState;
	if (!m_bIsDisable)
	{
		if(m_rc.PtInRect(point))
		{
			m_enButtonState = enBSHover;
			PostMessage(BUTTOM_UP, 0, 0);
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

void  CButtonEx::SetControlDisable(bool bIsDisable)
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
	}
}

void CButtonEx::DrawControl(CDC &dc, CRect rcParent, CRect rcUpdate)
{
	int nWidth = m_rc.Width();
	int nHeight = m_rc.Height();
	
	if(m_bUpdate)
	{		
		UpdateMemDC(dc, nWidth * 4, nHeight);

		Graphics graphics(m_memDC);
		CRect  rcTemp(0, 0, nWidth, nHeight);
		
		SetBkMode(m_memDC, TRANSPARENT);

		Image *pImage = m_Image.GetImage();
		CSize sizeImage = m_Image.GetImageSize();

		for(int i = 0; i < 4; i++)
		{			
			DrawImageFrame(graphics, pImage, rcTemp, i * sizeImage.cx, 0, sizeImage.cx, sizeImage.cy, 4);

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
			CPoint point = GetOriginPoint(nWidth, nHeight, size.Width, size.Height, m_uAlignment, m_uVAlignment);


			for(int i = 0; i < 4; i++)
			{
				SolidBrush solidBrush(enBSDisable == i ? Color(128, 128, 128) : Color(0, 0, 0));

				RectF rect(point.x + i * nWidth + (enBSDown == i ? 1 : 0), point.y + (enBSDown == i ? 1 : 0), nWidth - point.x, size.Height);
				graphics.DrawString(m_strTitle, (INT)_tcslen(m_strTitle), &font, 
					rect, &strFormat, &solidBrush);				
			}
		}
	}

	DrawControlEx(dc, rcParent, rcUpdate, m_enButtonState * nWidth, 0);
}