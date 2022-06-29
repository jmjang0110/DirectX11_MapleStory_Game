#pragma once
#include "CRes.h"

class CGameObject;

class CPrefab :
    public CRes
{
private:
    CGameObject* m_pProtoObj;


public:
    CGameObject* Instantiate();


public:
    virtual int Save(const wstring& _strFilePath) override;
    virtual int Load(const wstring& _strFilePath) override;

public:
    CPrefab();
    CPrefab(CGameObject* _pProtoObj);
    ~CPrefab();
};

