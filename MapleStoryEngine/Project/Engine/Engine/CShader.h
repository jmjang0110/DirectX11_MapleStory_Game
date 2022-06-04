#pragma once
#include "CRes.h"


class CShader :
    public CRes
{
protected:
    ComPtr<ID3DBlob>    m_ErrBlob;


public:
    virtual int Load(const wstring& _strFilePath) { return S_OK; };

public:
    CShader(RES_TYPE _eType);
    ~CShader();
};

