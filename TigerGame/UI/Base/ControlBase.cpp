#include "StdAfx.h"
#include <mmsystem.h> 
#include "ControlBase.h"


#pragma comment(lib,"Winmm.lib")  

CControlBase::CControlBase(HWND hWnd, CMessageInterface* pMessage, UINT uControlID, CRect rc, bool bIsVisible, bool bIsDisable, bool bRresponse)
{
	m_bBackColorUpdate = false;
	m_bComControl = false;
	m_pMessage = pMessage;
	m_hWnd = hWnd;
	m_uControlID = uControlID;
	m_CursorTpye = IDC_ARROW;
	m_rc = rc;
	m_bIsVisible = bIsVisible;
	m_bIsDisable = bIsDisable;
	m_bRresponse = bRresponse;
	m_bUpdate = TRUE;
	m_nDCWidth = 0;
	m_nDCHeight = 0;
	m_pControl = NULL;

	m_bLButtonDown = false;
	m_bRButtonDown = false;
	m_bLButtonDblClk = false;
	m_bIsRun = false;
	m_bRunTime = false;

	m_hBitmap = NULL;
	m_hOldBitmap = NULL;


}


CControlBase::~CControlBase(void)
{
	for (size_t i = 0; i < m_vecControl.size(); i++)
	{
		CControlBase * pControlBase = m_vecControl.at(i);
		if (pControlBase)
		{
			delete pControlBase;
		}
	}

	if (m_hBitmap != NULL)
	{
		if (m_memDC.m_hDC)
		{
			::SelectObject(m_memDC.GetSafeHdc(), m_hOldBitmap);
			m_memDC.DeleteDC();
		}
		::DeleteObject(m_hBitmap);
	}
}

void CControlBase::Draw(CDC &dc, CRect rcParent, CRect rcUpdate)
{
	CRect rc = m_rc & rcUpdate;
	if (m_bIsVisible && !rc.IsRectEmpty())
	{
		DrawControl(dc, rcParent, rcUpdate);

		for (size_t i = 0; i < m_vecControl.size(); i++)
		{
			CControlBase * pControlBase = m_vecControl.at(i);
			if (pControlBase && pControlBase->GetVisible())
			{
				pControlBase->Draw(dc, m_rc, rcUpdate);
			}
		}
	}
}

void CControlBase::DrawControlEx(CDC &dc, CRect rcParent, CRect rcUpdate, int nSrcOffsetX/* = 0*/, int nSrcOffsetY/* = 0*/)
{
	CRect rc = rcParent & m_rc & rcUpdate;

	if (rc.IsRectEmpty())
	{
		return;
	}

	int nDestDrawX = rc.left > rcUpdate.left ? rc.left - rcUpdate.left : 0;
	int nDestDrawY = rc.top > rcUpdate.top ? rc.top - rcUpdate.top : 0;
	int nSrcDrawX = m_rc.left >= rcUpdate.left ? 0 : rcUpdate.left - m_rc.left;
	int nSrcDrawY = m_rc.top >= rcUpdate.top ? 0 : rcUpdate.top - m_rc.top;

	int nDrawWidth = rc.Width();
	int nDrawHeight = rc.Height();

	BLENDFUNCTION tfn;
	tfn.AlphaFormat = AC_SRC_ALPHA;
	tfn.BlendFlags = 0;
	tfn.BlendOp = AC_SRC_OVER;
	tfn.SourceConstantAlpha = 255;
	dc.AlphaBlend(nDestDrawX, nDestDrawY, nDrawWidth, nDrawHeight, &m_memDC, nSrcDrawX + nSrcOffsetX, nSrcDrawY + nSrcOffsetY, nDrawWidth, nDrawHeight, tfn);
}

void CControlBase::DrawControlEx(CDC &dc, CRect rcParent, CRect rcUpdate, int nDestOffsetX, int nDestOffsetY, int nWidth, int nHeight, int nSrcOffsetX/* = 0*/, int nSrcOffsetY/* = 0*/)
{

	CRect rc = rcParent & m_rc & rcUpdate;

	if (rc.IsRectEmpty())
	{
		return;
	}


	int nDestDrawX = rc.left > rcUpdate.left ? rc.left - rcUpdate.left : 0;
	int nDestDrawY = rc.top > rcUpdate.top ? rc.top - rcUpdate.top : 0;
	int nSrcDrawX = 0; m_rc.left >= rcUpdate.left ? 0 : rcUpdate.left - m_rc.left;
	int nSrcDrawY = 0; m_rc.top >= rcUpdate.top ? 0 : rcUpdate.top - m_rc.top;

	int nDrawWidth = __min(nWidth, rc.Width());
	int nDrawHeight = __min(nHeight, rc.Height());

	CRect rcTemp(m_rc.left + nDestOffsetX, m_rc.top + nDestOffsetY, m_rc.left + nDestOffsetX + nWidth, m_rc.top + nDestOffsetY + nHeight);

	if ((rcTemp & rcParent & rcUpdate).IsRectEmpty())
	{
		return;
	}

	if (m_rc.left <= rcUpdate.left)
	{
		nDestOffsetX = m_rc.left - rcUpdate.left + nDestOffsetX;
	}
	if (m_rc.top <= rcUpdate.top)
	{
		nDestOffsetY = m_rc.top - rcUpdate.top + nDestOffsetY;

	}
	BLENDFUNCTION tfn;
	tfn.AlphaFormat = AC_SRC_ALPHA;
	tfn.BlendFlags = 0;
	tfn.BlendOp = AC_SRC_OVER;
	tfn.SourceConstantAlpha = 255;
	int srcx, srcy, dstx, dsty;

	if (nDestDrawX + nDestOffsetX < 0)
	{
		srcx = nSrcDrawX + nSrcOffsetX - (nDestDrawX + nDestOffsetX);
		dstx = 0;
	}
	else
	{
		srcx = nSrcDrawX + nSrcOffsetX;
		dstx = nDestDrawX + nDestOffsetX;
	}

	if (nDestDrawY + nDestOffsetY < 0)
	{
		srcy = nSrcDrawY + nSrcOffsetY - (nDestDrawY + nDestOffsetY);
		dsty = 0;
	}
	else
	{
		srcy = nSrcDrawY + nSrcOffsetY;
		dsty = nDestDrawY + nDestOffsetY;
	}

	dc.AlphaBlend(dstx, dsty, nDrawWidth, nDrawHeight, &m_memDC, srcx, srcy, nDrawWidth, nDrawHeight, tfn);
}
void CControlBase::DrawControlEx(CDC &dc, CRect rcMyRect, CRect rcParent, CRect rcUpdate, int nDestOffsetX, int nDestOffsetY, int nWidth, int nHeight, int nSrcOffsetX/* = 0*/, int nSrcOffsetY/* = 0*/)
{
	CRect rc = rcParent & rcMyRect & rcUpdate;

	if (rc.IsRectEmpty())
	{
		return;
	}


	int nDestDrawX = rc.left > rcUpdate.left ? rc.left - rcUpdate.left : 0;
	int nDestDrawY = rc.top > rcUpdate.top ? rc.top - rcUpdate.top : 0;
	int nSrcDrawX = 0; m_rc.left >= rcUpdate.left ? 0 : rcUpdate.left - m_rc.left;
	int nSrcDrawY = 0; m_rc.top >= rcUpdate.top ? 0 : rcUpdate.top - m_rc.top;

	int nDrawWidth = __min(nWidth, rc.Width());
	int nDrawHeight = __min(nHeight, rc.Height());

	CRect rcTemp(rcMyRect.left + nDestOffsetX, rcMyRect.top + nDestOffsetY, rcMyRect.left + nDestOffsetX + nWidth, rcMyRect.top + nDestOffsetY + nHeight);

	if ((rcTemp & rcParent & rcUpdate).IsRectEmpty())
	{
		return;
	}

	if (rcMyRect.left <= rcUpdate.left)
	{
		nDestOffsetX = rcMyRect.left - rcUpdate.left + nDestOffsetX;
	}
	if (rcMyRect.top <= rcUpdate.top)
	{
		nDestOffsetY = rcMyRect.top - rcUpdate.top + nDestOffsetY;
	}
	BLENDFUNCTION tfn;
	tfn.AlphaFormat = AC_SRC_ALPHA;
	tfn.BlendFlags = 0;
	tfn.BlendOp = AC_SRC_OVER;
	tfn.SourceConstantAlpha = 255;
	int srcx, srcy, dstx, dsty;

	if (nDestDrawX + nDestOffsetX < 0)
	{
		srcx = nSrcDrawX + nSrcOffsetX - (nDestDrawX + nDestOffsetX);
		dstx = 0;
	}
	else
	{
		srcx = nSrcDrawX + nSrcOffsetX;
		dstx = nDestDrawX + nDestOffsetX;
	}

	if (nDestDrawY + nDestOffsetY < 0)
	{
		srcy = nSrcDrawY + nSrcOffsetY - (nDestDrawY + nDestOffsetY);
		dsty = 0;
	}
	else
	{
		srcy = nSrcDrawY + nSrcOffsetY;
		dsty = nDestDrawY + nDestOffsetY;
	}

	dc.AlphaBlend(dstx, dsty, nDrawWidth, nDrawHeight, &m_memDC, srcx, srcy, nDrawWidth, nDrawHeight, tfn);
}


CRect CControlBase::IsUpdateRect(CRect rcUpdate)
{
	CRect rcControlUpdate;
	rcControlUpdate.SetRectEmpty();
	if (m_bIsVisible && !(rcUpdate & m_rc).IsRectEmpty())
	{
		rcControlUpdate = IsControlUpdateRect(rcUpdate);
		for (size_t i = 0; i < m_vecControl.size(); i++)
		{
			CControlBase * pControlBase = m_vecControl.at(i);
			if (pControlBase)
			{
				rcControlUpdate |= pControlBase->IsUpdateRect(rcUpdate);
			}
		}
	}

	return rcControlUpdate;
}

void  CControlBase::SetBackColor(COLORREF clr)
{
	m_clrBK = clr;
	SetControlBackColor(clr);
	for (size_t i = 0; i < m_vecControl.size(); i++)
	{
		CControlBase * pControlBase = m_vecControl.at(i);
		if (pControlBase)
		{
			pControlBase->SetBackColor(clr);
		}
	}
	if (m_bBackColorUpdate)
	{
		UpdateControl(true);
	}
}

void  CControlBase::SetUpdate(bool bUpdate)
{
	m_bUpdate = bUpdate;

	for (size_t i = 0; i < m_vecControl.size(); i++)
	{
		CControlBase * pControlBase = m_vecControl.at(i);
		if (pControlBase)
		{
			pControlBase->SetUpdate(bUpdate);
		}
	}
}

void CControlBase::UpdateMemDC(CDC &dc, int nWidth, int nHeight)
{
	m_bUpdate = false;

	if (m_nDCWidth != nWidth || m_nDCHeight != nHeight)
	{
		m_nDCWidth = nWidth;
		m_nDCHeight = nHeight;

		if (m_hBitmap != NULL)
		{
			if (m_memDC.m_hDC)
			{
				::SelectObject(m_memDC.GetSafeHdc(), m_hOldBitmap);
				m_memDC.DeleteDC();
			}
			::DeleteObject(m_hBitmap);
		}
		m_memDC.CreateCompatibleDC(&dc);
		BITMAPINFO bitmapinfo;
		bitmapinfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
		bitmapinfo.bmiHeader.biBitCount = 32;
		bitmapinfo.bmiHeader.biHeight = m_nDCHeight;
		bitmapinfo.bmiHeader.biWidth = m_nDCWidth;
		bitmapinfo.bmiHeader.biPlanes = 1;
		bitmapinfo.bmiHeader.biCompression = BI_RGB;
		bitmapinfo.bmiHeader.biXPelsPerMeter = 0;
		bitmapinfo.bmiHeader.biYPelsPerMeter = 0;
		bitmapinfo.bmiHeader.biClrUsed = 0;
		bitmapinfo.bmiHeader.biClrImportant = 0;
		bitmapinfo.bmiHeader.biSizeImage = bitmapinfo.bmiHeader.biWidth * bitmapinfo.bmiHeader.biHeight * bitmapinfo.bmiHeader.biBitCount / 8;
		m_hBitmap = ::CreateDIBSection(m_memDC.GetSafeHdc(), &bitmapinfo, 0, NULL, 0, 0);
		m_hOldBitmap = (HBITMAP)::SelectObject(m_memDC.GetSafeHdc(), m_hBitmap);
		m_memDC.SetBkMode(TRANSPARENT);
	}
	else
	{
		if (m_memDC.m_hDC != NULL)
		{
			m_memDC.FillSolidRect(0, 0, nWidth, nHeight, RGB(0, 0, 0));
		}
	}
}

bool CControlBase::PtInRect(CPoint point)
{
	if (!m_bIsVisible) return false;

	return m_rc.PtInRect(point);
}

CControlBase *CControlBase::GetControl(UINT uControlID)
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

bool CControlBase::OnMouseMove(UINT nFlags, CPoint point)
{
	if (!m_bIsVisible || !m_bRresponse || m_bIsDisable) return false;

	bool bRresponse = false;
	if (m_pControl)
	{
		if (m_pControl->PtInRect(point) || m_bLButtonDown)
		{
			m_pControl->OnMouseMove(nFlags, point);
			return true;
		}
	}

	CControlBase * pOldControl = m_pControl;
	m_pControl = NULL;

	bRresponse = OnControlMouseMove(nFlags, point);

	if (!m_bLButtonDown && !m_bRButtonDown)
	{
		for (size_t i = 0; i < m_vecControl.size(); i++)
		{
			CControlBase * pControlBase = m_vecControl.at(i);
			if (pControlBase)
			{
				if (pControlBase->OnMouseMove(nFlags, point))
				{
					if (pControlBase->PtInRect(point))
					{
						m_pControl = pControlBase;
					}
					bRresponse = true;
				}
			}
		}


		if (pOldControl)
		{
			bRresponse = true;
		}

		if (m_pControl)
		{
			bRresponse = true;
		}
	}

	return bRresponse;
}

bool CControlBase::OnLButtonDown(UINT nFlags, CPoint point)
{
	if (!m_bRresponse || m_bIsDisable) return false;


	m_bLButtonDown = m_rc.PtInRect(point);

	if (m_pControl != NULL)
	{
		if (m_pControl->OnLButtonDown(nFlags, point))
		{
			return true;
		}
	}
	else
	{
		return OnControlLButtonDown(nFlags, point);
	}

	return false;
}

bool CControlBase::OnLButtonUp(UINT nFlags, CPoint point)
{
	if (!m_bRresponse || m_bIsDisable) return false;

	m_bLButtonDown = false;
	if (m_pControl != NULL)
	{
		if (m_pControl->OnLButtonUp(nFlags, point))
		{
			return true;
		}
	}
	else
	{
		return OnControlLButtonUp(nFlags, point);
	}

	return false;
}

bool CControlBase::OnRButtonDown(UINT nFlags, CPoint point)
{
	if (!m_bRresponse || m_bIsDisable) return false;

	m_bRButtonDown = m_rc.PtInRect(point);

	if (m_pControl != NULL)
	{
		if (m_pControl->OnRButtonDown(nFlags, point))
		{
			return true;
		}
	}
	else
	{
		return OnControlRButtonDown(nFlags, point);
	}

	return false;
}

bool CControlBase::OnRButtonUp(UINT nFlags, CPoint point)
{
	if (!m_bRresponse || m_bIsDisable) return false;

	m_bRButtonDown = false;

	if (m_pControl != NULL)
	{
		if (m_pControl->OnRButtonUp(nFlags, point))
		{
			return true;
		}
	}
	else
	{
		return OnControlRButtonUp(nFlags, point);
	}

	return false;
}

bool CControlBase::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	if (!m_bRresponse || m_bIsDisable) return false;

	if (m_pControl != NULL)
	{
		if (m_pControl->OnLButtonDblClk(nFlags, point))
		{
			return true;
		}
	}
	else
	{
		return OnControlLButtonDblClk(nFlags, point);
	}

	return false;
}

bool CControlBase::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	if (!m_bRresponse || m_bIsDisable) return false;

	if (m_pControl != NULL)
	{
		if (m_pControl->OnMouseWheel(nFlags, zDelta, pt))
		{
			return true;
		}
	}
	else
	{
		return OnControlMouseWheel(nFlags, zDelta, pt);
	}

	return false;
}

bool CControlBase::OnTimer()
{
	bool bRresponse = false;
	CRect rcnControlUpdate;

	bRresponse = OnControlTimer();

	for (size_t i = 0; i < m_vecControl.size(); i++)
	{
		CControlBase * pControlBase = m_vecControl.at(i);
		if (pControlBase)
		{
			rcnControlUpdate.SetRectEmpty();
			if (pControlBase->OnTimer())
			{
				bRresponse = true;
			}
		}
	}

	return bRresponse;
}

LRESULT CControlBase::OnControlUpdate(CRect rcUpdate, CControlBase *pUpdateControlBase/* = NULL*/)
{
	if (pUpdateControlBase == NULL) return 0;

	CRect rcAllUpDate = rcUpdate;

	rcAllUpDate |= IsUpdateRect(rcUpdate);

	UpdateControl(rcAllUpDate, m_bUpdate);

	return 0L;
}

void CControlBase::SetRect(CRect rc)
{
	CRect rcOld = m_rc;
	CRect rcAll = m_rc | rc;
	bool bUpdate = SetControlRect(rc);
	if (rcOld != rc)
	{
		UpdateControl(rcAll, m_bUpdate || (rcOld.Width() != m_rc.Width() || rcOld.Height() != m_rc.Height()) && bUpdate);
	}
}

void CControlBase::SetVisible(bool bIsVisible)
{
	//if (m_bIsVisible != bIsVisible)
	{
		SetControlVisible(bIsVisible);
		UpdateControl(m_bUpdate);
	}
}

void  CControlBase::SetDisable(bool bIsDisable)
{
	//if(m_bIsDisable != bIsDisable)
	{
		SetControlDisable(bIsDisable);
		UpdateControl(true);
	}
}

void CControlBase::UpdateControl(bool bUpdate)
{
	m_bUpdate = bUpdate;

	CRect rcTemp = m_rc & m_pMessage->m_rc;
	if (!rcTemp.IsRectEmpty())
	{
		m_pMessage->OnControlUpdate(rcTemp, this);
	}
}

void CControlBase::UpdateControl(CRect rc, bool bUpdate)
{
	m_bUpdate = bUpdate;
	CRect rcTemp = rc & m_pMessage->m_rc;
	if (!rcTemp.IsRectEmpty())
	{
		m_pMessage->OnControlUpdate(rcTemp, this);
	}
}

// 发送通知消息
LRESULT CControlBase::SendMessage(UINT uMsg, void *pData/* = NULL*/, size_t lSize/* = 0*/)
{
	if (m_bComControl)
	{
		m_pMessage->OnControlMessage(m_uControlID, uMsg, pData, lSize);
		return 0;
	}

	tagControlMessage  *pControlMessage = new tagControlMessage;
	ZeroMemory(pControlMessage, sizeof(tagControlMessage));

	pControlMessage->uMsg = uMsg;

	if (pData != NULL)
	{
		pControlMessage->lSize = lSize;
		pControlMessage->pData = malloc(pControlMessage->lSize);
		CopyMemory(pControlMessage->pData, pData, pControlMessage->lSize);
	}

	return ::SendMessage(m_hWnd, CONTROL_MESSAGE, m_uControlID, (LPARAM)pControlMessage);
}

// 发送通知消息
LRESULT CControlBase::PostMessage(UINT uMsg, void *pData/* = NULL*/, size_t lSize/* = 0*/)
{
	if (m_bComControl)
	{
		m_pMessage->OnControlMessage(m_uControlID, uMsg, pData, lSize);
		return 0;
	}

	tagControlMessage  *pControlMessage = new tagControlMessage;
	ZeroMemory(pControlMessage, sizeof(tagControlMessage));

	pControlMessage->uMsg = uMsg;

	if (pData != NULL)
	{
		pControlMessage->lSize = lSize;
		pControlMessage->pData = malloc(pControlMessage->lSize);
		CopyMemory(pControlMessage->pData, pData, pControlMessage->lSize);
	}

	return ::PostMessage(m_hWnd, CONTROL_MESSAGE, m_uControlID, (LPARAM)pControlMessage);
}

CControlBaseFont::CControlBaseFont(HWND hWnd, CMessageInterface* pMessage, UINT uControlID, CRect rc, CString strTitle, bool bIsVisible/* = TRUE*/, bool bIsDisable/* = FALSE*/, bool bRresponse/* = TRUE*/,
	CString strFont/* = TEXT("Tahoma")*/, int nFontWidth/* = 12*/, FontStyle fontStyle/* = FontStyleRegular*/)
	: CControlBase(hWnd, pMessage, uControlID, rc, bIsVisible, bIsDisable, bRresponse)
{
	m_strTitle = strTitle;
	m_strFont = strFont;
	m_nFontWidth = nFontWidth;
	m_fontStyle = fontStyle;
	m_uAlignment = DT_LEFT;
	m_uVAlignment = DT_TOP;

	m_bBack = false;
	m_clrBack = Color(254, 0, 128, 0);
	m_clrText = Color(254, 255, 255, 255);
}

CControlBaseFont::~CControlBaseFont(void)
{
}

void CControlBaseFont::SetTitle(CString strTitle)
{
	if (strTitle != m_strTitle)
	{
		SetControlTitle(strTitle);
		UpdateControl(true);
	}
}

void CControlBaseFont::SetFontColor(Color clrText)
{
	//if (m_clrText != clrText)
	{
		SetControlFontColor(clrText);
		UpdateControl(true);
	}
}

void CControlBaseFont::SetAlignment(UINT uAlignment)
{
	if (uAlignment != m_uAlignment)
	{
		m_uAlignment = uAlignment;
		UpdateControl(true);
	}
}

void CControlBaseFont::SetVAlignment(UINT uVAlignment)
{
	if (uVAlignment != m_uVAlignment)
	{
		m_uVAlignment = uVAlignment;
		UpdateControl(true);
	}
}

void CControlBaseFont::SetAlignment(UINT uAlignment, UINT uVAlignment)
{
	if (uAlignment != m_uAlignment || uVAlignment != m_uVAlignment)
	{
		m_uAlignment = uAlignment;
		m_uVAlignment = uVAlignment;

		UpdateControl(true);
	}
}

void CControlBaseFont::SetFont(CString strFont, int nFontWidth, FontStyle fontStyle)
{
	if (m_strFont != strFont || m_nFontWidth != nFontWidth || m_fontStyle != fontStyle)
	{
		m_strFont = strFont;
		m_nFontWidth = nFontWidth;
		m_fontStyle = fontStyle;
		UpdateControl(true);
	}
}