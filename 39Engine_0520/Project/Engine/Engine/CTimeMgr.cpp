#include "pch.h"
#include "CTimeMgr.h"

#include "CCore.h"


CTimeMgr::CTimeMgr()
	: m_llFrequency{}
	, m_llPrevCount{}
	, m_llCurCount{}
	, m_iFPS(0)
	, m_fAddTime(0.f)
{

}

CTimeMgr::~CTimeMgr()
{

}

void CTimeMgr::init()
{
	// 8 바이트 정수를 돌려 받음
	// 초당 카운팅 횟수
	QueryPerformanceFrequency(&m_llFrequency);

	// 8 바이트 정수를 돌려 받음
	QueryPerformanceCounter(&m_llPrevCount);
}

void CTimeMgr::update()
{
	// 현재 카운팅 확인
	QueryPerformanceCounter(&m_llCurCount);

	// 이전 카운팅 과 현재 카운팅의 값의 차이를 초당 카운팅수로 나누어서 1프레임이 실행되었을때 걸린 시간을 구한다(Delta Seconds)
	m_fDT = (float)(m_llCurCount.QuadPart - m_llPrevCount.QuadPart) / (float)m_llFrequency.QuadPart;

#ifdef _DEBUG
	// 강제 60 FPS 확보
	if (m_fDT > (1.f / 60.f))
	{
		m_fDT = 1.f / 60.f;
	}
#endif

	// 현재 카운팅 값을 저장해둔다(다음번 체크 때 활용하기 위해서)
	m_llPrevCount.QuadPart = m_llCurCount.QuadPart;

	// FPS 카운팅
	++m_iFPS;

	// 시간 누적
	m_fAddTime += m_fDT;
	
	// 누적시간이 1초를 넘는 순간
	if (m_fAddTime >= 1.f)
	{
		m_fAddTime = m_fAddTime - 1.f; // 초과한 양을 남겨둔다.

		wchar_t szStr[256] = L"";
		wsprintf(szStr, L"FPS : %d", m_iFPS);		
		SetWindowTextW(CCore::GetInst()->GetMainHwnd(), szStr);

		m_iFPS = 0;
	}	

	m_fCurTime += m_fDT;

	g_global.fDT = m_fDT;
	g_global.fAccTime += m_fDT;
}
