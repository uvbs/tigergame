#pragma once
#include "..\Base\ControlBase.h"

struct tagMenuItemInfo
{
	bool		bSelect;			// Ñ¡Ôñ
	bool		bDown;				// °´ÏÂ
};

class CMenuItem : public CControlBaseFont
{
public:
	IceImage m_Image;
	enumButtonState m_enButtonState;
	bool m_bDown;
	bool m_bMouseDown;
	bool m_bSelect;
	int m_nLeft;
public:
	CMenuItem(HWND hWnd, CMessageInterface* pMessage, UINT uControlID, CRect rc, CString strTitle = TEXT(""), int nLeft = 30, bool bSelect = false, bool bIsVisible = TRUE, bool bIsDisable = FALSE, bool bIsPressDown = FALSE);
	bool SetBitmap(UINT nResourceID, CString strType = TEXT("PNG"));
	bool SetBitmap(CString strImage);

	bool GetCheck();
	bool SetCheck(bool bCheck);

	~CMenuItem(void);

protected:
	virtual bool OnControlMouseMove(UINT nFlags, CPoint point);
	virtual bool OnControlLButtonDown(UINT nFlags, CPoint point);
	virtual bool OnControlLButtonUp(UINT nFlags, CPoint point);

	virtual	void SetControlDisable(bool bIsDisable);
	void DrawControl(CDC &dc, CRect rcParent, CRect rcUpdate);
};