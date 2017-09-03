
// IceMusic.h : 头文件
//

#pragma once
#include "afxwin.h"
#include "afxcmn.h"

#include "./UI/IceDUI.h"

// CDlgLogin 对话框
class CDlgLogin : public CDlgBase
{
	// 构造
public:
	
	CDlgLogin(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_ICE_MUSIC_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg HCURSOR OnQueryDragIcon();

	DECLARE_MESSAGE_MAP()

	virtual void InitUI(CRect rcClient);
	virtual void OnSize(UINT nType, CRect rcClient);
	// 重置控件
	virtual void ResetControlBackHSL(COLOR_HSL colorHSL, COLOR_HSL OldColorHSL);

	// 消息响应
	virtual LRESULT OnControlMessage(UINT uControlID, UINT uMsg, void *pData, size_t lSize);
	virtual LRESULT OnWindowMessage(UINT uControlID, UINT uMsg, void *pData, size_t lSize);
	virtual void OnWindowMouseLeave();
	virtual void OnWindowMouseHover();
			
public:

	// 定时器消息
	virtual void OnTimer(UINT uTimerID);
};
