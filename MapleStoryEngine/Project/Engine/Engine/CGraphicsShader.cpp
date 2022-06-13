#include "pch.h"
#include "CGraphicsShader.h"

#include "CPathMgr.h"
#include "CDevice.h"

#ifdef _DEBUG
static UINT g_iFlag = D3DCOMPILE_DEBUG;
#else
static UINT g_iFlag = 0;
#endif

vector<D3D11_INPUT_ELEMENT_DESC> CGraphicsShader::g_vecLayout;


CGraphicsShader::CGraphicsShader()
	: CShader(RES_TYPE::GRAPHICS_SHADER)
	, m_eDomain(SHADER_DOMAIN::DOMAIN_FORWARD)
	, m_eRSType(RS_TYPE::CULL_BACK)
	, m_eDSType(DS_TYPE::LESS)
	, m_eBSType(BS_TYPE::DEFAULT)
	, m_eTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST)
{
}

CGraphicsShader::~CGraphicsShader()
{
}


int CGraphicsShader::CreateVertexShader(const wstring& _strRelativePath, const string& _strVSFunc)
{	
	SetRelativePath(_strRelativePath);


	wstring strContentPath = CPathMgr::GetInst()->GetContentPath();

	// 버텍스 쉐이더(HLSL) 컴파일
	HRESULT hr = D3DCompileFromFile(wstring(strContentPath + _strRelativePath).c_str(), nullptr
		, D3D_COMPILE_STANDARD_FILE_INCLUDE, _strVSFunc.c_str(), "vs_5_0", g_iFlag, 0
		, m_VSBlob.GetAddressOf(), m_ErrBlob.GetAddressOf());

	if (FAILED(hr))
	{
		MessageBoxA(nullptr, (char*)m_ErrBlob->GetBufferPointer(), "Shader Compile Failed!!", MB_OK);
		return E_FAIL;
	}

	// 컴파일 된 코드로 VertexShader 객체 만들기
	if (FAILED(DEVICE->CreateVertexShader(m_VSBlob->GetBufferPointer(), m_VSBlob->GetBufferSize()
		, nullptr, m_VS.GetAddressOf())))
	{
		return E_FAIL;
	}

	if (FAILED(DEVICE->CreateInputLayout(g_vecLayout.data(), (UINT)g_vecLayout.size()
		, m_VSBlob->GetBufferPointer(), m_VSBlob->GetBufferSize()
		, m_InputLayout.GetAddressOf())))
	{
		return E_FAIL;
	}


	return S_OK;
}

int CGraphicsShader::CreateGeometryShader(const wstring& _strRelativePath, const string& _strFunc)
{
	SetRelativePath(_strRelativePath);

	wstring strContentPath = CPathMgr::GetInst()->GetContentPath();

	// 버텍스 쉐이더(HLSL) 컴파일
	HRESULT hr = D3DCompileFromFile(wstring(strContentPath + _strRelativePath).c_str(), nullptr
		, D3D_COMPILE_STANDARD_FILE_INCLUDE, _strFunc.c_str(), "gs_5_0", g_iFlag, 0
		, m_GSBlob.GetAddressOf(), m_ErrBlob.GetAddressOf());

	if (FAILED(hr))
	{
		MessageBoxA(nullptr, (char*)m_ErrBlob->GetBufferPointer(), "Shader Compile Failed!!", MB_OK);
		return E_FAIL;
	}

	// 컴파일 된 코드로 VertexShader 객체 만들기
	if (FAILED(DEVICE->CreateGeometryShader(m_GSBlob->GetBufferPointer(), m_GSBlob->GetBufferSize()
		, nullptr, m_GS.GetAddressOf())))
	{
		return E_FAIL;
	}

	return S_OK;
}

int CGraphicsShader::CreatePixelShader(const wstring& _strRelativePath, const string& _strFunc)
{
	SetRelativePath(_strRelativePath);

	wstring strContentPath = CPathMgr::GetInst()->GetContentPath();

	// 버텍스 쉐이더(HLSL) 컴파일
	HRESULT hr = D3DCompileFromFile(wstring(strContentPath + _strRelativePath).c_str(), nullptr
		, D3D_COMPILE_STANDARD_FILE_INCLUDE, _strFunc.c_str(), "ps_5_0", g_iFlag, 0
		, m_PSBlob.GetAddressOf(), m_ErrBlob.GetAddressOf());

	if (FAILED(hr))
	{
		MessageBoxA(nullptr, (char*)m_ErrBlob->GetBufferPointer(), "Shader Compile Failed!!", MB_OK);
		return E_FAIL;
	}

	// 컴파일 된 코드로 VertexShader 객체 만들기
	if (FAILED(DEVICE->CreatePixelShader(m_PSBlob->GetBufferPointer(), m_PSBlob->GetBufferSize()
		, nullptr, m_PS.GetAddressOf())))
	{
		return E_FAIL;
	}

	return S_OK;
}

void CGraphicsShader::UpdateData()
{
	CONTEXT->IASetInputLayout(m_InputLayout.Get());
	CONTEXT->IASetPrimitiveTopology(m_eTopology);
		
	CONTEXT->VSSetShader(m_VS.Get(), 0, 0);
	CONTEXT->HSSetShader(m_HS.Get(), 0, 0);
	CONTEXT->DSSetShader(m_DS.Get(), 0, 0);
	CONTEXT->GSSetShader(m_GS.Get(), 0, 0);
	CONTEXT->PSSetShader(m_PS.Get(), 0, 0);
		
	CONTEXT->RSSetState(CDevice::GetInst()->GetRS(m_eRSType).Get());
	CONTEXT->OMSetDepthStencilState(CDevice::GetInst()->GetDS(m_eDSType).Get(), 0);
	CONTEXT->OMSetBlendState(CDevice::GetInst()->GetBS(m_eBSType).Get(), Vec4(), 0xffffffff);
}



void CGraphicsShader::AddScalarParamInfo(const wstring& _strDesc, SCALAR_PARAM _eParamType)
{
	m_vecScalarParamInfo.push_back(tScalarParamInfo{_strDesc, _eParamType});
}

void CGraphicsShader::AddTexParamInfo(const wstring& _strDesc, TEX_PARAM _eParamType)
{
	m_vecTexParamInfo.push_back(tTexParamInfo{ _strDesc ,_eParamType });
}

void CGraphicsShader::AddInputLayout(D3D11_INPUT_ELEMENT_DESC _desc)
{
	g_vecLayout.push_back(_desc);
}




	// ======= Todo ===========================
	
	// 등록된 pipieline을 반환 
PIPELINE_STAGE CGraphicsShader::GetPipelineStage()
{
	PIPELINE_STAGE pipeline= PIPELINE_STAGE::ALL;

	// 비트연산 XOR ( ^ )이용 :  
	/*
		0 XOR 0 = 0
		0 XOR 1 = 1
		1 XOR 0 = 1
		1 XOR 1 = 0
	*/

	if (nullptr == m_VS.Get())
		pipeline = (PIPELINE_STAGE)(pipeline ^ PIPELINE_STAGE::VS);

	if (nullptr == m_HS.Get())
		pipeline = (PIPELINE_STAGE)(pipeline ^ PIPELINE_STAGE::HS);

	if (nullptr == m_DS.Get())
		pipeline = (PIPELINE_STAGE)(pipeline ^ PIPELINE_STAGE::DS);

	if (nullptr == m_GS.Get())
		pipeline = (PIPELINE_STAGE)(pipeline ^ PIPELINE_STAGE::GS);

	if (nullptr == m_PS.Get())
		pipeline = (PIPELINE_STAGE)(pipeline ^ PIPELINE_STAGE::PS);


	return pipeline;

}