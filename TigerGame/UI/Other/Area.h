#pragma once
#include "..\Base\ControlBase.h"

class CArea : public CControlBase
{
public:
	CArea(HWND hWnd, CMessageInterface* pMessage, UINT uControlID, CRect rc, Color clr, 
		int nBeginTransparent = 128, int nEndTransparent = 128, bool bIsVisible = TRUE);
	~CArea(void);

	void SetColor(Color clr);

protected:
	void DrawControl(CDC &dc, CRect rcParent, CRect rcUpdate);
	int		m_nBeginTransparent;
	int		m_nEndTransparent;
	Color	m_clr;
};
