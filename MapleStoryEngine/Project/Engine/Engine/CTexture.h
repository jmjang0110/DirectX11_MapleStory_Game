#pragma once
#include "CRes.h"

#include <DirectXTex/DirectXTex.h>

#ifdef _DEBUG
#pragma comment(lib, "DirectXTex/DirectXTex_debug")
#else
#pragma comment(lib, "DirectXTex/DirectXTex")
#endif


class CTexture :
    public CRes
{
private:
    ScratchImage                        m_Image;    // 파일로부터 로딩한 텍스쳐 데이터의 시스템 메모리 관리
    ComPtr<ID3D11Texture2D>             m_pTex2D;   // GPU 에 로딩된 텍스쳐 데이터를 관리하는 인터페이스


    ComPtr<ID3D11ShaderResourceView>    m_pSRV;     // Texture2D 를 ShaderResource 용도로 전달하는 중간 매개체
    ComPtr<ID3D11UnorderedAccessView>   m_pUAV;     // Texture2D 를 RWTexture 용도로 전달하는 View
    ComPtr<ID3D11RenderTargetView>      m_pRTV;     // Texture2D 를 RenderTarget 용도로 전달
    ComPtr<ID3D11DepthStencilView>      m_pDSV;     // Texture2D 를 DepthStencil 용도로 전달



    D3D11_TEXTURE2D_DESC                m_tDesc;

protected:
    virtual int Load(const wstring& _strFilePath);
    virtual int Save(const wstring& _strFilePath);

    void Create(UINT _iWidth, UINT _iHeight, DXGI_FORMAT _format, UINT _flag);
    void Create(ComPtr<ID3D11Texture2D> _pTex2D);

public:
    float Width() { return (float)m_tDesc.Width; }
    float Height() { return (float)m_tDesc.Height; }

    ComPtr<ID3D11Texture2D> GetTex2D() {return m_pTex2D;}
    ComPtr<ID3D11ShaderResourceView>  GetSRV() { return m_pSRV; }
    ComPtr<ID3D11UnorderedAccessView> GetUAV() { return m_pUAV; }
    ComPtr<ID3D11RenderTargetView> GetRTV() { return m_pRTV; }
    ComPtr<ID3D11DepthStencilView> GetDSV() { return m_pDSV; }


public:
    // PIPELINE_STAGE
    void UpdateData(UINT _PipelineStage, UINT _iRegisterNum);
    void UpdateData_CS(UINT _iRegisterNum, bool _bShaderResource);

    static void Clear(int _iRegisterNum);
    static void ClearCS(int _iRegisterNum);

public:
    CTexture();
    ~CTexture();

    friend class CResMgr;
};

