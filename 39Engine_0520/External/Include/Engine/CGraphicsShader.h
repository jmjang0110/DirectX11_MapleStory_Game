#pragma once
#include "CShader.h"


struct tScalarParamInfo
{
    wstring         strDesc;
    SCALAR_PARAM    eScalarParam;
};


struct tTexParamInfo
{
    wstring      strDesc;
    TEX_PARAM    eTexParam;
};



class CGraphicsShader :
    public CShader
{
private:
    static vector<D3D11_INPUT_ELEMENT_DESC> g_vecLayout;
    static UINT g_iOffset;

public:
    static void AddInputLayout(D3D11_INPUT_ELEMENT_DESC _desc);



private:
    ComPtr<ID3DBlob>                m_VSBlob;
    ComPtr<ID3DBlob>                m_HSBlob;
    ComPtr<ID3DBlob>                m_DSBlob;
    ComPtr<ID3DBlob>                m_GSBlob;
    ComPtr<ID3DBlob>                m_PSBlob;

    ComPtr<ID3D11VertexShader>      m_VS;
    ComPtr<ID3D11HullShader>        m_HS;
    ComPtr<ID3D11DomainShader>      m_DS;
    ComPtr<ID3D11GeometryShader>    m_GS;
    ComPtr<ID3D11PixelShader>       m_PS;

    SHADER_DOMAIN                   m_eDomain;

    RS_TYPE                         m_eRSType;
    DS_TYPE                         m_eDSType;
    BS_TYPE                         m_eBSType;

    ComPtr<ID3D11InputLayout>       m_InputLayout;
    D3D11_PRIMITIVE_TOPOLOGY        m_eTopology;

    vector<tScalarParamInfo>        m_vecScalarParamInfo;
    vector<tTexParamInfo>           m_vecTexParamInfo;
    

public:
    int CreateVertexShader(const wstring& _strRelativePath, const string& _strFunc);
    //void CreateHullShader();
    //void CreateDomainShader();
    int CreateGeometryShader(const wstring& _strRelativePath, const string& _strFunc);
    int CreatePixelShader(const wstring& _strRelativePath, const string& _strFunc);

    void SetShaderDomain(SHADER_DOMAIN _eDomain) { m_eDomain = _eDomain; }
    void SetRSType(RS_TYPE _eType) { m_eRSType = _eType; }
    void SetDSType(DS_TYPE _eType) { m_eDSType = _eType; }
    void SetBSType(BS_TYPE _eType) { m_eBSType = _eType; }
    void SetTopology(D3D11_PRIMITIVE_TOPOLOGY _eTopology){m_eTopology = _eTopology;}

    D3D11_PRIMITIVE_TOPOLOGY GetTopology() { return m_eTopology; }
    SHADER_DOMAIN GetShaderDomain() { return m_eDomain; }

    virtual void UpdateData() override;


public:
    void AddScalarParamInfo(const wstring& _strDesc, SCALAR_PARAM _eParamType);
    void AddTexParamInfo(const wstring& _strDesc, TEX_PARAM _eParamType);

    const vector<tScalarParamInfo>& GetScalarParamInfo() { return m_vecScalarParamInfo; }
    const vector<tTexParamInfo>& GetTexParamInfo() { return m_vecTexParamInfo; }

public:
    CGraphicsShader();
    ~CGraphicsShader();
};

