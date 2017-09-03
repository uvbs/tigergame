#pragma once

#include "./UI/IceDUI.h"

class CDlgUserInfo : public CDlgPopup
{
	bool m_bImage;
public:
	CDlgUserInfo(void);
	~CDlgUserInfo(void);


	virtual void DrawWindowEx(CDC &dc, CRect rcClient);
	virtual void InitUI(CRect rcClient);

	// œ˚œ¢œÏ”¶
	virtual LRESULT OnControlMessage(UINT uControlID, UINT uMsg, void *pData, size_t lSize);
	virtual LRESULT OnWindowMessage(UINT uControlID, UINT uMsg, void *pData, size_t lSize);
};
