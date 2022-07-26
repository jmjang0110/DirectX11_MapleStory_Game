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

// ====== Todo - Delete Obj ======
void CSceneMgr::DestroyObject(CGameObject* _pDestroyObject)
{
	tEventInfo info = {};
	info.eType = EVENT_TYPE::DELETE_OBJ;
	info.lParam = (DWORD_PTR)_pDestroyObject;
	
	CEventMgr::GetInst()->AddEvent(info);

}
void CSceneMgr::DeRegisterObjInLayer(CGameObject* _pDeObj, int _LayerIdx)
{
	tEventInfo info = {};
	info.eType = EVENT_TYPE::DEREGISTER_OBJ_IN_LAYER;
	info.lParam = (DWORD_PTR)_pDeObj;
	info.wParam = (DWORD_PTR)_LayerIdx;

	CEventMgr::GetInst()->AddEvent(info);

}
void CSceneMgr::RegisterObhInLayer(CGameObject* _pObj, int _LayerIdx)
{
	tEventInfo info = {};
	info.eType = EVENT_TYPE::REGISTER_OBJ_IN_LAYER;
	info.lParam = (DWORD_PTR)_pObj;
	info.wParam = (DWORD_PTR)_LayerIdx;

	CEventMgr::GetInst()->AddEvent(info);

}
void CSceneMgr::SwapLayer(int _Lidx, int _Ridx)
{
	tEventInfo info = {};
	info.eType = EVENT_TYPE::SWAP_LAYER;
	info.lParam = (DWORD_PTR)_Lidx;
	info.wParam = (DWORD_PTR)_Ridx;

	CEventMgr::GetInst()->AddEvent(info);

}
void CSceneMgr::SceneChangeEvent(CScene* _pcurscene, wstring _nextScenePath)
{
	tEventInfo info = {};
	info.eType = EVENT_TYPE::SCENE_CHANGE;
	info.lParam = (DWORD_PTR)_pcurscene;
	info.wParam = (DWORD_PTR)_nextScenePath;

	CEventMgr::GetInst()->AddEvent(info);

}
// ====== ====== ====== ====== ======


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

 


