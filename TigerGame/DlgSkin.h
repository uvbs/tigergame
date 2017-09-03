#pragma once

#include "./UI/IceDUI.h"

struct tagSkinInfo
{
	int nType;
	Color clr;
	UINT uIndex;
};

class CDlgSkin : public CDlgPopup
{
	bool m_bImage;
public:
	CDlgSkin(void);
	~CDlgSkin(void);


	virtual void DrawWindowEx(CDC &dc, CRect rcClient);
	virtual void InitUI(CRect rcClient);

	// œ˚œ¢œÏ”¶
	virtual LRESULT OnControlMessage(UINT uControlID, UINT uMsg, void *pData, size_t lSize);
	virtual LRESULT OnWindowMessage(UINT uControlID, UINT uMsg, void *pData, size_t lSize);
};
