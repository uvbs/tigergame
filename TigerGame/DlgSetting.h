#pragma once

#include "./UI/IceDUI.h"

class CDlgSetting : public CDlgPopup
{
	bool m_bImage;
public:
	CDlgSetting(void);
	~CDlgSetting(void);


	virtual void DrawWindowEx(CDC &dc, CRect rcClient);
	virtual void InitUI(CRect rcClient);

	// œ˚œ¢œÏ”¶
	virtual LRESULT OnControlMessage(UINT uControlID, UINT uMsg, void *pData, size_t lSize);
	virtual LRESULT OnWindowMessage(UINT uControlID, UINT uMsg, void *pData, size_t lSize);
};
