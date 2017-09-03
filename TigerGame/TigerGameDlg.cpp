
// TigerGame.cpp : 实现文件
//

#include "stdafx.h"
#include "TigerGameDlg.h"
#include "DlgLogin.h"
#include "DlgAbout.h"
#include "DlgSetting.h"
#include "DlgSelectGame.h"
#include "DlgSelectServer.h"
#include "DlgSkin.h"
#include "DlgUserInfo.h"

#define MYWM_NOTIFYICON (WM_USER + 2000)

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


CTigerGameDlg::CTigerGameDlg(CWnd* pParent /*=NULL*/)
	: CDlgBase(CTigerGameDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_uBackImage = IDB_SKIN_PIC_0;
	m_MinSize.SetSize(360, 560);
	m_MaxSize.SetSize(0, 0);

	m_bOverRegio = false;
	m_bChangeSize = true;
	m_bShowTaskbar = true;
	m_pDlgAbout = NULL;
	m_pDlgSetting = NULL;
	m_pDlgSelectGame = NULL;
	m_pDlgSelectServer = NULL;
	m_pDlgUserInfo = NULL;
	// 登录界面
	
	m_pDlgLogin = new CDlgLogin;
	if (IDOK == m_pDlgLogin->DoModal())
	{
		AfxMessageBox(TEXT("OK"));
	}
	else
	{
		DestroyWindow();
		exit(0);
	}
	
}

CTigerGameDlg::~CTigerGameDlg()
{
	// 删除托盘
	//DelSystrayIcon();
}

void CTigerGameDlg::DoDataExchange(CDataExchange* pDX)
{
	CDlgBase::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CTigerGameDlg, CDlgBase)
	ON_WM_QUERYDRAGICON()

	//}}AFX_MSG_MAP
	ON_WM_DROPFILES()
	ON_WM_MOVE()
END_MESSAGE_MAP()


// CTigerGameDlg 消息处理程序

BOOL CTigerGameDlg::OnInitDialog()
{
	CDlgBase::OnInitDialog();

	SetIcon(m_hIcon, TRUE);
	SetIcon(m_hIcon, FALSE);



	// 增加托盘
	//AddSystrayIcon();
	//::SetWindowPos(GetSafeHwnd(), HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);

	return TRUE;
}

inline void CTigerGameDlg::InitUI(CRect rcClient)
{
	CRect rcTemp;
	int nStartX = 0;
	int nStartY = 0;
	CControlBase * pControlBase = NULL;

	int nXStartButton = 20;
	int nYStartButton = 60;

	// 加载背景
	pControlBase = new CPicture(GetSafeHwnd(), this, FRAME, rcClient);
	((CPicture *)pControlBase)->SetBitmap(IDB_WINDOWS_BACK);
	((CPicture *)pControlBase)->SetShowMode(enSMFrame, 3);
	m_vecArea.push_back(pControlBase);
	// 设置背景颜色
	//m_bAssignBackColor = true;
	//SetBackColor(RGB(224, 107, 87), true);
	// 加载样式
	LoadImage(IDB_SKIN_PIC_0);

	// 菜单
	nXStartButton = rcClient.right - 45 - 31 - 31;
	nYStartButton = 0;
	rcTemp.SetRect(nXStartButton, 0, nXStartButton + 31, 29);
	pControlBase = new CImageButton(GetSafeHwnd(), this, BT_MENU, rcTemp);
	((CImageButton *)pControlBase)->SetBitmap(IDB_BT_MENU);
	m_vecControl.push_back(pControlBase);

	// 切换风格
	nXStartButton -= 31;
	rcTemp.SetRect(nXStartButton, 0, nXStartButton + 31, 29);
	pControlBase = new CImageButton(GetSafeHwnd(), this, BT_SKIN, rcTemp);
	((CImageButton *)pControlBase)->SetBitmap(IDB_BT_SKIN);
	m_vecControl.push_back(pControlBase);

	// 状态栏
	nXStartButton = 10;
	nYStartButton = rcClient.bottom - 25;
	rcTemp.SetRect(nXStartButton, nYStartButton, nXStartButton + 200, nYStartButton + 16);
	pControlBase = new CStaticText(GetSafeHwnd(), this, TEXT_STATUS, rcTemp, TEXT("状态：等待连接中..."), Color(0, 0, 0), TEXT("微软雅黑"), 13);
	m_vecControl.push_back(pControlBase);
	
	// 主LOGO
	rcTemp.SetRect(3, 3, 3 + 125, 3 + 30);
	pControlBase = new CPicture(GetSafeHwnd(), this, PIC_LOGO, rcTemp);
	((CPicture *)pControlBase)->SetBitmap(IDB_PNG_LOGO);
	m_vecArea.push_back(pControlBase);

	// 中间选择的游戏名称
	nXStartButton = rcClient.left;
	nYStartButton = 140;
	rcTemp.SetRect(nXStartButton, nYStartButton, rcClient.right, nYStartButton + 30);
	pControlBase = new CStaticText(GetSafeHwnd(), this, TEXT_GAME_NAME, rcTemp, TEXT("绝地求生（东南亚）"), Color(0, 0, 0), TEXT("微软雅黑"), 16);
	((CStaticText *)pControlBase)->SetAlignment(StringAlignmentCenter);
	m_vecControl.push_back(pControlBase);

	// 中间游戏图标
	rcTemp.SetRect(104, 184, 104 + 153, 184 + 153);
	pControlBase = new CPicture(GetSafeHwnd(), this, PIC_MAIN_GAME, rcTemp);
	((CPicture *)pControlBase)->SetBitmap(IDB_GAME_PUBG);
	m_vecArea.push_back(pControlBase);

	// 选择游戏按钮
	nXStartButton = rcClient.left + 250;
	nYStartButton = 184;
	rcTemp.SetRect(nXStartButton, nYStartButton, nXStartButton + 28, nYStartButton + 28);
	pControlBase = new CImageButton(GetSafeHwnd(), this, BT_SELECT_GAME, rcTemp, false);
	((CImageButton *)pControlBase)->SetBitmap(IDB_BT_SELECT_GAME);
	m_vecControl.push_back(pControlBase);

	// 开始按钮
	nXStartButton = rcClient.left + 140;
	nYStartButton = 400;
	rcTemp.SetRect(nXStartButton, nYStartButton, nXStartButton + 80, nYStartButton + 80);
	pControlBase = new CImageButton(GetSafeHwnd(), this, BT_START, rcTemp, false);
	((CImageButton *)pControlBase)->SetBitmap(IDB_BT_START);
	m_vecControl.push_back(pControlBase);

	// 线路按钮
	nXStartButton = rcClient.left + 50;
	nYStartButton = 410;
	rcTemp.SetRect(nXStartButton, nYStartButton, nXStartButton + 60, nYStartButton + 60);
	pControlBase = new CImageButton(GetSafeHwnd(), this, BT_SELECT_SERVER, rcTemp, false);
	((CImageButton *)pControlBase)->SetBitmap(IDB_BT_LINE);
	m_vecControl.push_back(pControlBase);

	// 设置按钮
	nXStartButton = rcClient.left + 250;
	nYStartButton = 410;
	rcTemp.SetRect(nXStartButton, nYStartButton, nXStartButton + 60, nYStartButton + 60);
	pControlBase = new CImageButton(GetSafeHwnd(), this, BT_SETTING, rcTemp, false);
	((CImageButton *)pControlBase)->SetBitmap(IDB_BT_SETTING);
	m_vecControl.push_back(pControlBase);

	// TAB切换
	rcTemp.SetRect(10, 300, rcClient.right - 10, 300 + 30);
	pControlBase = new CTab(GetSafeHwnd(), this, TAB_SERVER_AREA, rcTemp);
	CTab *pTab = (CTab *)pControlBase;;
	// 分割线
	pTab->SetSeperator(true, IDB_TAB_SEPERATOR);
	pTab->InsertItem(-1, 1, TEXT("全国适用"), IDB_PNG_AREA_0);
	pTab->InsertItem(-1, 2, TEXT("华北地区"), IDB_PNG_AREA_1);
	pTab->InsertItem(-1, 2, TEXT("华南地区"), IDB_PNG_AREA_2);
	m_vecControl.push_back(pControlBase);
	/*
	rcTemp.SetRect(10, 150, rcClient.right - 10, 150 + 30);
	pControlBase = new CPicture(GetSafeHwnd(), this, TAB_SERVER_AREA, rcTemp);
	((CPicture *)pControlBase)->SetBitmap(IDB_LISTGRID_TITLE_BG);
	((CPicture *)pControlBase)->SetShowMode(enSMFrame);
	m_vecControl.push_back(pControlBase);
	*/
}

void CTigerGameDlg::OnSize(UINT nType, CRect rcClient)
{

	CControlBase * pControlBase = NULL;
	UINT uControlID = 0;
	CRect rc;
	CRect rcTemp;
	int nXStartButton = 0;
	int nYStartButton = 0;

	for (size_t i = 0; i < m_vecBaseArea.size(); i++)
	{
		pControlBase = m_vecBaseArea.at(i);		
		if (pControlBase)
		{
			//SetControlRect(pControlBase, rc);
		}
	}

	for (size_t i = 0; i < m_vecArea.size(); i++)
	{
		pControlBase = m_vecArea.at(i);
		if (pControlBase)
		{
			uControlID = pControlBase->GetControlID();
			{
				continue;
			}
			SetControlRect(pControlBase, rc);
		}
	}

	for (size_t i = 0; i < m_vecBaseControl.size(); i++)
	{
		pControlBase = m_vecBaseControl.at(i);
		if (pControlBase)
		{
			uControlID = pControlBase->GetControlID();			
		}
	}

	for (size_t i = 0; i < m_vecControl.size(); i++)
	{
		pControlBase = m_vecControl.at(i);
		if (pControlBase)
		{
			uControlID = pControlBase->GetControlID();
			if ( BT_MENU == uControlID )
			{
				//nXStartButton = rcClient.right - 45 - 31 - 31;
				//nYStartButton = 0;
				//rcTemp.SetRect(nXStartButton, 0, nXStartButton + 31, 29);

				rc.SetRect(rcClient.right - 107, 0, rcClient.right - 107 + 31, 29);
			}
			else if ( BT_SKIN == uControlID )
			{
				rc.SetRect(rcClient.right - 107 - 31, 0, rcClient.right - 107, 29);
			}
			else
			{
				continue;
			}
			SetControlRect(pControlBase, rc);
		}
	}

}

// 重置控件
void CTigerGameDlg::ResetControlBackHSL(COLOR_HSL colorHSL, COLOR_HSL OldColorHSL)
{
	if (colorHSL.luminance > 65 && OldColorHSL.luminance <= 65 || colorHSL.luminance <= 65 && OldColorHSL.luminance > 65)
	{
		/*
		CControlBase * pControlBase = GetControl(TEXT_TITLE_NAME);
		if (pControlBase != NULL)
		{
			((CStaticText *)pControlBase)->SetFontColor(colorHSL.luminance > 65 ? Color(254, 0, 0, 0) : Color(254, 255, 255, 255));
		}
		pControlBase = GetControl(TEXT_ARTIST_NAME);
		if (pControlBase != NULL)
		{
			((CStaticText *)pControlBase)->SetFontColor(colorHSL.luminance > 65 ? Color(200, 0, 0, 0) : Color(200, 255, 255, 255));
		}
		*/
// 
// 		pControlBase = GetControl(LIST_PLAY);
// 		if (pControlBase != NULL)
// 		{
// 			((CListEx *)pControlBase)->SetFontColor(colorHSL.luminance > 65 ? Color(200, 0, 0, 0) : Color(200, 255, 255, 255));
// 		}
	}
}

HCURSOR CTigerGameDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

LRESULT CTigerGameDlg::OnControlMessage(UINT uControlID, UINT uMsg, void *pData, size_t lSize)
{
	if(uControlID == BT_SKIN)
	{
		if(uMsg == BUTTOM_UP)
		{
			CControlBase * pControlBase = GetControl(BT_SKIN);
			if (pControlBase)
			{
				CDlgSkin *pDlgSkin = new CDlgSkin;
				CRect rc = pControlBase->GetRect();
				rc.OffsetRect(-95, rc.Height());
				ClientToScreen(&rc);
				pDlgSkin->Create(this, rc, WM_SKIN, IDB_SKIN_BKG, enBMImage);
				pDlgSkin->ShowWindow(SW_SHOW);
			}
		}
	}
	// 显示选择节点窗口
	else if (uControlID == BT_SELECT_SERVER)
	{
		if (uMsg == BUTTOM_UP)
		{
			ShowChildDialog(m_pDlgSelectServer, WM_SELECT_SERVER);
		}
	}
	// 显示选择游戏窗口
	else if (uControlID == BT_SELECT_GAME)
	{
		if (uMsg == BUTTOM_UP)
		{
			ShowChildDialog(m_pDlgSelectGame, WM_SELECT_GAME);
		}
	}
	// 显示高级设置窗口
	else if (uControlID == BT_SETTING)
	{
		if (uMsg == BUTTOM_UP)
		{
			ShowChildDialog(m_pDlgSetting, WM_SETTING);
		}
	}
	/*
	else if (uControlID == BT_USERINFO)
	{
		if (uMsg == BUTTOM_UP)
		{
			ShowChildDialog(m_pDlgUserInfo, WM_USERINFO);
		}
	}
	*/
	else if(uControlID == BT_MENU)
	{
		if(uMsg == BUTTOM_UP)
		{
			CControlBase * pControlBase = GetControl(BT_MENU);
			if(pControlBase)
			{
				CMenuEx *pMenuEx = new CMenuEx(TEXT("微软雅黑"), 12);
				CPoint point;
				CRect rc = pControlBase->GetRect();
				point.SetPoint(rc.left + rc.Width() / 2 - 10, rc.bottom);
				ClientToScreen(&point);
				pMenuEx->Create(this, point, WM_MIAN_MENU, IDB_MENUEX_BACK, 30);
				pMenuEx->AddMenu(TEXT("设置中心"), MENU_MAIN_SETING);
				pMenuEx->AddMenu(TEXT("检查更新"), MENU_MAIN_UPDATE, IDB_MENU_UPDATE);
				pMenuEx->AddSeparator();
				pMenuEx->AddMenu(TEXT("管家论坛"), MENU_MAIN_BBS, IDB_MENU_BBS);
				pMenuEx->AddMenu(TEXT("帮助"), MENU_MAIN_HELP);
				pMenuEx->AddMenu(TEXT("关于"), MENU_MAIN_ABOUT);
				pMenuEx->ShowWindow(SW_SHOW);
			}
		}
	}
	return 0;
}

LRESULT CTigerGameDlg::OnWindowMessage(UINT uControlID, UINT uMsg, void *pData, size_t lSize)
{
	if (uControlID == WM_SKIN)
	{
		OnMessageSkin(uMsg, pData, lSize);
	}
	else if (uControlID == WM_MIAN_MENU)
	{
		OnMessageMainMenu(uMsg, pData, lSize);
	}
	return 0;
}

void CTigerGameDlg::OnWindowMouseLeave()
{
	
}

void CTigerGameDlg::OnWindowMouseHover()
{
 	
}

// 增加托盘
void CTigerGameDlg::AddSystrayIcon()
{
	return;
	// 将图标放入系统托盘
	m_nd.cbSize = sizeof(NOTIFYICONDATA);
	m_nd.hWnd = GetSafeHwnd();
	m_nd.uID = IDR_MAINFRAME;
	m_nd.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
	m_nd.uCallbackMessage = MYWM_NOTIFYICON;
	m_nd.hIcon = m_hIcon;
	_tcscpy(m_nd.szTip, TEXT("加速器"));
	Shell_NotifyIcon(NIM_ADD, &m_nd);
}

// 删除托盘
void CTigerGameDlg::DelSystrayIcon()
{
	return;
	Shell_NotifyIcon(NIM_DELETE, &m_nd);
}

// 换肤
LRESULT CTigerGameDlg::OnMessageSkin(UINT uMsg, void *pData, size_t lSize)
{
	if (MSG_CLOSE == uMsg)
	{

	}
	else if (MSG_SKIN == uMsg)
	{		
		if (pData != NULL)
		{
			tagSkinInfo *pSkinInfo = (tagSkinInfo *)pData;
			if(pSkinInfo->nType == 1)
			{
				m_bAssignBackColor = false;
				DrawBackground(pSkinInfo->clr.ToCOLORREF());
			}
			else if(pSkinInfo->nType == 0)
			{
				m_bAssignBackColor = true;
				m_uBackImage = IDB_SKIN_PIC_0 + pSkinInfo->uIndex;
				SetBackColor(pSkinInfo->clr.ToCOLORREF(), true);
				LoadImage(IDB_SKIN_PIC_0 + pSkinInfo->uIndex, TEXT("JPG"));
			}
		}
	}
	else if (MSG_SELECT_SKIN == uMsg)
	{		
 		CFileDialog DlgFile(TRUE,NULL,NULL, OFN_OVERWRITEPROMPT | OFN_HIDEREADONLY ,
 			TEXT("皮肤主题(*.bmp;*.png;*.jpg;*.jpeg)|*.bmp;*.png;*.jpg;*.jpeg||"),this, 0, 0);
 		DlgFile.m_ofn.nFilterIndex=1;
 		DlgFile.m_ofn.hwndOwner=m_hWnd;
 		DlgFile.m_ofn.lStructSize=sizeof(OPENFILENAME);
 		DlgFile.m_ofn.lpstrTitle=TEXT("打开");
 		DlgFile.m_ofn.nMaxFile=MAX_PATH;
 		if(DlgFile.DoModal() == IDOK)
 		{
 			CString strFileName = DlgFile.GetPathName();
 			CString strFileType = strFileName.Right(3);
 			if (0 == strFileType.CompareNoCase(TEXT("bmp")) || 0 == strFileType.CompareNoCase(TEXT("jpg")) || 0 == strFileType.CompareNoCase(TEXT("png")))
 			{
 				LoadImage(strFileName);	
 			}
 		}
	}

	return 0;
}

// 主菜单
LRESULT CTigerGameDlg::OnMessageMainMenu(UINT uMsg, void *pData, size_t lSize)
{	
	if (uMsg == BUTTOM_UP)
	{
		if (pData != NULL)
		{
			tagMenuExInfo *pMenuExInfo = (tagMenuExInfo *)pData;
			if ( pMenuExInfo->uMenuID == MENU_MAIN_ABOUT )
			{
				if (m_pDlgAbout == NULL)
				{
					CDlgAbout AboutDlg(this);
					m_pDlgAbout = &AboutDlg;
					AboutDlg.SetBackColor(m_clrBK, false);
					AboutDlg.DoModal();
					m_pDlgAbout = NULL;
				}
				else
				{
					ShowTopWindow(m_pDlgAbout->GetSafeHwnd());
				}
			}
			else if (pMenuExInfo->uMenuID == MENU_MAIN_EXIT)
			{
				OnClose();
			}
		}
	}
	return 0;
}

void CTigerGameDlg::OnTimer(UINT uTimerID)
{
	if (m_uTimerSkin == uTimerID)
	{
		
	}

	CDlgBase::OnTimer(uTimerID);
}

LRESULT CTigerGameDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	return CDlgBase::WindowProc(message, wParam, lParam);
}

void CTigerGameDlg::OnMove(int x, int y)
{
	CDlgBase::OnMove(x, y);

	// TODO: 在此处添加消息处理程序代码
	// 移动窗口位置
	MoveChildDialog(m_pDlgSetting);
	MoveChildDialog(m_pDlgSelectGame);
	MoveChildDialog(m_pDlgSelectServer);
	MoveChildDialog(m_pDlgUserInfo);
}


BOOL CTigerGameDlg::DestroyWindow()
{
	// TODO: 在此添加专用代码和/或调用基类
	DeleteChildDialog(m_pDlgSetting);
	DeleteChildDialog(m_pDlgSelectGame);
	DeleteChildDialog(m_pDlgSelectServer);
	DeleteChildDialog(m_pDlgUserInfo);
	exit(0);
	return CDlgBase::DestroyWindow();
}

template <class T> void CTigerGameDlg::ShowChildDialog(T* &pDlg, int wmCode)
{
	if (pDlg == NULL )
	{
		pDlg = new T;
		CRect rc;
		GetClientRect(&rc);
		ClientToScreen(&rc);
		rc.left = rc.right + 2;
		rc.right += 360;
		rc.top += 40;
		pDlg->Create(this, rc, wmCode, IDB_DLGCHILD_BKG, enBMImage);
		pDlg->ShowWindow(SW_SHOW);
	}
	else
	{
		CRect rc;
		GetClientRect(&rc);
		ClientToScreen(&rc);
		// 改变位置
		pDlg->SetWindowPos(this, rc.right + 2, rc.top + 40, 360, 520, NULL);
		pDlg->ShowWindow(!pDlg->IsWindowVisible());
	}
	HideDlgAll(pDlg);
}

template <class T> void CTigerGameDlg::HideChildDialog(T* &pDlg)
{
	if (pDlg != NULL)
	{
		pDlg->ShowWindow(SW_HIDE);
	}
}

template <class T> void CTigerGameDlg::HideDlgAll(T* &pDlg)
{
	if (m_pDlgSetting != NULL && pDlg != NULL)
	{
		if (*m_pDlgSetting != *pDlg) {
			HideChildDialog(m_pDlgSetting);
		}
	}
	if (m_pDlgSelectGame != NULL && pDlg != NULL)
	{
		if (*m_pDlgSelectGame != *pDlg) {
			HideChildDialog(m_pDlgSelectGame);
		}
	}
	if (m_pDlgSelectServer != NULL && pDlg != NULL)
	{
		if (*m_pDlgSelectServer != *pDlg) {
			HideChildDialog(m_pDlgSelectServer);
		}
	}
	if (m_pDlgUserInfo != NULL && pDlg != NULL)
	{
		if (*m_pDlgUserInfo != *pDlg) {
			HideChildDialog(m_pDlgUserInfo);
		}
	}
}

template <class T> void CTigerGameDlg::DeleteChildDialog(T* &pDlg)
{
	if (pDlg != NULL) {
		delete pDlg;
		pDlg = NULL;
	}
}

template <class T> void CTigerGameDlg::MoveChildDialog(T* &pDlg)
{
	if (pDlg != NULL) {
		if (pDlg->IsWindowVisible()) {
			CRect rc;
			GetClientRect(&rc);
			ClientToScreen(&rc);
			// 改变位置
			pDlg->SetWindowPos(this, rc.right + 2, rc.top + 40, 360, 520, NULL);
		}
	}
}