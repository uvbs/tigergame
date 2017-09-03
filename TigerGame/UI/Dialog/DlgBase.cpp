

#include "StdAfx.h"
#include <winuser.h>
#include <Windows.h>
#include <Windowsx.h>
#include <shlwapi.h>
#include <cmath>
#include "DlgBase.h"
#include "..\..\Resource.h"

#define TIMER_ANIMATION_ID		1

IMPLEMENT_DYNAMIC(CDlgBase, CDialog)

CDlgBase::CDlgBase(UINT nIDTemplate, CWnd* pParent /*=NULL*/)
	: CDialog(nIDTemplate, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_MinSize.SetSize(320, 160);
	m_MaxSize.SetSize(0, 0);
	m_bMinBotton = true;
	m_bOverRegio = true;
	m_bWindowMove = true;
	m_bChangeSize = false;
	m_bShowTaskbar = true;
	m_bInit = false;
	m_bAssignBackColor = false;

	m_nFrameTopBottomSpace = 3;
	m_nFrameLeftRightSpace = 3;

	m_nOverRegioX = 100;
	m_nOverRegioY = 100;
	m_sizeBKImage.cx = 100;
	m_sizeBKImage.cy = 100;

	m_bTracking = false;
	m_bIsSetCapture = false;
	m_clrBK = RGB(186, 226, 239);
	m_bDrawImage = false;

	m_bLButtonDown = false;
	m_bRButtonDown = false;
	m_bIsLButtonDblClk = false;
	m_pOldMemBK = NULL;
	m_pControl = NULL;
	m_pFocusControl = NULL;
	m_uWindowID = 0;
	m_hWndMsg = NULL;
	m_bChild = false;

	m_strPath = GetCurrentDirectory();
}

CDlgBase::CDlgBase(CWnd* pParent, UINT uWindowID)
	: CDialog(IDD_ICE_MUSIC_DIALOG, pParent)
{
	m_uWindowID = uWindowID;
	m_hWndMsg = pParent->GetSafeHwnd();

	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_MinSize.SetSize(320, 160);
	m_MaxSize.SetSize(0, 0);
	m_bMinBotton = true;
	m_bOverRegio = true;
	m_bWindowMove = true;
	m_bChangeSize = false;
	m_bShowTaskbar = true;
	m_bInit = false;
	m_bAssignBackColor = false;

	m_nFrameTopBottomSpace = 3;
	m_nFrameLeftRightSpace = 3;

	m_nOverRegioX = 100;
	m_nOverRegioY = 100;
	m_sizeBKImage.cx = 100;
	m_sizeBKImage.cy = 100;

	m_bTracking = false;
	m_bIsSetCapture = false;
	m_clrBK = RGB(186, 226, 239);
	m_bDrawImage = false;

	m_bLButtonDown = false;
	m_bRButtonDown = false;
	m_bIsLButtonDblClk = false;
	m_pOldMemBK = NULL;
	m_pControl = NULL;
	m_pFocusControl = NULL;
	m_uWindowID = 0;
	m_hWndMsg = NULL;

	m_strPath = GetCurrentDirectory();

}

bool CDlgBase::Create(CWnd* pParent, HWND hWndMsg, UINT uWindowID, bool bChild/* = false*/)
{
	m_hWndMsg = hWndMsg;
	m_uWindowID = uWindowID;
	m_bChild = bChild;
	return CDialog::Create(IDD_ICE_MUSIC_DIALOG, pParent);
}

CDlgBase::~CDlgBase()
{
	for (size_t i = 0; i < m_vecControl.size(); i++)
	{
		CControlBase * pControlBase = m_vecControl.at(i);
		if (pControlBase)
		{
			delete pControlBase;
		}
	}

	for (size_t i = 0; i < m_vecBaseControl.size(); i++)
	{
		CControlBase * pControlBase = m_vecBaseControl.at(i);
		if (pControlBase)
		{
			delete pControlBase;
		}
	}

	for (size_t i = 0; i < m_vecArea.size(); i++)
	{
		CControlBase * pControlBase = m_vecArea.at(i);
		if (pControlBase)
		{
			delete pControlBase;
		}
	}

	for (size_t i = 0; i < m_vecBaseArea.size(); i++)
	{
		CControlBase * pControlBase = m_vecBaseArea.at(i);
		if (pControlBase)
		{
			delete pControlBase;
		}
	}

	if (m_hIcon)
	{
		DestroyIcon(m_hIcon);
		m_hIcon = NULL;
	}

	if (m_BKImage.m_hObject != NULL)
	{
		m_BKImage.DeleteObject();
		m_MemBKDC.SelectObject(m_pOldMemBK);
		m_MemBK.DeleteObject();
	}

	m_Rgn.DeleteObject();
}

BEGIN_MESSAGE_MAP(CDlgBase, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_NCPAINT()
	ON_WM_ERASEBKGND()
	ON_WM_NCHITTEST()
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_NCCALCSIZE()
	ON_WM_GETMINMAXINFO()
	ON_WM_WINDOWPOSCHANGING()
	ON_WM_NCDESTROY()
	ON_WM_CLOSE()
	ON_WM_NCACTIVATE()
	ON_WM_MOUSEMOVE()
	ON_MESSAGE(WM_MOUSELEAVE, OnMouseLeave)
	ON_MESSAGE(WM_MOUSEHOVER, OnMouseHover)
	ON_WM_SYSCOMMAND()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_DROPFILES()
	ON_WM_DESTROY()
	ON_WM_SETCURSOR()
	ON_MESSAGE(CONTROL_MESSAGE, OnBaseControlMessage)
	ON_MESSAGE(WINDOW_MESSAGE, OnBaseWindowMessage)

	ON_WM_MOUSEWHEEL()
	ON_WM_TIMER()
END_MESSAGE_MAP()

// 设置最小窗口大小
void CDlgBase::SetMinSize(int iWidth, int iHeight)
{
	m_MinSize.cx = iWidth;
	m_MinSize.cy = iHeight;
}

CSize CDlgBase::GetMinSize()
{
	return	m_MinSize;
}

void CDlgBase::SetBackColor(COLORREF clr, bool bDrawImage)
{
	m_clrBK = clr;
	m_bDrawImage = bDrawImage;
}

void CDlgBase::OnGetMinMaxInfo(MINMAXINFO* lpMMI)
{
	CDialog::OnGetMinMaxInfo(lpMMI);
	lpMMI->ptMinTrackSize.x = m_MinSize.cx;
	lpMMI->ptMinTrackSize.y = m_MinSize.cy;

	CRect	rc(0, 0, 1024, 768);
	SystemParametersInfo(SPI_GETWORKAREA, 0, &rc, 0);
	lpMMI->ptMaxPosition.x = rc.left;
	lpMMI->ptMaxPosition.y = rc.top;
	lpMMI->ptMaxSize.x = rc.Width();
	lpMMI->ptMaxSize.y = rc.Height();
}

// CDlgBase message handlers

BOOL CDlgBase::OnInitDialog()
{
	CDialog::OnInitDialog();

	if (m_MaxSize.cx == 0 || m_MaxSize.cy == 0)
	{
		m_MaxSize.SetSize(GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN));
	}

	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	//SetWindowText(TEXT("Ice音乐播放器"));

	::SetWindowPos(m_hWnd, NULL, 0, 0, m_MinSize.cx, m_MinSize.cy, SWP_HIDEWINDOW | SWP_NOMOVE);

	CRect rc(0, 0, m_MinSize.cx, m_MinSize.cy);
	InitBaseUI(rc);
	InitUI(rc);

	CenterWindow();
	ShowWindow(SW_SHOW);

	//启动定时器
	SetTimer(TIMER_ANIMATION_ID, 30, NULL);

	srand((unsigned)time(NULL));

	m_bInit = true;

	return TRUE;
}

void CDlgBase::InitBaseUI(CRect rcClient)
{
	CRect rcTemp;
	CControlBase * pControlBase = NULL;

	// 关闭按钮
	int nStartButton = rcClient.right - 45;
	rcTemp.SetRect(nStartButton, 0, nStartButton + 45, 29);
	pControlBase = new CImageButton(GetSafeHwnd(), this, BT_CLOSE, rcTemp);
	((CImageButton *)pControlBase)->SetBitmap(IDB_BT_CLOSE);
	m_vecBaseControl.push_back(pControlBase);
	// 最小化按钮
	if (m_bMinBotton)
	{
		nStartButton -= 31;
		rcTemp.SetRect(nStartButton, 0, nStartButton + 31, 29);
		pControlBase = new CImageButton(GetSafeHwnd(), this, BT_MIN, rcTemp);
		((CImageButton *)pControlBase)->SetBitmap(IDB_BT_MIN);
		m_vecBaseControl.push_back(pControlBase);
	}
}

CControlBase *CDlgBase::GetControl(UINT uControlID)
{
	for (size_t i = 0; i < m_vecControl.size(); i++)
	{
		CControlBase * pControlBase = m_vecControl.at(i);
		if (pControlBase)
		{
			if (pControlBase->GetControlID() == uControlID)
			{
				return pControlBase;
			}
		}
	}

	for (size_t i = 0; i < m_vecArea.size(); i++)
	{
		CControlBase * pControlBase = m_vecArea.at(i);
		if (pControlBase)
		{
			if (pControlBase->GetControlID() == uControlID)
			{
				return pControlBase;
			}
		}
	}

	return GetBaseControl(uControlID);
}

CControlBase *CDlgBase::GetBaseControl(UINT uControlID)
{
	for (size_t i = 0; i < m_vecBaseControl.size(); i++)
	{
		CControlBase * pControlBase = m_vecBaseControl.at(i);
		if (pControlBase)
		{
			if (pControlBase->GetControlID() == uControlID)
			{
				return pControlBase;
			}
		}
	}

	for (size_t i = 0; i < m_vecBaseArea.size(); i++)
	{
		CControlBase * pControlBase = m_vecBaseArea.at(i);
		if (pControlBase)
		{
			if (pControlBase->GetControlID() == uControlID)
			{
				return pControlBase;
			}
		}
	}

	return NULL;
}

int CDlgBase::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	// 设置窗口风格
	DWORD dwStyle = 0;
	if (m_bChild)
	{
		dwStyle = ::GetWindowLong(m_hWnd, GWL_STYLE)
			| WS_CHILD;
		dwStyle &= ~(WS_POPUP);
		dwStyle &= ~(WS_BORDER);
		SetParent(GetParent());
	}
	else
	{
		dwStyle = ::GetWindowLong(m_hWnd, GWL_STYLE)
			| WS_MAXIMIZEBOX | WS_MINIMIZEBOX
			| WS_SYSMENU | WS_SIZEBOX | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;
	}

	dwStyle &= ~(WS_CAPTION);

	// 改变窗口大小
	if (!m_bChangeSize)
	{
		dwStyle &= ~(WS_THICKFRAME);
	}

	// 任务栏显示
	if (!m_bShowTaskbar)
	{
		ModifyStyleEx(WS_EX_APPWINDOW, WS_EX_TOOLWINDOW);
	}

	::SetWindowLong(m_hWnd, GWL_STYLE, dwStyle);

	if (m_hWndMsg == NULL)
	{
		m_hWndMsg = GetParent()->GetSafeHwnd();
	}

	return CDialog::OnCreate(lpCreateStruct);
}

void CDlgBase::OnDropFiles(HDROP hDropInfo)
{
	TCHAR szFileName[MAX_PATH + 1] = { 0 };
	UINT nFiles = DragQueryFile(hDropInfo, 0xFFFFFFFF, NULL, 0);
	for (UINT i = 0; i < nFiles; i++)
	{
		DragQueryFile(hDropInfo, i, szFileName, MAX_PATH);
		if (PathIsDirectory(szFileName))
		{
			continue;
		}
		CString strFileName = szFileName;
		strFileName = strFileName.Right(3);
		if (0 == strFileName.CompareNoCase(TEXT("bmp")) || 0 == strFileName.CompareNoCase(TEXT("jpg")) || 0 == strFileName.CompareNoCase(TEXT("png")))
		{
			m_bAssignBackColor = false;
			LoadImage(szFileName);
			break;
		}
	}

	// CDialog::OnDropFiles(hDropInfo);
}


void CDlgBase::LoadImage(UINT nIDResource, CString strType)
{
	m_strImageFile = TEXT("");
	CBitmap bitBackground;
	::LoadImage(nIDResource, bitBackground, m_sizeBKImage, strType);
	DrawBackground(bitBackground);
}

void CDlgBase::LoadImage(CString strFileName)
{
	CBitmap bitBackground;
	::LoadImage(strFileName, bitBackground, m_sizeBKImage);
	m_strImageFile = strFileName;
	DrawBackground(bitBackground);
}

// 画背景图片
void CDlgBase::DrawBackground(CBitmap &bitBackground)
{
	if ((m_MaxSize.cx - 2 > m_sizeBKImage.cx || m_MaxSize.cy - 2 > m_sizeBKImage.cy) && m_bOverRegio)
	{
		if (m_MaxSize.cx - 2 > m_sizeBKImage.cx)
		{
			m_nOverRegioX = __min(100, m_sizeBKImage.cx - 2);
		}
		else
		{
			m_nOverRegioX = 0;
		}

		if (m_MaxSize.cy - 2 > m_sizeBKImage.cy)
		{
			m_nOverRegioY = __min(100, m_sizeBKImage.cy - 2);
		}
		else
		{
			m_nOverRegioY = 0;
		}
	}
	else
	{
		m_nOverRegioX = 0;
		m_nOverRegioY = 0;
	}

	m_bDrawImage = bitBackground.m_hObject;

	if (bitBackground.m_hObject)
	{
		int nWidth = m_sizeBKImage.cx;
		int nHeight = m_sizeBKImage.cy;

		if (m_MemBKDC.m_hDC)
		{
			m_MemBKDC.DeleteDC();
		}

		CDC *pDC = GetDC();

		if (!m_bAssignBackColor)
		{
			::GetAverageColor(pDC, bitBackground, m_sizeBKImage, m_clrBK);
		}

		m_MemBKDC.CreateCompatibleDC(pDC);

		if (m_MemBK.m_hObject)
		{
			m_MemBK.DeleteObject();
		}
		m_MemBK.CreateCompatibleBitmap(pDC, m_sizeBKImage.cx, m_sizeBKImage.cy);
		m_pOldMemBK = m_MemBKDC.SelectObject(&m_MemBK);

		CDC TempDC;
		TempDC.CreateCompatibleDC(pDC);

		CBitmap* pOldBitmap = TempDC.SelectObject(&bitBackground);

		// 画出平均图片
		m_MemBKDC.FillSolidRect(0, 0, nWidth, nHeight, m_clrBK);

		if (m_nOverRegioX > 0 && m_nOverRegioY > 0)
		{
			int nOverRegio = __min(m_nOverRegioX, m_nOverRegioY);

			// 左上
			m_MemBKDC.BitBlt(0, 0, nWidth - nOverRegio, nHeight - nOverRegio, &TempDC, 0, 0, SRCCOPY);

			// 中间
			CRect rc(0, 0, nWidth, nHeight);
			DrawRightBottomTransition(m_MemBKDC, TempDC, rc, nOverRegio, m_clrBK);

			// 右上
			rc.SetRect(nWidth - nOverRegio, 0, nWidth, nHeight - nOverRegio);
			DrawHorizontalTransition(m_MemBKDC, TempDC, rc, rc, 0, 100);

			// 左下
			rc.SetRect(0, nHeight - nOverRegio, nWidth - nOverRegio, nHeight);
			DrawVerticalTransition(m_MemBKDC, TempDC, rc, rc, 0, 100);
		}
		else if (m_nOverRegioX > 0 && m_nOverRegioY == 0)
		{
			// 左边
			m_MemBKDC.BitBlt(0, 0, nWidth - m_nOverRegioX, nHeight, &TempDC, 0, 0, SRCCOPY);

			// 右边
			CRect rc(nWidth - m_nOverRegioX, 0, nWidth, nHeight);
			DrawHorizontalTransition(m_MemBKDC, TempDC, rc, rc, 0, 100);
		}
		else if (m_nOverRegioX == 0 && m_nOverRegioY > 0)
		{
			// 边上
			m_MemBKDC.BitBlt(0, 0, nWidth, nHeight - m_nOverRegioY, &TempDC, 0, 0, SRCCOPY);

			// 下边
			CRect rc(0, nHeight - m_nOverRegioY, nWidth, nHeight);
			DrawVerticalTransition(m_MemBKDC, TempDC, rc, rc, 0, 100);
		}
		else
		{
			m_MemBKDC.BitBlt(0, 0, nWidth, nHeight, &TempDC, 0, 0, SRCCOPY);
		}

		TempDC.SelectObject(pOldBitmap);
		TempDC.DeleteDC();
		ReleaseDC(pDC);

		ResetControl();
	}
}

// 画背景图片
void CDlgBase::DrawBackground(COLORREF clr)
{
	if (m_clrBK != clr)
	{
		m_clrBK = clr;
		m_bDrawImage = FALSE;
		ResetControl();
	}
}

// 画背景图片
void CDlgBase::DrawBackgroundColor(Image *&pImage)
{
	COLORREF clr;
	CDC *pDC = GetDC();
	GetAverageColor(pDC, pImage, clr);
	ReleaseDC(pDC);
	DrawBackground(clr);
}

// 画背景图片
void CDlgBase::DrawBackgroundColor(CBitmap &bitBackground)
{
	COLORREF clr;
	CDC *pDC = GetDC();
	BITMAP bmInfo;
	::GetObject(bitBackground.m_hObject, sizeof(BITMAP), &bmInfo);
	CSize sizeBitmap(bmInfo.bmWidth, bmInfo.bmHeight);
	GetAverageColor(pDC, bitBackground, sizeBitmap, clr);
	ReleaseDC(pDC);
	DrawBackground(clr);
}

BOOL CDlgBase::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
}

HCURSOR CDlgBase::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CDlgBase::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CRect rcUpdate;
		GetUpdateRect(&rcUpdate);

		CRect	rcClient;
		GetClientRect(&rcClient);

		CPaintDC	dc(this);
		CDC MemDC;
		MemDC.CreateCompatibleDC(&dc);
		CBitmap memBmp;
		memBmp.CreateCompatibleBitmap(&dc, rcUpdate.Width(), rcUpdate.Height());
		CBitmap *pOldmap = MemDC.SelectObject(&memBmp);

		DrawImageStyle(MemDC, rcClient, rcUpdate);

		dc.BitBlt(rcUpdate.left, rcUpdate.top, rcUpdate.Width(), rcUpdate.Height(), &MemDC, 0, 0, SRCCOPY);

		MemDC.SelectObject(pOldmap);
		MemDC.DeleteDC();
		memBmp.DeleteObject();
	}
}


void CDlgBase::DrawImageStyle(CDC &dc, const CRect &rcClient, const CRect &rcUpdate)
{
	dc.FillSolidRect(0, 0, rcUpdate.Width(), rcUpdate.Height(), m_clrBK);

	if (m_bDrawImage)
	{
		// 背景
		CRect rcBk(1, 1, 1 + m_sizeBKImage.cx, 1 + m_sizeBKImage.cy);
		CRect rc = rcBk & rcUpdate;
		if (!rc.IsRectEmpty())
		{
			int nDestDrawX = rcBk.left > rcUpdate.left ? rcBk.left - rcUpdate.left : 0;
			int nDestDrawY = rcBk.top > rcUpdate.top ? rcBk.top - rcUpdate.top : 0;
			int nSrcDrawX = rcBk.left >= rcUpdate.left ? 0 : rcUpdate.left - rcBk.left;
			int nSrcDrawY = rcBk.top >= rcUpdate.top ? 0 : rcUpdate.top - rcBk.top;

			dc.BitBlt(nDestDrawX, nDestDrawY, rc.Width(), rc.Height(), &m_MemBKDC, nSrcDrawX, nSrcDrawY, SRCCOPY);
		}
	}

	// 控件
	DrawControl(dc, rcClient, rcUpdate);
}

// 重置控件
void CDlgBase::ResetControl()
{
	for (size_t i = 0; i < m_vecArea.size(); i++)
	{
		CControlBase * pControlBase = m_vecArea.at(i);
		if (pControlBase)
		{
			pControlBase->SetBackColor(m_clrBK);
		}
	}

	for (size_t i = 0; i < m_vecBaseArea.size(); i++)
	{
		CControlBase * pControlBase = m_vecBaseArea.at(i);
		if (pControlBase)
		{
			pControlBase->SetBackColor(m_clrBK);
		}
	}

	for (size_t i = 0; i < m_vecControl.size(); i++)
	{
		CControlBase * pControlBase = m_vecControl.at(i);
		if (pControlBase)
		{
			pControlBase->SetBackColor(m_clrBK);
		}
	}

	for (size_t i = 0; i < m_vecBaseControl.size(); i++)
	{
		CControlBase * pControlBase = m_vecBaseControl.at(i);
		if (pControlBase)
		{
			pControlBase->SetBackColor(m_clrBK);
		}
	}

	COLOR_HSL OldColorHSL = m_hslBK;

	RGBtoHSL(m_clrBK, &m_hslBK);
	ResetBackHSL(m_hslBK, OldColorHSL);
	InvalidateRect(NULL);
}

// 重置控件
void CDlgBase::ResetBackHSL(COLOR_HSL colorHSL, COLOR_HSL OldColorHSL)
{
	ResetControlBackHSL(colorHSL, OldColorHSL);
}

// 控件位置
CRect CDlgBase::GetControlRect(UINT uControlID)
{
	CControlBase *pControlBase = GetControl(uControlID);
	if (pControlBase)
	{
		return pControlBase->GetRect();
	}

	return CRect(0, 0, 0, 0);
}

// 移动控件
CControlBase * CDlgBase::SetControlRect(UINT uControlID, CRect rc)
{
	CControlBase *pControlBase = GetControl(uControlID);
	if (pControlBase)
	{
		pControlBase->SetRect(rc);
		UpdateHover();
	}
	return pControlBase;
}

// 移动控件
CControlBase * CDlgBase::SetControlRect(CControlBase *pControlBase, CRect rc)
{
	if (pControlBase)
	{
		pControlBase->SetRect(rc);
		UpdateHover();
	}
	return pControlBase;
}

// 显示控件
CControlBase * CDlgBase::SetControlVisible(UINT uControlID, bool bVisible)
{
	CControlBase *pControlBase = GetControl(uControlID);
	if (pControlBase)
	{
		pControlBase->SetVisible(bVisible);
		UpdateHover();
	}
	return pControlBase;
}

// 显示控件
CControlBase * CDlgBase::SetControlVisible(CControlBase *pControlBase, bool bVisible)
{
	if (pControlBase)
	{
		pControlBase->SetVisible(bVisible);
		UpdateHover();
	}
	return pControlBase;
}

// 禁用控件
CControlBase * CDlgBase::SetControlDisable(UINT uControlID, bool bDisable)
{
	CControlBase *pControlBase = GetControl(uControlID);
	if (pControlBase)
	{
		pControlBase->SetDisable(bDisable);
		UpdateHover();
	}
	return pControlBase;
}

// 禁用控件
CControlBase * CDlgBase::SetControlDisable(CControlBase *pControlBase, bool bDisable)
{
	if (pControlBase)
	{
		pControlBase->SetDisable(bDisable);
		UpdateHover();
	}
	return pControlBase;
}

// 更新选中
void CDlgBase::UpdateHover()
{
	CPoint point;
	GetCursorPos(&point);
	OnMouseMove(0xFFFFFFFF, point);
}

void CDlgBase::DrawControl(CDC &dc, const CRect &rcClient, const CRect &rcUpdate)
{
	for (size_t i = 0; i < m_vecArea.size(); i++)
	{
		CControlBase * pControlBase = m_vecArea.at(i);
		if (pControlBase)
		{
			pControlBase->Draw(dc, rcClient, rcUpdate);
		}
	}

	for (size_t i = 0; i < m_vecBaseArea.size(); i++)
	{
		CControlBase * pControlBase = m_vecBaseArea.at(i);
		if (pControlBase)
		{
			pControlBase->Draw(dc, rcClient, rcUpdate);
		}
	}

	for (size_t i = 0; i < m_vecControl.size(); i++)
	{
		CControlBase * pControlBase = m_vecControl.at(i);
		if (pControlBase)
		{
			pControlBase->Draw(dc, rcClient, rcUpdate);
		}
	}

	for (size_t i = 0; i < m_vecBaseControl.size(); i++)
	{
		CControlBase * pControlBase = m_vecBaseControl.at(i);
		if (pControlBase)
		{
			pControlBase->Draw(dc, rcClient, rcUpdate);
		}
	}
}

void CDlgBase::OnNcPaint()
{
}

LRESULT CDlgBase::OnNcHitTest(CPoint point)
{
	// 不能改变窗口大小
	if (!m_bChangeSize)
	{
		return	HTCLIENT;
	}
	CRect	rc;
	GetWindowRect(rc);
	rc.OffsetRect(-rc.left, -rc.top);
	ScreenToClient(&point);
	int x = point.x;
	int y = point.y;

	if (x < m_nFrameLeftRightSpace && y < m_nFrameTopBottomSpace)
	{
		return	HTTOPLEFT;
	}
	if (x <= m_nFrameLeftRightSpace && y >= rc.bottom - m_nFrameTopBottomSpace)
	{
		return	HTBOTTOMLEFT;
	}
	if (x > rc.right - m_nFrameLeftRightSpace && y < m_nFrameTopBottomSpace)
	{
		return	HTTOPRIGHT;
	}
	if (x >= rc.right - m_nFrameLeftRightSpace && y >= rc.bottom - m_nFrameTopBottomSpace)
	{
		return	HTBOTTOMRIGHT;
	}

	if (x < m_nFrameLeftRightSpace)
	{
		return	HTLEFT;
	}
	if (x >= rc.right - m_nFrameLeftRightSpace)
	{
		return	HTRIGHT;
	}
	if (y < m_nFrameTopBottomSpace)
	{
		return	HTTOP;
	}
	if (y > rc.bottom - m_nFrameTopBottomSpace)
	{
		return	HTBOTTOM;
	}
	// 	if ( y <= m_nFrameTopSpace)
	// 	{
	// 		return	HTCAPTION;
	// 	}
	return	HTCLIENT;
	//	return CDialog::OnNcHitTest(point);
}

void CDlgBase::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	CRect rc;
	GetClientRect(&rc);

	if (!IsIconic())
	{
		BOOL bIsMaximize = IsZoomed() || rc.Width() == GetSystemMetrics(SM_CXSCREEN) && rc.Height() == GetSystemMetrics(SM_CYSCREEN);
		int border_offset[] = {/*3, 2, */1 };
		if (bIsMaximize)
		{
			SetupRegion(border_offset, 0);
			m_nFrameLeftRightSpace = m_nFrameTopBottomSpace = 0;
		}
		else
		{
			SetupRegion(border_offset, 0/*3*/);
			m_nFrameLeftRightSpace = m_nFrameTopBottomSpace = 3;
		}

		CControlBase *pControlBase = GetBaseControl(FRAME);
		if (pControlBase)
		{
			pControlBase->SetVisible(!bIsMaximize);
		}

		OnBaseSize(nType, rc);
		OnSize(nType, rc);
	}
	m_rc = rc;
	InvalidateRect(NULL);
}

void CDlgBase::OnBaseSize(UINT nType, CRect rcClient)
{
	for (size_t i = 0; i < m_vecBaseArea.size(); i++)
	{
		CControlBase * pControlBase = m_vecBaseArea.at(i);
		CRect rcTemp;
		if (pControlBase)
		{
			UINT uControlID = pControlBase->GetControlID();

			if (FRAME == uControlID)
			{
				rcTemp = rcClient;
			}
			else
			{
				continue;
			}
			SetControlRect(pControlBase, rcTemp);
		}
	}
	for (size_t i = 0; i < m_vecBaseControl.size(); i++)
	{
		CControlBase * pControlBase = m_vecBaseControl.at(i);
		CRect rcTemp;
		if (pControlBase)
		{
			UINT uControlID = pControlBase->GetControlID();
			if (BT_CLOSE == uControlID)
			{
				rcTemp.SetRect(rcClient.right - 45, 0, rcClient.right, 29);
			}
			else if (BT_MIN == uControlID)
			{
				rcTemp.SetRect(rcClient.right - 45 - 31, 0, rcClient.right - 45, 29);
			}
			else
			{
				continue;
			}
			SetControlRect(pControlBase, rcTemp);
		}
	}
}

// 设置窗口区域
void CDlgBase::SetupRegion(int border_offset[], int nSize)
{
	CDC* pDC = GetDC();

	CRect	rc;
	GetWindowRect(rc);
	rc.OffsetRect(-rc.left, -rc.top);

	CRgn	rgn;
	rgn.CreateRectRgn(0, 0, rc.Width(), rc.Height());
	CRgn	rgn_xor;
	CRect	rcXor;

	for (int y = 0; y < nSize; ++y)
	{
		rcXor.SetRect(0, y, border_offset[y], y + 1);
		rgn_xor.CreateRectRgn(0, y, border_offset[y], y + 1);
		rgn.CombineRgn(&rgn, &rgn_xor, RGN_XOR);
		rgn_xor.DeleteObject();
	}

	for (int y = 0; y < nSize; ++y)
	{
		rcXor.SetRect(rc.right - border_offset[y], y, rc.right, y + 1);
		rgn_xor.CreateRectRgn(rc.right - border_offset[y], y, rc.right, y + 1);
		rgn.CombineRgn(&rgn, &rgn_xor, RGN_XOR);
		rgn_xor.DeleteObject();
	}

	for (int y = 0; y < nSize; ++y)
	{
		rcXor.SetRect(0, rc.bottom - y - 1, border_offset[y], rc.bottom - y);
		rgn_xor.CreateRectRgn(0, rc.bottom - y - 1, border_offset[y], rc.bottom - y);
		rgn.CombineRgn(&rgn, &rgn_xor, RGN_XOR);
		rgn_xor.DeleteObject();
	}

	for (int y = 0; y < nSize; ++y)
	{
		rcXor.SetRect(rc.right - border_offset[y], rc.bottom - y - 1, rc.right, rc.bottom - y);
		rgn_xor.CreateRectRgn(rc.right - border_offset[y], rc.bottom - y - 1, rc.right, rc.bottom - y);
		rgn.CombineRgn(&rgn, &rgn_xor, RGN_XOR);
		rgn_xor.DeleteObject();
	}
	// 	HWND hWnd = GetSafeHwnd();
	// 	SetWindowLong(hWnd,GWL_EXSTYLE,GetWindowLong(hWnd,GWL_EXSTYLE) | WS_EX_LAYERED);
	// 	SetLayeredWindowAttributes(RGB(255, 0, 255), 0, LWA_COLORKEY );	

	SetWindowRgn((HRGN)rgn, TRUE);
	m_Rgn.DeleteObject();
	m_Rgn.Attach(rgn.Detach());
	ReleaseDC(pDC);
}

void CDlgBase::OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS* lpncsp)
{
	CDialog::OnNcCalcSize(bCalcValidRects, lpncsp);

	CRect	rcWindow;

	if (bCalcValidRects && lpncsp->lppos)
	{
		rcWindow.SetRect(lpncsp->lppos->x, lpncsp->lppos->y,
			lpncsp->lppos->x + lpncsp->lppos->cx,
			lpncsp->lppos->y + lpncsp->lppos->cy);
	}
	else
	{
		GetWindowRect(rcWindow);
	}

	lpncsp->rgrc[0] = rcWindow;

}

void CDlgBase::OnWindowPosChanging(WINDOWPOS* lpwndpos)
{
	if (lpwndpos->cx < m_MinSize.cx)
	{
		lpwndpos->cx = m_MinSize.cx;
	}
	if (lpwndpos->cy < m_MinSize.cy)
	{
		lpwndpos->cy = m_MinSize.cy;
	}
}

void CDlgBase::OnWindowPosChanged(WINDOWPOS* lpwndpos)
{
	CDialog::OnWindowPosChanged(lpwndpos);
}

void CDlgBase::PostNcDestroy()
{
}

void CDlgBase::OnClose()
{
	OnCancel();
	PostMessageEx(MSG_CLOSE);
}

void CDlgBase::OnMinimize()
{
	ShowWindow(m_bShowTaskbar ? SW_MINIMIZE : SW_HIDE);
}

BOOL CDlgBase::OnMaximize()
{
	if (IsZoomed())
	{
		ShowWindow(SW_RESTORE);
		return FALSE;
	}
	else
	{
		ShowWindow(SW_SHOWMAXIMIZED);
		return TRUE;
	}
}

void CDlgBase::ShowTopWindow(HWND hWnd)
{
	::PostMessage(hWnd, WM_SYSCOMMAND, SC_RESTORE, 0);
	bool bTop = ::GetWindowLong(hWnd, GWL_EXSTYLE) & WS_EX_TOPMOST;
	::SetWindowPos(hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
	if (!bTop)
	{
		::SetWindowPos(hWnd, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
	}
}

LRESULT CDlgBase::OnMessageButtomMin(UINT uMsg, void *pData, size_t lSize)
{
	if (BUTTOM_UP == uMsg)
	{
		OnMinimize();
	}

	return 0;
}

LRESULT CDlgBase::OnMessageButtomClose(UINT uMsg, void *pData, size_t lSize)
{
	if (BUTTOM_UP == uMsg)
	{
		OnClose();
	}

	return 0;
}

// 控件消息
LRESULT CDlgBase::OnBaseControlMessage(WPARAM wParam, LPARAM lParam)
{
	UINT uControlID = wParam;
	tagControlMessage  *pControlMessage = (tagControlMessage  *)lParam;

	if (uControlID == BT_MIN)
	{
		OnMessageButtomMin(pControlMessage->uMsg, pControlMessage->pData, pControlMessage->lSize);
	}
	else if (uControlID == BT_CLOSE)
	{
		OnMessageButtomClose(pControlMessage->uMsg, pControlMessage->pData, pControlMessage->lSize);
	}
	else
	{
		OnControlMessage(uControlID, pControlMessage->uMsg, pControlMessage->pData, pControlMessage->lSize);
	}

	if (pControlMessage->pData != NULL)
	{
		free(pControlMessage->pData);
		pControlMessage->pData = NULL;
	}
	delete pControlMessage;

	return 0;
}
// 窗口消息
LRESULT CDlgBase::OnBaseWindowMessage(WPARAM wParam, LPARAM lParam)
{
	UINT uControlID = wParam;
	tagWindowMessage  *pWindowMessage = (tagWindowMessage  *)lParam;

	OnWindowMessage(uControlID, pWindowMessage->uMsg, pWindowMessage->pData, pWindowMessage->lSize);

	if (pWindowMessage->pData != NULL)
	{
		free(pWindowMessage->pData);
		pWindowMessage->pData = NULL;
	}
	delete pWindowMessage;


	return 0;
}

// 发送通知消息
bool CDlgBase::SendMessageEx(UINT uMsg, void *pData/* = NULL*/, size_t lSize/* = 0*/)
{
	HWND hParent = m_hWndMsg;
	if (hParent && m_uWindowID != 0)
	{
		tagWindowMessage  *pWindowMessage = new tagWindowMessage;
		ZeroMemory(pWindowMessage, sizeof(tagWindowMessage));

		pWindowMessage->uMsg = uMsg;
		if (pData != NULL)
		{
			pWindowMessage->lSize = lSize;
			pWindowMessage->pData = malloc(pWindowMessage->lSize);
			CopyMemory(pWindowMessage->pData, pData, pWindowMessage->lSize);
		}

		::SendMessage(hParent, WINDOW_MESSAGE, m_uWindowID, (LPARAM)pWindowMessage);
		return true;
	}
	return false;
}

// 发送通知消息
bool CDlgBase::PostMessageEx(UINT uMsg, void *pData/* = NULL*/, size_t lSize/* = 0*/)
{
	HWND hParent = m_hWndMsg;
	if (hParent && m_uWindowID != 0)
	{
		tagWindowMessage  *pWindowMessage = new tagWindowMessage;
		ZeroMemory(pWindowMessage, sizeof(tagWindowMessage));

		pWindowMessage->uMsg = uMsg;
		if (pData != NULL)
		{
			pWindowMessage->lSize = lSize;
			pWindowMessage->pData = malloc(pWindowMessage->lSize);
			CopyMemory(pWindowMessage->pData, pData, pWindowMessage->lSize);
		}

		::PostMessage(hParent, WINDOW_MESSAGE, m_uWindowID, (LPARAM)pWindowMessage);
		return true;
	}
	return false;
}

BOOL CDlgBase::OnNcActivate(BOOL bActive)
{
	m_bNCActive = bActive;

	if (m_bNCActive)
	{

	}
	else
	{
		m_bTracking = false;
		m_bIsSetCapture = false;
		m_bIsLButtonDblClk = FALSE;

		if (m_bLButtonDown)
		{
			m_bLButtonDown = FALSE;
			if (m_pControl)
			{
				m_pControl->OnLButtonUp(0, CPoint(-1, -1));
				m_pControl = NULL;
			}
		}
		else
		{
			if (m_pControl)
			{
				m_pControl->OnMouseMove(0, CPoint(-1, -1));
				m_pControl = NULL;
			}
		}
	}
	InvalidateRect(NULL);

	return TRUE;
}


void CDlgBase::PreSubclassWindow()
{
	DragAcceptFiles(TRUE);

	CDialog::PreSubclassWindow();
}


void CDlgBase::OnMouseMove(UINT nFlags, CPoint point)
{
	if (!m_bTracking && nFlags != 0xFFFFFFFF)
	{
		TRACKMOUSEEVENT tme;
		tme.cbSize = sizeof(tme);
		tme.hwndTrack = m_hWnd;
		tme.dwFlags = TME_LEAVE | TME_HOVER;
		tme.dwHoverTime = 1;
		m_bTracking = ::_TrackMouseEvent(&tme);
	}

	if (m_pControl)
	{
		if ((m_pControl->PtInRect(point) || m_bLButtonDown) && m_bTracking)
		{
			m_pControl->OnMouseMove(nFlags, point);
			return;
		}
	}

	if (!m_bLButtonDown && !m_bRButtonDown)
	{
		CControlBase * pOldControl = m_pControl;
		bool bIsSelect = FALSE;
		bool bIsSystemSelect = FALSE;

		if (m_bTracking)
		{
			// 默认控件
			for (size_t i = 0; i < m_vecBaseControl.size(); i++)
			{
				CControlBase * pControlBase = m_vecBaseControl.at(i);
				if (pControlBase)
				{
					pControlBase->OnMouseMove(nFlags, point);
					if (pControlBase->PtInRect(point) && pControlBase->GetRresponse())
					{
						m_pControl = pControlBase;
						bIsSystemSelect = TRUE;
					}
				}
			}

			// 用户控件
			for (size_t i = 0; i < m_vecControl.size(); i++)
			{
				CControlBase * pControlBase = m_vecControl.at(i);
				if (pControlBase)
				{
					pControlBase->OnMouseMove(nFlags, point);
					if (!bIsSystemSelect)
					{
						if (pControlBase->PtInRect(point) && pControlBase->GetRresponse())
						{
							m_pControl = pControlBase;
							bIsSelect = TRUE;
						}
					}
				}
			}
		}

		if (!bIsSelect && !bIsSystemSelect)
		{
			m_pControl = NULL;
		}
	}

	//CDialog::OnMouseMove(nFlags, point);
}

LRESULT CDlgBase::OnMouseLeave(WPARAM wParam, LPARAM lParam)
{
	OnWindowMouseLeave();

	m_bTracking = FALSE;
	if (!m_bLButtonDown)
	{
		if (m_pControl && !m_pControl->GetFocus())
		{
			m_pControl->OnMouseMove(0, CPoint(-1, -1));
			m_pControl = NULL;
		}
	}

	return 0;
}

LRESULT CDlgBase::OnMouseHover(WPARAM wParam, LPARAM lParam)
{
	//if (m_pControl)
	//{
	//	CPoint point;
	//	GetCursorPos(&point);
	//	//ScreenToClient(&point);
	//m_pControl->OnMouseMove(0, point);
	//}
	OnWindowMouseHover();
	return 0;
}

BOOL CDlgBase::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	if (m_pControl)
	{
		m_pControl->OnMouseWheel(nFlags, zDelta, pt);
	}

	return __super::OnMouseWheel(nFlags, zDelta, pt);
}

void CDlgBase::OnLButtonDown(UINT nFlags, CPoint point)
{
	bool bIsSelect = false;
	if (m_pFocusControl != m_pControl && m_pFocusControl != NULL)
	{
		m_pFocusControl->OnFocus(false);
		m_pFocusControl = NULL;
	}
	if (m_pControl)
	{
		if (m_pControl->GetVisible())
		{
			if (m_pControl->PtInRect(point))
			{
				bIsSelect = TRUE;
				m_bLButtonDown = TRUE;

				m_pFocusControl = m_pControl;
				m_pControl->OnLButtonDown(nFlags, point);
			}
		}
	}

	if (bIsSelect && !m_bIsSetCapture)
	{
		SetCapture();
		m_bIsSetCapture = TRUE;

		return;
	}

	if (m_bWindowMove)
	{
		PostMessage(WM_NCLBUTTONDOWN, HTCAPTION, MAKELPARAM(point.x, point.y));
	}

	CDialog::OnLButtonDown(nFlags, point);
}

void CDlgBase::OnLButtonUp(UINT nFlags, CPoint point)
{
	if (m_bIsSetCapture)
	{
		ReleaseCapture();
		m_bIsSetCapture = false;
	}

	m_bLButtonDown = FALSE;
	m_bIsLButtonDblClk = FALSE;

	if (m_pControl)
	{
		if (m_pControl->GetVisible())
		{
			CRect rc = m_pControl->GetRect();
			m_pControl->OnLButtonUp(nFlags, point);

			if (!rc.PtInRect(point))
			{
				m_pControl = NULL;
			}
		}
		else
		{
			m_pControl = NULL;
		}
	}

	CDialog::OnLButtonUp(nFlags, point);
}

void CDlgBase::OnRButtonDown(UINT nFlags, CPoint point)
{
	bool bIsSelect = false;
	if (m_pFocusControl != m_pControl && m_pFocusControl != NULL)
	{
		m_pFocusControl->OnFocus(false);
		m_pFocusControl = NULL;
	}
	if (m_pControl)
	{
		if (m_pControl->GetVisible())
		{
			if (m_pControl->PtInRect(point))
			{
				bIsSelect = true;
				m_bRButtonDown = true;

				m_pFocusControl = m_pControl;
				m_pControl->OnRButtonDown(nFlags, point);
			}
		}
	}

	if (bIsSelect && !m_bIsSetCapture)
	{
		SetCapture();
		m_bIsSetCapture = true;

		return;
	}

	PostMessage(WM_NCLBUTTONDOWN, HTCAPTION, MAKELPARAM(point.x, point.y));

	__super::OnRButtonDown(nFlags, point);
}

void CDlgBase::OnRButtonUp(UINT nFlags, CPoint point)
{
	if (m_bIsSetCapture)
	{
		ReleaseCapture();
		m_bIsSetCapture = false;
	}

	m_bRButtonDown = false;

	if (m_pControl)
	{
		if (m_pControl->GetVisible())
		{
			CRect rc = m_pControl->GetRect();
			m_pControl->OnRButtonUp(nFlags, point);

			if (!rc.PtInRect(point))
			{
				m_pControl = NULL;
			}
		}
		else
		{
			m_pControl = NULL;
		}
	}

	__super::OnRButtonUp(nFlags, point);
}

void CDlgBase::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	m_bIsLButtonDblClk = TRUE;

	if (m_pControl)
	{
		if (m_pControl->GetLButtonDblClk())
		{
			m_pControl->OnLButtonDblClk(nFlags, point);
		}
		else
		{
			return OnLButtonDown(nFlags, point);
		}
	}

	CDialog::OnLButtonDblClk(nFlags, point);
}

BOOL CDlgBase::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_ESCAPE)
		{
			return TRUE;
		}
		if (pMsg->wParam == VK_RETURN)
		{
			return TRUE;
		}
	}

	return CDialog::PreTranslateMessage(pMsg);
}

// 定时器消息
void CDlgBase::OnTimer(UINT nIDEvent)
{
	// 动画定时器
	if (TIMER_ANIMATION_ID == nIDEvent)
	{
		for (size_t i = 0; i < m_vecBaseControl.size(); i++)
		{
			CControlBase * pControlBase = m_vecBaseControl.at(i);
			if (pControlBase)
			{
				pControlBase->OnTimer();
			}
		}

		for (size_t i = 0; i < m_vecControl.size(); i++)
		{
			CControlBase * pControlBase = m_vecControl.at(i);
			if (pControlBase)
			{
				pControlBase->OnTimer();
			}
		}
	}

	OnTimerEx(nIDEvent);
}

LRESULT CDlgBase::OnControlUpdate(CRect rcUpdate, CControlBase *pUpdateControlBase)
{
	if (pUpdateControlBase == NULL && IsWindow(GetSafeHwnd())) return 0;

	CRect rcAllUpDate = rcUpdate;

	if (m_bInit)
	{
		for (size_t i = 0; i < m_vecArea.size(); i++)
		{
			CControlBase * pControlBase = m_vecArea.at(i);
			if (pControlBase != NULL && pUpdateControlBase != pControlBase)
			{
				rcAllUpDate |= pControlBase->IsUpdateRect(rcUpdate);
			}
		}

		for (size_t i = 0; i < m_vecBaseArea.size(); i++)
		{
			CControlBase * pControlBase = m_vecBaseArea.at(i);
			if (pControlBase != NULL && pUpdateControlBase != pControlBase)
			{
				rcAllUpDate |= pControlBase->IsUpdateRect(rcUpdate);
			}
		}

		for (size_t i = 0; i < m_vecControl.size(); i++)
		{
			CControlBase * pControlBase = m_vecControl.at(i);
			if (pControlBase != NULL && pUpdateControlBase != pControlBase)
			{
				rcAllUpDate |= pControlBase->IsUpdateRect(rcUpdate);
			}
		}

		for (size_t i = 0; i < m_vecBaseControl.size(); i++)
		{
			CControlBase * pControlBase = m_vecBaseControl.at(i);
			if (pControlBase != NULL && pUpdateControlBase != pControlBase)
			{
				rcAllUpDate |= pControlBase->IsUpdateRect(rcUpdate);
			}
		}
	}

	InvalidateRect(&rcAllUpDate);
	//TRACE(TEXT("CDlgBase::OnControlUpdate更新区域 rcAllUpDate(%d, %d , %d, %d)(w:%d h:%d) \n"), rcAllUpDate.left, rcAllUpDate.top, rcAllUpDate.right, rcAllUpDate.bottom, rcAllUpDate.Width(), rcAllUpDate.Height());
	return 0L;
};
void CDlgBase::OnDestroy()
{
	KillTimer(TIMER_ANIMATION_ID);
	__super::OnDestroy();
}

BOOL CDlgBase::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	if (m_pControl)
	{
		MOUSE_CURSOR cursor = m_pControl->GetCursorTpye();
		if (cursor != IDC_ARROW)
		{
			::SetCursor(LoadCursor(NULL, cursor));
			return TRUE;
		}
	}


	return __super::OnSetCursor(pWnd, nHitTest, message);
}



