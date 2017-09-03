#pragma once
#include "..\Base\ControlBase.h"

struct ItemInfo
{
	IceImage image;
	CRect rc;
	CString strText;
	UINT nItemID;
};


class CTab : public CControlBaseFont
{
public:
	CTab(HWND hWnd, CMessageInterface* pMessage, UINT uControlID, CRect rc, CString strTitle = TEXT(""), bool bIsVisible = TRUE, bool bIsDisable = FALSE, bool bIsPressDown = FALSE);

	bool InsertItem(int nItem, UINT nItemID, CString strItemText, UINT nResourceID, int nItemWidth = 0, CString strType = TEXT("PNG"));
	bool InsertItem(int nItem, UINT nItemID, CString strItemText, CString strImage, int nItemWidth = 0);

	bool SetSeperator(bool bSeperator, UINT nResourceID = 0, CString strType = TEXT("PNG"));
	bool SetSeperator(bool bSeperator, CString strImage = TEXT(""));

	int SetSelectItem(int nItem);

	~CTab(void);
protected:
	virtual bool OnControlMouseMove(UINT nFlags, CPoint point);
	virtual bool OnControlLButtonDown(UINT nFlags, CPoint point);
	virtual bool OnControlLButtonUp(UINT nFlags, CPoint point);

	void DrawControl(CDC &dc, CRect rcParent, CRect rcUpdate);

	bool InsertItem(int nItem, ItemInfo &itemInfo);
	void UpdateRect();

	virtual bool SetControlRect(CRect rc);
	
public:
	vector<ItemInfo> m_vecItemInfo;
	vector<CRect> m_vecRcSeperator;
	IceImage m_ImageSeperator;

	int m_nHoverItem;
	int m_nDownItem;
};