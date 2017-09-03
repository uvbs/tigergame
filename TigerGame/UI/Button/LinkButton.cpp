#include "StdAfx.h"
#include "LinkButton.h"

CLinkButton::CLinkButton(HWND hWnd, CMessageInterface* pMessage, UINT uControlID, CRect rc, CString strTitle, CString strLink,
	Color clrTextNormal, Color clrTextHover, Color clrTextDown, Color clrTextDisable, bool bIsVisible, bool bIsDisable)
									: CControlBaseFont(hWnd, pMessage, uControlID, rc, strTitle, bIsVisible, bIsDisable)
{
	m_enButtonState = enBSNormal;
	m_clrTextNormal = clrTextNormal;
	m_clrTextHover = clrTextHover;
	m_clrTextDown = clrTextDown;
	m_clrTextDisable = clrTextDisable;
	m_strLink = strLink;
	m_bDown = FALSE;

	SetTitleText(strTitle);
}

CLinkButton::~CLinkButton(void)
{

}

void CLinkButton::SetTitleText(CString strTitle)
{
	if (m_strTitle != strTitle)
	{
		m_strTitle = strTitle;
		UpdateControl(true);
	}
}

bool CLinkButton::OnControlMouseMove(UINT nFlags, CPoint point)
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

bool CLinkButton::OnControlLButtonDown(UINT nFlags, CPoint point)
{
	enumButtonState buttonState = m_enButtonState;
	if (!m_bIsDisable)
	{
		if(m_rc.PtInRect(point))
		{
			m_enButtonState = enBSDown;
		}
	}
	
	if(buttonState != m_enButtonState)
	{
		UpdateControl(m_bUpdate);
		return true;
	}
	return false;
}

bool CLinkButton::OnControlLButtonUp(UINT nFlags, CPoint point)
{
	enumButtonState buttonState = m_enButtonState;
	if (!m_bIsDisable)
	{
		if(m_rc.PtInRect(point))
		{
			if(m_enButtonState == enBSDown)
			{
				ShellExecute(NULL, TEXT("open"), m_strLink, NULL,NULL,SW_SHOWMAXIMIZED); 
			}
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

void CLinkButton::DrawControl(CDC &dc, CRect rcParent, CRect rcUpdate)
{
	int nWidth = m_rc.Width();
	int nHeight = m_rc.Height();

	if(m_bUpdate)
	{		
		UpdateMemDC(dc, nWidth * 4, nHeight);

		if (!m_strTitle.IsEmpty())
		{
			Color clrText[4] = { m_clrTextNormal, m_clrTextHover, m_clrTextDown, m_clrTextDisable };

			Graphics graphics(m_memDC);

			graphics.SetTextRenderingHint(TextRenderingHintAntiAliasGridFit);

			FontFamily fontFamily(m_strFont);
			Font font(&fontFamily, (REAL)m_nFontWidth, m_fontStyle, UnitPixel);

			StringFormat strFormat;
			strFormat.SetAlignment(StringAlignmentNear);
			strFormat.SetTrimming(StringTrimmingEllipsisCharacter);
			strFormat.SetFormatFlags(StringFormatFlagsNoWrap | StringFormatFlagsMeasureTrailingSpaces);

			Rect rectBounds = GetTextBoundsRect(font, strFormat, m_strTitle);
			CPoint point = GetOriginPoint(nWidth, nHeight, rectBounds.Width, rectBounds.Height, m_uAlignment, m_uVAlignment);


			for (int i = 0; i < 4; i++)
			{
				SolidBrush solidBrush(clrText[i]);

				RectF rect(point.x + i * nWidth + (enBSDown == i ? 1 : 0), point.y + (enBSDown == i ? 1 : 0), nWidth - point.x, rectBounds.Height);
				graphics.DrawString(m_strTitle, (INT)_tcslen(m_strTitle), &font,
					rect, &strFormat, &solidBrush);
				if (i == enBSHover || i == enBSDown)
				{
					graphics.FillRectangle(&solidBrush, point.x + i * nWidth + (enBSDown == i ? 1 : 0) + rectBounds.X, point.y + (enBSDown == i ? 1 : 0) + rectBounds.Height - rectBounds.Y, rectBounds.Width + rectBounds.X, 1);
				}
			}
		}

	}

	DrawControlEx(dc, rcParent, rcUpdate, m_enButtonState * nWidth, 0);
}