#pragma once
#include "..\Base\ControlBase.h"

enum enumShowMode
{
	enSMNormal = 0,			// 正常
	enSMTile,				// 平铺
	enSMExtrude,			// 拉伸
	enSMFrame				// 边框
};

class CPicture : public CControlBase
{
public:
	CPicture(HWND hWnd, CMessageInterface* pMessage, UINT uControlID, CRect rc, 
		UINT uVerticalAlignment = DT_CENTER, UINT uHorizontalAlignment = DT_VCENTER, bool bIsVisible = TRUE);
	~CPicture(void);

	bool SetBitmap(UINT nResourceID, CString strType = TEXT("PNG"));
	bool SetBitmap(CString strImage);
	bool SetBitmap(Image *pImage);

	bool SetShowMode(enumShowMode enShowMode, int nFrameSize = 2);

protected:
	void DrawControl(CDC &dc, CRect rcParent, CRect rcUpdate);
	IceImage m_Image;
	UINT m_uVerticalAlignment;
	UINT m_uHorizontalAlignment;
	enumShowMode m_enShowMode;
	int m_nFrameSize;
};
