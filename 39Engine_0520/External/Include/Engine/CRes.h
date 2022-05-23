#pragma once
#include "CEntity.h"

#include "Ptr.h"

class CRes : 
    public CEntity
{
private:
    wstring     m_strKey;
    wstring     m_strRelativePath;
    UINT        m_iRefCount;

    bool        m_bEngineRes;   // 엔진 내부에서 사용하는 자원


protected:
    void SetKey(const wstring& _strKey) { m_strKey = _strKey; }
    void SetRelativePath(const wstring& _strRelativePath){m_strRelativePath = _strRelativePath;}
    bool CheckFail(HRESULT _hr);

public:
    const wstring& GetKey() { return m_strKey; }
    const wstring& GetRelativePath() { return m_strRelativePath; }

    bool IsEngineRes() { return m_bEngineRes; }


protected:    
    virtual int Load(const wstring& _strFilePath) = 0;    
    CLONE_DISABLE(CRes)

private:
    void AddRef()   {   ++m_iRefCount;  }
    void SubRef()   {   --m_iRefCount;  }

public:
    CRes();
    virtual ~CRes();

    friend class CResMgr;

    template<typename T>
    friend class Ptr;
};

