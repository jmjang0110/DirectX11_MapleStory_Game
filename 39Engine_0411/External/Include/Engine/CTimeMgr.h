#pragma once



class CTimeMgr
	: public CSingleton<CTimeMgr>
{
	SINGLE(CTimeMgr);
private:
	LARGE_INTEGER	m_llFrequency;
	LARGE_INTEGER	m_llPrevCount;
	LARGE_INTEGER	m_llCurCount;

	UINT			m_iFPS; // Frame per second
	float			m_fDT;  // delta seconds, 1 프레임에 걸린 시간
	float			m_fAddTime;

	float			m_fCurTime;


public:
	void init();
	void update();

public:
	float GetDT() { return m_fDT; }
	float GetCurTime() { return m_fCurTime; }
};

