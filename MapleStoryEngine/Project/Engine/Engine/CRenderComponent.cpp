#include "pch.h"
#include "CRenderComponent.h"
#include "CResMgr.h"


CRenderComponent::CRenderComponent(COMPONENT_TYPE _type)
	: CComponent(_type)
	, m_pMesh(nullptr)
	, m_pMtrl(nullptr)
{
}

CRenderComponent::CRenderComponent(const CRenderComponent& _origin)
	: CComponent(_origin)
	, m_pMesh(_origin.m_pMesh)
	, m_pMtrl(nullptr)
	, m_pSharedMtrl(_origin.m_pMtrl)
	, m_pDynamicMtrl(nullptr)
{
	if (nullptr != _origin.m_pSharedMtrl)
	{
		SetSharedMaterial(m_pSharedMtrl);
	}
}

CRenderComponent::~CRenderComponent()
{
	if (nullptr != m_pDynamicMtrl)
		delete m_pDynamicMtrl.Get();
}



void CRenderComponent::SetSharedMaterial(Ptr<CMaterial> _pMtrl)
{
	m_pSharedMtrl = _pMtrl;
	m_pMtrl = m_pSharedMtrl;
}

Ptr<CMaterial> CRenderComponent::GetSharedMaterial()
{
	m_pMtrl = m_pSharedMtrl;

	return m_pMtrl;
}

Ptr<CMaterial> CRenderComponent::GetDynamicMaterial()
{
	if (nullptr != m_pDynamicMtrl && m_pDynamicMtrl->GetMasterMtrl() != m_pSharedMtrl)
	{
		CMaterial* pMtrl = m_pDynamicMtrl.Get();
		m_pDynamicMtrl = nullptr;
		delete pMtrl;
	}

	if (nullptr == m_pDynamicMtrl)
	{
		m_pDynamicMtrl = m_pSharedMtrl->GetMtrlInst();
	}

	m_pMtrl = m_pDynamicMtrl;
	return m_pMtrl;
}

void CRenderComponent::SaveToFile(FILE* _pFile)
{
	CComponent::SaveToFile(_pFile);

	// 이름을 저장하자 !! Key 값

	// ============= Mesh =====================
	bool IsEmpty = false;
	if (nullptr == m_pMesh)
	{
		IsEmpty = true;
		fwrite(&IsEmpty, sizeof(bool), 1, _pFile);
	}
	else
	{
		IsEmpty = false;
		fwrite(&IsEmpty, sizeof(bool), 1, _pFile);
		wstring key = m_pMesh->GetKey();
		SaveWString(key, _pFile);
	}
	
	// ================ Mtrl ==================
	if (nullptr == m_pMtrl)
	{
		IsEmpty = true;
		fwrite(&IsEmpty, sizeof(bool), 1, _pFile);
	}
	else
	{
		IsEmpty = false;
		fwrite(&IsEmpty, sizeof(bool), 1, _pFile);
		wstring key = m_pMtrl->GetKey();
		SaveWString(key, _pFile);
	}

	// ================ m_pSharedMtrl ==================
	if (nullptr == m_pSharedMtrl)
	{
		IsEmpty = true;
		fwrite(&IsEmpty, sizeof(bool), 1, _pFile);
	}
	else
	{
		IsEmpty = false;
		fwrite(&IsEmpty, sizeof(bool), 1, _pFile);
		wstring key = m_pSharedMtrl->GetKey();
		SaveWString(key, _pFile);
	}

	// ================ m_pSharedMtrl ==================
	if (nullptr == m_pDynamicMtrl)
	{
		IsEmpty = true;
		fwrite(&IsEmpty, sizeof(bool), 1, _pFile);
	}
	else
	{
		IsEmpty = false;
		fwrite(&IsEmpty, sizeof(bool), 1, _pFile);
		wstring key = m_pDynamicMtrl->GetKey();
		SaveWString(key, _pFile);
	}

	
}

void CRenderComponent::LoadFromFile(FILE* _pFile, bool IsPrevRead)
{
	CComponent::LoadFromFile(_pFile);

	bool IsEmpty = false;
	
	// ============ m_pMesh ===================
	fread(&IsEmpty, sizeof(bool), 1, _pFile);
	if (false == IsEmpty)
	{
		wstring key = L"";
		LoadWString(key, _pFile);
		m_pMesh = CResMgr::GetInst()->FindRes<CMesh>(key);
	}

	// ============ m_pMtrl ===================
	fread(&IsEmpty, sizeof(bool), 1, _pFile);
	if (false == IsEmpty)
	{
		wstring key = L"";
		LoadWString(key, _pFile);
		m_pMtrl = CResMgr::GetInst()->FindRes<CMaterial>(key);
	}

	// ============ m_pSharedMtrl ===================
	fread(&IsEmpty, sizeof(bool), 1, _pFile);
	if (false == IsEmpty)
	{
		wstring key = L"";
		LoadWString(key, _pFile);
		m_pSharedMtrl = CResMgr::GetInst()->FindRes<CMaterial>(key);
	}

	// ============ m_pDynamicMtrl ===================
	fread(&IsEmpty, sizeof(bool), 1, _pFile);
	if (false == IsEmpty)
	{
		wstring key = L"";
		LoadWString(key, _pFile);
		m_pDynamicMtrl = CResMgr::GetInst()->FindRes<CMaterial>(key);
	}


}
