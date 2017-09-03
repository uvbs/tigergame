#include "StdAfx.h"
#include "ImageAnimation.h"

CImageAnimation::CImageAnimation(HWND hWnd, CMessageInterface* pMessage, UINT uControlID, CRect rc, 
				 UINT uAlignment/* = DT_CENTER*/, UINT uVAlignment/* = DT_VCENTER*/, bool bIsVisible/* = TRUE*/, bool bIsDisable/* = FALSE*/)
	: CControlBase(hWnd, pMessage, uControlID, rc, bIsVisible, bIsDisable, false)
{
	m_bRunTime = false;
	m_nIndex = 0;
	m_nMaxIndex = 10;
	m_nCount = 0;

	m_uAlignment = uAlignment;
	m_uVAlignment = uVAlignment;
}

CImageAnimation::~CImageAnimation(void)
{
	
}

bool CImageAnimation::SetBitmap(UINT nResourceID, int nMaxIndex, CString strType)
{
	if(nMaxIndex != 0)
	{
		m_nMaxIndex = nMaxIndex;
		if (m_Image.LoadImage(m_hWnd, nResourceID, strType, nMaxIndex))
		{
			UpdateControl(true);
			return true;
		}
	}
	return false;
}

bool CImageAnimation::SetBitmap(CString strImage, int nMaxIndex)
{
	if(nMaxIndex != 0)
	{
		m_nMaxIndex = nMaxIndex;
		if (m_Image.LoadImage(m_hWnd, strImage, nMaxIndex))
		{
			UpdateControl(true);
			return true;
		}
	}
	return false;
}

bool CImageAnimation::SetRun(bool bRun, int nIndex/* = -1*/)
{
	bool bOldRunTime = m_bRunTime;
	int nOldIndex = m_nIndex;
	m_bRunTime = bRun;

	if(nIndex >= 0 && nIndex < m_nMaxIndex)
	{
		m_nIndex = nIndex;
	}

	if(bOldRunTime != m_bRunTime || nOldIndex != m_nIndex)
	{
		UpdateControl(m_bUpdate);
	}

	return bOldRunTime;
}

bool CImageAnimation::OnControlTimer()
{
	if(!m_bRunTime || !m_bIsVisible)
	{
		return FALSE;
	}

	if(++m_nCount == 3)
	{
		m_nCount = 0;
		if(++m_nIndex >= m_nMaxIndex)
		{
			m_nIndex = 0;
		}
		
		UpdateControl(m_bUpdate);
		return true;
	}

	return false;
}

void CImageAnimation::DrawControl(CDC &dc, CRect rcParent, CRect rcUpdate)
{
	int nWidth = m_rc.Width();
	int nHeight = m_rc.Height();

	if(m_bUpdate)
	{
		UpdateMemDC(dc, nWidth * m_nMaxIndex, nHeight);

		Graphics graphics(m_memDC);
		
		Image *pImage = m_Image.GetImage();
		CSize sizeImage = m_Image.GetImageSize();

		CPoint point = GetOriginPoint(nWidth, nHeight, sizeImage.cx, sizeImage.cy, m_uAlignment, m_uVAlignment);

		for(int i = 0; i < m_nMaxIndex; i++)
		{
			graphics.DrawImage(pImage, Rect(point.x, point.y, sizeImage.cx, sizeImage.cy),
				i * sizeImage.cx, 0, sizeImage.cx, sizeImage.cy, UnitPixel);

			point.x += nWidth;
		}
	}

	DrawControlEx(dc, rcParent, rcUpdate, m_nIndex * nWidth, 0);
}