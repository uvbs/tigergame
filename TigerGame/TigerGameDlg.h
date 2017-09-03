
// TigerGame.h : ͷ�ļ�
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

// CTigerGameDlg �Ի���
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
	// ����
public:
	
	CTigerGameDlg(CWnd* pParent = NULL);	// ��׼���캯��
	~CTigerGameDlg();

// �Ի�������
	enum { IDD = IDD_TIGERGAME_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg HCURSOR OnQueryDragIcon();

	DECLARE_MESSAGE_MAP()

	virtual void InitUI(CRect rcClient);
	virtual void OnSize(UINT nType, CRect rcClient);
	// ���ÿؼ�
	virtual void ResetControlBackHSL(COLOR_HSL colorHSL, COLOR_HSL OldColorHSL);

	// ��Ϣ��Ӧ
	virtual LRESULT OnControlMessage(UINT uControlID, UINT uMsg, void *pData, size_t lSize);
	virtual LRESULT OnWindowMessage(UINT uControlID, UINT uMsg, void *pData, size_t lSize);
	virtual void OnWindowMouseLeave();
	virtual void OnWindowMouseHover();

	// ��������
	void AddSystrayIcon();
	// ɾ������
	void DelSystrayIcon();

	// ����
	afx_msg LRESULT OnMessageSkin(UINT uMsg, void *pData, size_t lSize);
	// ���˵�
	afx_msg LRESULT OnMessageMainMenu(UINT uMsg, void *pData, size_t lSize);

		
public:

	// ��ʱ����Ϣ
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
