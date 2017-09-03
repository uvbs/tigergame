#pragma once
#include "..\Base\ControlBase.h"


class CImageString : public CControlBaseFont
{
public:
	CImageString(HWND hWnd, CMessageInterface* pMessage, UINT uControlID, CRect rc, int nNumber, 
		UINT uAlignment = DT_CENTER, UINT uVAlignment = DT_VCENTER, bool bIsVisible = TRUE, bool bIsDisable = FALSE);
	CImageString(HWND hWnd, CMessageInterface* pMessage, UINT uControlID, CRect rc, CString strTitle, 
		UINT uAlignment = DT_CENTER, UINT uVAlignment = DT_VCENTER, bool bIsVisible = TRUE, bool bIsDisable = FALSE);
	~CImageString(void);

	bool SetBitmap(UINT nResourceID, CString strMask, CString strType = TEXT("PNG"));
	bool SetBitmap(CString strImage, CString strMask);
	void SetNumnerTitle(int nNumber);

protected:
	void DrawControl(CDC &dc, CRect rcParent, CRect rcUpdate);

	//
	int GetImageIndex(int nIndex);

public:
	IceImage m_Image;
	CString m_strMask;

	UINT m_uAlignment;
	UINT m_uVAlignment;
};