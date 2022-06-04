#include "pch.h"
#include "CLayer.h"

#include "CGameObject.h"


CLayer::CLayer()
	: m_iLayerIdx(-1)
{
}

CLayer::~CLayer()
{
	Safe_Del_Vec(m_vecRoot);
}

void CLayer::start()
{
	for (size_t i = 0; i < m_vecRoot.size(); ++i)
	{
		m_vecRoot[i]->start();
	}
}

void CLayer::update()
{
	for (size_t i = 0; i < m_vecRoot.size(); ++i)
	{
		if(m_vecRoot[i]->IsActive())
			m_vecRoot[i]->update();
	}
}

void CLayer::lateupdate()
{
	for (size_t i = 0; i < m_vecRoot.size(); ++i)
	{
		if (m_vecRoot[i]->IsActive())
			m_vecRoot[i]->lateupdate();
	}
}

void CLayer::finalupdate()
{
	vector<CGameObject*>::iterator iter = m_vecRoot.begin();

	for (; iter != m_vecRoot.end(); )
	{
		(*iter)->finalupdate();

		if ((*iter)->IsDead())
		{
			iter = m_vecRoot.erase(iter);
		}
		else
		{
			++iter;
		}
	}
}

void CLayer::AddObject(CGameObject* _pObj)
{
	m_vecRoot.push_back(_pObj);
}

void CLayer::DeregisterObject(CGameObject* _pObj)
{
	if (_pObj->GetParent())
		return;

	vector<CGameObject*>::iterator iter = m_vecRoot.begin();
	for (; iter != m_vecRoot.end(); ++iter)
	{
		if ((*iter) == _pObj)
		{
			m_vecRoot.erase(iter);
			_pObj->m_iLayerIdx = -1;
			return;
		}
	}
}



void CLayer::SaveToFile(FILE* _pFile)
{
	int RootObjCnt = m_vecRoot.size();
	fwrite(&RootObjCnt, sizeof(int), 1, _pFile);

	for (int i = 0; i < m_vecRoot.size(); ++i)
	{
		m_vecRoot[i]->SaveToFile(_pFile);

	}

}

void CLayer::LoadFromFile(FILE* _pFile)
{
	m_vecRoot.clear(); // 기존 오브젝트들을 지운다 .

	int RootObjCnt = 0;
	fread(&RootObjCnt, sizeof(int), 1, _pFile);



	for (int i = 0; i < RootObjCnt; ++i)
	{
		CGameObject* RootObj = new CGameObject;
		RootObj->LoadFromFile(_pFile);
		m_vecRoot.push_back(RootObj);

	}

}

