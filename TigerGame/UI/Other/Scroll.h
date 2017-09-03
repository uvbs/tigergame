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
	// ��Ϣ��Ӧ
	virtual LRESULT OnControlMessage(UINT uControlID, UINT uMsg, void *pData, size_t lSize);

protected:	
	IceImage m_Image;	
	int m_nImageFrame;
	bool m_bButton;
	enumButtonState m_enButtonState;
	bool m_bHover;
	bool m_bShowScroll;

	int m_nMaxRange;			//���
	int m_nCurrentPos;			//��ǰ
	int m_nPageRange;			//һҳ������
	int m_nRowRange;			//һ�з�����
	CRect m_rcBlock;			//�������С
	int m_nDownPoint;
	int m_nOffsetBlock;
};
