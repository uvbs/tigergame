#ifndef __DLG_MY_BASE_X_H__
#define __DLG_MY_BASE_X_H__
#include <vector>
#include "..\IceDUI.h"

using namespace  std;

// 控件
#define					BT_MIN								1000
#define					BT_MAX								1001
#define					BT_CLOSE							1002
#define					FRAME								1003

class  CDlgBase : public CDialog, public CMessageInterface
{
	DECLARE_DYNAMIC(CDlgBase)

public:
	CString			m_strPath;
	bool			m_bInit;


protected:

	bool			m_bAssignBackColor;			// 指定背景色
	bool			m_bOverRegio;				// 是否过度
	bool			m_bWindowMove;				// 移动窗口
	bool			m_bChangeSize;				// 改变窗口大小
	bool			m_bShowTaskbar;				// 显示任务栏
	bool			m_bMinBotton;				// 最小化按钮
	CSize			m_MinSize;					// 窗口限定最小大小
	CSize			m_MaxSize;					// 窗口限定最大大小
	CRgn			m_Rgn;						// 不规则窗口区域
	COLORREF		m_clrBK;					// 自定义前景颜色
	COLOR_HSL		m_hslBK;					// 自定义前景颜色

	HICON			m_hIcon;

	CString			m_strImageFile;
	CBitmap			m_BKImage;					// 框架背景图片
	CSize			m_sizeBKImage;
	CDC				m_MemBKDC;					// 背景dc
	CBitmap			*m_pOldMemBK;
	CBitmap			m_MemBK;
	bool			m_bDrawImage;				// 图片或纯色背景


	int				m_nFrameTopBottomSpace;
	int				m_nFrameLeftRightSpace;

	int				m_nOverRegioX;				//过度的大小
	int				m_nOverRegioY;				//过度的大小
	bool			m_bNCActive;

	bool			m_bTracking;
	bool			m_bLButtonDown;
	bool			m_bRButtonDown;
	bool			m_bIsLButtonDblClk;
	bool			m_bIsSetCapture;

	vector<CControlBase *>	m_vecControl;
	vector<CControlBase *>	m_vecArea;
	CControlBase *m_pControl;
	CControlBase *m_pFocusControl;

	UINT			m_uWindowID;
	HWND			m_hWndMsg;
	bool			m_bChild;

protected:
	vector<CControlBase *>	m_vecBaseControl;
	vector<CControlBase *>	m_vecBaseArea;


public:
	CDlgBase(CWnd* pParent, UINT uWindowID);
	CDlgBase(UINT nIDTemplate, CWnd* pParent = NULL);
	bool Create(CWnd* pParent, HWND hWndMsg, UINT uWindowID, bool bChild = false);
	virtual ~CDlgBase();


	void SetMinSize(int iWidth, int iHeight);	// 设置最小窗口大小
	CSize GetMinSize();
	void SetBackColor(COLORREF clr, bool bDrawImage);
	// 加载图片
	void LoadImage(UINT nIDResource, CString strType = TEXT("PNG"));
	void LoadImage(CString strFileName);

protected:

	// 设置不规则窗体区域
	void SetupRegion(int border_offset[], int nSize);
	void DrawImageStyle(CDC &dc, const CRect &rcClient, const CRect &rcUpdate);
	// 初始化窗口控件的默认值
	void InitUIState();
	// 设置不规则窗体区域
	void SetupRegion(int nSize);
	// 画背景图片
	void DrawBackground(CBitmap &bitBackground);
	// 画背景图片
	void DrawBackground(COLORREF clr);
	// 画背景图片
	void DrawBackgroundColor(Image *&pImage);
	// 画背景图片
	void DrawBackgroundColor(CBitmap &bitBackground);
	// 前景图片
	virtual void DrawControl(CDC &dc, const CRect &rcClient, const CRect &rcUpdate);
	// 重置控件
	void ResetControl();
	// 重置控件
	void ResetBackHSL(COLOR_HSL colorHSL, COLOR_HSL OldColorHSL);
	// 重置控件
	virtual void ResetControlBackHSL(COLOR_HSL colorHSL, COLOR_HSL OldColorHSL) {}
	// 控件位置
	virtual CRect GetControlRect(UINT uControlID);
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


	void InitBaseUI(CRect rcClient);
	virtual void InitUI(CRect rcClient) = 0;
	virtual void OnSize(UINT nType, CRect rcClient) = 0;
	virtual void OnBaseSize(UINT nType, CRect rcFrame);
	CControlBase *GetControl(UINT uControlID);
	CControlBase *GetBaseControl(UINT uControlID);

	// 定时器消息
	void OnTimer(UINT nIDEvent);
	virtual void OnTimerEx(UINT nIDEvent) {};


	// 消息响应
	virtual LRESULT OnControlUpdate(CRect rcUpdate, CControlBase *pUpdateControlBase = NULL);
	virtual LRESULT OnControlMessage(UINT uControlID, UINT uMsg, void *pData, size_t lSize) = 0;
	virtual LRESULT OnWindowMessage(UINT uControlID, UINT uMsg, void *pData, size_t lSize) = 0;
	afx_msg LRESULT OnBaseControlMessage(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnBaseWindowMessage(WPARAM wParam, LPARAM lParam);

	// 发送通知消息
	void SetMsgHwnd(HWND hWndMsg) { m_hWndMsg = hWndMsg; }
	void SetWindowID(UINT uWindowID) { m_uWindowID = uWindowID; }
	bool SendMessageEx(UINT uMsg, void *pData = NULL, size_t lSize = 0);
	bool PostMessageEx(UINT uMsg, void *pData = NULL, size_t lSize = 0);

	// 重载绘制窗体内容		
	virtual void OnClose();
	virtual void OnMinimize();
	virtual BOOL OnMaximize();
	void ShowTopWindow(HWND hWnd);

	virtual BOOL OnInitDialog();
	virtual void PostNcDestroy();
	afx_msg void OnDropFiles(HDROP hDropInfo);
	afx_msg void OnSize(UINT nType, int cx, int cy);

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg LRESULT OnNcHitTest(CPoint point);
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnPaint();
	afx_msg void OnNcPaint();
	afx_msg void OnGetMinMaxInfo(MINMAXINFO* lpMMI);
	afx_msg void OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS* lpncsp);
	afx_msg BOOL OnNcActivate(BOOL bActive);
	afx_msg void OnWindowPosChanging(WINDOWPOS* lpwndpos);
	afx_msg void OnWindowPosChanged(WINDOWPOS* lpwndpos);


	LRESULT OnMessageButtomMin(UINT uMsg, void *pData, size_t lSize);
	LRESULT OnMessageButtomClose(UINT uMsg, void *pData, size_t lSize);
	virtual void OnWindowMouseLeave() {}
	virtual void OnWindowMouseHover() {}

	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

protected:
	virtual void PreSubclassWindow();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg LRESULT OnMouseLeave(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnMouseHover(WPARAM wParam, LPARAM lParam);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);

public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnDestroy();
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);

};

#endif __DLG_MY_BASE_X_H__

