#include "pch.h"
#include "CPrefab.h"

#include "CGameObject.h"

#include "CSceneMgr.h"



CPrefab::CPrefab()
    : CRes(RES_TYPE::PREFAB)
    , m_pProtoObj(nullptr)
{

}

CPrefab::CPrefab(CGameObject* _pProtoObj)
    : CRes(RES_TYPE::PREFAB)
    , m_pProtoObj(_pProtoObj)
{

}


CPrefab::~CPrefab()
{
    SAFE_DELETE(m_pProtoObj);
}

CGameObject* CPrefab::Instantiate()
{
    return m_pProtoObj->Clone();
}

int CPrefab::Save(const wstring& _strFilePath)
{
    CRes::SetKey(_strFilePath);
    CRes::SetRelativePath(_strFilePath);
    return S_OK;
}

int CPrefab::Load(const wstring& _strFilePath)
{
    CRes::SetKey(_strFilePath);
    CRes::SetRelativePath(_strFilePath);

	return S_OK;
}
