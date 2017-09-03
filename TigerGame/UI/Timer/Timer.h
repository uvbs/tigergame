#pragma once
#include <vector>
using namespace std;

class CTimer
{
public:
	CTimer(void);
	~CTimer(void);

	//����ʾ�����
	vector<UINT>			m_vecTimer;				// ��ʱ��ID
	static UINT				m_uAccuracy;			// ����ֱ��� 
	static bool				m_bIsTmierOK;			// �Ƿ���Զ�ʱ 

	bool GetTiemrIsRun(UINT uTimerID);
	int SetTimerEx(UINT nTimerResolution);
	int KillTimerEx(UINT uTimerID);
	void KillTimerEx();
	static void CALLBACK TimerCallProc(UINT uTimerID, UINT msg, DWORD dwData, DWORD dwa, DWORD dwb);
	virtual void OnTimerEx(UINT uTimerID) = 0;
};
