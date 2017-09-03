#pragma once

class IceImage
{
	HWND m_hWnd;
	Image * m_pImage;
	CSize m_sizeImage;
	HGLOBAL m_hMem;
public:
	IceImage();
	virtual ~IceImage();

	bool LoadImage(HWND hWnd, UINT nID, CString strType, UINT nXCount = 1, UINT nYCount = 1);
	bool LoadImage(HWND hWnd, CString strImage, UINT nXCount = 1, UINT nYCount = 1);
	bool LoadImage(HWND hWnd, Image *pImage, UINT nXCount = 1, UINT nYCount = 1);
	void Destroy();

	Image *GetImage() { return  m_pImage; }
	CSize GetImageSize() { return  m_sizeImage; }
};