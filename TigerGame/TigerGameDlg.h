
// TigerGame.h : 头文件
//

#pragma once
#include "afxwin.h"
#include "afxcmn.h"

#include "./UI/IceDUI.h"

class CDlgAbout;
class CDlgSetting;
class CDlgSelectGame;
class CDlgSelectServer;
class CDlgLogin;
class CDlgUserInfo;

// CTigerGameDlg 对话框
class CTigerGameDlg : public CDlgBase
{
	UINT m_uBackImage;
	UINT m_uTimerSkin;

	NOTIFYICONDATA  m_nd;
	CDlgLogin *m_pDlgLogin;
	CDlgAbout *m_pDlgAbout;
	CDlgSetting *m_pDlgSetting;
	CDlgSelectGame *m_pDlgSelectGame;
	CDlgSelectServer *m_pDlgSelectServer;
	CDlgUserInfo *m_pDlgUserInfo;
	// 构造
public:
	
	CTigerGameDlg(CWnd* pParent = NULL);	// 标准构造函数
	~CTigerGameDlg();

// 对话框数据
	enum { IDD = IDD_TIGERGAME_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg HCURSOR OnQueryDragIcon();

	DECLARE_MESSAGE_MAP()

	virtual void InitUI(CRect rcClient);
	virtual void OnSize(UINT nType, CRect rcClient);
	// 重置控件
	virtual void ResetControlBackHSL(COLOR_HSL colorHSL, COLOR_HSL OldColorHSL);

	// 消息响应
	virtual LRESULT OnControlMessage(UINT uControlID, UINT uMsg, void *pData, size_t lSize);
	virtual LRESULT OnWindowMessage(UINT uControlID, UINT uMsg, void *pData, size_t lSize);
	virtual void OnWindowMouseLeave();
	virtual void OnWindowMouseHover();

	// 增加托盘
	void AddSystrayIcon();
	// 删除托盘
	void DelSystrayIcon();

	// 换肤
	afx_msg LRESULT OnMessageSkin(UINT uMsg, void *pData, size_t lSize);
	// 主菜单
	afx_msg LRESULT OnMessageMainMenu(UINT uMsg, void *pData, size_t lSize);

		
public:

	// 定时器消息
	virtual void OnTimer(UINT uTimerID);
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	afx_msg void OnMove(int x, int y);
	virtual BOOL DestroyWindow();
	
	template <class T> void ShowChildDialog(T* &pDlg, int wmCode);
	template <class T> void DeleteChildDialog(T* &pDlg);
	template <class T> void MoveChildDialog(T* &pDlg);
	template <class T> void HideChildDialog(T* &pDlg);
	template <class T> void HideDlgAll(T* &pDlg);
};
