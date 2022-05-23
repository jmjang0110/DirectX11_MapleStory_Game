#include "pch.h"
#include "CConstBuffer.h"

#include "CDevice.h"

CConstBuffer::CConstBuffer(CB_TYPE _eType)
	: m_eCBType(_eType)	
	, m_Desc{}
{
}

CConstBuffer::~CConstBuffer()
{
}

void CConstBuffer::UpdateData()
{
	CONTEXT->VSSetConstantBuffers((UINT)m_eCBType, 1, m_CB.GetAddressOf());
	CONTEXT->HSSetConstantBuffers((UINT)m_eCBType, 1, m_CB.GetAddressOf());
	CONTEXT->DSSetConstantBuffers((UINT)m_eCBType, 1, m_CB.GetAddressOf());
	CONTEXT->GSSetConstantBuffers((UINT)m_eCBType, 1, m_CB.GetAddressOf());
	CONTEXT->PSSetConstantBuffers((UINT)m_eCBType, 1, m_CB.GetAddressOf());	
}

void CConstBuffer::UpdateData_CS()
{
	CONTEXT->CSSetConstantBuffers((UINT)m_eCBType, 1, m_CB.GetAddressOf());
}

int CConstBuffer::Create(UINT _iBufferSize)
{
	m_Desc.ByteWidth = _iBufferSize;

	// 버퍼 생성 이후에도, 버퍼의 내용을 수정 할 수 있는 옵션
	m_Desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	m_Desc.Usage = D3D11_USAGE::D3D11_USAGE_DYNAMIC;

	// 정점을 저장하는 목적의 버퍼 임을 알림
	m_Desc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_CONSTANT_BUFFER;
	m_Desc.MiscFlags = 0;
	m_Desc.StructureByteStride = 0;

	if (FAILED(DEVICE->CreateBuffer(&m_Desc, nullptr, m_CB.GetAddressOf())))
	{
		assert(nullptr);
		return E_FAIL;
	}

	return S_OK;
}

void CConstBuffer::SetData(void* _pData, UINT _iSize)
{
	D3D11_MAPPED_SUBRESOURCE tSub = {};

	CONTEXT->Map(m_CB.Get(), 0, D3D11_MAP::D3D11_MAP_WRITE_DISCARD, 0, &tSub);
	memcpy(tSub.pData, _pData, _iSize);
	CONTEXT->Unmap(m_CB.Get(), 0);
}

