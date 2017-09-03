#pragma once

#include "MessageInterface.h"
#include "BaseClass.h"
#include <vector>

using namespace  std;

class CDlgBase;

// 按钮状态
enum enumButtonState
{
	enBSNormal = 0,
	enBSHover,
	enBSDown,
	enBSDisable,
	enBSHoverDown,
	enBSDisableDown
};

#define				BUTTOM_DOWN			1
#define				BUTTOM_UP			2
#define				CUSTOM_INFO			3

struct tagControlMessage
{
	UINT				uMsg;
	size_t				lSize;
	void*				pData;
};

#define				CONTROL_MESSAGE		(WM_USER + 5000)

typedef LPTSTR MOUSE_CURSOR;

class CControlBase : public CMessageInterface
{
public:
	CControlBase(HWND hWnd, CMessageInterface* pMessage, UINT uControlID, CRect rc, bool bIsVisible = TRUE, bool bIsDisable = FALSE, bool bRresponse = TRUE);
	virtual ~CControlBase(void);

	void Draw(CDC &dc, CRect rcParent, CRect rcUpdate);
	virtual void DrawControl(CDC &dc, CRect rcParent, CRect rcUpdate) = 0;
	void DrawControlEx(CDC &dc, CRect rcParent, CRect rcUpdate, int nSrcOffsetX = 0, int nSrcOffsetY = 0);
	void DrawControlEx(CDC &dc, CRect rcParent, CRect rcUpdate, int nDestOffsetX, int nDestOffsetY, int nWidth, int nHeight, int nSrcOffsetX = 0, int nSrcOffsetY = 0);
	void DrawControlEx(CDC &dc, CRect rcMyRect, CRect rcParent, CRect rcUpdate, int nDestOffsetX, int nDestOffsetY, int nWidth, int nHeight, int nSrcOffsetX/* = 0*/, int nSrcOffsetY/* = 0*/);
	CRect IsUpdateRect(CRect rcUpdate);
	virtual CRect IsControlUpdateRect(CRect rcUpdate) { return rcUpdate & m_rc; }
	virtual bool OnFocus(bool bFocus) { return FALSE; };
	virtual bool GetFocus() { return FALSE; };
	void SetBackColor(COLORREF clr);
	virtual void SetControlBackColor(COLORREF clr) {};
	void SetControlBackColorUpdate(bool bBackColorUpdate) { m_bBackColorUpdate = bBackColorUpdate; };
	void SetUpdate(bool bUpdate);
	bool GetUpdate() { return m_bUpdate; };
	void UpdateMemDC(CDC &dc, int nWidth, int nHeight);

	void SetRect(CRect rc);
	virtual bool SetControlRect(CRect rc) { m_rc = rc; return true; };
	virtual CRect GetRect() { return m_rc; };

	void SetVisible(bool bIsVisible);
	virtual void SetControlVisible(bool bIsVisible) { m_bIsVisible = bIsVisible; };
	bool GetVisible() { return m_bIsVisible; };
	void SetDisable(bool bIsDisable);
	virtual	void SetControlDisable(bool bIsDisable) { m_bIsDisable = bIsDisable; };
	bool GetDisable() { return m_bIsDisable; };

	virtual	bool PtInRect(CPoint point);
	bool GetLButtonDblClk() { return m_bLButtonDblClk; }
	UINT GetControlID() { return m_uControlID; };
	CControlBase *GetControl(UINT uControlID);
	MOUSE_CURSOR GetCursorTpye() { return m_CursorTpye; };

	void UpdateControl(bool bUpdate);
	void UpdateControl(CRect rc, bool bUpdate);

	void SetRresponse(bool bRresponse) { m_bRresponse = bRresponse; };
	bool GetRresponse() { return m_bRresponse; };

	void SetComControl(bool bComControl) { m_bComControl = bComControl; };
	bool GetComControl() { return m_bComControl; };

	bool OnMouseMove(UINT nFlags, CPoint point);
	bool OnLButtonDown(UINT nFlags, CPoint point);
	bool OnLButtonUp(UINT nFlags, CPoint point);
	bool OnRButtonDown(UINT nFlags, CPoint point);
	bool OnRButtonUp(UINT nFlags, CPoint point);
	bool OnLButtonDblClk(UINT nFlags, CPoint point);
	bool OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);

	bool OnTimer();

	virtual bool OnControlMouseMove(UINT nFlags, CPoint point) { return false; };
	virtual bool OnControlLButtonDown(UINT nFlags, CPoint point) { return false; };
	virtual bool OnControlLButtonUp(UINT nFlags, CPoint point) { return false; };
	virtual bool OnControlRButtonDown(UINT nFlags, CPoint point) { return false; };
	virtual bool OnControlRButtonUp(UINT nFlags, CPoint point) { return false; };
	virtual bool OnControlLButtonDblClk(UINT nFlags, CPoint point) { return false; };
	virtual bool OnControlMouseWheel(UINT nFlags, short zDelta, CPoint pt) { return false; };

	// 消息响应
	virtual LRESULT OnControlUpdate(CRect rcUpdate, CControlBase *pUpdateControlBase = NULL);
	virtual	bool OnControlTimer() { return FALSE; };


	// 发送通知消息
	LRESULT SendMessage(UINT uMsg, void *pData = NULL, size_t lSize = 0);
	LRESULT PostMessage(UINT uMsg, void *pData = NULL, size_t lSize = 0);


protected:

	bool m_bComControl;					// 组合控件
	CMessageInterface*	m_pMessage;		// 父类控件
	HWND	m_hWnd;

	UINT	m_uControlID;					// 控件 ID
	MOUSE_CURSOR	m_CursorTpye;			// 光标类型
	bool	m_bIsVisible;			// 可见
	bool	m_bIsDisable;			// 是否可用
	bool	m_bRresponse;			// 响应
	bool	m_bLButtonDown;
	bool	m_bRButtonDown;
	bool	m_bLButtonDblClk;


	bool	m_bUpdate;				// 更新贴图

	bool	m_bIsRun;				// 是否有执行功能
	bool	m_bRunTime;				// 执行定时器

	vector<CControlBase *>	m_vecControl;
	CControlBase *m_pControl;

	int m_nDCWidth;
	int m_nDCHeight;
	HBITMAP m_hBitmap;
	CDC m_memDC;
	HBITMAP m_hOldBitmap;
	COLORREF m_clrBK;
	bool m_bBackColorUpdate;
};

class CControlBaseFont : public CControlBase
{
public:
	CControlBaseFont(HWND hWnd, CMessageInterface* pMessage, UINT uControlID, CRect rc, CString strTitle, bool bIsVisible = TRUE, bool bIsDisable = FALSE, bool bRresponse = TRUE,
		CString strFont = TEXT("Tahoma"), int nFontWidth = 12, FontStyle fontStyle = FontStyleRegular);
	virtual ~CControlBaseFont(void);

	void SetTitle(CString strTitle);
	virtual void SetControlTitle(CString strTitle) { m_strTitle = strTitle; };
	virtual CString GetTitle() { return m_strTitle; };
	void SetFontColor(Color clrText);
	virtual void SetControlFontColor(Color clrText) { m_clrText = clrText; };

	virtual void SetAlignment(UINT uAlignment);
	virtual void SetVAlignment(UINT uVAlignment);
	virtual void SetAlignment(UINT uAlignment, UINT uVAlignment);

	virtual void SetFont(CString strFont = TEXT("Tahoma"), int nFontWidth = 12, FontStyle fontStyle = FontStyleRegular);




protected:
	CString	m_strTitle;				// 标题
	CString m_strFont;
	int m_nFontWidth;
	FontStyle m_fontStyle;
	UINT m_uAlignment;
	UINT m_uVAlignment;
	bool m_bBack;
	Color m_clrBack;
	Color m_clrText;
};
