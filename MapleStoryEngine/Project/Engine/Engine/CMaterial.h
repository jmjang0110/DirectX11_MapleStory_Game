#pragma once
#include "CRes.h"

#include "CGraphicsShader.h"
#include "CTexture.h"


class CMaterial :
    public CRes
{
private:
    tScalarParam                m_Param;
    Ptr<CTexture>               m_arrTex[(UINT)TEX_PARAM::END];
    Ptr<CGraphicsShader>        m_pShader;

    vector<tScalarParamInfo>    m_vecScalarParamInfo;
    vector<tTexParamInfo>       m_vecTexParamInfo;

    CMaterial* m_pMasterMtrl;

public:
    void SetShader(Ptr<CGraphicsShader> _pShader);
    Ptr<CGraphicsShader> GetShader() { return m_pShader; }

    void SetScalarParam(SCALAR_PARAM _eType, void* _pData);
    void SetScalarParam(const wstring& _strParamName, void* _pData);

    void SetTexParam(TEX_PARAM _eType, Ptr<CTexture> _pTex);
    void SetTexParam(const wstring& _strParamName, Ptr<CTexture> _pTex);

    void* GetScalarParam(SCALAR_PARAM _eType);
    Ptr<CTexture> GetTexParam(TEX_PARAM _eType);

    void UpdateData();

    CMaterial* GetMtrlInst();
    CMaterial* GetMasterMtrl() { return m_pMasterMtrl; }

public:
    virtual int Load(const wstring& _strFilePath) override { return S_OK; }

    CLONE(CMaterial);

public:
    CMaterial();
    ~CMaterial();
};

