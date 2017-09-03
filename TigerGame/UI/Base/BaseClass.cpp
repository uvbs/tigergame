#include "StdAfx.h"
#include "BaseClass.h"

IceImage::IceImage()
{
	m_hWnd = NULL;
	m_pImage = NULL;
	m_hMem = NULL;
	m_sizeImage.SetSize(0, 0);
}

IceImage::~IceImage()
{
	Destroy();
}

bool IceImage::LoadImage(HWND hWnd, UINT nID, CString strType, UINT nXCount/* = 1*/, UINT nYCount/* = 1*/)
{
	ASSERT(nXCount != 0 && nYCount != 0);
	Destroy();
	m_hWnd = hWnd;

	HINSTANCE hInst = AfxGetResourceHandle();
	HRSRC hRsrc = ::FindResource(hInst, MAKEINTRESOURCE(nID), strType);
	if (!hRsrc)
	{
		return false;
	}

	DWORD len = SizeofResource(hInst, hRsrc);
	BYTE* lpRsrc = (BYTE*)LoadResource(hInst, hRsrc);
	if (!lpRsrc)
	{
		return false;
	}
	m_hMem = GlobalAlloc(GMEM_FIXED, len);
	BYTE* pmem = (BYTE*)GlobalLock(m_hMem);
	memcpy(pmem, lpRsrc, len);
	IStream* pstm;
	CreateStreamOnHGlobal(m_hMem, FALSE, &pstm);
	m_pImage = Gdiplus::Image::FromStream(pstm);
	GlobalUnlock(m_hMem);
	pstm->Release();
	FreeResource(lpRsrc);

	m_sizeImage.SetSize(m_pImage->GetWidth() / nXCount, m_pImage->GetHeight() / nYCount);

	return true;
}

bool IceImage::LoadImage(HWND hWnd, CString strImage, UINT nXCount/* = 1*/, UINT nYCount/* = 1*/)
{
	ASSERT(nXCount != 0 && nYCount != 0);
	Destroy();
	m_hWnd = hWnd;

	m_pImage = Image::FromFile(strImage, TRUE);

	if (m_pImage->GetLastStatus() == Ok)
	{
		m_sizeImage.SetSize(m_pImage->GetWidth() / nXCount, m_pImage->GetHeight() / nYCount);

		return true;
	}
	return false;
}

bool IceImage::LoadImage(HWND hWnd, Image *pImage, UINT nXCount/* = 1*/, UINT nYCount/* = 1*/)
{
	ASSERT(nXCount != 0 && nYCount != 0);
	Destroy();
	m_hWnd = hWnd;

	m_pImage = pImage;

	if (m_pImage->GetLastStatus() == Ok)
	{
		m_sizeImage.SetSize(m_pImage->GetWidth() / nXCount, m_pImage->GetHeight() / nYCount);

		return true;
	}
	return false;
}

void IceImage::Destroy()
{
	m_sizeImage.SetSize(0, 0);

	if (::IsWindow(m_hWnd))
	{
		if (m_pImage != NULL)
		{
			delete m_pImage;
			m_pImage = NULL;
		}
	}

	if (m_hMem != NULL)
	{
		GlobalFree(m_hMem);
		m_hMem = NULL;
	}
}