#include "StdAfx.h"
#include "EditEx.h"
#include ".\editex.h"

CEditEx::CEditEx(HWND hWnd, CMessageInterface* pMessage, UINT uControlID, CRect rc, CString strTitle/* = ""*/, bool bPassWord/* = FALSE*/, bool bIsVisible/* = TRUE*/,
	bool bIsDisable/* = FALSE*/, bool bIsPressDown/* = FALSE*/)
	: CControlBaseFont(hWnd, pMessage, uControlID, rc, strTitle, bIsVisible, bIsDisable)
{
	m_bDown = false;
	m_bDownTemp = false;
	m_smallImage_left = false;
	m_buttonState = enBSNormal;
	m_EditState = enBSNormal;
	m_pEdit = NULL;
	m_bPassWord = bPassWord;
	m_uAlignment = DT_LEFT;
	m_uVAlignment = DT_VCENTER;
	SetRect(rc);
	VERIFY(m_fontTemp.CreateFont(
		16,							// 字体的高度  
		0,							// 字体的宽度  
		0,							// 字体显示的角度
		0,							// 字体的角度
		FW_DONTCARE,				// 字体的磅数
		FALSE,						// 斜体字体
		FALSE,						// 带下划线的字体
		0,							// 带删除线的字体
		GB2312_CHARSET,				// 所需的字符集
		OUT_DEFAULT_PRECIS,			// 输出的精度
		CLIP_DEFAULT_PRECIS,		// 裁减的精度
		DEFAULT_QUALITY,			// 逻辑字体与输出设备的实际
		DEFAULT_PITCH | FF_SWISS,	// 字体间距和字体集
		L"宋体"));
}

CEditEx::~CEditEx(void)
{
	if (m_fontTemp.m_hObject)
	{
		m_fontTemp.DeleteObject();
	}
}

bool CEditEx::SetBitmap(UINT nResourceID, CString strType/* = TEXT("PNG")*/)
{
	if (m_Image.LoadImage(m_hWnd, nResourceID, strType, 2))
	{
		UpdateControl(true);
		return true;
	}
	return false;
}

bool CEditEx::SetBitmap(CString strImage)
{
	if (m_Image.LoadImage(m_hWnd, strImage, 2))
	{
		UpdateControl(true);
		return true;
	}
	return false;
}

bool CEditEx::SetSmallBitmap(UINT nResourceID, CString strType/* = TEXT("PNG")*/)
{
	if (m_smallImage.LoadImage(m_hWnd, nResourceID, strType, 4))
	{
		return true;
	}
	return false;
}

bool CEditEx::SetSmallBitmap(CString strImage)
{
	if (m_smallImage.LoadImage(m_hWnd, strImage, 4))
	{
		return true;
	}
	return false;
}

void  CEditEx::SetRect(CRect rc)
{
	m_rc = rc;
	m_rcText = m_rc;
	m_rcText.top += 4;
	m_rcText.left += 6;
	m_rcText.bottom -= 4;
	m_rcText.right -= (3 + (m_smallImage.GetImage() != NULL ? m_smallImage.GetImageSize().cx : 0));
}

bool CEditEx::OnFocus(bool bFocus)
{
	enumButtonState buttonState = m_buttonState;
	enumButtonState editState = m_EditState;

	m_bDown = false;
	m_buttonState = enBSNormal;
	m_EditState = enBSNormal;
	HideEdit();

	if (buttonState != m_buttonState || editState != m_EditState)
	{
		UpdateControl(m_bUpdate);
		return true;
	}

	return false;
}

bool CEditEx::OnControlMouseMove(UINT nFlags, CPoint point)
{
	enumButtonState buttonState = m_buttonState;
	enumButtonState editState = m_EditState;
	if (!m_bIsDisable)
	{
		if (m_rc.PtInRect(point))
		{
			CSize sizeSmallImage = m_smallImage.GetImageSize();

			m_EditState = enBSHover;
			if (m_buttonState != enBSDown)
			{
				CRect  rc;
				rc.left = m_rc.right - sizeSmallImage.cx - 2;
				rc.top = m_rc.top + (m_rc.Height() - sizeSmallImage.cy) / 2;
				rc.right = rc.left + sizeSmallImage.cx;
				rc.bottom = rc.top + sizeSmallImage.cy;

				if (rc.PtInRect(point))
				{
					m_buttonState = enBSHover;
				}
				else
				{
					//ShowEdit();
					m_buttonState = enBSNormal;
				}
			}
		}
		else
		{
			if (m_buttonState != enBSDown)
			{
				m_buttonState = enBSNormal;
				m_EditState = enBSNormal;
			}
		}
	}
	if (buttonState != m_buttonState || editState != m_EditState)
	{
		UpdateControl(m_bUpdate);
		return true;
	}

	return false;
}

bool CEditEx::OnControlLButtonDown(UINT nFlags, CPoint point)
{
	enumButtonState buttonState = m_buttonState;
	enumButtonState editState = m_EditState;
	if (!m_bIsDisable)
	{
		if (m_rc.PtInRect(point))
		{
			CSize sizeSmallImage = m_smallImage.GetImageSize();
			CRect  rc;
			rc.left = m_rc.right - sizeSmallImage.cx - 2;
			rc.top = m_rc.top + (m_rc.Height() - sizeSmallImage.cy) / 2;
			rc.right = rc.left + sizeSmallImage.cx;
			rc.bottom = rc.top + sizeSmallImage.cy;

			if (rc.PtInRect(point))
			{
				m_bDown = !m_bDown;
				m_bDownTemp = true;
				if (m_bDown)
				{
					m_buttonState = enBSDown;
				}
				else
				{
					m_buttonState = enBSHover;
				}

				UINT uOperate = BUTTOM_DOWN;
				PostMessage(BUTTOM, &uOperate, sizeof(uOperate));
				HideEdit();
			}
			else
			{
				if (m_bDown)
				{
					m_bDown = false;
					m_buttonState = enBSHover;
				}
				ShowEdit();

				UINT uOperate = BUTTOM_DOWN;
				PostMessage(EDIT, &uOperate, sizeof(uOperate));
			}
		}
		else
		{
			m_buttonState = enBSNormal;
			m_EditState = enBSNormal;
		}
	}

	if (buttonState != m_buttonState || editState != m_EditState)
	{
		UpdateControl(m_bUpdate);
		return true;
	}

	return false;;
}

bool CEditEx::OnControlLButtonUp(UINT nFlags, CPoint point)
{
	enumButtonState buttonState = m_buttonState;
	enumButtonState editState = m_EditState;
	if (!m_bIsDisable)
	{
		
		if (m_rc.PtInRect(point))
		{
			m_EditState = enBSHover;

			CSize sizeSmallImage = m_smallImage.GetImageSize();
			CRect  rc;
			rc.left = m_rc.right - sizeSmallImage.cx - 2;
			rc.top = m_rc.top + (m_rc.Height() - sizeSmallImage.cy) / 2;
			rc.right = rc.left + sizeSmallImage.cx;
			rc.bottom = rc.top + sizeSmallImage.cy;

			if (rc.PtInRect(point))
			{
				if (m_bDown)
				{
					m_buttonState = enBSDown;
				}
				else
				{
					m_buttonState = enBSHover;
				}

				UINT uOperate = BUTTOM_UP;
				PostMessage(BUTTOM, &uOperate, sizeof(uOperate));
			}
			else
			{
				if (m_bDown)
				{
					m_buttonState = enBSDown;
				}
				else
				{
					m_buttonState = enBSNormal;
				}
				UINT uOperate = BUTTOM_UP;
				PostMessage(EDIT, &uOperate, sizeof(uOperate));
			}
		}
		else
		{
			if (!m_bDown)
			{
				m_buttonState = enBSNormal;
				m_EditState = enBSNormal;
			}
		}
	}

	m_bDownTemp = false;
	if (buttonState != m_buttonState || editState != m_EditState)
	{
		UpdateControl(m_bUpdate);
		return true;
	}

	return false;;
}

void CEditEx::DrawControl(CDC &dc, CRect rcParent, CRect rcUpdate)
{

	int nWidth = m_rc.Width();
	int nHeight = m_rc.Height();

	if (m_bUpdate)
	{
		UpdateMemDC(dc, nWidth * 2 + m_smallImage.GetImageSize().cx * 4, nHeight);

		Graphics graphics(m_memDC);
		CRect  rcTemp(0, 0, nWidth, nHeight);
		//SetBkMode(m_memDC, TRANSPARENT);

		Image *pImage = m_Image.GetImage();
		CSize sizeImage = m_Image.GetImageSize();
		for (int i = 0; i < 2; i++)
		{
			DrawImageFrame(graphics, pImage, rcTemp, i * sizeImage.cx, 0, sizeImage.cx, sizeImage.cy, 4);
			rcTemp.OffsetRect(nWidth, 0);
		}
		Image*pSmallImage = m_smallImage.GetImage();
		CSize sizeSmallImage = m_smallImage.GetImageSize();
		int nXPos = 2 * nWidth;
		for (int i = 0; i < 4; i++)
		{
			//ODS1(L"EditEx nXPos=%d nWidth=%d \r\n", nXPos, nWidth);
			// 绘制图片
			graphics.DrawImage(pSmallImage, Rect(nXPos, 0, sizeSmallImage.cx, sizeSmallImage.cy),
				sizeSmallImage.cx * i, 0, sizeSmallImage.cx, sizeSmallImage.cy, UnitPixel);
			nXPos += sizeSmallImage.cx;
		}
		if (!m_strTitle.IsEmpty())
		{
			//FontFamily fontFamily(m_strFont);
			FontFamily fontFamily(L"宋体");
			Font font(&fontFamily, (REAL)16, FontStyleRegular, UnitPixel);
			SolidBrush solidBrush(Color(128, 255, 0, 0));
			graphics.SetTextRenderingHint(TextRenderingHintAntiAliasGridFit);

			StringFormat strFormat;
			strFormat.SetAlignment(StringAlignmentNear);
			strFormat.SetTrimming(StringTrimmingEllipsisCharacter);
			strFormat.SetFormatFlags(StringFormatFlagsNoWrap | StringFormatFlagsMeasureTrailingSpaces);

			Size size = GetTextBounds(font, strFormat, m_strTitle);
			CPoint point = GetOriginPoint(nWidth - m_smallImage.GetImageSize().cx - 2, nHeight, size.Width, size.Height, m_uAlignment, m_uVAlignment);
			RectF  offRect(point.x + 3, point.y + 1, nWidth - point.x, size.Height);
			graphics.DrawString(m_strTitle, (INT)_tcslen(m_strTitle), &font, offRect, &strFormat, &solidBrush);
			offRect.X = offRect.X + nWidth;
			offRect.Y = offRect.Y;
			offRect.Width = m_rcText.Width();
			offRect.Height = m_rcText.Height();
			graphics.DrawString(m_strTitle, (INT)_tcslen(m_strTitle), &font, offRect, &strFormat, &solidBrush);

		}
	}
	DrawControlEx(dc, rcParent, rcUpdate, 0, 0, nWidth, nHeight, m_EditState* nWidth, 0);

	CRect  rc;
	rc.left = m_rc.right - m_smallImage.GetImageSize().cx - 2;
	rc.top = m_rc.top + (m_rc.Height() - m_smallImage.GetImageSize().cy) / 2;
	rc.right = rc.left + m_smallImage.GetImageSize().cx;
	rc.bottom = rc.top + m_smallImage.GetImageSize().cy;
	DrawControlEx(dc, rc, rcParent, rcUpdate, 0, 0, m_smallImage.GetImageSize().cx, nHeight, nWidth * 2 + m_buttonState*m_smallImage.GetImageSize().cx, 0);
}

bool CEditEx::GetLButtonDown()
{
	return m_buttonState == enBSDown;;
}

bool CEditEx::GetLButtonUp()
{
	return FALSE;
}

void CEditEx::ShowEdit()
{
	if (NULL == m_pEdit)
	{
		CRect rc;
		rc = m_rcText;
		rc.left--;
		rc.top += 2;

		m_pEdit = new CEditExx;
		m_pEdit->Create(WS_CHILD | WS_VISIBLE | WS_TABSTOP, rc, CWnd::FromHandle(m_hWnd), 11111);
		m_pEdit->SetFont(&m_fontTemp);
		m_pEdit->SetWindowText(m_strTitle);
		if (m_bPassWord)
		{
			m_pEdit->SetPasswordChar('*');
		}
		m_pEdit->SetSel(0, -1);
		m_pEdit->SetFocus();
	}
}

void CEditEx::HideEdit()
{
	if (m_pEdit)
	{
		m_pEdit->GetWindowText(m_strTitle);
		if (m_bPassWord)
		{
			int nlen = m_strTitle.GetLength();
			m_strTitle = "";
			for (int i = 0; i < nlen; i++)
			{
				m_strTitle += '*';
			}
		}
		delete m_pEdit;
		m_pEdit = NULL;
		UpdateControl(true);
	}

}
BOOL CEditExx::PreTranslateMessage(MSG* pMsg)
{

	if (pMsg->message == WM_MOUSEMOVE)//鼠标移动消息
	{
		CWnd *pWnd = GetParent(); //获取父窗口指针
		if (pWnd && pWnd->GetSafeHwnd()) //父窗口指针有效
		{

			//计算此控件与父窗口的位置偏差
			CRect this_rect;
			GetWindowRect(&this_rect);
			CRect parent_rect;
			pWnd->GetWindowRect(&parent_rect);
			CPoint offset(this_rect.left - parent_rect.left, this_rect.top - parent_rect.top);
			//当前窗口中的鼠标位置
			CPoint point(LOWORD(pMsg->lParam), HIWORD(pMsg->lParam));
			//换算到父窗口位置
			CPoint parentPoint = point + offset;
			//传递消息给父窗口
			::PostMessage(pWnd->m_hWnd, WM_MOUSEMOVE, pMsg->wParam, MAKELONG(parentPoint.x, parentPoint.y));
		}
	}

	return CEdit::PreTranslateMessage(pMsg);
}
