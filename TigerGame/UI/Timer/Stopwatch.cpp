#include "StdAfx.h"
#include "Stopwatch.h"

CStopwatch::CStopwatch(HWND hWnd, CMessageInterface* pMessage, UINT uControlID, CRect rc, bool bIsVisible/* = TRUE*/)
	: CControlBase(hWnd, pMessage, uControlID, rc, bIsVisible, FALSE, FALSE)
{
	m_uTimerStopwatch = 0;
	m_cbHour = 0;
	m_cbBranch = 0;
	m_cbSecond = 0;
	m_cbMillisecond = 0;
}

CStopwatch::~CStopwatch(void)
{
}

bool CStopwatch::SetBitmap(UINT nRangeResourceID, CString strType)
{
	if (m_Image.LoadImage(m_hWnd, nRangeResourceID, strType, 12, 1))
	{
		UpdateControl(true);
		return true;
	}
	return false;
}

bool CStopwatch::SetBitmap(CString strRangeImage)
{
	if (m_Image.LoadImage(m_hWnd, strRangeImage, 12, 1))
	{
		UpdateControl(true);
		return true;
	}
	return false;
}

void CStopwatch::StartTimer(bool bResetTimer)
{
	if (bResetTimer)
	{
		m_uTimerStopwatch = 0;
		m_cbHour = 0;
		m_cbBranch = 0;
		m_cbSecond = 0;
		m_cbMillisecond = 0;
	}

	UpdateControl(m_bUpdate);

	//启动定时器
	if (m_uTimerStopwatch != 0 && GetTiemrIsRun(m_uTimerStopwatch))
	{
		return;
	}
	m_uTimerStopwatch = CTimer::SetTimerEx(10);
}

DWORD CStopwatch::StopTimer()
{
	CTimer::KillTimerEx(m_uTimerStopwatch);
	m_uTimerStopwatch = 0;
	return m_cbMillisecond * (DWORD)100 + m_cbSecond * (DWORD)1000 + m_cbBranch * 60 * (DWORD)1000 + m_cbHour * 60 * 60 * (DWORD)1000;
}

bool CStopwatch::GetRunTimer()
{
	if (m_uTimerStopwatch != 0)
	{
		return GetTiemrIsRun(m_uTimerStopwatch);
	}
	return false;
}

void CStopwatch::DrawControl(CDC &dc, CRect rcParent, CRect rcUpdate)
{
	int nWidth = m_rc.Width();
	int nHeight = m_rc.Height();

	CSize sizeImage = m_Image.GetImageSize();

	if (m_bUpdate)
	{
		UpdateMemDC(dc, sizeImage.cx * 12, sizeImage.cy);

		Graphics graphics(m_memDC);
		graphics.DrawImage(m_Image.GetImage(), Rect(0, 0, sizeImage.cx * 12, sizeImage.cy),
			0, 0, sizeImage.cx * 12, sizeImage.cy, UnitPixel);

	}

	//DrawControlEx(dc, rcParent, rcUpdate, (nWidth - sizeImage.cx * 11) / 2, (nHeight - sizeImage.cy) / 2, sizeImage.cx * 11, sizeImage.cy);
	//时
	DrawControlEx(dc, rcParent, rcUpdate, (nWidth - sizeImage.cx * 11) / 2 + sizeImage.cx * 0, (nHeight - sizeImage.cy) / 2, sizeImage.cx, sizeImage.cy, (m_cbHour / 10) * sizeImage.cx, 0);
	DrawControlEx(dc, rcParent, rcUpdate, (nWidth - sizeImage.cx * 11) / 2 + sizeImage.cx * 1, (nHeight - sizeImage.cy) / 2, sizeImage.cx, sizeImage.cy, (m_cbHour & 10) * sizeImage.cx, 0);

	DrawControlEx(dc, rcParent, rcUpdate, (nWidth - sizeImage.cx * 11) / 2 + sizeImage.cx * 2, (nHeight - sizeImage.cy) / 2, sizeImage.cx, sizeImage.cy, 11 * sizeImage.cx, 0);

	//分
	DrawControlEx(dc, rcParent, rcUpdate, (nWidth - sizeImage.cx * 11) / 2 + sizeImage.cx * 3, (nHeight - sizeImage.cy) / 2, sizeImage.cx, sizeImage.cy, (m_cbBranch / 10) * sizeImage.cx, 0);
	DrawControlEx(dc, rcParent, rcUpdate, (nWidth - sizeImage.cx * 11) / 2 + sizeImage.cx * 4, (nHeight - sizeImage.cy) / 2, sizeImage.cx, sizeImage.cy, (m_cbBranch % 10) * sizeImage.cx, 0);

	DrawControlEx(dc, rcParent, rcUpdate, (nWidth - sizeImage.cx * 11) / 2 + sizeImage.cx * 5, (nHeight - sizeImage.cy) / 2, sizeImage.cx, sizeImage.cy, 11 * sizeImage.cx, 0);

	//秒
	DrawControlEx(dc, rcParent, rcUpdate, (nWidth - sizeImage.cx * 11) / 2 + sizeImage.cx * 6, (nHeight - sizeImage.cy) / 2, sizeImage.cx, sizeImage.cy, (m_cbSecond / 10) * sizeImage.cx, 0);
	DrawControlEx(dc, rcParent, rcUpdate, (nWidth - sizeImage.cx * 11) / 2 + sizeImage.cx * 7, (nHeight - sizeImage.cy) / 2, sizeImage.cx, sizeImage.cy, (m_cbSecond % 10) * sizeImage.cx, 0);

	DrawControlEx(dc, rcParent, rcUpdate, (nWidth - sizeImage.cx * 11) / 2 + sizeImage.cx * 8, (nHeight - sizeImage.cy) / 2, sizeImage.cx, sizeImage.cy, 10 * sizeImage.cx, 0);

	//毫秒
	DrawControlEx(dc, rcParent, rcUpdate, (nWidth - sizeImage.cx * 11) / 2 + sizeImage.cx * 9, (nHeight - sizeImage.cy) / 2, sizeImage.cx, sizeImage.cy, (m_cbMillisecond / 10) * sizeImage.cx, 0);
	DrawControlEx(dc, rcParent, rcUpdate, (nWidth - sizeImage.cx * 11) / 2 + sizeImage.cx * 10, (nHeight - sizeImage.cy) / 2, sizeImage.cx, sizeImage.cy, (m_cbMillisecond % 10) * sizeImage.cx, 0);

}

void CStopwatch::OnTimer(UINT uTimerID)
{
	if (m_uTimerStopwatch == uTimerID)
	{
		CSize sizeImage = m_Image.GetImageSize();
		CRect rc(m_rc.left + (m_rc.Width() - sizeImage.cx * 11) / 2 + sizeImage.cx * 9, m_rc.top + (m_rc.Height() - sizeImage.cy) / 2,
			m_rc.left + (m_rc.Width() - sizeImage.cx * 11) / 2 + sizeImage.cx * 9 + sizeImage.cx * 2, m_rc.top + (m_rc.Height() - sizeImage.cy) / 2 + sizeImage.cy);

		static BYTE cbMillisecond = 0;
		m_cbMillisecond++;
		if (m_cbMillisecond == 100)
		{
			rc.left -= sizeImage.cx * 3;
			m_cbMillisecond = 0;
			m_cbSecond++;
			cbMillisecond = 3;
			if (m_cbSecond == 60)
			{
				rc.left -= sizeImage.cx * 3;
				m_cbSecond = 0;
				m_cbBranch++;
				if (m_cbBranch == 60)
				{
					rc.left -= sizeImage.cx * 3;
					m_cbBranch = 0;
					m_cbHour++;
					if (m_cbHour == 24)
					{
						m_cbHour = 0;
					}
				}
			}
		}


		cbMillisecond++;
		if (cbMillisecond >= 3)
		{
			cbMillisecond = 0;
			UpdateControl(rc, m_bUpdate);
		}
	}
}