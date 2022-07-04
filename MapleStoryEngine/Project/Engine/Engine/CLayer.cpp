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





// 자신의 Root 오브젝트들에 자신의 레이어 인덱스를 알린다. 
void CLayer::UpdateLayerIdx(int _LayerIdx)
{
	for (int i = 0; i < m_vecRoot.size(); ++i)
	{
		m_vecRoot[i]->m_iLayerIdx = _LayerIdx;
		// 자식 오브젝트 
		vector<CGameObject*> vecChildObj = m_vecRoot[i]->GetChild();
		for (int k = 0; k < vecChildObj.size(); ++k)
		{
			vecChildObj[k]->m_iLayerIdx = _LayerIdx;

		}

	}


}
