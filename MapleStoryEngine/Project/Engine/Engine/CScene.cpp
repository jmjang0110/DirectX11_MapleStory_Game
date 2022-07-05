#include "pch.h"
#include "CScene.h"

#include "CLayer.h"
#include "CGameObject.h"



CScene::CScene()
	: m_arrLayer{}
	, m_eSceneState(SCENE_STATE::STOP)

{
	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		m_arrLayer[i] = new CLayer;
		m_arrLayer[i]->m_iLayerIdx = i;
	}
}

CScene::~CScene()
{
	Safe_Del_Arr(m_arrLayer);
}


void CScene::start()
{
	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		m_arrLayer[i]->start();
	}
}

void CScene::update()
{
	for (UINT i = 0; i < MAX_LAYER; ++i)
	{		
		m_arrLayer[i]->update();
	}
}

void CScene::lateupdate()
{
	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		m_arrLayer[i]->lateupdate();
	}
}

void CScene::finalupdate()
{
	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		m_arrLayer[i]->finalupdate();
	}
}


void CScene::SetLayerName(int _iLayerIdx, const wstring& _strName)
{
	assert(0 <= _iLayerIdx && _iLayerIdx < MAX_LAYER);
	m_arrLayer[_iLayerIdx]->SetName(_strName);
}




int CScene::GetLayerIdxFromName(const wstring& _strName)
{
	for (int i = 0; i < MAX_LAYER; ++i)
	{
		if (m_arrLayer[i]->GetName() == _strName)
		{
			return i;
		}
	}

	return -1;
}

void CScene::AddObject(CGameObject* _pRootObj, const wstring& _strLayerName)
{
	int iLayerIdx = GetLayerIdxFromName(_strLayerName);

	assert(iLayerIdx != -1);
	assert(!_pRootObj->m_pParent);

	AddObject(_pRootObj, iLayerIdx);
}

void CScene::AddObject(CGameObject* _pRootObj, int _iLayerIdx)
{
	assert(0 <= _iLayerIdx && _iLayerIdx < MAX_LAYER);
	assert(!_pRootObj->m_pParent);

	m_arrLayer[_iLayerIdx]->AddObject(_pRootObj);

	// 자식오브젝트들도 해당 레이어의 인덱스를 알려준다.
	list<CGameObject*> queue;
	queue.push_back(_pRootObj);

	// 부모 오브젝트 포함, 자식들 모두 해당 레이어의 인덱스를 알려준다(특정 레이어 소속이 아닌경우에)
	while (!queue.empty())
	{
		CGameObject* pTargetObj = queue.front();
		queue.pop_front();

		if(-1 == pTargetObj->m_iLayerIdx)
			pTargetObj->m_iLayerIdx = _iLayerIdx;

		const vector<CGameObject*>& vecChild = pTargetObj->GetChild();
		for (size_t i = 0; i < vecChild.size(); ++i)
		{
			queue.push_back(vecChild[i]);
		}
	}
}

CLayer* CScene::GetLayer(const wstring& _strLayerName)
{
	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		if (_strLayerName == m_arrLayer[i]->GetName())
		{
			return m_arrLayer[i];
		}
	}

	return nullptr;
}

Ptr<CSceneFile> CScene::GetSceneFile()
{
	return CResMgr::GetInst()->FindRes<CSceneFile>(m_strResKey);
}


void CScene::SetSceneState(SCENE_STATE _eState)
{
	if (m_eSceneState == _eState)
		return;

	if (SCENE_STATE::STOP == m_eSceneState)
	{
		if (SCENE_STATE::PLAY == _eState)
		{
			m_eSceneState = _eState;
			start();
		}
	}

	else if (SCENE_STATE::PLAY == m_eSceneState)
	{
		if (SCENE_STATE::PAUSE == _eState)
		{
			m_eSceneState = _eState;
		}
		else if (SCENE_STATE::STOP == _eState)
		{
			m_eSceneState = _eState;
			assert(CResMgr::GetInst()->FindRes<CSceneFile>(m_strResKey).Get());
		}
	}
	else if (SCENE_STATE::PAUSE == m_eSceneState)
	{
		if (SCENE_STATE::PLAY == _eState)
		{
			m_eSceneState = _eState;
		}
	}
}

void CScene::CopyLayer(CLayer* _pLayer, int _TargetLayerIdx)
{
	if (m_arrLayer[_TargetLayerIdx] == nullptr)
		return;
	if (m_arrLayer[_TargetLayerIdx]->GetRootObjects().size() != 0)
		return;

	assert(0 <= _TargetLayerIdx && _TargetLayerIdx < MAX_LAYER);
	wstring LayerName = _pLayer->GetName() + wstring(L"_Copy");
	vector<CGameObject*> vecRoot = _pLayer->GetRootObjects();


	m_arrLayer[_TargetLayerIdx]->SetName(LayerName);
	for (int i = 0; i < vecRoot.size(); ++i)
	{
		m_arrLayer[_TargetLayerIdx]->RegisterObjectAsRoot(vecRoot[i]->Clone());
		m_arrLayer[_TargetLayerIdx]->UpdateLayerIdx(_TargetLayerIdx);
	}

}



void CScene::SetLayer(CLayer* _pLayer, int _TargetLayerIdx)
{
	assert(!(_TargetLayerIdx < 0 || MAX_LAYER <= _TargetLayerIdx));

	if (m_arrLayer[_TargetLayerIdx] != nullptr)
		return;

	m_arrLayer[_TargetLayerIdx] = _pLayer;

}

bool CScene::SwapLayer(int _Lidx, int _Ridx)
{

	assert(!(_Lidx < 0 || MAX_LAYER <= _Lidx));
	assert(!(_Ridx < 0 || MAX_LAYER <= _Ridx));

	CLayer* Left = m_arrLayer[_Lidx];
	CLayer* Right = m_arrLayer[_Ridx];
	CLayer* Empty = nullptr;



	Empty = Left;
	m_arrLayer[_Lidx] = Right;
	m_arrLayer[_Ridx] = Empty;

	Left->UpdateLayerIdx(_Ridx);
	Right->UpdateLayerIdx(_Lidx);

	return true;
}
