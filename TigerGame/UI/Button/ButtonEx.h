#pragma once
#include "..\Base\ControlBase.h"

class CButtonEx : public CControlBaseFont
{
public:
	CButtonEx(HWND hWnd, CMessageInterface* pMessage, UINT uControlID, CRect rc, CString strTitle = TEXT(""), bool bIsVisible = TRUE, bool bIsDisable = FALSE, bool bIsPressDown = FALSE);
	bool SetBitmap(UINT nResourceID, CString strType = TEXT("PNG"));
	bool SetBitmap(CString strImage);
	~CButtonEx(void);
protected:

	virtual bool OnControlMouseMove(UINT nFlags, CPoint point);
	virtual bool OnControlLButtonDown(UINT nFlags, CPoint point);
	virtual bool OnControlLButtonUp(UINT nFlags, CPoint point);

	virtual	void SetControlDisable(bool bIsDisable);
	void DrawControl(CDC &dc, CRect rcParent, CRect rcUpdate);
public:
	IceImage m_Image;
	enumButtonState m_enButtonState;
};