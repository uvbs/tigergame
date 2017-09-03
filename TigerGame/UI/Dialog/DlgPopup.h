#pragma once

#include <vector>
#include "../IceDUI.h"

using namespace  std;

struct tagWindowMessage
{
	UINT				uMsg;
	size_t				lSize;
	void*				pData;
};

enum enumBackMode
{
	enBMImage = 0,			// 正常
	enBMFrame				// 边框
};

// CDlgPopup

class CDlgPopup : public CWnd, public CMessageInterface
{
	bool			m_bInit;
	bool			m_bTracking;
	bool			m_bLButtonDown;
	bool			m_bIsLButtonDblClk;
	bool			m_bIsSetCapture;
	HWND			m_hWndMsg;

protected:
	Image*			m_pImage;
	CSize			m_sizeBKImage;
	CPoint			m_point;
	CSize			m_size;
	UINT			m_uControlID;

	enumBackMode	m_enBackMode;
	int				m_nFrameSize;
	bool			m_bInitFinish;
	bool			m_bAutoClose;

	bool			m_bNCActive;
	vector<CControlBase *>	m_vecControl;
	CControlBase *m_pControl;
	CControlBase *m_pFocusControl;

	DECLARE_DYNAMIC(CDlgPopup)

public:
	CDlgPopup();
	virtual ~CDlgPopup();

	void SetBackMode(enumBackMode enBackMode, int nFrameSize = 4);
	void SetBackBitmap(UINT nResourceID, int nFrameSize = 4);
	void SetBackBitmap(CString strImage, int nFrameSize = 4);
	bool Create(CWnd *pParent, CRect rc, UINT uControlID, UINT nResourceID, enumBackMode enBackMode = enBMFrame, int nFrameSize = 4);
	bool Create(CWnd *pParent, CRect rc, UINT uControlID, CString strImage, enumBackMode enBackMode = enBMFrame, int nFrameSize = 4);
	void CloseWindow();

	void DrawWindow();
	void DrawWindow(CDC &dc);
	void DrawWindow(CDC *pDC);
	virtual void DrawWindow(CDC &dc, CRect rcClient) {};
	virtual void DrawWindowEx(CDC &dc, CRect rcClient) {};
	virtual void InitUI(CRect rcClient) = 0;
	virtual bool OnMouseMove(CPoint point) { return false; };
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	virtual bool OnLButtonDown(CPoint point) { return false; };
	virtual bool OnLButtonUp(CPoint point) { return false; };

	// 定时器消息
	void OnTimer(UINT nIDEvent);
	virtual void OnTimerEx(UINT nIDEvent) {};

	// 消息响应
	virtual LRESULT OnControlUpdate(CRect rcUpdate, CControlBase *pUpdateControlBase = NULL);
	virtual LRESULT OnControlMessage(UINT uControlID, UINT uMsg, void *pData, size_t lSize) = 0;
	virtual LRESULT OnWindowMessage(UINT uControlID, UINT uMsg, void *pData, size_t lSize) = 0;
	afx_msg LRESULT OnBaseControlMessage(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnBaseWindowMessage(WPARAM wParam, LPARAM lParam);
	virtual void OnWindowMouseLeave() {}
	virtual void OnWindowMouseHover() {}

	// 发送通知消息
	void SetMsgHwnd(HWND hWndMsg) { m_hWndMsg = hWndMsg; }
	bool SendMessageEx(UINT uMsg, void *pData = NULL, size_t lSize = 0);
	bool PostMessageEx(UINT uMsg, void *pData = NULL, size_t lSize = 0);

	CControlBase *GetControl(UINT uControlID);

	// 移动控件
	virtual CControlBase * SetControlRect(UINT uControlID, CRect rc);
	// 移动控件
	virtual CControlBase * SetControlRect(CControlBase *pControlBase, CRect rc);
	// 显示控件
	virtual CControlBase * SetControlVisible(UINT uControlID, bool bVisible);
	// 显示控件
	virtual CControlBase * SetControlVisible(CControlBase *pControlBase, bool bVisible);
	// 禁用控件
	virtual CControlBase * SetControlDisable(UINT uControlID, bool bDisable);
	// 禁用控件
	virtual CControlBase * SetControlDisable(CControlBase *pControlBase, bool bDisable);
	// 更新选中
	void UpdateHover();

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg LRESULT OnNcHitTest(CPoint point);
	afx_msg void OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS* lpncsp);
	afx_msg BOOL OnNcActivate(BOOL bActive);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg LRESULT OnMouseLeave(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnMouseHover(WPARAM wParam, LPARAM lParam);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);

	DECLARE_MESSAGE_MAP()
	virtual void PostNcDestroy();
public:
	afx_msg void OnClose();
	afx_msg void OnPaint();
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);

};


