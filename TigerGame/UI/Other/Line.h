#pragma once
#include "..\Base\ControlBase.h"

class CLine : public CControlBase
{
public:
	CLine(HWND hWnd, CMessageInterface* pMessage, UINT uControlID, CRect rc, 
		Color clr = Color(255, 255, 255, 255), bool bIsVisible = TRUE);
	~CLine(void);

protected:
	void DrawControl(CDC &dc, CRect rcParent, CRect rcUpdate);

	Color m_clr;
};
