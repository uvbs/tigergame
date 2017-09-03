#pragma once

#include "./UI/IceDUI.h"

class CDlgSelectServer : public CDlgPopup
{
	bool m_bImage;
public:
	CDlgSelectServer(void);
	~CDlgSelectServer(void);


	virtual void DrawWindowEx(CDC &dc, CRect rcClient);
	virtual void InitUI(CRect rcClient);

	// œ˚œ¢œÏ”¶
	virtual LRESULT OnControlMessage(UINT uControlID, UINT uMsg, void *pData, size_t lSize);
	virtual LRESULT OnWindowMessage(UINT uControlID, UINT uMsg, void *pData, size_t lSize);

	void UpdateAreaBtnStatus(UINT uID);
	void UpdateAreaServerList(UINT uID);
};
