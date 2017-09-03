#pragma once
#include <vector>
using namespace std;

class CTimer
{
public:
	CTimer(void);
	~CTimer(void);

	//定显示器相关
	vector<UINT>			m_vecTimer;				// 定时器ID
	static UINT				m_uAccuracy;			// 定义分辨率 
	static bool				m_bIsTmierOK;			// 是否可以定时 

	bool GetTiemrIsRun(UINT uTimerID);
	int SetTimerEx(UINT nTimerResolution);
	int KillTimerEx(UINT uTimerID);
	void KillTimerEx();
	static void CALLBACK TimerCallProc(UINT uTimerID, UINT msg, DWORD dwData, DWORD dwa, DWORD dwb);
	virtual void OnTimerEx(UINT uTimerID) = 0;
};
