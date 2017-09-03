#include "StdAfx.h"
#include "Progress.h"

CProgress::CProgress(HWND hWnd, CMessageInterface* pMessage, UINT uControlID, CRect rc, int nProgress/* = 0*/, 
					 bool bIsVisible/* = TRUE*/, bool bIsDisable/* = FALSE*/)
	: CControlBase(hWnd, pMessage, uControlID, rc, bIsVisible, bIsDisable, false)
{
	m_bRunTime = false;
	m_nIndex = 0;
	m_nMaxIndex = 10;
	m_nCount = 0;

	m_nProgress = 0;

	SetProgress(nProgress);
}

CProgress::~CProgress(void)
{

}

bool CProgress::SetBitmap(UINT nResourceID, CString strType)
{
	if (m_Image.LoadImage(m_hWnd, nResourceID, strType, 2))
	{
		UpdateControl(true);
		return true;
	}
	return false;
}

int CProgress::SetProgress(int nProgress)
{
	int nOldProgress = m_nProgress;
	if(nProgress >= 0 && nProgress <= 100 && m_nProgress != nProgress)
	{
		m_nProgress = nProgress;
		UpdateControl(true);
	}
	return nOldProgress;
}

bool CProgress::SetBitmap(CString strImage)
{
	if (m_Image.LoadImage(m_hWnd, strImage, 2))
	{
		UpdateControl(true);
		return true;
	}

	return false;
}

bool CProgress::OnControlTimer()
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

void CProgress::DrawControl(CDC &dc, CRect rcParent, CRect rcUpdate)
{
	int nWidth = m_rc.Width();
	int nHeight = m_rc.Height();

	if(m_bUpdate)
	{
		UpdateMemDC(dc, nWidth, nHeight);

		Graphics graphics(m_memDC);		

		Image *pImage = m_Image.GetImage();
		CSize sizeImage = m_Image.GetImageSize();

		DrawImageFrame(graphics, pImage, CRect(0, 0, nWidth, nHeight),
			0, 0, sizeImage.cx, sizeImage.cy, 2);

		if(m_nProgress != 0)
		{
			DrawImageFrame(graphics, pImage, CRect(0, 0, nWidth * m_nProgress / 100, nHeight),
				sizeImage.cx, 0, sizeImage.cx, sizeImage.cy, 2);
		}
	}

	DrawControlEx(dc, rcParent, rcUpdate);
}