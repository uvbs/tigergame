#pragma once
#include "..\Base\ControlBase.h"


class CImageAnimation : public CControlBase
{
public:
	CImageAnimation(HWND hWnd, CMessageInterface* pMessage, UINT uControlID, CRect rc, 
		UINT uAlignment = DT_CENTER, UINT uVAlignment = DT_VCENTER, bool bIsVisible = TRUE, bool bIsDisable = FALSE);
	~CImageAnimation(void);
	
	bool SetBitmap(UINT nResourceID, int nMaxIndex, CString strType= TEXT("PNG"));
	bool SetBitmap(CString strImage, int nMaxIndex);
	bool SetRun(bool bRun, int nIndex = -1);
	
protected:
	virtual	bool OnControlTimer();
	void DrawControl(CDC &dc, CRect rcParent, CRect rcUpdate);
	
public:
	IceImage m_Image;
	
	//¹ý³ÌË÷Òý
	int m_nIndex;
	int m_nMaxIndex;
	int m_nCount;

	UINT m_uAlignment;
	UINT m_uVAlignment;
};