#pragma once
#include "..\Base\ControlBase.h"


class CProgress : public CControlBase
{
public:
	CProgress(HWND hWnd, CMessageInterface* pMessage, UINT uControlID, CRect rc, int nProgress = 0,
		bool bIsVisible = TRUE, bool bIsDisable = FALSE);
	~CProgress(void);
	
	bool SetBitmap(UINT nResourceID, CString strType = TEXT("PNG"));
	bool SetBitmap(CString strImage);

	int SetProgress(int nProgress);
	int GetProgress() { return m_nProgress; };
	
protected:
	virtual	bool OnControlTimer();
	void DrawControl(CDC &dc, CRect rcParent, CRect rcUpdate);
	
public:
	IceImage m_Image;
	
	//¹ý³ÌË÷Òý
	int m_nIndex;
	int m_nMaxIndex;
	int m_nCount;
	
	int m_nProgress;
};