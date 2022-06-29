#include "pch.h"
#include "CPrefab.h"

#include "CGameObject.h"

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
    assert(m_pProtoObj);

    CRes::Save(_strFilePath);
    CRes::SetRelativePath(CRes::GetKey());

    wstring strContent = CPathMgr::GetInst()->GetContentPath();

    FILE* pFile = nullptr;
    _wfopen_s(&pFile, _strFilePath.c_str(), L"wb");

    if (nullptr == pFile)
        return E_FAIL;

    
    m_pProtoObj->SaveToScene(pFile);

    fclose(pFile);
}

int CPrefab::Load(const wstring& _strFilePath)
{
    return 0;

}
