#include "StdAfx.h"
#include "HideButton.h"

CHideButton::CHideButton(HWND hWnd, CMessageInterface* pMessage, UINT uControlID, CRect rc, CString strTip, CString strTitle, Color clrTip,
						 Color clrTextNormal, Color clrTextHover, Color clrTextDown ,Color clrTextDisable,
						 bool bIsVisible, bool bIsDisable)
						 : CControlBaseFont(hWnd, pMessage, uControlID, rc, strTitle, bIsVisible, bIsDisable)
{
	m_enButtonState = enBSNormal;
	m_clrTextNormal = clrTextNormal;
	m_clrTextHover = clrTextHover;
	m_clrTextDown = clrTextDown;
	m_clrTextDisable = clrTextDisable;
	m_clrTip = clrTip;
	m_bDown = FALSE;
	m_bShowBitton = false;

	m_strTip = strTip;
	SetTitleText(strTitle);
}

CHideButton::~CHideButton(void)
{
}

void CHideButton::SetTitleText(CString strTitle)
{
	m_strTitle = strTitle;
	//CDC dc;
	//dc.Attach(::GetDC(this->m_hWnd));

	//CFont *pOldFont = dc.SelectObject(&m_font);
	//CSize sizeText = dc.GetTextExtent(strTitle);

	//m_rcText = m_rc;
	//m_rcText.right = m_rcText.left + sizeText.cx;
	//m_rcText.bottom = m_rcText.top + sizeText.cy;

	//m_rc.right = m_rc.left + __min(m_rc.Width(), sizeText.cx);
	//m_rc.bottom = m_rc.top + 1 + __min(m_rc.Height(), sizeText.cy);

	//dc.SelectObject(pOldFont);

	//dc.Detach();
}

bool CHideButton::OnControlMouseMove(UINT nFlags, CPoint point)
{
	enumButtonState buttonState = m_enButtonState;
	bool bOldShowBitton = m_bShowBitton;
	if (!m_bIsDisable && m_enButtonState != enBSDown)
	{
		if(m_rc.PtInRect(point))
		{
			m_bShowBitton = true;
			if(m_rcText.PtInRect(point))
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
			m_bShowBitton = false;
			m_enButtonState = enBSNormal;
		}
		
	}

	if(buttonState != m_enButtonState ||  bOldShowBitton != m_bShowBitton)
	{
		if(m_enButtonState == enBSHover)
		{
			m_CursorTpye = IDC_HAND;
		}
		else
		{
			m_CursorTpye = IDC_ARROW;
		}
		UpdateControl(m_bUpdate);
		return true;
	}
	else
	{
		return false;
	}
}

bool CHideButton::OnControlLButtonDown(UINT nFlags, CPoint point)
{
	enumButtonState buttonState = m_enButtonState;
	if (!m_bIsDisable)
	{
		if(m_rcText.PtInRect(point))
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
	else
	{
		return false;
	}
}

bool CHideButton::OnControlLButtonUp(UINT nFlags, CPoint point)
{
	enumButtonState buttonState = m_enButtonState;
	if (!m_bIsDisable)
	{
		if(m_rcText.PtInRect(point))
		{
			if(m_enButtonState == enBSDown)
			{
				PostMessage(BUTTOM_UP, 0, 0);
			}
			m_enButtonState = enBSHover;			
		}
		else
		{
			m_enButtonState = enBSNormal;
			if(!m_rc.PtInRect(point))
			{
				m_bShowBitton = false;
			}
		}
	}
	
	if(buttonState != m_enButtonState)
	{
		UpdateControl(m_bUpdate);
		return true;
	}
	else
	{
		return false;
	}
}

void CHideButton::DrawControl(CDC &dc, CRect rcParent, CRect rcUpdate)
{
	int nWidth = m_rc.Width();
	int nHeight = m_rc.Height();

	if(m_bUpdate)
	{		
		UpdateMemDC(dc, nWidth * 5, nHeight);

		Color clrText[4] = {m_clrTextNormal, m_clrTextHover, m_clrTextDown, m_clrTextDisable};

		Graphics graphics(m_memDC);
		FontFamily fontFamily(m_strFont);
		Font font(&fontFamily, (REAL)m_nFontWidth, m_fontStyle, UnitPixel);
		graphics.SetTextRenderingHint( TextRenderingHintAntiAliasGridFit );

		StringFormat strFormat;
		strFormat.SetAlignment(StringAlignmentNear);
		strFormat.SetFormatFlags( StringFormatFlagsNoWrap | StringFormatFlagsMeasureTrailingSpaces);
		Size sizeTip = GetTextBounds(font, strFormat, m_strTip);
		Size sizeText = GetTextBounds(font, strFormat, m_strTitle);
		CPoint point = GetOriginPoint(nWidth, nHeight, sizeText.Width, sizeText.Height, m_uAlignment, m_uVAlignment);
		
		m_rcText.SetRect(m_rc.left + sizeTip.Width + 10, m_rc.top + point.y, m_rc.left + sizeTip.Width + 10 + sizeText.Width, m_rc.top + point.y + sizeText.Height);
		
		SolidBrush solidBrushTip(m_clrTip);

		for(int i = 0; i < 4; i++)
		{
			RectF rect(i * nWidth, point.y, sizeTip.Width + 10, sizeTip.Height);

			graphics.DrawString(m_strTip, (INT)_tcslen(m_strTip), &font, 
				rect, &strFormat, &solidBrushTip);	

			if(i > 0)
			{
				SolidBrush solidBrush(clrText[i - 1]);	
				RectF rect(sizeTip.Width + 10 + i * nWidth, point.y, nWidth - (sizeTip.Width + 10), sizeText.Height);
				graphics.DrawString(m_strTitle, (INT)_tcslen(m_strTitle), &font, 
					rect, &strFormat, &solidBrush);	
			}
		}
	}

	DrawControlEx(dc, rcParent, rcUpdate, m_bShowBitton ? (1 + m_enButtonState) * nWidth : 0, 0);
}