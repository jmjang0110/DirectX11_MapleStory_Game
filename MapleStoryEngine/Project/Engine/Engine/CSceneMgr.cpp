#include "pch.h"
#include "CSceneMgr.h"

#include "CEventMgr.h"
#include "CCollisionMgr.h"

#include "CResMgr.h"
#include "CMesh.h"
#include "CGraphicsShader.h"
#include "CGameObject.h"
#include "CTransform.h"


#include "CDevice.h"
#include "CPathMgr.h"

#include "CScene.h"
#include "CLayer.h"

CSceneMgr::CSceneMgr()
	: m_pCurScene(nullptr)
{

}

CSceneMgr::~CSceneMgr()
{
	SAFE_DELETE(m_pCurScene);
}


void CSceneMgr::init()
{

}

void CSceneMgr::progress()
{
	//if (KEY_TAP(KEY::T))
	//{
	//	CFileMgr::GetInst()->LoadFromFile<CScene>((DWORD_PTR)m_pCurScene);

	//	//CSceneMgr::GetInst()->LoadFromFile();
	//	//CSceneMgr::GetInst()->SaveToFile();

	//}
	//if (KEY_TAP(KEY::U))
	//{

	//	CFileMgr::GetInst()->SaveToFile<CScene>((DWORD_PTR)m_pCurScene);

	//}

	SCENE_STATE eState = m_pCurScene->GetSceneState();

	if (SCENE_STATE::PLAY == eState)
	{
		m_pCurScene->update();
		m_pCurScene->lateupdate();
	}
	
	m_pCurScene->finalupdate();

	// Collision Check
	if (SCENE_STATE::PLAY == eState)
	{
		CCollisionMgr::GetInst()->update();
	}
}

void CSceneMgr::ChangeScene(CScene* _pNextScene)
{
	if (nullptr != m_pCurScene)
		delete m_pCurScene;

	m_pCurScene = _pNextScene;
}





void CSceneMgr::SpawnObject(CGameObject* _pSpawnObject, Vec3 _vWorldPos, wstring _strName, UINT _iLayerIdx)
{
	tEventInfo info = {};
	info.eType = EVENT_TYPE::CREATE_OBJ;
	info.lParam = (DWORD_PTR)_pSpawnObject;
	info.wParam = (DWORD_PTR)_iLayerIdx;

	_pSpawnObject->Transform()->SetRelativePos(_vWorldPos);
	_pSpawnObject->SetName(_strName);

	CEventMgr::GetInst()->AddEvent(info);
}

void CSceneMgr::SpawnObject(CGameObject* _pSpawnObject, UINT _iLayerIdx)
{
	tEventInfo info = {};
	info.eType = EVENT_TYPE::CREATE_OBJ;
	info.lParam = (DWORD_PTR)_pSpawnObject;
	info.wParam = (DWORD_PTR)_iLayerIdx;

	CEventMgr::GetInst()->AddEvent(info);
}

void CSceneMgr::AddChild(CGameObject* _pParentObject, CGameObject* _pChildObject)
{
	tEventInfo info = {};
	info.eType = EVENT_TYPE::ADD_CHILD;
	info.lParam = (DWORD_PTR)_pParentObject;
	info.wParam = (DWORD_PTR)_pChildObject;

	CEventMgr::GetInst()->AddEvent(info);
}

void CSceneMgr::DisconnectParent(CGameObject* _pObject)
{
	tEventInfo info = {};
	info.eType = EVENT_TYPE::DISCONNECT_PARENT;
	info.lParam = (DWORD_PTR)_pObject;

	CEventMgr::GetInst()->AddEvent(info);
}


void CSceneMgr::ClearLayer()
{
	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		m_pCurScene->GetLayer(i)->Clear();
	}
}

CGameObject* CSceneMgr::FindObjectByName(const wstring& _strName)
{
	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		CLayer* pLayer = m_pCurScene->GetLayer(i);
		const vector<CGameObject*>& vecObj = pLayer->GetObjects();

		for (size_t j = 0; j < vecObj.size(); ++j)
		{
			if (_strName == vecObj[j]->GetName())
			{
				return vecObj[j];
			}
		}
	}

	return nullptr;
}

void CSceneMgr::FindObjectsByName(wstring& _strName, vector<CGameObject*>& _vecOut)
{
	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		CLayer* pLayer = m_pCurScene->GetLayer(i);
		const vector<CGameObject*>& vecObj = pLayer->GetObjects();

		for (size_t j = 0; j < vecObj.size(); ++j)
		{
			if (_strName == vecObj[j]->GetName())
			{
				_vecOut.push_back(vecObj[j]);
			}
		}
	}
}

 


