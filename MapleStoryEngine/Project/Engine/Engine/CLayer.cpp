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





// �ڽ��� Root ������Ʈ�鿡 �ڽ��� ���̾� �ε����� �˸���. 
void CLayer::UpdateLayerIdx(int _LayerIdx)
{
	assert(!(_LayerIdx < 0 || MAX_LAYER <= _LayerIdx));

	m_iLayerIdx = _LayerIdx;

	for (int i = 0; i < m_vecRoot.size(); ++i)
	{
		m_vecRoot[i]->m_iLayerIdx = _LayerIdx;
		// �ڽ� ������Ʈ 
		vector<CGameObject*> vecChildObj = m_vecRoot[i]->GetChild();
		for (int k = 0; k < vecChildObj.size(); ++k)
		{
			vecChildObj[k]->UpdateLayerIdx(_LayerIdx);

		}

	}


}

void CLayer::SetLayerIdx(int _LayerIdx)
{
	assert(!(_LayerIdx < 0 || MAX_LAYER <= _LayerIdx));
	
	if (m_iLayerIdx < 0 || MAX_LAYER <= m_iLayerIdx)
		m_iLayerIdx = _LayerIdx;
}

CGameObject* CLayer::FindObj(wstring _objname)
{
	CGameObject* pObj = nullptr;

	for (size_t i = 0; i < m_vecRoot.size(); ++i)
	{
		if (m_vecRoot[i]->GetName() == _objname)
		{
			pObj = m_vecRoot[i];
			break;

		}
	}
	return pObj;
}


