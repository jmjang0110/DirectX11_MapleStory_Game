#include "pch.h"
#include "CTexture.h"

#include "CDevice.h"

CTexture::CTexture()
    : CRes(RES_TYPE::TEXTURE)
{
}

CTexture::~CTexture()
{
}

int CTexture::Load(const wstring& _strFilePath)
{
    // Texture File 을 SystemMem 에 로드한다.

    wchar_t szExt[25] = {};
    _wsplitpath_s(_strFilePath.c_str(), nullptr, 0, nullptr, 0, nullptr, 0, szExt, 25);

    HRESULT hr = S_OK;
    int iErr = 0;

    if (!wcscmp(szExt, L".dds") || !wcscmp(szExt, L".DDS"))
    {
        hr = LoadFromDDSFile(_strFilePath.c_str(), DDS_FLAGS_NONE, nullptr, m_Image);
    }

    else if (!wcscmp(szExt, L".tga") || !wcscmp(szExt, L".TGA"))
    {
        hr = LoadFromTGAFile(_strFilePath.c_str(), nullptr, m_Image);
    }

    else // WIC (.png, jpg, jpeg, bmp...)
    {
        hr = LoadFromWICFile(_strFilePath.c_str(), WIC_FLAGS_NONE, nullptr, m_Image);
    }

    if (CheckFail(hr))
        return E_FAIL;


    // System memory 에 불러와진 픽셀 데이터를 GPU 에 보낸다   
    hr = CreateShaderResourceView(DEVICE, m_Image.GetImages()
        , m_Image.GetImageCount()
        , m_Image.GetMetadata()
        , m_pSRV.GetAddressOf());

    if (CheckFail(hr))
        return E_FAIL;

    m_pSRV->GetResource((ID3D11Resource**)m_pTex2D.GetAddressOf());

    m_pTex2D->GetDesc(&m_tDesc);

    return S_OK;
}

void CTexture::Create(UINT _iWidth, UINT _iHeight, DXGI_FORMAT _format, UINT _flag)
{
    // Texture 만들기    
    m_tDesc.Width = _iWidth;
    m_tDesc.Height = _iHeight;
    m_tDesc.MipLevels = 1;    
    m_tDesc.ArraySize = 1;

    m_tDesc.CPUAccessFlags = 0;
    m_tDesc.BindFlags = _flag;
    m_tDesc.Usage = D3D11_USAGE::D3D11_USAGE_DEFAULT;
    m_tDesc.MiscFlags = 0;

    m_tDesc.SampleDesc.Count = 1;
    m_tDesc.SampleDesc.Quality = 0;

    m_tDesc.Format = _format;

    DEVICE->CreateTexture2D(&m_tDesc, nullptr, m_pTex2D.GetAddressOf());
    assert(m_pTex2D);


    if (D3D11_BIND_DEPTH_STENCIL & _flag)
    {
        DEVICE->CreateDepthStencilView(m_pTex2D.Get(), nullptr, m_pDSV.GetAddressOf());
        assert(m_pDSV);
    }
    else
    {
        if (D3D11_BIND_RENDER_TARGET & _flag)
        {
            DEVICE->CreateRenderTargetView(m_pTex2D.Get(), nullptr, m_pRTV.GetAddressOf());
            assert(m_pRTV);
        }

        if (D3D11_BIND_SHADER_RESOURCE & _flag)
        {

            // SRV 생성
            D3D11_SHADER_RESOURCE_VIEW_DESC tSRVDesc = {};
            tSRVDesc.ViewDimension = D3D11_SRV_DIMENSION::D3D11_SRV_DIMENSION_TEXTURE2D;
            tSRVDesc.Texture2D.MipLevels = 1;
            tSRVDesc.Texture2D.MostDetailedMip = 0;

            DEVICE->CreateShaderResourceView(m_pTex2D.Get(), &tSRVDesc, m_pSRV.GetAddressOf());
            assert(m_pSRV);

        }

        if (D3D11_BIND_UNORDERED_ACCESS & _flag)
        {
            // UAV 생성
            D3D11_UNORDERED_ACCESS_VIEW_DESC tUAVDesc = {};
            tUAVDesc.ViewDimension = D3D11_UAV_DIMENSION::D3D11_UAV_DIMENSION_TEXTURE2D;
            tUAVDesc.Texture2D.MipSlice = 0;

            DEVICE->CreateUnorderedAccessView(m_pTex2D.Get(), &tUAVDesc, m_pUAV.GetAddressOf());
            assert(m_pUAV);
        }
    }   
}

void CTexture::Create(ComPtr<ID3D11Texture2D> _pTex2D)
{
    // Texture 만들기    
    m_pTex2D = _pTex2D;
    m_pTex2D->GetDesc(&m_tDesc);
    
       
    if (D3D11_BIND_DEPTH_STENCIL & m_tDesc.BindFlags)
    {
        DEVICE->CreateDepthStencilView(m_pTex2D.Get(), nullptr, m_pDSV.GetAddressOf());
        assert(m_pDSV);
    }
    else
    {
        if (D3D11_BIND_RENDER_TARGET & m_tDesc.BindFlags)
        {
            DEVICE->CreateRenderTargetView(m_pTex2D.Get(), nullptr, m_pRTV.GetAddressOf());
            assert(m_pRTV);
        }

        if (D3D11_BIND_SHADER_RESOURCE & m_tDesc.BindFlags)
        {

            // SRV 생성
            D3D11_SHADER_RESOURCE_VIEW_DESC tSRVDesc = {};
            tSRVDesc.ViewDimension = D3D11_SRV_DIMENSION::D3D11_SRV_DIMENSION_TEXTURE2D;
            tSRVDesc.Texture2D.MipLevels = 1;
            tSRVDesc.Texture2D.MostDetailedMip = 0;

            DEVICE->CreateShaderResourceView(m_pTex2D.Get(), &tSRVDesc, m_pSRV.GetAddressOf());
            assert(m_pSRV);

        }

        if (D3D11_BIND_UNORDERED_ACCESS & m_tDesc.BindFlags)
        {
            // UAV 생성
            D3D11_UNORDERED_ACCESS_VIEW_DESC tUAVDesc = {};
            tUAVDesc.ViewDimension = D3D11_UAV_DIMENSION::D3D11_UAV_DIMENSION_TEXTURE2D;
            tUAVDesc.Texture2D.MipSlice = 0;

            DEVICE->CreateUnorderedAccessView(m_pTex2D.Get(), &tUAVDesc, m_pUAV.GetAddressOf());
            assert(m_pUAV);
        }
    }
}

void CTexture::UpdateData(UINT _PipelineStage, UINT _iRegisterNum)
{
    if (_PipelineStage & (UINT)PIPELINE_STAGE::VS)
        CONTEXT->VSSetShaderResources(_iRegisterNum, 1, m_pSRV.GetAddressOf());

    if (_PipelineStage & (UINT)PIPELINE_STAGE::HS)
        CONTEXT->HSSetShaderResources(_iRegisterNum, 1, m_pSRV.GetAddressOf());

    if (_PipelineStage & (UINT)PIPELINE_STAGE::DS)
        CONTEXT->DSSetShaderResources(_iRegisterNum, 1, m_pSRV.GetAddressOf());

    if (_PipelineStage & (UINT)PIPELINE_STAGE::GS)
        CONTEXT->GSSetShaderResources(_iRegisterNum, 1, m_pSRV.GetAddressOf());

    if (_PipelineStage & (UINT)PIPELINE_STAGE::PS)
        CONTEXT->PSSetShaderResources(_iRegisterNum, 1, m_pSRV.GetAddressOf());
}

void CTexture::UpdateData_CS(UINT _iRegisterNum, bool _bShaderResource)
{
    if (_bShaderResource)
    {
        CONTEXT->CSSetShaderResources(_iRegisterNum, 1, m_pSRV.GetAddressOf());
    }
    else
    {
        UINT i = -1; // -1 넣으면 모든 비트가 전부다 1로 된다 
        CONTEXT->CSSetUnorderedAccessViews(_iRegisterNum, 1, m_pUAV.GetAddressOf(), &i);
    }    
}

void CTexture::Clear(int _iRegisterNum)
{
    ID3D11ShaderResourceView* pSRV = nullptr;
	CONTEXT->VSSetShaderResources(_iRegisterNum, 1, &pSRV);
	CONTEXT->HSSetShaderResources(_iRegisterNum, 1, &pSRV);
	CONTEXT->DSSetShaderResources(_iRegisterNum, 1, &pSRV);
	CONTEXT->GSSetShaderResources(_iRegisterNum, 1, &pSRV);
	CONTEXT->PSSetShaderResources(_iRegisterNum, 1, &pSRV);
}

void CTexture::ClearCS(int _iRegisterNum)
{
    ID3D11UnorderedAccessView* pUAV = nullptr;
    ID3D11ShaderResourceView* pSRV = nullptr;
    UINT i = -1;

    CONTEXT->CSSetUnorderedAccessViews(_iRegisterNum, 1, &pUAV, &i);
    CONTEXT->CSSetShaderResources(_iRegisterNum, 1, &pSRV);
}

