#pragma once

#include "..\Base\ControlBase.h"

class CScrollV : public CControlBase
{
public:
	CScrollV(HWND hWnd, CMessageInterface* pMessage, UINT uControlID, CRect rc, bool bIsVisible = TRUE);
	~CScrollV(void);

	bool SetBitmap(UINT nRangeResourceID, CString strType = TEXT("PNG"));
	bool SetBitmap(CString strImage);
	bool SetBitmap(UINT nRangeResourceID, UINT nUpResourceID, UINT nDownResourceID, CString strType = TEXT("PNG"));
	bool SetBitmap(CString strRangeImage, CString strUpImage, CString strDownImage);

	void SetScrollCurrentPos(int nCurrentPos);
	void SetScrollMaxRange(int nMaxRange);
	void SetScrollRowRange(int nRowRange);
	void SetScrollInfo(int nMaxRange, int nCurrentPos, int nRowRange);

	virtual bool OnControlMouseMove(UINT nFlags, CPoint point);
	virtual bool OnControlLButtonDown(UINT nFlags, CPoint point);
	virtual bool OnControlLButtonUp(UINT nFlags, CPoint point);
	virtual bool OnControlMouseWheel(UINT nFlags, short zDelta, CPoint pt);

	virtual bool SetControlRect(CRect rc);

	void DrawControl(CDC &dc, CRect rcParent, CRect rcUpdate);

	void DrawRange(HDC hDC, CRect rcUpdate, int nXPos);
	int MoveRange(int nMove);
	int SetRange(bool bUpdate = false, bool bSendMessage = false);
	void SetCurrentPos(int nCurrentPos);

protected:	
	// 消息响应
	virtual LRESULT OnControlMessage(UINT uControlID, UINT uMsg, void *pData, size_t lSize);

protected:	
	IceImage m_Image;	
	int m_nImageFrame;
	bool m_bButton;
	enumButtonState m_enButtonState;
	bool m_bHover;
	bool m_bShowScroll;

	int m_nMaxRange;			//最大
	int m_nCurrentPos;			//当前
	int m_nPageRange;			//一页翻多少
	int m_nRowRange;			//一行翻多少
	CRect m_rcBlock;			//滚动块大小
	int m_nDownPoint;
	int m_nOffsetBlock;
};
