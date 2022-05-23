#include "pch.h"
#include "CPrefab.h"

#include "CGameObject.h"

CPrefab::CPrefab()
    : m_pProtoObj(nullptr)
{
}

CPrefab::CPrefab(CGameObject* _pProtoObj)
    : m_pProtoObj(_pProtoObj)
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