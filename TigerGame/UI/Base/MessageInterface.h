#pragma once

class CControlBase;

class CMessageInterface
{
public:
	CRect m_rc;
public:
	CMessageInterface(void);
	~CMessageInterface(void);
	// ��Ϣ��Ӧ
	virtual LRESULT OnControlMessage(UINT uControlID, UINT uMsg, void *pData, size_t lSize) { return 0L; }
	virtual LRESULT OnControlUpdate(CRect rcUpdate, CControlBase *pUpdateControlBase = NULL) { return 0L; }
};
