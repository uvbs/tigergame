#ifndef __DLG_MY_BASE_X_H__
#define __DLG_MY_BASE_X_H__
#include <vector>
#include "..\IceDUI.h"

using namespace  std;

// �ؼ�
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

	bool			m_bAssignBackColor;			// ָ������ɫ
	bool			m_bOverRegio;				// �Ƿ����
	bool			m_bWindowMove;				// �ƶ�����
	bool			m_bChangeSize;				// �ı䴰�ڴ�С
	bool			m_bShowTaskbar;				// ��ʾ������
	bool			m_bMinBotton;				// ��С����ť
	CSize			m_MinSize;					// �����޶���С��С
	CSize			m_MaxSize;					// �����޶�����С
	CRgn			m_Rgn;						// �����򴰿�����
	COLORREF		m_clrBK;					// �Զ���ǰ����ɫ
	COLOR_HSL		m_hslBK;					// �Զ���ǰ����ɫ

	HICON			m_hIcon;

	CString			m_strImageFile;
	CBitmap			m_BKImage;					// ��ܱ���ͼƬ
	CSize			m_sizeBKImage;
	CDC				m_MemBKDC;					// ����dc
	CBitmap			*m_pOldMemBK;
	CBitmap			m_MemBK;
	bool			m_bDrawImage;				// ͼƬ��ɫ����


	int				m_nFrameTopBottomSpace;
	int				m_nFrameLeftRightSpace;

	int				m_nOverRegioX;				//���ȵĴ�С
	int				m_nOverRegioY;				//���ȵĴ�С
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


	void SetMinSize(int iWidth, int iHeight);	// ������С���ڴ�С
	CSize GetMinSize();
	void SetBackColor(COLORREF clr, bool bDrawImage);
	// ����ͼƬ
	void LoadImage(UINT nIDResource, CString strType = TEXT("PNG"));
	void LoadImage(CString strFileName);

protected:

	// ���ò�����������
	void SetupRegion(int border_offset[], int nSize);
	void DrawImageStyle(CDC &dc, const CRect &rcClient, const CRect &rcUpdate);
	// ��ʼ�����ڿؼ���Ĭ��ֵ
	void InitUIState();
	// ���ò�����������
	void SetupRegion(int nSize);
	// ������ͼƬ
	void DrawBackground(CBitmap &bitBackground);
	// ������ͼƬ
	void DrawBackground(COLORREF clr);
	// ������ͼƬ
	void DrawBackgroundColor(Image *&pImage);
	// ������ͼƬ
	void DrawBackgroundColor(CBitmap &bitBackground);
	// ǰ��ͼƬ
	virtual void DrawControl(CDC &dc, const CRect &rcClient, const CRect &rcUpdate);
	// ���ÿؼ�
	void ResetControl();
	// ���ÿؼ�
	void ResetBackHSL(COLOR_HSL colorHSL, COLOR_HSL OldColorHSL);
	// ���ÿؼ�
	virtual void ResetControlBackHSL(COLOR_HSL colorHSL, COLOR_HSL OldColorHSL) {}
	// �ؼ�λ��
	virtual CRect GetControlRect(UINT uControlID);
	// �ƶ��ؼ�
	virtual CControlBase * SetControlRect(UINT uControlID, CRect rc);
	// �ƶ��ؼ�
	virtual CControlBase * SetControlRect(CControlBase *pControlBase, CRect rc);
	// ��ʾ�ؼ�
	virtual CControlBase * SetControlVisible(UINT uControlID, bool bVisible);
	// ��ʾ�ؼ�
	virtual CControlBase * SetControlVisible(CControlBase *pControlBase, bool bVisible);
	// ���ÿؼ�
	virtual CControlBase * SetControlDisable(UINT uControlID, bool bDisable);
	// ���ÿؼ�
	virtual CControlBase * SetControlDisable(CControlBase *pControlBase, bool bDisable);
	// ����ѡ��
	void UpdateHover();


	void InitBaseUI(CRect rcClient);
	virtual void InitUI(CRect rcClient) = 0;
	virtual void OnSize(UINT nType, CRect rcClient) = 0;
	virtual void OnBaseSize(UINT nType, CRect rcFrame);
	CControlBase *GetControl(UINT uControlID);
	CControlBase *GetBaseControl(UINT uControlID);

	// ��ʱ����Ϣ
	void OnTimer(UINT nIDEvent);
	virtual void OnTimerEx(UINT nIDEvent) {};


	// ��Ϣ��Ӧ
	virtual LRESULT OnControlUpdate(CRect rcUpdate, CControlBase *pUpdateControlBase = NULL);
	virtual LRESULT OnControlMessage(UINT uControlID, UINT uMsg, void *pData, size_t lSize) = 0;
	virtual LRESULT OnWindowMessage(UINT uControlID, UINT uMsg, void *pData, size_t lSize) = 0;
	afx_msg LRESULT OnBaseControlMessage(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnBaseWindowMessage(WPARAM wParam, LPARAM lParam);

	// ����֪ͨ��Ϣ
	void SetMsgHwnd(HWND hWndMsg) { m_hWndMsg = hWndMsg; }
	void SetWindowID(UINT uWindowID) { m_uWindowID = uWindowID; }
	bool SendMessageEx(UINT uMsg, void *pData = NULL, size_t lSize = 0);
	bool PostMessageEx(UINT uMsg, void *pData = NULL, size_t lSize = 0);

	// ���ػ��ƴ�������		
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

