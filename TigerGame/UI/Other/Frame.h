#pragma once
#include "..\Base\ControlBase.h"

class CFrame : public CControlBase
{
public:
	CFrame(HWND hWnd, CMessageInterface* pMessage, UINT uControlID, CRect rc, int nBeginTransparent = 50, int nEndTransparent = 50, 
		COLORREF clr = RGB(255, 255, 255), bool bIsVisible = TRUE);
	~CFrame(void);

protected:
	void DrawControl(CDC &dc, CRect rcParent, CRect rcUpdate);
	int		m_nBeginTransparent;
	int		m_nEndTransparent;
	COLORREF m_clr;
};
