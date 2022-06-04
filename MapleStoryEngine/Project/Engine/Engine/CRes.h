#pragma once
#include "CEntity.h"

#include "Ptr.h"

class CRes : 
    public CEntity
{
private:
    const RES_TYPE      m_eResType;

    wstring     m_strKey;
    wstring     m_strRelativePath;
    UINT        m_iRefCount;

    bool        m_bEngineRes;   // 엔진 내부에서 사용하는 자원


protected:
    void SetKey(const wstring& _strKey) { m_strKey = _strKey; }
    void SetRelativePath(const wstring& _strRelativePath) { m_strRelativePath = _strRelativePath; }
    bool CheckFail(HRESULT _hr);


public:
    const wstring& GetKey() { return m_strKey; }
    const wstring& GetRelativePath() { return m_strRelativePath; }
    RES_TYPE GetResType() { return m_eResType; }
    bool IsEngineRes() { return m_bEngineRes; }


protected:
    virtual int Load(const wstring& _strFilePath) = 0;
    CLONE_DISABLE(CRes)

private:
    void AddRef() { ++m_iRefCount; }
    void SubRef() { --m_iRefCount; }

public:
    CRes(RES_TYPE _eType);
    virtual ~CRes();

    friend class CResMgr;

    template<typename T>
    friend class Ptr;
};

