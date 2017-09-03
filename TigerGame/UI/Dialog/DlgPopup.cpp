// DlgPopup.cpp : 实现文件
//

#include "stdafx.h"
#include "DlgPopup.h"

#define TIMER_ANIMATION_ID		1
// CDlgPopup

IMPLEMENT_DYNAMIC(CDlgPopup, CWnd)

CDlgPopup::CDlgPopup()
{
	m_hWndMsg = NULL;
	m_bInit = false;
	m_pImage = NULL;
	m_bTracking = false;
	m_bLButtonDown = false;
	m_bIsSetCapture = false;
	m_uControlID = 0;
	m_enBackMode = enBMFrame;
	m_bInitFinish = false;
	m_bAutoClose = true;

	m_pControl = NULL;
	m_pFocusControl = NULL;
}

CDlgPopup::~CDlgPopup()
{
	for (size_t i = 0; i < m_vecControl.size(); i++)
	{
		CControlBase * pControlBase = m_vecControl.at(i);
		if (pControlBase)
		{
			delete pControlBase;
		}
	}

	if (m_pImage != NULL)
	{
		delete m_pImage;
		m_pImage = NULL;
	}
}


BEGIN_MESSAGE_MAP(CDlgPopup, CWnd)
	ON_WM_CREATE()
	ON_WM_NCCALCSIZE()
	ON_WM_NCHITTEST()
	ON_WM_ERASEBKGND()
	ON_MESSAGE(WM_MOUSELEAVE, OnMouseLeave)
	ON_MESSAGE(WM_MOUSEHOVER, OnMouseHover)
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_NCACTIVATE()
	ON_WM_LBUTTONUP()
	ON_WM_CLOSE()
	ON_WM_PAINT()
	ON_WM_DESTROY()
	ON_MESSAGE(CONTROL_MESSAGE, OnBaseControlMessage)
	ON_MESSAGE(WINDOW_MESSAGE, OnBaseWindowMessage)

	ON_WM_SIZE()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_MOUSEWHEEL()
	ON_WM_TIMER()
END_MESSAGE_MAP()

bool CDlgPopup::Create(CWnd *pParent, CRect rc, UINT uControlID, UINT nResourceID, enumBackMode enBackMode/* = enBMFrame*/, int nFrameSize/* = 4*/)
{
	CString sWindowClassName = AfxRegisterWndClass(CS_DBLCLKS, ::LoadCursor(NULL, IDC_ARROW));

	m_hWndMsg = pParent->GetSafeHwnd();
	m_uControlID = uControlID;
	m_point.SetPoint(rc.left, rc.top);
	m_size.SetSize(rc.Width(), rc.Height());

	m_enBackMode = enBackMode;
	m_nFrameSize = nFrameSize;

	SetBackBitmap(nResourceID, m_nFrameSize);

	if (!CWnd::CreateEx(WS_EX_TOPMOST | WS_EX_TOOLWINDOW, sWindowClassName, NULL, WS_POPUP, rc, pParent, 0, NULL))
	{
		return	FALSE;
	}

	return TRUE;
}

bool CDlgPopup::Create(CWnd *pParent, CRect rc, UINT uControlID, CString strImage, enumBackMode enBackMode/* = enBMFrame*/, int nFrameSize/* = 4*/)
{

	CString sWindowClassName = AfxRegisterWndClass(CS_DBLCLKS, ::LoadCursor(NULL, IDC_ARROW));

	m_hWndMsg = pParent->GetSafeHwnd();
	m_uControlID = uControlID;
	m_point.SetPoint(rc.left, rc.top);
	m_size.SetSize(rc.Width(), rc.Height());

	m_enBackMode = enBackMode;
	m_nFrameSize = nFrameSize;

	SetBackBitmap(strImage, m_nFrameSize);

	if (!CWnd::CreateEx(WS_EX_TOPMOST | WS_EX_TOOLWINDOW, sWindowClassName, NULL, WS_POPUP, rc, pParent, 0, NULL))
	{
		return	FALSE;
	}

	return TRUE;
}

int CDlgPopup::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	// 设置窗口风格
	DWORD dwStyle = ::GetWindowLong(m_hWnd, GWL_STYLE)
		| WS_SIZEBOX | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_EX_LAYERED;
	dwStyle &= ~(WS_CAPTION);

	::SetWindowLong(m_hWnd, GWL_STYLE, dwStyle);
	::SetWindowLong(m_hWnd, GWL_EXSTYLE, WS_EX_TOOLWINDOW);

	SetBackMode(m_enBackMode, m_nFrameSize);

	if (m_hWndMsg == NULL)
	{
		m_hWndMsg = GetParent()->GetSafeHwnd();
	}

	m_bInitFinish = true;

	CRect rcClient;
	GetClientRect(&rcClient);

	InitUI(rcClient);

	//DrawWindow();

	m_bInit = true;

	SetForegroundWindow();

	//启动定时器
	SetTimer(TIMER_ANIMATION_ID, 30, NULL);

	return TRUE;
}

void CDlgPopup::OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS* lpncsp)
{
	CWnd::OnNcCalcSize(bCalcValidRects, lpncsp);

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

LRESULT CDlgPopup::OnNcHitTest(CPoint point)
{
	return	HTCLIENT;
}

BOOL CDlgPopup::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
}

void CDlgPopup::SetBackMode(enumBackMode enBackMode, int nFrameSize)
{
	if (m_enBackMode == enBMImage && m_pImage != NULL)
	{
		m_enBackMode = enBackMode;
		SetWindowPos(NULL, m_point.x, m_point.y, m_sizeBKImage.cx, m_sizeBKImage.cy, SWP_SHOWWINDOW);
		m_size = m_sizeBKImage;
	}
	else
	{
		m_enBackMode = enBackMode;
		m_nFrameSize = nFrameSize;
	}
	//DrawWindow();
	InvalidateRect(NULL);
}

void CDlgPopup::SetBackBitmap(UINT nResourceID, int nFrameSize)
{
	if (nFrameSize != 0)
	{
		if (ImageFromIDResource(nResourceID, TEXT("PNG"), m_pImage))
		{
			m_sizeBKImage.SetSize(m_pImage->GetWidth(), m_pImage->GetHeight());
			m_nFrameSize = nFrameSize;
			//DrawWindow();
			if (m_bInitFinish)
			{
				InvalidateRect(NULL);
			}
		}
	}
}

void CDlgPopup::SetBackBitmap(CString strImage, int nFrameSize)
{
	m_pImage = Image::FromFile(strImage, TRUE);

	if (m_pImage)
	{
		m_sizeBKImage.SetSize(m_pImage->GetWidth(), m_pImage->GetHeight());
		m_nFrameSize = nFrameSize;
		//DrawWindow();
		if (m_bInitFinish)
		{
			InvalidateRect(NULL);
		}
	}
}

void CDlgPopup::CloseWindow()
{
	CWnd::PostMessage(WM_CLOSE);
}

void CDlgPopup::DrawWindow(CDC &dc)
{
	if (!m_bInitFinish) return;
	DrawWindow(&dc);
}

void CDlgPopup::DrawWindow()
{
	if (!m_bInitFinish) return;

	CDC *pDC = GetDC();
	DrawWindow(pDC);
	ReleaseDC(pDC);
}

void CDlgPopup::DrawWindow(CDC *pDC)
{
	if (!m_bInitFinish) return;

	CRect rcClient;
	GetClientRect(&rcClient);

	CDC MemDC;
	MemDC.CreateCompatibleDC(pDC);

	BITMAPINFO bitmapinfo;
	bitmapinfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bitmapinfo.bmiHeader.biBitCount = 32;
	bitmapinfo.bmiHeader.biHeight = rcClient.Height();
	bitmapinfo.bmiHeader.biWidth = rcClient.Width();
	bitmapinfo.bmiHeader.biPlanes = 1;
	bitmapinfo.bmiHeader.biCompression = BI_RGB;
	bitmapinfo.bmiHeader.biXPelsPerMeter = 0;
	bitmapinfo.bmiHeader.biYPelsPerMeter = 0;
	bitmapinfo.bmiHeader.biClrUsed = 0;
	bitmapinfo.bmiHeader.biClrImportant = 0;
	bitmapinfo.bmiHeader.biSizeImage = bitmapinfo.bmiHeader.biWidth * bitmapinfo.bmiHeader.biHeight * bitmapinfo.bmiHeader.biBitCount / 8;
	HBITMAP hBitmap = ::CreateDIBSection(MemDC.GetSafeHdc(), &bitmapinfo, 0, NULL, 0, 0);
	HBITMAP hOldBitmap = (HBITMAP)::SelectObject(MemDC.GetSafeHdc(), hBitmap);
	MemDC.SetBkMode(TRANSPARENT);
	if (m_pImage)
	{
		Graphics graphics(MemDC);
		if (m_enBackMode == enBMFrame)
		{
			DrawImageFrame(graphics, m_pImage, rcClient, 0, 0, m_sizeBKImage.cx, m_sizeBKImage.cy, m_nFrameSize);
		}
		else
		{
			graphics.DrawImage(m_pImage, Rect(0, 0, m_sizeBKImage.cx, m_sizeBKImage.cy), 0, 0, m_sizeBKImage.cx, m_sizeBKImage.cy, UnitPixel);
		}
	}

	DrawWindowEx(MemDC, rcClient);

	for (size_t i = 0; i < m_vecControl.size(); i++)
	{
		CControlBase * pControlBase = m_vecControl.at(i);
		if (pControlBase)
		{
			pControlBase->Draw(MemDC, rcClient, rcClient);
		}
	}

	ClientToScreen(&rcClient);
	POINT pointDes;
	pointDes.x = rcClient.left;
	pointDes.y = rcClient.top;
	POINT pointSrc;
	pointSrc.x = 0;
	pointSrc.y = 0;
	SIZE sizeDes;
	sizeDes.cx = rcClient.Width();
	sizeDes.cy = rcClient.Height();

	BLENDFUNCTION blend;
	memset(&blend, 0, sizeof(blend));
	blend.AlphaFormat = AC_SRC_ALPHA;
	blend.SourceConstantAlpha = 255;

	HWND hWnd = GetSafeHwnd();
	SetWindowLong(hWnd, GWL_EXSTYLE, GetWindowLong(hWnd, GWL_EXSTYLE) | WS_EX_LAYERED);
	UpdateLayeredWindow(pDC, &pointDes, &sizeDes, &MemDC, &pointSrc, 0, &blend, ULW_ALPHA);

	::SelectObject(MemDC.GetSafeHdc(), hOldBitmap);
	::DeleteObject(hBitmap);
	MemDC.DeleteDC();
}

LRESULT CDlgPopup::OnMouseLeave(WPARAM wParam, LPARAM lParam)
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

LRESULT CDlgPopup::OnMouseHover(WPARAM wParam, LPARAM lParam)
{
	OnWindowMouseHover();

	return 0;
}

void CDlgPopup::OnMouseMove(UINT nFlags, CPoint point)
{
	if (!m_bTracking)
	{
		TRACKMOUSEEVENT tme;
		tme.cbSize = sizeof(tme);
		tme.hwndTrack = m_hWnd;
		tme.dwFlags = TME_LEAVE | TME_HOVER;
		tme.dwHoverTime = 1;
		m_bTracking = ::_TrackMouseEvent(&tme);
	}

	bool bInvalidateRect = false;

	if (m_pControl)
	{
		if ((m_pControl->PtInRect(point) || m_bLButtonDown) && m_bTracking)
		{
			m_pControl->OnMouseMove(nFlags, point);
			return;
		}
	}

	if (!m_bLButtonDown)
	{
		CControlBase * pOldControl = m_pControl;
		bool bIsSelect = FALSE;
		bool bIsSystemSelect = FALSE;

		if (m_bTracking)
		{
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
}

BOOL CDlgPopup::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	if (m_pControl)
	{
		m_pControl->OnMouseWheel(nFlags, zDelta, pt);
	}

	return __super::OnMouseWheel(nFlags, zDelta, pt);
}

void CDlgPopup::OnLButtonDown(UINT nFlags, CPoint point)
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
		else
		{
			m_pControl = NULL;
		}
	}

	if (bIsSelect && !m_bIsSetCapture)
	{
		SetCapture();
		m_bIsSetCapture = TRUE;

		return;
	}

	//PostMessage(WM_NCLBUTTONDOWN,HTCAPTION,MAKELPARAM(point.x, point.y));

	CWnd::OnLButtonDown(nFlags, point);
}

void CDlgPopup::OnLButtonDblClk(UINT nFlags, CPoint point)
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

	__super::OnLButtonDblClk(nFlags, point);
}

void CDlgPopup::OnLButtonUp(UINT nFlags, CPoint point)
{
	if (m_bIsSetCapture)
	{
		ReleaseCapture();
		m_bIsSetCapture = false;
	}

	m_bLButtonDown = FALSE;

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

	m_bIsLButtonDblClk = FALSE;

	CWnd::OnLButtonUp(nFlags, point);
}

void CDlgPopup::PostNcDestroy()
{
	CWnd::PostNcDestroy();
	delete this;
}

BOOL CDlgPopup::OnNcActivate(BOOL bActive)
{
	if (!bActive && m_bAutoClose)
	{
		CloseWindow();
	}
	else
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
					CRect rcControlUpdate;
					m_pControl->OnLButtonUp(0, CPoint(-1, -1));
					m_pControl = NULL;
				}
			}
			else
			{
				if (m_pControl)
				{
					CRect rcControlUpdate;
					m_pControl->OnMouseMove(0, CPoint(-1, -1));
					m_pControl = NULL;
				}
			}
		}
		//DrawWindow();
		InvalidateRect(NULL);

		return TRUE;
	}
	return TRUE;
}

// 定时器消息
void CDlgPopup::OnTimer(UINT nIDEvent)
{
	if (!::IsWindow(GetSafeHwnd())) return;

	// 动画定时器
	if (TIMER_ANIMATION_ID == nIDEvent)
	{
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


LRESULT CDlgPopup::OnControlUpdate(CRect rcUpdate, CControlBase *pUpdateControlBase)
{
	if (pUpdateControlBase == NULL) return 0;

	CRect rcAllUpDate = rcUpdate;

	if (m_bInit)
	{
		for (size_t i = 0; i < m_vecControl.size(); i++)
		{
			CControlBase * pControlBase = m_vecControl.at(i);
			if (pControlBase != NULL && pUpdateControlBase != pControlBase)
			{
				rcAllUpDate |= pControlBase->IsUpdateRect(rcUpdate);
			}
		}
	}

	InvalidateRect(&rcAllUpDate);

	return 0L;
};

// 控件消息
LRESULT CDlgPopup::OnBaseControlMessage(WPARAM wParam, LPARAM lParam)
{
	UINT uControlID = wParam;
	tagControlMessage  *pControlMessage = (tagControlMessage  *)lParam;

	OnControlMessage(uControlID, pControlMessage->uMsg, pControlMessage->pData, pControlMessage->lSize);

	if (pControlMessage->pData != NULL)
	{
		free(pControlMessage->pData);
		pControlMessage->pData = NULL;
	}
	delete pControlMessage;

	return 0;
}
// 窗口消息
LRESULT CDlgPopup::OnBaseWindowMessage(WPARAM wParam, LPARAM lParam)
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
bool CDlgPopup::SendMessageEx(UINT uMsg, void *pData/* = NULL*/, size_t lSize/* = 0*/)
{
	HWND hParent = m_hWndMsg;
	if (hParent)
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

		::SendMessage(hParent, WINDOW_MESSAGE, m_uControlID, (LPARAM)pWindowMessage);
		return true;
	}
	return false;
}

// 发送通知消息
bool CDlgPopup::PostMessageEx(UINT uMsg, void *pData/* = NULL*/, size_t lSize/* = 0*/)
{
	HWND hParent = m_hWndMsg;
	if (hParent)
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

		::PostMessage(hParent, WINDOW_MESSAGE, m_uControlID, (LPARAM)pWindowMessage);
		return true;
	}
	return false;
}

CControlBase *CDlgPopup::GetControl(UINT uControlID)
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
	return NULL;
}

// 移动控件
CControlBase * CDlgPopup::SetControlRect(UINT uControlID, CRect rc)
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
CControlBase * CDlgPopup::SetControlRect(CControlBase *pControlBase, CRect rc)
{
	if (pControlBase)
	{
		pControlBase->SetRect(rc);
		UpdateHover();
	}
	return pControlBase;
}

// 显示控件
CControlBase * CDlgPopup::SetControlVisible(UINT uControlID, bool bVisible)
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
CControlBase * CDlgPopup::SetControlVisible(CControlBase *pControlBase, bool bVisible)
{
	if (pControlBase)
	{
		pControlBase->SetVisible(bVisible);
		UpdateHover();
	}
	return pControlBase;
}

// 禁用控件
CControlBase * CDlgPopup::SetControlDisable(UINT uControlID, bool bDisable)
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
CControlBase * CDlgPopup::SetControlDisable(CControlBase *pControlBase, bool bDisable)
{
	if (pControlBase)
	{
		pControlBase->SetDisable(bDisable);
		UpdateHover();
	}
	return pControlBase;
}

// 更新选中
void CDlgPopup::UpdateHover()
{
	CPoint point;
	GetCursorPos(&point);
	OnMouseMove(0, point);
}

void CDlgPopup::OnClose()
{
	PostMessageEx(MSG_CLOSE);

	CWnd::OnClose();
}

void CDlgPopup::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	DrawWindow(&dc);
}

void CDlgPopup::OnDestroy()
{
	KillTimer(TIMER_ANIMATION_ID);
	__super::OnDestroy();
}


void CDlgPopup::OnSize(UINT nType, int cx, int cy)
{
	__super::OnSize(nType, cx, cy);

	GetClientRect(&m_rc);
}
