#pragma once
#include "..\Base\ControlBase.h"

class CTextButton : public CControlBaseFont
{
public:
	CTextButton(HWND hWnd, CMessageInterface* pMessage, UINT uControlID, CRect rc, CString strTitle,
		Color clrTextNormal = Color(254, 39, 134, 230) ,Color clrTextHover = Color(254, 93, 169, 242),
		Color clrTextDown = Color(254, 21, 112, 235) ,Color clrTextDisable =  Color(254, 128, 128, 128),
		bool bIsVisible = TRUE, bool bIsDisable = FALSE);
	~CTextButton(void);
	void SetTitleText(CString strTitle);

protected:
	virtual bool OnControlMouseMove(UINT nFlags, CPoint point);
	virtual bool OnControlLButtonDown(UINT nFlags, CPoint point);
	virtual bool OnControlLButtonUp(UINT nFlags, CPoint point);

	void DrawControl(CDC &dc, CRect rcParent, CRect rcUpdate);

public:
	Color m_clrTextNormal;
	Color m_clrTextHover;
	Color m_clrTextDown;
	Color m_clrTextDisable;
	CRect m_rcText;
	enumButtonState m_enButtonState;
	bool m_bDown;
	// ����������ɫ����
	Color m_clrBkgNormal;
	Color m_clrBkgHover;
	Color m_clrBkgDown;
	Color m_clrBkgDisable;
	bool m_bIsBkg;				// �Ƿ����ñ�����ɫ
	bool m_bAutoWidth;			// ����Ӧ���ֿ�� ��ʱ����
	void SetControlBkg(Color clrBkgNormal = Color(128, 221, 221, 221), Color clrBkgHover = Color(254, 146, 146, 146),
		Color clrBkgDown = Color(254, 42, 184, 246), Color clrBkgDisable = Color(128, 0, 0, 0));
};