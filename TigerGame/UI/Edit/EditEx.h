#pragma once
#include "..\Base\ControlBase.h"

#define					BUTTOM				1
#define					EDIT				2

class CEditExx : public CEdit
{

public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};

class CEditEx : public CControlBaseFont
{
public:
	CEditEx(HWND hWnd, CMessageInterface* pMessage, UINT uControlID, CRect rc, CString strTitle = TEXT(""), bool bPassWord = FALSE, bool bIsVisible = TRUE, bool bIsDisable = FALSE, bool bIsPressDown = FALSE);
	bool SetBitmap(UINT nResourceID, CString strType = TEXT("PNG"));
	bool SetBitmap(CString strImage);
	bool SetSmallBitmap(UINT nResourceID, CString strType = TEXT("PNG"));
	bool SetSmallBitmap(CString strImage);
	~CEditEx(void);

	virtual void SetRect(CRect rc);
	virtual bool OnFocus(bool bFocus);
	virtual bool GetFocus() { return TRUE; };
	bool GetLButtonDown();
	bool GetLButtonUp();
protected:
	virtual bool OnControlMouseMove(UINT nFlags, CPoint point);
	virtual bool OnControlLButtonDown(UINT nFlags, CPoint point);
	virtual bool OnControlLButtonUp(UINT nFlags, CPoint point);
	void DrawControl(CDC &dc, CRect rcParent, CRect rcUpdate);

	void ShowEdit();
	void HideEdit();

public:
	CString m_strFileImage;
	IceImage m_Image;
	IceImage m_smallImage;
	bool m_smallImage_left;
	bool m_bPassWord;
	bool m_bDown;
	bool m_bDownTemp;
	enumButtonState m_buttonState;
	enumButtonState m_EditState;
	CRect m_rcText;
	CFont	m_fontTemp;

	CEditExx	*m_pEdit;
};