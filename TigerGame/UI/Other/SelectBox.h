#pragma once
#include "..\Base\ControlBase.h"
#include <vector>

using namespace  std;

#define						MSG_SKIN						1001
#define						MSG_SELECT_SKIN					1002

struct tagSelectBox
{
	int nType;
	Color clr;
	UINT uIndex;	
};

class CSelectBox : public CControlBase
{
public:
	CSelectBox(HWND hWnd, CMessageInterface* pMessage, UINT uControlID, CRect rc, int nXCount, int nYCount, bool bImage,
		Color clrFrame = Color(254, 0, 0, 0), Color clrHover = Color(128, 0, 0, 0), Color clrSelect = Color(254, 255, 255, 255), bool bIsVisible = TRUE);
	~CSelectBox(void);

	bool SetBitmap(UINT nResourceID, Color clr = Color(255, 0, 0, 0), int nIndex = -1, CString strType = TEXT("PNG"));
	bool SetBitmap(CString strImage, Color clr = Color(255, 0, 0, 0), int nIndex = -1);
	bool SetColor(Color clr, int nIndex = -1);
	bool SetColor(Color clr[], int nColorCount);

	virtual void SetControlVisible(bool bIsVisible);

protected:
	void DrawControl(CDC &dc, CRect rcParent, CRect rcUpdate);

	virtual bool OnControlMouseMove(UINT nFlags, CPoint point);
	virtual bool OnControlLButtonDown(UINT nFlags, CPoint point);

	bool m_bImage;
	vector<IceImage *> m_vecImage;
	vector<Color>  m_vecclr;
	int m_nXCount;
	int m_nYCount;
	Color m_clrFrame;
	Color m_clrHover;
	Color m_clrSelect;

	int m_nXHover;
	int m_nYHover;
	int m_nXSelect;
	int m_nYSelect;
};
