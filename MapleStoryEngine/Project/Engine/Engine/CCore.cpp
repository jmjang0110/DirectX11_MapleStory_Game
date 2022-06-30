#include "pch.h"
#include "CCore.h"

#include "CDevice.h"
#include "CTimeMgr.h"
#include "CPathMgr.h"
#include "CKeyMgr.h"
#include "CResMgr.h"
#include "CSceneMgr.h"
#include "CRenderMgr.h"
#include "CCollisionMgr.h"
#include "CEventMgr.h"



CCore::CCore()
	: m_hWnd(nullptr)
	, m_ptResolution{}
{

}

CCore::~CCore()
{
	
}

int CCore::init(HWND _hWnd, POINT _ptResolution)
{
	RECT rt = { 0, 0, _ptResolution.x, _ptResolution.y };
	AdjustWindowRect(&rt, WS_OVERLAPPEDWINDOW, false);
	SetWindowPos(_hWnd, nullptr, 10, 10, rt.right - rt.left, rt.bottom - rt.top, 0);

	m_hWnd = _hWnd;
	m_ptResolution = _ptResolution;

	// Manager 초기화
	if (FAILED(CDevice::GetInst()->init(m_hWnd, Vec2((float)m_ptResolution.x, (float)m_ptResolution.y))))
	{
		return E_FAIL;
	}

	CPathMgr::GetInst()->init();
	CKeyMgr::GetInst()->init();
	CTimeMgr::GetInst()->init();

	CRenderMgr::GetInst()->init();
	CResMgr::GetInst()->init();
	CSceneMgr::GetInst()->init();

	return S_OK;
}


void CCore::Frame_Init()
{
	CSceneMgr::GetInst()->ClearLayer();

	CTimeMgr::GetInst()->update();
	CKeyMgr::GetInst()->update();

	CRenderMgr::GetInst()->ClearCamera();
}

void CCore::Frame_Clear()
{
	
}

void CCore::progress()
{
	//ShowCursor(FALSE);
	/* - 이거 하면 윈도우 상에서 내 커서가 안보인다.
	 - 안보이게 한다음에 내가 임의로 Curor GameObject 를 만든다음에
	 - 그걸로 마우스 커서 역할을 하면 될까 ???? */

	// 프레임 시작
	Frame_Init();	

	// Scene Update
	CSceneMgr::GetInst()->progress();


	// Render	
	CRenderMgr::GetInst()->render();

	// EventMgr update
	CEventMgr::GetInst()->update();

	// 프레임 종료
	Frame_Clear();
}

