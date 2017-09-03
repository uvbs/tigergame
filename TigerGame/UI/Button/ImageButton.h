#pragma once
#include "..\Base\ControlBase.h"


class CImageButton : public CControlBaseFont
{
public:
	CImageButton(HWND hWnd, CMessageInterface* pMessage, UINT uControlID, CRect rc, bool bAnimation = true, CString strTitle = TEXT(""), bool bIsVisible = TRUE, bool bIsDisable = FALSE, bool bIsPressDown = FALSE);
	bool SetBitmap(UINT nResourceID, CString strType = TEXT("PNG"));
	bool SetBitmap(CString strImage);
	~CImageButton(void);
protected:
	virtual bool OnControlMouseMove(UINT nFlags, CPoint point);
	virtual bool OnControlLButtonDown(UINT nFlags, CPoint point);
	virtual bool OnControlLButtonUp(UINT nFlags, CPoint point);

	virtual	void SetControlDisable(bool bIsDisable);

	virtual	bool OnControlTimer();

	void DrawControl(CDC &dc, CRect rcParent, CRect rcUpdate);
	
public:
	IceImage m_Image;
	enumButtonState m_enButtonState;
	
	//¹ý³ÌË÷Òý
	int m_nIndex;
	int m_nMaxIndex;
	bool m_bTimer;
};