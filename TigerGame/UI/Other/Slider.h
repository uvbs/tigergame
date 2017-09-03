#pragma once

#include "..\Base\ControlBase.h"

class CSlider : public CControlBase
{
public:
	CSlider(HWND hWnd, CMessageInterface* pMessage, UINT uControlID, CRect rc, CSize sizeBlock, bool bIsVisible = TRUE);
	~CSlider(void);

	bool SetBitmap(UINT nRangeResourceID, CString strType = TEXT("PNG"));
	bool SetBitmap(CString strImage);
	bool SetBitmap(UINT nRangeResourceID, UINT nBlockResourceID, CString strType = TEXT("PNG"));
	bool SetBitmap(CString strRangeImage, CString strBlockImage);

	void SetCurrentPos(int nCurrentPos);
	void SetSliderMaxRange(int nMaxRange);
	void SetSliderInfo(int nMaxRange, int nCurrentPos);

	virtual bool OnControlMouseMove(UINT nFlags, CPoint point);
	virtual bool OnControlLButtonDown(UINT nFlags, CPoint point);
	virtual bool OnControlLButtonUp(UINT nFlags, CPoint point);
	virtual bool OnControlMouseWheel(UINT nFlags, short zDelta, CPoint pt);

	virtual bool SetControlRect(CRect rc);

	void DrawControl(CDC &dc, CRect rcParent, CRect rcUpdate);

	int MoveRange(int nMove);
	int SetRange(bool bUpdate = false, bool bSendMessage = false);

protected:	
	// 消息响应
	virtual LRESULT OnControlMessage(UINT uControlID, UINT uMsg, void *pData, size_t lSize);

protected:	
	IceImage m_ImageBack;	
	IceImage m_ImageBlock;
	bool m_bHoverControl;
	enumButtonState m_enButtonState;
	CSize m_sizeBlock;
	int m_nMaxRange;			//最大
	int m_nCurrentPos;			//当前
	CRect m_rcBlock;			//滚动块大小
	int m_nDownPoint;
	int m_nOffsetBlock;
};
