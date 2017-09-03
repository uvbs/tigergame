#pragma once
#include "..\Dialog\DlgPopup.h"

#include <vector>
using namespace std;

struct tagMenuExInfo
{
	UINT		uMenuID;			// �˵�ID
	bool		bSelect;			// ѡ��
	bool		bDown;				// ����
};


class CMenuEx :  public CDlgPopup
{
	CString	m_strTitle;				// ����
	CString m_strFont;
	int m_nFontWidth;
	FontStyle m_fontStyle;
	UINT m_uAlignment;
	UINT m_uVAlignment;

	CPoint m_point;				// ��갴��λ��
	int m_nLeft;				// ��߼��
	int m_nHeight;				// �˵���Ĭ�ϸ߶�
	int m_nWidth;				// �˵��������
	int m_nSeparatorHeight;		// �ָ��߸߶�
public:
	CMenuEx(CString strFont = TEXT("Tahoma"), int nFontWidth = 12, FontStyle fontStyle = FontStyleRegular);
	~CMenuEx(void);

	bool Create(CWnd *pParent, CPoint point, UINT uControlID, UINT nResourceID, int nFrameSize = 4, enumBackMode enBackMode = enBMFrame, int nMinWidth = 113);
	bool Create(CWnd *pParent, CPoint point, UINT uControlID, CString strImage, int nFrameSize = 4, enumBackMode enBackMode = enBMFrame, int nMinWidth = 113);

	int AddMenu(CString strText, UINT uMenuID, int nResourceID = -1, bool bSelect = false, int nIndex = -1);
	int AddSeparator(int nIndex = -1);

	// ���ò˵���λ��
	void SetMenuPoint();

	virtual void DrawWindowEx(CDC &dc, CRect rcClient);
	virtual void InitUI(CRect rcClient);

	// ��Ϣ��Ӧ
	virtual LRESULT OnControlMessage(UINT uControlID, UINT uMsg, void *pData, size_t lSize);
	virtual LRESULT OnWindowMessage(UINT uControlID, UINT uMsg, void *pData, size_t lSize);
};
