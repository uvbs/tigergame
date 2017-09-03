#include "StdAfx.h"
#include "TextButton.h"

CTextButton::CTextButton(HWND hWnd, CMessageInterface* pMessage, UINT uControlID, CRect rc, CString strTitle,
						 Color clrTextNormal ,Color clrTextHover, Color clrTextDown ,Color clrTextDisable,
						 bool bIsVisible, bool bIsDisable)
						 : CControlBaseFont(hWnd, pMessage, uControlID, rc, strTitle, bIsVisible, bIsDisable)
{
	m_enButtonState = enBSNormal;
	m_clrTextNormal = clrTextNormal;
	m_clrTextHover = clrTextHover;
	m_clrTextDown = clrTextDown;
	m_clrTextDisable = clrTextDisable;
	m_bDown = FALSE;
	m_bIsBkg = false;
	m_clrBkgNormal = NULL;
	m_clrBkgHover = NULL;
	m_clrBkgDown = NULL;
	m_clrBkgDisable = NULL;
	m_bAutoWidth = false;
	
	SetTitleText(strTitle);
}

CTextButton::~CTextButton(void)
{

}

void CTextButton::SetTitleText(CString strTitle)
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

bool CTextButton::OnControlMouseMove(UINT nFlags, CPoint point)
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

bool CTextButton::OnControlLButtonDown(UINT nFlags, CPoint point)
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

bool CTextButton::OnControlLButtonUp(UINT nFlags, CPoint point)
{
	enumButtonState buttonState = m_enButtonState;
	if (!m_bIsDisable)
	{
		if(m_rc.PtInRect(point))
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
		}
	}
	
	if(buttonState != m_enButtonState)
	{
		UpdateControl(m_bUpdate);
		return true;
	}
	return false;
}

void CTextButton::DrawControl(CDC &dc, CRect rcParent, CRect rcUpdate)
{
	int nWidth = m_rc.Width();
	int nHeight = m_rc.Height();

	if(m_bUpdate)
	{		
		UpdateMemDC(dc, nWidth * 4, nHeight);
		
		Color clrText[4] = {m_clrTextNormal, m_clrTextHover, m_clrTextDown, m_clrTextDisable};

		Graphics graphics(m_memDC);
		FontFamily fontFamily(m_strFont);
		Font font(&fontFamily, (REAL)m_nFontWidth, m_fontStyle, UnitPixel);
		graphics.SetTextRenderingHint( TextRenderingHintAntiAliasGridFit );

		StringFormat strFormat;
		strFormat.SetAlignment(StringAlignmentNear);
		strFormat.SetFormatFlags( StringFormatFlagsNoWrap | StringFormatFlagsMeasureTrailingSpaces);
		Size size = GetTextBounds(font, strFormat, m_strTitle);
		CPoint point = GetOriginPoint(nWidth, nHeight, size.Width, size.Height, m_uAlignment, m_uVAlignment);

		// 新增绘制背景色
		//SolidBrush brush1(Color(128, 0, 0, 0));
		//graphics.FillRectangle(&brush1, point.x, point.y, nWidth - point.x, size.Height);

		Color clrBkg[4] = { m_clrBkgNormal, m_clrBkgHover, m_clrBkgDown, m_clrBkgDisable };

		for(int i = 0; i < 4; i++)
		{
			// 绘制背景
			if (m_bIsBkg)
			{
				SolidBrush solidBrush2(clrBkg[i]);
				graphics.FillRectangle(&solidBrush2, 0 + i * nWidth, 0, nWidth, nHeight);
			}

			SolidBrush solidBrush(clrText[i]);
			RectF rect(point.x + i * nWidth , point.y , nWidth - point.x, size.Height);
			graphics.DrawString(m_strTitle, (INT)_tcslen(m_strTitle), &font, 
				rect, &strFormat, &solidBrush);
		}
	}
	DrawControlEx(dc, rcParent, rcUpdate, m_enButtonState * nWidth, 0);
}

void CTextButton::SetControlBkg(Color clrBkgNormal /*= Color(128, 221, 221, 221)*/, Color clrBkgHover /*= Color(254, 146, 146, 146)*/,
	Color clrBkgDown /*= Color(128, 42, 184, 246)*/, Color clrBkgDisable /*= Color(128, 0, 0, 0)*/)
{
	m_clrBkgNormal = clrBkgNormal;
	m_clrBkgHover = clrBkgHover;
	m_clrBkgDown = clrBkgDown;
	m_clrBkgDisable = clrBkgDisable;
	m_bIsBkg = true;
}