#include "StdAfx.h"
#include "StaticText.h"

CStaticText::CStaticText(HWND hWnd, CMessageInterface* pMessage, UINT uControlID, CRect rc, CString m_strTitle, Color clr/* = Color(0, 0, 0)*/,
						 CString strFont/* = TEXT("Tahoma")*/, int nFontWidth/* = 12*/, FontStyle fontStyle/* = FontStyleRegular*/, bool bIsVisible/* = TRUE*/)
			 : CControlBaseFont(hWnd, pMessage, uControlID, rc, m_strTitle, bIsVisible, FALSE, FALSE, strFont, nFontWidth, fontStyle)
{
	m_bBack = false;
	m_clrBack = Color(254, 0, 128, 0);
	m_clrText = clr;
	m_nStart = 0;
	m_clrMark = Color(254, 237, 128, 27);
}

CStaticText::~CStaticText(void)
{
}

void CStaticText::SetMarkText(CString strMark)
{
	if(m_strMark != strMark)
	{
		m_strMark = strMark;
		UpdateControl(true);
	}
}

void CStaticText::SetTitleMarkText(CString strTitle, CString strMark, int nStart)
{
	if(m_strTitle != strTitle || m_strMark != strMark)
	{
		m_strTitle = strTitle;
		m_strMark = strMark;
		UpdateControl(true);
	}
}

void CStaticText::SetMarkText(CString strMark, Color clrMark, int nStart)
{
	if(m_strMark != strMark || *((DWORD *)&m_clrMark) != *((DWORD *)&clrMark) || m_nStart != nStart)
	{
		m_strMark = strMark;
		m_clrMark = clrMark ;
		m_nStart = nStart;
		UpdateControl(true);
	}
}

void CStaticText::SetMarkText(CString strTitle, CString strMark, Color clrMark, int nStart)
{
	if(m_strTitle != strTitle || m_strMark != strMark || *((DWORD *)&m_clrMark) != *((DWORD *)&clrMark) || m_nStart != nStart)
	{
		m_strTitle = strTitle;
		m_strMark = strMark;
		m_clrMark = clrMark ;
		m_nStart = nStart;
		UpdateControl(true);
	}
}

bool CStaticText::SetBitmap(UINT nResourceID, CString strType)
{
	if (m_Image.LoadImage(m_hWnd, nResourceID, strType))
	{
		UpdateControl(true);
		return true;
	}
	return false;
}

bool CStaticText::SetBitmap(CString strImage)
{
	if (m_Image.LoadImage(m_hWnd, strImage))
	{
		UpdateControl(true);
		return true;
	}
	return false;
}

void CStaticText::DrawControl(CDC &dc, CRect rcParent, CRect rcUpdate)
{
	int nWidth = m_rc.Width();
	int nHeight = m_rc.Height();

	if(m_bUpdate)
	{
		UpdateMemDC(dc, nWidth, nHeight);

		Graphics graphics(m_memDC);

		FontFamily fontFamily(m_strFont);
		Font font(&fontFamily, (REAL)m_nFontWidth, m_fontStyle, UnitPixel);

		SolidBrush solidBrush(m_clrText);

		graphics.SetTextRenderingHint(TextRenderingHintAntiAliasGridFit);

		StringFormat strFormat;
		strFormat.SetAlignment(StringAlignmentNear);
		strFormat.SetTrimming(StringTrimmingEllipsisCharacter);
		strFormat.SetFormatFlags(StringFormatFlagsNoWrap | StringFormatFlagsMeasureTrailingSpaces);

		Image *pImage = m_Image.GetImage();
		CSize sizeImage = m_Image.GetImageSize();

		int nXPos = 0;
		if (pImage != NULL)
		{
			graphics.DrawImage(pImage, Rect(0, (nHeight - sizeImage.cy) / 2, sizeImage.cx, sizeImage.cy),
				0, 0, sizeImage.cx, sizeImage.cy, UnitPixel);
			nXPos += sizeImage.cx + 5;
		}

		Size size = GetTextBounds(font, strFormat, m_strTitle);
		CPoint point = GetOriginPoint(nWidth - nXPos, nHeight, size.Width, size.Height, m_uAlignment, m_uVAlignment);

		if (m_strMark.IsEmpty())
		{
			RectF rect(nXPos + point.x, point.y, nWidth - nXPos - point.x, size.Height);

			graphics.DrawString(m_strTitle, (INT)_tcslen(m_strTitle), &font,
				rect, &strFormat, &solidBrush);
		}
		else
		{
			int nStart = m_strTitle.Find(m_strMark, m_nStart);
			if (nStart >= 0)
			{
				SolidBrush solidBrushM(m_clrMark);

				CString srtL = m_strTitle.Left(nStart);
				CString srtR = m_strTitle.Right(m_strTitle.GetLength() - m_strMark.GetLength() - nStart);
				Size sizeL = GetTextBounds(font, strFormat, srtL);
				Size sizeM = GetTextBounds(font, strFormat, m_strMark);
				Size sizeR = GetTextBounds(font, strFormat, srtR);

				graphics.DrawString(srtL, (INT)_tcslen(srtL), &font,
					PointF(nXPos + point.x, point.y), &strFormat, &solidBrush);
				graphics.DrawString(m_strMark, (INT)_tcslen(m_strMark), &font,
					PointF(nXPos + point.x + sizeL.Width + 2, point.y), &strFormat, &solidBrushM);

				RectF rect(nXPos + point.x + sizeL.Width + sizeM.Width + 4, point.y, nWidth - (nXPos + sizeL.Width + sizeM.Width + 4 + point.x), size.Height);
				


				graphics.DrawString(srtR, (INT)_tcslen(srtR), &font,
					PointF(nXPos + point.x + sizeL.Width + sizeM.Width + 4, point.y), &strFormat, &solidBrush);
			}
			else
			{
				RectF rect(nXPos + point.x, point.y, nWidth - nXPos, size.Height);


				graphics.DrawString(m_strTitle, (INT)_tcslen(m_strTitle), &font,
					rect, &strFormat, &solidBrush);
			}
		}
	}

	DrawControlEx(dc, rcParent, rcUpdate);
}