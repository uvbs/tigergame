#include "StdAfx.h"
#include "Picture.h"

CPicture::CPicture(HWND hWnd, CMessageInterface* pMessage, UINT uControlID, CRect rc,
	UINT uVerticalAlignment/* = DT_CENTER*/, UINT uHorizontalAlignment/* = DT_VCENTER*/, bool bIsVisible/* = TRUE*/)
			: CControlBase(hWnd, pMessage, uControlID, rc, bIsVisible, FALSE, FALSE)
{
	m_enShowMode = enSMNormal;

	m_uVerticalAlignment = uVerticalAlignment;
	m_uHorizontalAlignment = uHorizontalAlignment;

}

CPicture::~CPicture(void)
{
}

bool CPicture::SetBitmap(UINT nResourceID, CString strType)
{
	if (m_Image.LoadImage(m_hWnd, nResourceID, strType))
	{
		UpdateControl(true);
		return true;
	}
	return false;
}

bool CPicture::SetBitmap(Image *pImage)
{
	if (m_Image.LoadImage(m_hWnd, pImage))
	{
		UpdateControl(true);
		return true;
	}
	return false;
}

bool CPicture::SetBitmap(CString strImage)
{ 
	if (m_Image.LoadImage(m_hWnd, strImage))
	{
		UpdateControl(true);
		return true;
	}
	return false;
}

bool CPicture::SetShowMode(enumShowMode enShowMode, int nFrameSize)
{
	if(enShowMode != m_enShowMode || m_enShowMode == enSMFrame && nFrameSize != m_nFrameSize)
	{
		m_nFrameSize = nFrameSize;
		m_enShowMode = enShowMode;
		UpdateControl(true);
		return true;
	}
	return false;
}

void CPicture::DrawControl(CDC &dc, CRect rcParent, CRect rcUpdate)
{
	int nWidth = m_rc.Width();
	int nHeight = m_rc.Height();

	if(m_bUpdate)
	{
		UpdateMemDC(dc, nWidth, nHeight);

		Image *pImage = m_Image.GetImage();
		CSize sizeImage = m_Image.GetImageSize();

		Graphics graphics(m_memDC);
		if(m_enShowMode == enSMNormal)
		{
			Rect rect;
			if(m_uHorizontalAlignment == DT_LEFT)
			{
				rect.X = 0;
			}
			else if(m_uHorizontalAlignment == DT_RIGHT)
			{
				rect.X = nWidth - sizeImage.cx;
			}
			else
			{
				rect.X = (nWidth - sizeImage.cx) / 2;
			}

			if(m_uVerticalAlignment == DT_TOP)
			{
				rect.Y = 0;
			}
			else if(m_uVerticalAlignment == DT_BOTTOM)
			{
				rect.Y = (nHeight - sizeImage.cy);
			}
			else
			{
				rect.Y = (nHeight - sizeImage.cy) / 2;
			}

			rect.Width = sizeImage.cx;
			rect.Height = sizeImage.cy;

			graphics.DrawImage(pImage, rect, 0, 0, sizeImage.cx, sizeImage.cy, UnitPixel);
		}
		else if(m_enShowMode == enSMFrame)
		{
			CRect  rcTemp(0, 0, nWidth, nHeight);
			DrawImageFrame(graphics, pImage, rcTemp, 0, 0, sizeImage.cx, sizeImage.cy, m_nFrameSize);
		}
		else if (m_enShowMode == enSMExtrude)
		{
			Rect  rect(0, 0, nWidth, nHeight);
			graphics.DrawImage(pImage, rect, 0, 0, sizeImage.cx, sizeImage.cy, UnitPixel);
		}
	}

	DrawControlEx(dc, rcParent, rcUpdate);
}