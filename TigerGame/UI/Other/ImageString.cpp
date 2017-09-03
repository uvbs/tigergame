#include "StdAfx.h"
#include "ImageString.h"

CImageString::CImageString(HWND hWnd, CMessageInterface* pMessage, UINT uControlID, CRect rc, int nNumber, 
						   UINT uAlignment/* = DT_CENTER*/, UINT uVAlignment/* = DT_VCENTER*/, bool bIsVisible/* = TRUE*/, bool bIsDisable/* = FALSE*/)
: CControlBaseFont(hWnd, pMessage, uControlID, rc, TEXT(""), bIsVisible, bIsDisable, false)
{
	m_uAlignment = uAlignment;
	m_uVAlignment = uVAlignment;

	SetNumnerTitle(nNumber);
}

CImageString::CImageString(HWND hWnd, CMessageInterface* pMessage, UINT uControlID, CRect rc, CString strTitle, 
						   UINT uAlignment/* = DT_CENTER*/, UINT uVAlignment/* = DT_VCENTER*/, bool bIsVisible/* = TRUE*/, bool bIsDisable/* = FALSE*/)
: CControlBaseFont(hWnd, pMessage, uControlID, rc, strTitle, bIsVisible, bIsDisable, false)
{
	m_uAlignment = uAlignment;
	m_uVAlignment = uVAlignment;
}

CImageString::~CImageString(void)
{
	
}

bool CImageString::SetBitmap(UINT nResourceID, CString strMask, CString strType)
{
	if(!strMask.IsEmpty())
	{
		m_strMask = strMask;
		if (m_Image.LoadImage(m_hWnd, nResourceID, strType, m_strMask.GetLength()))
		{
			UpdateControl(true);
			return true;
		}
	}
	return false;
}

bool CImageString::SetBitmap(CString strImage, CString strMask)
{
	if(!strMask.IsEmpty())
	{
		m_strMask = strMask;

		if (m_Image.LoadImage(m_hWnd, strImage, m_strMask.GetLength()))
		{
			UpdateControl(true);
			return true;
		}
	}
	return false;
}

void CImageString::SetNumnerTitle(int nNumber)
{
	CString strTitle;
	strTitle.Format(TEXT("%d"), nNumber);
	__super::SetTitle(strTitle);
}

void CImageString::DrawControl(CDC &dc, CRect rcParent, CRect rcUpdate)
{
	int nWidth = m_rc.Width();
	int nHeight = m_rc.Height();

	if(m_bUpdate)
	{
		int nLen = m_strTitle.GetLength();
		UpdateMemDC(dc, nWidth * nLen, nHeight);

		Graphics graphics(m_memDC);

		Image *pImage = m_Image.GetImage();
		CSize sizeImage = m_Image.GetImageSize();

		int nXPos = 0;
		int nYPos = 0;
		if(m_uAlignment == DT_CENTER)
		{
			nXPos = (nWidth - nLen * sizeImage.cx) / 2;
		}
		else if(m_uAlignment == DT_RIGHT)
		{
			nXPos = nWidth - nLen * sizeImage.cx;
		}

		if(m_uVAlignment == DT_VCENTER)
		{
			nYPos = (nHeight - sizeImage.cy) / 2;
		}
		else if(m_uVAlignment == DT_BOTTOM)
		{
			nYPos = nHeight - sizeImage.cy;
		}

		for(int i = 0; i < nLen; i++)
		{
			int nImageIndex = GetImageIndex(i);
			if(nImageIndex != -1)
			{
				graphics.DrawImage(pImage, Rect(nXPos , nYPos,  sizeImage.cx, sizeImage.cy),
					nImageIndex * sizeImage.cx, 0, sizeImage.cx, sizeImage.cy, UnitPixel);

				nXPos += sizeImage.cx;
			}
		}
	}

	DrawControlEx(dc, rcParent, rcUpdate);
}

int CImageString::GetImageIndex(int nIndex)
{
	int nLen = m_strTitle.GetLength();
	if(nIndex < nLen)
	{
		int nMaskLen = m_strMask.GetLength();
		for(int i = 0; i < nMaskLen; i++)
		{
			if(m_strTitle[nIndex] == m_strMask[i])
			{
				return i;
			}
		}
	}

	return -1;
}