#pragma once
#include "..\Dialog\DlgPopup.h"

#include <vector>
using namespace std;

struct tagMenuExInfo
{
	UINT		uMenuID;			// 菜单ID
	bool		bSelect;			// 选择
	bool		bDown;				// 按下
};


class CMenuEx :  public CDlgPopup
{
	CString	m_strTitle;				// 标题
	CString m_strFont;
	int m_nFontWidth;
	FontStyle m_fontStyle;
	UINT m_uAlignment;
	UINT m_uVAlignment;

	CPoint m_point;				// 鼠标按下位置
	int m_nLeft;				// 左边间隔
	int m_nHeight;				// 菜单项默认高度
	int m_nWidth;				// 菜单项最大宽度
	int m_nSeparatorHeight;		// 分隔线高度
public:
	CMenuEx(CString strFont = TEXT("Tahoma"), int nFontWidth = 12, FontStyle fontStyle = FontStyleRegular);
	~CMenuEx(void);

	bool Create(CWnd *pParent, CPoint point, UINT uControlID, UINT nResourceID, int nFrameSize = 4, enumBackMode enBackMode = enBMFrame, int nMinWidth = 113);
	bool Create(CWnd *pParent, CPoint point, UINT uControlID, CString strImage, int nFrameSize = 4, enumBackMode enBackMode = enBMFrame, int nMinWidth = 113);

	int AddMenu(CString strText, UINT uMenuID, int nResourceID = -1, bool bSelect = false, int nIndex = -1);
	int AddSeparator(int nIndex = -1);

	// 设置菜单项位置
	void SetMenuPoint();

	virtual void DrawWindowEx(CDC &dc, CRect rcClient);
	virtual void InitUI(CRect rcClient);

	// 消息响应
	virtual LRESULT OnControlMessage(UINT uControlID, UINT uMsg, void *pData, size_t lSize);
	virtual LRESULT OnWindowMessage(UINT uControlID, UINT uMsg, void *pData, size_t lSize);
};
