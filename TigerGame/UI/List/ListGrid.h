#pragma once
#include "..\Base\ControlBase.h"

#define				LEST_LBUTTONDOWN			1
#define				LEST_LBUTTONDBLCLK			2
#define				LEST_RBUTTONDOWN			3
#define				LEST_RBUTTONUP				4

class CListGrid : public CControlBaseFont
{
	int m_nControlHeight;
	int m_nContentHeight;
	int m_nCurShowTop;
	int m_nItemHeight;
	int m_nPlayItem;
	int m_nHoverItem;
	int m_nSelectItem;
	bool m_bPlay;
	vector<CString > m_vecItemText;
public:
	CListGrid(HWND hWnd, CMessageInterface* pMessage, UINT uControlID, CRect rc, bool bIsVisible = TRUE);
	~CListGrid(void);

	int AddItem(CString strItemText);
	bool DeleteItem(UINT uID);
	void DeleteAllItem();
	int SetSelect(int nPlayItem);
	void SetSelectPlay(bool bPlay);
	void UpdateScroll();
	int GetSelectID();

protected:
	// ��Ϣ��Ӧ
	virtual LRESULT OnControlMessage(UINT uControlID, UINT uMsg, void *pData, size_t lSize);
	virtual bool OnControlMouseMove(UINT nFlags, CPoint point);
	virtual bool OnControlLButtonDown(UINT nFlags, CPoint point);
	virtual bool OnControlLButtonUp(UINT nFlags, CPoint point);
	virtual bool OnControlRButtonDown(UINT nFlags, CPoint point);
	virtual bool OnControlRButtonUp(UINT nFlags, CPoint point);
	virtual bool OnControlLButtonDblClk(UINT nFlags, CPoint point);
	virtual bool OnControlMouseWheel(UINT nFlags, short zDelta, CPoint pt);


	virtual bool SetControlRect(CRect rc);
	void DrawControl(CDC &dc, CRect rcParent, CRect rcUpdate);
	void UpdateControlRect(bool bSelect);
};
