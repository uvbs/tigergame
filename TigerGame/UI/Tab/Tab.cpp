#include "StdAfx.h"
#include "Tab.h"
CTab::CTab(HWND hWnd, CMessageInterface* pMessage, UINT uControlID, CRect rc, CString strTitle/*= TEXT("")*/, BOOL bIsVisible/* = TRUE*/,
	BOOL bIsDisable/* = FALSE*/, BOOL bIsPressDown/* = FALSE*/)
	: CControlBaseFont(hWnd, pMessage, uControlID, rc, strTitle, bIsVisible, bIsDisable)
{
	m_nHoverItem = -1;
	m_nDownItem = -1;

}

CTab::~CTab(void)
{
	for (size_t i = 0; i < m_vecItemInfo.size(); i++)
	{
		ItemInfo * itemInfo = m_vecItemInfo.at(i);
		if (itemInfo)
		{
			delete itemInfo;
		}
	}
}

bool CTab::InsertItem(int nItem, UINT nItemID, CString strItemText, UINT nResourceID, int nItemWidth/* = 0*/, CString strType/*= TEXT("PNG")*/)
{
	ItemInfo *itemInfo = NULL;
	itemInfo = new ItemInfo;
	itemInfo->nItemID = nItemID;
	itemInfo->strText = strItemText;

	itemInfo->image.LoadImage(m_hWnd, nResourceID, strType, 3);

	itemInfo->rc.SetRect(m_rc.left, m_rc.top, m_rc.left + (nItemWidth == 0 ? itemInfo->image.GetImageSize().cx : nItemWidth), m_rc.bottom);
	return InsertItem(nItem, itemInfo);
}

bool CTab::InsertItem(int nItem, UINT nItemID, CString strItemText, CString strImage, int nItemWidth/* = 0*/)
{
	ItemInfo *itemInfo = NULL;
	itemInfo->nItemID = nItemID;
	itemInfo->strText = strItemText;
	itemInfo->image.LoadImage(m_hWnd, strImage, 3);
	itemInfo->rc.SetRect(m_rc.left, m_rc.top, m_rc.left + (nItemWidth == 0 ? itemInfo->image.GetImageSize().cx : nItemWidth), m_rc.bottom);
	return InsertItem(nItem, itemInfo);
}

bool CTab::InsertItem(int nItem, ItemInfo *itemInfo)
{
	if (m_vecItemInfo.size() > 0)
	{
		CRect rc;
		m_vecRcSeperator.push_back(rc);
	}
	if (nItem <= -1 || nItem >= (int)m_vecItemInfo.size())
	{
		m_vecItemInfo.push_back(itemInfo);
	}
	else
	{
		m_vecItemInfo.insert(m_vecItemInfo.begin() + nItem, itemInfo);
	}

	int nXPos = 0;
	int nYPos = 0;

	for (size_t i = 0; i < m_vecItemInfo.size(); i++)
	{
		ItemInfo *itemInfoTemp = m_vecItemInfo.at(i);
		int nItemWidth = itemInfoTemp->rc.Width();
		itemInfoTemp->rc.SetRect(nXPos, nYPos, nXPos + nItemWidth, nYPos + itemInfoTemp->image.GetImageSize().cy);

		nXPos += nItemWidth;

		if (i < m_vecItemInfo.size() - 1 && m_ImageSeperator.GetImage() != NULL)
		{
			CSize sizeSeperator = m_ImageSeperator.GetImageSize();
			CRect &rc = m_vecRcSeperator.at(i);
			rc.SetRect(nXPos, nYPos, nXPos + sizeSeperator.cx, nYPos + sizeSeperator.cy);
			nXPos += sizeSeperator.cx;
		}
	}

	UpdateControl(true);
	return true;
}

void CTab::UpdateRect()
{
	int nSize = m_vecItemInfo.size();
	if (nSize > 0)
	{
		int nItemWidth = (m_rc.Width() + nSize) / nSize;
		int nItemHeight = m_rc.Height();
		int nXPos = 0;
		int nYPos = 0;

		/*	for(size_t i = 0; i < nSize; i++)
		{
		ItemInfo *itemInfoTemp = m_vecItemInfo.at(i);
		if(i == nSize - 1)
		{
		itemInfoTemp->rc.SetRect(nXPos, nYPos, m_rc.Width() -1, nYPos + nItemHeight);
		}
		else
		{
		itemInfoTemp->rc.SetRect(nXPos, nYPos, nXPos + nItemWidth, nYPos + nItemHeight);
		}

		nXPos += nItemWidth-1;
		}*/
		for (size_t i = 0; i < m_vecItemInfo.size(); i++)
		{
			ItemInfo *itemInfoTemp = m_vecItemInfo.at(i);
			int nItemWidth = itemInfoTemp->rc.Width();
			itemInfoTemp->rc.SetRect(nXPos, nYPos, nXPos + nItemWidth, nYPos + itemInfoTemp->image.GetImageSize().cy);

			nXPos += nItemWidth;

			if (i<m_vecItemInfo.size() - 1 && m_ImageSeperator.GetImage() != NULL)
			{
				CSize sizeSeperator = m_ImageSeperator.GetImageSize();
				CRect &rc = m_vecRcSeperator.at(i);
				rc.SetRect(nXPos, nYPos, nXPos + sizeSeperator.cx, nYPos + sizeSeperator.cy);
				nXPos += sizeSeperator.cx;
			}
		}
	}
}

bool CTab::SetControlRect(CRect rc)
{
	m_rc = rc;
	UpdateRect();

	return true;
}


bool CTab::SetSeperator(BOOL bSeperator, UINT nResourceID/* = 0*/, CString strType/*= TEXT("PNG")*/)
{
	if (m_ImageSeperator.LoadImage(m_hWnd, nResourceID, strType))
	{
		UpdateControl(true);
		return true;
	}
	return false;
}

bool CTab::SetSeperator(BOOL bSeperator, CString strImage/* = TEXT("")*/)
{
	if (m_ImageSeperator.LoadImage(m_hWnd, strImage))
	{
		return true;
	}
	return false;
}

int CTab::SetSelectItem(int nItem)
{
	int nOldDownItem = m_nDownItem;
	if (m_nDownItem != nItem && nItem >= 0 && nItem < (int)m_vecItemInfo.size())
	{
		m_nDownItem = nItem;
		if (m_nDownItem == m_nHoverItem)
		{
			m_nHoverItem = -1;
		}
		UpdateControl(m_bUpdate);
	}

	return nOldDownItem;
}

bool CTab::OnControlMouseMove(UINT nFlags, CPoint point)
{
	int nOldHoverItem = m_nHoverItem;

	if (m_rc.PtInRect(point))
	{
		point.x -= m_rc.left;
		point.y -= m_rc.top;

		if (m_nHoverItem != -1)
		{
			ItemInfo *itemInfo = m_vecItemInfo.at(m_nHoverItem);
			if (itemInfo->rc.PtInRect(point))
			{
				return false;
			}
			m_nHoverItem = -1;
		}

		BOOL bMousenDown = false;
		if (m_nDownItem != -1)
		{
			ItemInfo *itemInfo = m_vecItemInfo.at(m_nDownItem);
			if (itemInfo->rc.PtInRect(point))
			{
				bMousenDown = true;
				m_nHoverItem = -1;
			}
		}

		if (!bMousenDown)
		{
			for (size_t i = 0; i < m_vecItemInfo.size(); i++)
			{
				ItemInfo *itemInfo = m_vecItemInfo.at(i);
				if (itemInfo->rc.PtInRect(point))
				{
					m_nHoverItem = i;
					break;
				}
			}
		}
	}
	else
	{
		m_nHoverItem = -1;
	}

	if (nOldHoverItem != m_nHoverItem)
	{
		UpdateControl(m_bUpdate);
		return true;
	}

	return false;
}

bool CTab::OnControlLButtonDown(UINT nFlags, CPoint point)
{
	if (m_nHoverItem != -1)
	{
		point.x -= m_rc.left;
		point.y -= m_rc.top;

		ItemInfo *itemInfo = m_vecItemInfo.at(m_nHoverItem);
		if (itemInfo->rc.PtInRect(point))
		{
			if (m_nDownItem != m_nHoverItem)
			{
				m_nDownItem = m_nHoverItem;
				PostMessage(BUTTOM_DOWN, &m_nDownItem, sizeof(m_nDownItem));

				UpdateControl(m_bUpdate);

				return true;
			}
		}
	}

	return false;
}

bool CTab::OnControlLButtonUp(UINT nFlags, CPoint point)
{
	return false;
}

void CTab::DrawControl(CDC &dc, CRect rcParent, CRect rcUpdate)
{
	int nWidth = m_rc.Width();
	int nHeight = m_rc.Height();

	if (m_bUpdate)
	{
		UpdateMemDC(dc, nWidth, nHeight * 3);

		Graphics graphics(m_memDC);

		for (int i = 0; i < 3; i++)
		{
			int nXPos = 0;
			int nYPos = i * nHeight;
			for (size_t j = 0; j < m_vecItemInfo.size(); j++)
			{
				ItemInfo *itemInfo = m_vecItemInfo.at(j);

				Image *pImage = itemInfo->image.GetImage();
				CSize sizeImage = itemInfo->image.GetImageSize();

				graphics.DrawImage(pImage, Rect(nXPos, nYPos, itemInfo->rc.Width(), itemInfo->rc.Height()),
					sizeImage.cx * i, 0, sizeImage.cx, sizeImage.cy, UnitPixel);

				nXPos += itemInfo->rc.Width();

				if (j < m_vecItemInfo.size() - 1 && m_ImageSeperator.GetImage() != NULL)
				{
					CSize sizeSeperator = m_ImageSeperator.GetImageSize();
					CRect &rc = m_vecRcSeperator.at(j);
					graphics.DrawImage(m_ImageSeperator.GetImage(), Rect(nXPos, nYPos + 10, sizeSeperator.cx, sizeSeperator.cy + 20),
						0, 0, sizeSeperator.cx, sizeSeperator.cy, UnitPixel);

					nXPos += sizeSeperator.cx;
				}
			}
		}
	}

	for (size_t i = 0; i < m_vecItemInfo.size(); i++)
	{
		ItemInfo *itemInfo = m_vecItemInfo.at(i);
		if (m_nHoverItem == i)
		{
			DrawControlEx(dc, rcParent, rcUpdate, itemInfo->rc.left, itemInfo->rc.top, itemInfo->rc.Width(), itemInfo->rc.Height(), itemInfo->rc.left, itemInfo->rc.top + m_rc.Height());
		}
		else if (m_nDownItem == i)
		{
			DrawControlEx(dc, rcParent, rcUpdate, itemInfo->rc.left, itemInfo->rc.top, itemInfo->rc.Width(), itemInfo->rc.Height(), itemInfo->rc.left, itemInfo->rc.top + m_rc.Height() * 2);
		}
		else
		{
			DrawControlEx(dc, rcParent, rcUpdate, itemInfo->rc.left, itemInfo->rc.top, itemInfo->rc.Width(), itemInfo->rc.Height(), itemInfo->rc.left, itemInfo->rc.top);
		}
	}

	for (size_t i = 0; i < m_vecRcSeperator.size(); i++)//显示分割线
	{
		CRect &rc = m_vecRcSeperator.at(i);
		DrawControlEx(dc, rcParent, rcUpdate, rc.left, rc.top, rc.Width(), rc.Height() + 30, rc.left, rc.top);
	}
}