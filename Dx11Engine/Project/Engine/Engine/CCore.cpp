#include "pch.h"
#include "CCore.h"

#include "CDevice.h"
#include "CTimeMgr.h"
#include "CPathMgr.h"
#include "CKeyMgr.h"
#include "CResMgr.h"
#include "CSceneMgr.h"
#include "CCollisionMgr.h"
#include "CEventMgr.h"
#include "CRenderMgr.h"

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

	// Manager ÃÊ±âÈ­
	if (FAILED(CDevice::GetInst()->init(m_hWnd, Vec2((float)m_ptResolution.x, (float)m_ptResolution.y))))
	{
		return E_FAIL;
	}


	CKeyMgr::GetInst()->init();
	CPathMgr::GetInst()->init();
	CTimeMgr::GetInst()->init();
	CRenderMgr::GetInst()->init();
	CResMgr::GetInst()->init();
	CSceneMgr::GetInst()->init();
	

	return S_OK;
}
void CCore::Frame_Init()
{
	CTimeMgr::GetInst()->update();
	CKeyMgr::GetInst()->update();
	CRenderMgr::GetInst()->ClearCamera();
}

void CCore::Frame_Clear()
{

	CSceneMgr::GetInst()->ClearLayer();

}



void CCore::progress()
{
	// frame start  
	Frame_Init();


	// Scene Update
	CSceneMgr::GetInst()->progress();

	// Collision Check
	CCollisionMgr::GetInst()->update();

	// Scene Render
	//CSceneMgr::GetInst()->render();	
	CRenderMgr::GetInst()->render();


	// EventMgr update
	CEventMgr::GetInst()->update();

	Frame_Clear();

}


