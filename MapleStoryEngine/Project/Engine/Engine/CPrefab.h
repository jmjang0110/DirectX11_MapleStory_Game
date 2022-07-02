#pragma once
#include "CRes.h"

class CGameObject;

class CPrefab :
    public CRes
{
private:
    CGameObject* m_pProtoObj;

public:
    typedef void (*SaveFunc) (CPrefab*, const wstring&);
    typedef int (*LoadFunc) (CPrefab*, const wstring&);
    static SaveFunc        m_pSaveFunc;
    static LoadFunc        m_pLoadFunc;

public:
    CGameObject* Instantiate();
    CGameObject* GetProto() { return m_pProtoObj; }
    void SetProto(CGameObject* _pProto) { m_pProtoObj = _pProto; }


public:
    virtual int Save(const wstring& _strFilePath) override;
    virtual int Load(const wstring& _strFilePath) override;

public:
    CPrefab();
    CPrefab(CGameObject* _pProtoObj);
    ~CPrefab();
};

