#include "pch.h"
#include "CTexture.h"

#include "CDevice.h"

CTexture::CTexture()
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


    // System memory 에 불려와진 픽셀 데이터를 GPU 에 보낸다   
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

void CTexture::UpdateData(UINT _PipelineStage, int _iRegisterNum)
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

void CTexture::Clear(int _iRegisterNum)
{
    ID3D11ShaderResourceView* pSRV = nullptr;

    CONTEXT->VSSetShaderResources(_iRegisterNum, 1, &pSRV);
    CONTEXT->HSSetShaderResources(_iRegisterNum, 1, &pSRV);
    CONTEXT->DSSetShaderResources(_iRegisterNum, 1, &pSRV);
    CONTEXT->GSSetShaderResources(_iRegisterNum, 1, &pSRV);
    CONTEXT->PSSetShaderResources(_iRegisterNum, 1, &pSRV);
    

}

