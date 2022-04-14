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


protected:
    void SetKey(const wstring& _strKey) { m_strKey = _strKey; }
    void SetRelativePath(const wstring& _strRelativePath){m_strRelativePath = _strRelativePath;}
    bool CheckFail(HRESULT _hr);

public:
    const wstring& GetKey() { return m_strKey; }
    const wstring& GetRelativePath() { return m_strRelativePath; }

protected:    
    virtual int Load(const wstring& _strFilePath) = 0;    
    CLONE_DISABLE(CRes)

private:
    void AddRef() { ++m_iRefCount; 
    int i = 0; }
    void SubRef()   {   --m_iRefCount;  }

public:
    CRes();
    virtual ~CRes();

    friend class CResMgr;

    template<typename T>
    friend class Ptr;
};

