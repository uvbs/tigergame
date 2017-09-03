#pragma once
#include "..\Base\ControlBase.h"

class CHideButton : public CControlBaseFont
{
public:
	CHideButton(HWND hWnd, CMessageInterface* pMessage, UINT uControlID, CRect rc, CString strTip, CString strTitle, Color clrTip = Color(56, 56, 56),
		Color clrTextNormal = Color(39, 134, 230) ,Color clrTextHover = Color(93, 169, 242),
		Color clrTextDown = Color(21, 112, 235) ,Color clrTextDisable =  Color(128, 128, 128),
		bool bIsVisible = TRUE, bool bIsDisable = FALSE);
	~CHideButton(void);
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
	Color m_clrTip;
	CRect m_rcText;
	enumButtonState m_enButtonState;
	bool m_bDown;
	CString m_strTip;
	bool m_bShowBitton;
};