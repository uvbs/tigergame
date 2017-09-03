#pragma once
#include "..\Base\ControlBase.h"

class CStopwatch : public CControlBase, public CTimer
{
	IceImage m_Image;

	UINT m_uTimerStopwatch;			// ¶¯»­¶¨Ê±Æ÷
	BYTE m_cbHour;
	BYTE m_cbBranch;
	BYTE m_cbSecond;
	BYTE m_cbMillisecond;

public:
	CStopwatch(HWND hWnd, CMessageInterface* pMessage, UINT uControlID, CRect rc, bool bIsVisible = TRUE);
	~CStopwatch(void);

	bool SetBitmap(UINT nRangeResourceID, CString strType = TEXT("PNG"));
	bool SetBitmap(CString strImage);

	void StartTimer(bool bResetTimer);
	DWORD StopTimer();
	bool GetRunTimer();

protected:
	void DrawControl(CDC &dc, CRect rcParent, CRect rcUpdate);
	virtual void OnTimer(UINT uTimerID);
};
