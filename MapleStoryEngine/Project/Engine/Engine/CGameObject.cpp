#include "pch.h"
#include "CGameObject.h"

#include "CSceneMgr.h"
#include "CScene.h"
#include "CLayer.h"
#include "CEventMgr.h"

#include "CComponent.h"
#include "CTransform.h"
#include "CMeshRender.h"
#include "CCollider2D.h"
#include "CRenderComponent.h"
#include "CAnimator2D.h"


#include "CScript.h"

#include "CPlayerScript.h"
#include "CCameraMoveScript.h"

CGameObject::CGameObject()
	: m_arrCom{}
	, m_pParent(nullptr)
	, m_pRenderComponent(nullptr)
	, m_iLayerIdx(-1)
	, m_bActive(true)
	, m_bDead(false)
{
}

CGameObject::CGameObject(const CGameObject& _origin)
	: CEntity(_origin)
	, m_arrCom{}
	, m_pParent(nullptr)
	, m_pRenderComponent(nullptr)
	, m_iLayerIdx(_origin.m_iLayerIdx)
	, m_bActive(true)
	, m_bDead(false)
{
	for (UINT i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
	{
		if (nullptr != _origin.m_arrCom[i])
		{
			AddComponent(_origin.m_arrCom[i]->Clone());
		}		
	}

	for (size_t i = 0; i < m_vecScript.size(); ++i)
	{
		AddComponent(m_vecScript[i]->Clone());
	}

	for (size_t i = 0; i < _origin.m_vecChild.size(); ++i)
	{
		AddChild(_origin.m_vecChild[i]->Clone());
	}
}

CGameObject::~CGameObject()
{
	Safe_Del_Arr(m_arrCom);
	Safe_Del_Vec(m_vecScript);
	Safe_Del_Vec(m_vecChild);
}

void CGameObject::start()
{
	for (UINT i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
	{
		if (nullptr != m_arrCom[i])
			m_arrCom[i]->start();
	}

	for (size_t i = 0; i < m_vecScript.size(); ++i)
	{
		m_vecScript[i]->start();
	}


	for (size_t i = 0; i < m_vecChild.size(); ++i)
	{
		m_vecChild[i]->start();
	}
}

void CGameObject::update()
{
	for (UINT i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
	{
		if(nullptr != m_arrCom[i] && m_arrCom[i]->IsActive())
			m_arrCom[i]->update();
	}

	for (size_t i = 0; i < m_vecScript.size(); ++i)
	{
		if (m_vecScript[i]->IsActive())
			m_vecScript[i]->update();
	}



	for (size_t i = 0; i < m_vecChild.size(); ++i)
	{
		if(m_vecChild[i]->IsActive())
			m_vecChild[i]->update();
	}
}

void CGameObject::lateupdate()
{
	for (UINT i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
	{
		if (nullptr != m_arrCom[i] && m_arrCom[i]->IsActive())
			m_arrCom[i]->lateupdate();
	}

	for (size_t i = 0; i < m_vecScript.size(); ++i)
	{
		if (m_vecScript[i]->IsActive())
			m_vecScript[i]->lateupdate();
	}

	for (size_t i = 0; i < m_vecChild.size(); ++i)
	{
		if (m_vecChild[i]->IsActive())
			m_vecChild[i]->lateupdate();
	}
}

void CGameObject::finalupdate()
{
	for (UINT i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
	{
		if (nullptr != m_arrCom[i])
			m_arrCom[i]->finalupdate();
	}

	// Layer �� ���
	CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();
	CLayer* pLayer = pCurScene->GetLayer(m_iLayerIdx);
	pLayer->RegisterObject(this);

	for (size_t i = 0; i < m_vecChild.size(); ++i)
	{
		m_vecChild[i]->finalupdate();
	}	
}

void CGameObject::render()
{
	if (m_pRenderComponent->IsActive())
		m_pRenderComponent->render();

	if (nullptr != Collider2D())
		Collider2D()->render();
}

CScript* CGameObject::GetScript(UINT _iIdx)
{
	return m_vecScript[_iIdx];
}

CScript* CGameObject::GetScriptByName(const wstring& _strName)
{
	for (size_t i = 0; i < m_vecScript.size(); ++i)
	{
		if (m_vecScript[i]->GetName() == _strName)
			return m_vecScript[i];
	}

	return nullptr;
}




void CGameObject::active()
{
	for (UINT i = 1; i < (UINT)COMPONENT_TYPE::END; ++i)
	{
		if (nullptr != m_arrCom[i])
			m_arrCom[i]->active();
	}

	for (size_t i = 1; i < m_vecScript.size(); ++i)
	{
		m_vecScript[i]->active();
	}




	for (size_t i = 0; i < m_vecChild.size(); ++i)
	{
		m_vecChild[i]->active();
	}
}

void CGameObject::deactive()
{
	for (UINT i = 1; i < (UINT)COMPONENT_TYPE::END; ++i)
	{
		if (nullptr != m_arrCom[i])
			m_arrCom[i]->deactive();
	}

	for (size_t i = 1; i < m_vecScript.size(); ++i)
	{
		m_vecScript[i]->deactive();
	}

	for (size_t i = 0; i < m_vecChild.size(); ++i)
	{
		m_vecChild[i]->deactive();
	}
}

void CGameObject::Deregister()
{
	if (-1 == m_iLayerIdx)
	{
		return;
	}

	CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();
	CLayer* pCurLayer = pCurScene->GetLayer(m_iLayerIdx);
	pCurLayer->DeregisterObject(this);	
}

void CGameObject::DisconnectBetweenParent()
{
	assert(m_pParent);

	vector<CGameObject*>::iterator iter = m_pParent->m_vecChild.begin();
	for (; iter != m_pParent->m_vecChild.end(); ++iter)
	{
		if ((*iter) == this)
		{
			m_pParent->m_vecChild.erase(iter);
			m_pParent = nullptr;
			return;
		}		
	}
}

void CGameObject::Activate()
{
	tEventInfo info = {};

	info.eType = EVENT_TYPE::ACTIVATE_OBJECT;
	info.lParam = (DWORD_PTR)this;

	CEventMgr::GetInst()->AddEvent(info);
}

void CGameObject::Deactivate()
{
	tEventInfo info = {};

	info.eType = EVENT_TYPE::DEACTIVATE_OBJECT;
	info.lParam = (DWORD_PTR)this;

	CEventMgr::GetInst()->AddEvent(info);
}



void CGameObject::AddChild(CGameObject* _pChild)
{
	int iLayerIdx = _pChild->m_iLayerIdx;

	// �ڽ����� ������ ������Ʈ�� ��Ʈ ������Ʈ�̰�, Ư�� ���̾� �Ҽ��̶��
	if (nullptr == _pChild->GetParent() && -1 != iLayerIdx)
	{
		// ���̾�� ��Ʈ ������Ʈ�μ� ��� ����
		_pChild->Deregister();
		_pChild->m_iLayerIdx = iLayerIdx;
	}

	// �ٸ� �θ������Ʈ�� �̹� �ִٸ�
	if (_pChild->GetParent())
	{
		_pChild->DisconnectBetweenParent();
	}
		

	m_vecChild.push_back(_pChild);
	_pChild->m_pParent = this;
}

void CGameObject::AddComponent(CComponent* _component)
{
	COMPONENT_TYPE eType = _component->GetType();

	if (COMPONENT_TYPE::SCRIPT != eType)
	{
		assert(nullptr == m_arrCom[(UINT)eType]);

		m_arrCom[(UINT)eType] = _component;
		_component->m_pOwner = this;

		switch (_component->GetType())
		{
		case COMPONENT_TYPE::MESHRENDER:
		case COMPONENT_TYPE::TILEMAP:
		case COMPONENT_TYPE::PARTICLESYSTEM:
		case COMPONENT_TYPE::LANDSCAPE:
		case COMPONENT_TYPE::DECAL:
		{
			// �ϳ��� ������Ʈ�� Render ����� ���� ������Ʈ�� 2���̻� ���� �� ����.
			assert(!m_pRenderComponent);
			m_pRenderComponent = (CRenderComponent*)_component;
		}
		break;
		}
	}
	else
	{
		m_vecScript.push_back((CScript*)_component);
		_component->m_pOwner = this;
	}
}

void CGameObject::Destroy()
{	
	if (m_bDead)
		return;

	tEventInfo info = {};

	info.eType = EVENT_TYPE::DELETE_OBJ;
	info.lParam = (DWORD_PTR)this;

	CEventMgr::GetInst()->AddEvent(info);
}


void CGameObject::SaveToFile(FILE* _pFile)
{
	CEntity::SaveToFile(_pFile);

	fwrite(&m_iLayerIdx, sizeof(int), 1, _pFile);
	fwrite(&m_bActive, sizeof(bool), 1, _pFile);
	fwrite(&m_bDead, sizeof(bool), 1, _pFile);

	bool IsEmpty = false;
	for (int i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
	{
		

		if (m_arrCom[i] != nullptr)
		{
			IsEmpty = false;
			fwrite(&IsEmpty, sizeof(bool), 1, _pFile);
			m_arrCom[i]->SaveToFile(_pFile);
		}
		else
		{
			IsEmpty = true;
			fwrite(&IsEmpty, sizeof(bool), 1, _pFile);
		}

	}

	

	int vecChildCnt = m_vecChild.size();
	fwrite(&vecChildCnt, sizeof(int), 1, _pFile);
	for (int i = 0; i < m_vecChild.size(); ++i)
	{
		m_vecChild[i]->SaveToFile(_pFile);
	}

	int vecScriptCnt = m_vecScript.size();
	fwrite(&vecScriptCnt, sizeof(int), 1, _pFile); // 1. ���� 
	for (int i = 0; i < m_vecScript.size(); ++i)
	{
		m_vecScript[i]->SaveToFile(_pFile);
	}


	

}

void CGameObject::LoadFromFile(FILE* _pFile)
{
	CEntity::LoadFromFile(_pFile);

	fread(&m_iLayerIdx, sizeof(int), 1, _pFile);
	fread(&m_bActive, sizeof(bool), 1, _pFile);
	fread(&m_bDead, sizeof(bool), 1, _pFile);

	bool IsEmpty = false;
	CComponent* pCom = nullptr;
	for (int i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
	{
		fread(&IsEmpty, sizeof(bool), 1, _pFile);
		pCom = nullptr;

		if (false == IsEmpty)
		{
			switch (COMPONENT_TYPE(i))
			{
			case COMPONENT_TYPE::TRANSFORM:
			{
				pCom = new CTransform();
				pCom->m_pOwner = this;
				pCom->LoadFromFile(_pFile);

			}

			break;
			case COMPONENT_TYPE::CAMERA:
				break;
			case COMPONENT_TYPE::COLLIDER2D:
			{
				pCom = new CCollider2D();
				pCom->m_pOwner = this;
				pCom->LoadFromFile(_pFile);
			}

				break;
			case COMPONENT_TYPE::COLLIDER3D:
				break;
			case COMPONENT_TYPE::ANIMATOR2D:
			{
				pCom = new CAnimator2D();
				pCom->m_pOwner = this;
				pCom->LoadFromFile(_pFile);

			}

				break;
			case COMPONENT_TYPE::ANIMATOR3D:
				break;
			case COMPONENT_TYPE::BOUNDINGBOX:
				break;
			case COMPONENT_TYPE::MESHRENDER:
			{
				pCom = new CMeshRender();
				pCom->m_pOwner = this;
				pCom->LoadFromFile(_pFile);

				m_pRenderComponent = (CRenderComponent*)pCom;
			}
				break;
			case COMPONENT_TYPE::TILEMAP:
				break;
			case COMPONENT_TYPE::PARTICLESYSTEM:
				break;
			case COMPONENT_TYPE::LANDSCAPE:
				break;
			case COMPONENT_TYPE::DECAL:
				break;
			case COMPONENT_TYPE::END:
				break;
			case COMPONENT_TYPE::SCRIPT:
				break;
			default:
				break;
			}
		}
		m_arrCom[(UINT)i] = pCom;
	}



	int vecChildCnt = 0;
	fread(&vecChildCnt, sizeof(int), 1, _pFile);
	for (int i = 0; i < vecChildCnt; ++i)
	{
		CGameObject* ChildObj = new CGameObject;
		ChildObj->LoadFromFile(_pFile);
		AddChild(ChildObj);
	}

	int vecScriptCnt = 0;
	fread(&vecScriptCnt, sizeof(int), 1, _pFile); // 1. ���� 
	for (int i = 0; i < vecScriptCnt; ++i)
	{
		wstring ScriptName = L"";
		LoadWString(ScriptName, _pFile);
		CScript* ScriptObj = nullptr;

		if (ScriptName == L"CPlayerScript")
			ScriptObj = new CPlayerScript;
		else if (ScriptName == L"CameraMoveScript")
			ScriptObj = new CCameraMoveScript;

		if (ScriptObj != nullptr)
		{
			ScriptObj->LoadFromFile(_pFile);
			CComponent* ScriptCom = (CComponent*)ScriptObj;

			ScriptCom->m_pOwner = this;
			m_vecScript.push_back(ScriptObj);
		}

	}


	
	


}