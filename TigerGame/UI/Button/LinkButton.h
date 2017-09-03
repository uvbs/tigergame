#pragma once
#include "..\Base\ControlBase.h"

class CLinkButton : public CControlBaseFont
{
public:
	CLinkButton(HWND hWnd, CMessageInterface* pMessage, UINT uControlID, CRect rc, CString strTitle, CString strLink,
		Color clrTextNormal = Color(254, 39, 134, 230), Color clrTextHover = Color(254, 21, 112, 235),
		Color clrTextDown = Color(254, 21, 112, 235), Color clrTextDisable = Color(254, 128, 128, 128),
		bool bIsVisible = TRUE, bool bIsDisable = FALSE);
	~CLinkButton(void);
	void SetTitleText(CString strTitle);

protected:
	virtual bool OnControlMouseMove(UINT nFlags, CPoint point);
	virtual bool OnControlLButtonDown(UINT nFlags, CPoint point);
	virtual bool OnControlLButtonUp(UINT nFlags, CPoint point);

	void DrawControl(CDC &dc, CRect rcParent, CRect rcUpdate);

public:
	Color m_clrTextNormal;
	Color m_clrTextHover;
	Color m_clrTextDown;
	Color m_clrTextDisable;
	CString m_strLink;
	CRect m_rcText;
	enumButtonState m_enButtonState;
	bool m_bDown;
};