#pragma once
#include "..\Base\ControlBase.h"

class CStaticText : public CControlBaseFont
{
public:
	CStaticText(HWND hWnd, CMessageInterface* pMessage, UINT uControlID, CRect rc, CString strTitle, 
		Color clr = Color(254, 0, 0, 0), CString strFont = TEXT("Tahoma"), int nFontWidth = 12, FontStyle fontStyle = FontStyleRegular, bool bIsVisible = TRUE);
	~CStaticText(void);

	void SetMarkText(CString strMark);
	void SetTitleMarkText(CString strTitle, CString strMark, int nStart = 0);
	void SetMarkText(CString strMark, Color clrMark = Color(237, 128, 27), int nStart = 0);
	void SetMarkText(CString strTitle, CString strMark, Color clrMark = Color(237, 128, 27), int nStart = 0);
	bool SetBitmap(UINT nResourceID, CString strType = TEXT("PNG"));
	bool SetBitmap(CString strImage);

protected:
	void DrawControl(CDC &dc, CRect rcParent, CRect rcUpdate);

	IceImage m_Image;
	Color m_clrMark;
	CString m_strMark;
	int m_nStart;
};