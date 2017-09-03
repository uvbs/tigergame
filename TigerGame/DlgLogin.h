
// IceMusic.h : ͷ�ļ�
//

#pragma once
#include "afxwin.h"
#include "afxcmn.h"

#include "./UI/IceDUI.h"

// CDlgLogin �Ի���
class CDlgLogin : public CDlgBase
{
	// ����
public:
	
	CDlgLogin(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_ICE_MUSIC_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg HCURSOR OnQueryDragIcon();

	DECLARE_MESSAGE_MAP()

	virtual void InitUI(CRect rcClient);
	virtual void OnSize(UINT nType, CRect rcClient);
	// ���ÿؼ�
	virtual void ResetControlBackHSL(COLOR_HSL colorHSL, COLOR_HSL OldColorHSL);

	// ��Ϣ��Ӧ
	virtual LRESULT OnControlMessage(UINT uControlID, UINT uMsg, void *pData, size_t lSize);
	virtual LRESULT OnWindowMessage(UINT uControlID, UINT uMsg, void *pData, size_t lSize);
	virtual void OnWindowMouseLeave();
	virtual void OnWindowMouseHover();
			
public:

	// ��ʱ����Ϣ
	virtual void OnTimer(UINT uTimerID);
};
