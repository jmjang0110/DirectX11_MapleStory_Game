#include "pch.h"
#include "CStructuredBuffer.h"

#include "CDevice.h"

CStructuredBuffer::CStructuredBuffer()
	: m_desc{}
	, m_iElementSize(0)
	, m_iElementCount(0)
	, m_iRecentBindNumSRV(-1)
	, m_iRecentBindNumUAV(-1)
	, m_eType(SB_TYPE::READ_ONLY)
{
}

CStructuredBuffer::~CStructuredBuffer()
{
}

int CStructuredBuffer::Create(UINT _iElementSize, UINT _iElementCount, SB_TYPE _eType
	, bool _bCpuAccess, void* _pInitialData)
{
	m_SB = nullptr;
	m_SRV = nullptr;


	m_iElementSize = _iElementSize;
	m_iElementCount = _iElementCount;
	m_eType = _eType;

	m_bCpuAccess = _bCpuAccess;
	
	// desc 작성
	m_desc.ByteWidth = m_iElementSize * m_iElementCount;		
	m_desc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
	m_desc.StructureByteStride = m_iElementSize;

	m_desc.Usage = D3D11_USAGE::D3D11_USAGE_DEFAULT;
	m_desc.CPUAccessFlags = 0;


	if (SB_TYPE::READ_ONLY == m_eType)
	{
		m_desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	}
	else 
	{
		m_desc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS;
	}	

	
	// 초기 Initial Data 가 있는 경우
	if (nullptr != _pInitialData)
	{
		D3D11_SUBRESOURCE_DATA tSub = {};
		tSub.pSysMem = _pInitialData;
		DEVICE->CreateBuffer(&m_desc, &tSub, m_SB.GetAddressOf());
	}
	else
	{
		DEVICE->CreateBuffer(&m_desc, nullptr, m_SB.GetAddressOf());
	}	

	// Shader Resource View 생성
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};

	srvDesc.ViewDimension = D3D_SRV_DIMENSION_BUFFER;
	srvDesc.BufferEx.NumElements = m_iElementCount;

	if (FAILED(DEVICE->CreateShaderResourceView(m_SB.Get(), &srvDesc, m_SRV.GetAddressOf())))
	{
		return E_FAIL;
	}


	// SB_TYPE 이 Read_Write
	if (SB_TYPE::READ_WRITE == m_eType)
	{
		D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc = {};

		uavDesc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
		uavDesc.Buffer.NumElements = m_iElementCount;

		if (FAILED(DEVICE->CreateUnorderedAccessView(m_SB.Get(), &uavDesc, m_UAV.GetAddressOf())))
		{
			return E_FAIL;
		}
	}	


	if (m_bCpuAccess)
	{		
		// SysMem -> GPU Buffer
		m_desc_write.ByteWidth = m_iElementSize * m_iElementCount;
		m_desc_write.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
		m_desc_write.StructureByteStride = m_iElementSize;

		m_desc_write.Usage = D3D11_USAGE::D3D11_USAGE_DYNAMIC;
		m_desc_write.CPUAccessFlags = D3D11_CPU_ACCESS_FLAG::D3D11_CPU_ACCESS_WRITE;

		m_desc_write.BindFlags = D3D11_BIND_SHADER_RESOURCE;

		// 초기 Initial Data 가 있는 경우
		if (nullptr != _pInitialData)
		{
			D3D11_SUBRESOURCE_DATA tSub = {};
			tSub.pSysMem = _pInitialData;
			DEVICE->CreateBuffer(&m_desc_write, &tSub, m_SB_Write.GetAddressOf());
		}
		else
		{
			DEVICE->CreateBuffer(&m_desc_write, nullptr, m_SB_Write.GetAddressOf());
		}

		// GPU -> SysMem Buffer
		m_desc_read.ByteWidth = m_iElementSize * m_iElementCount;
		m_desc_read.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
		m_desc_read.StructureByteStride = m_iElementSize;

		m_desc_read.Usage = D3D11_USAGE::D3D11_USAGE_STAGING;
		m_desc_read.CPUAccessFlags = D3D11_CPU_ACCESS_FLAG::D3D11_CPU_ACCESS_READ;

		m_desc_read.BindFlags = 0;

		// 초기 Initial Data 가 있는 경우
		if (nullptr != _pInitialData)
		{
			D3D11_SUBRESOURCE_DATA tSub = {};
			tSub.pSysMem = _pInitialData;
			DEVICE->CreateBuffer(&m_desc_read, &tSub, m_SB_Read.GetAddressOf());
		}
		else
		{
			DEVICE->CreateBuffer(&m_desc_read, nullptr, m_SB_Read.GetAddressOf());
		}
	}

	return S_OK;
}

void CStructuredBuffer::SetData(void* _pSrc, UINT _iElementCount)
{
	D3D11_MAPPED_SUBRESOURCE tSub = {};

	// SysMem -> Wirte Buffer
	CONTEXT->Map(m_SB_Write.Get(), 0, D3D11_MAP::D3D11_MAP_WRITE_DISCARD, 0, &tSub);
	memcpy(tSub.pData, _pSrc, (size_t)m_iElementSize * (size_t)_iElementCount);
	CONTEXT->Unmap(m_SB_Write.Get(), 0);

	// Write Buffer -> Main Buffer
	CONTEXT->CopyResource(m_SB.Get(), m_SB_Write.Get());
}

void CStructuredBuffer::GetData(void* _pDst)
{
	// Main Buffer -> Read Buffer
	CONTEXT->CopyResource(m_SB_Read.Get(), m_SB.Get());

	// Read Buffer -> SysMem
	D3D11_MAPPED_SUBRESOURCE tSub = {};
	CONTEXT->Map(m_SB_Read.Get(), 0, D3D11_MAP::D3D11_MAP_READ, 0, &tSub);	
	memcpy(_pDst, tSub.pData, (size_t)m_iElementSize * (size_t)m_iElementCount);
	CONTEXT->Unmap(m_SB_Read.Get(), 0);
}

void CStructuredBuffer::UpdateData(UINT _iStage, UINT _iRegisterNum)
{
	if (_iStage & PIPELINE_STAGE::VS)	
		CONTEXT->VSSetShaderResources(_iRegisterNum, 1, m_SRV.GetAddressOf());

	if (_iStage & PIPELINE_STAGE::HS)
		CONTEXT->HSSetShaderResources(_iRegisterNum, 1, m_SRV.GetAddressOf());

	if (_iStage & PIPELINE_STAGE::DS)
		CONTEXT->DSSetShaderResources(_iRegisterNum, 1, m_SRV.GetAddressOf());

	if (_iStage & PIPELINE_STAGE::GS)
		CONTEXT->GSSetShaderResources(_iRegisterNum, 1, m_SRV.GetAddressOf());

	if (_iStage & PIPELINE_STAGE::PS)
		CONTEXT->PSSetShaderResources(_iRegisterNum, 1, m_SRV.GetAddressOf());	

	m_iRecentBindNumSRV = _iRegisterNum;
}

void CStructuredBuffer::UpdateData_CS(UINT _iRegisterNum, bool _bShaderResource)
{
	if (_bShaderResource)
	{
		m_iRecentBindNumSRV = _iRegisterNum;
		CONTEXT->CSSetShaderResources(_iRegisterNum, 1, m_SRV.GetAddressOf());
	}
	else
	{
		assert(m_UAV.Get());

		m_iRecentBindNumUAV = _iRegisterNum;
		UINT i = -1;
		CONTEXT->CSSetUnorderedAccessViews(_iRegisterNum, 1, m_UAV.GetAddressOf(), &i);
	}
}

void CStructuredBuffer::Clear()
{	
	if (-1 != m_iRecentBindNumSRV)
	{
		ID3D11ShaderResourceView* pSRV = nullptr;
		CONTEXT->VSSetShaderResources(m_iRecentBindNumSRV, 1, &pSRV);
		CONTEXT->HSSetShaderResources(m_iRecentBindNumSRV, 1, &pSRV);
		CONTEXT->DSSetShaderResources(m_iRecentBindNumSRV, 1, &pSRV);
		CONTEXT->GSSetShaderResources(m_iRecentBindNumSRV, 1, &pSRV);
		CONTEXT->PSSetShaderResources(m_iRecentBindNumSRV, 1, &pSRV);
		CONTEXT->CSSetShaderResources(m_iRecentBindNumSRV, 1, &pSRV);
		m_iRecentBindNumSRV = -1;
	}
	
	if (-1 != m_iRecentBindNumUAV)
	{
		ID3D11UnorderedAccessView* pUAV = nullptr;
		UINT i = -1;
		CONTEXT->CSSetUnorderedAccessViews(m_iRecentBindNumUAV, 1, &pUAV, &i);
		m_iRecentBindNumUAV = -1;
	}	
}