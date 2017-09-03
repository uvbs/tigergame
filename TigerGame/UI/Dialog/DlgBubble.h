#pragma once

#include "./UI/IceDUI.h"

class CDlgBubble : public CDlgPopup
{
	int m_nArrowHeigth;
	int m_nArrowPos;
	Color m_clrFrame;
	Color m_clrFill;

public:
	CDlgBubble(void);
	~CDlgBubble(void);

	void SetArrowInfo(int nArrowPos, int nArrowHeigth);
	void SetColor(Color clrFrame, Color clrFill);

	virtual void DrawWindowEx(CDC &dc, CRect rcClient);
	virtual void InitUI(CRect rcClient);

	// œ˚œ¢œÏ”¶
	virtual LRESULT OnControlMessage(UINT uControlID, UINT uMsg, void *pData, size_t lSize);
	virtual LRESULT OnWindowMessage(UINT uControlID, UINT uMsg, void *pData, size_t lSize);
};
