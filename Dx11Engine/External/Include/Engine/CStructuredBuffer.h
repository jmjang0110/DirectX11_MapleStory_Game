#pragma once
#include "CEntity.h"

enum class SB_TYPE
{
    READ_ONLY,      // : binding 된 Texture 정보(GPU 버퍼 정보)를 쉐이더 에서 읽는다. 
    READ_WRITE,     // : binding 된 Texture 정보(GPU 버퍼 정보)를 쉐이더코드에서 수정가능 
};

class CStructuredBuffer :
    public CEntity
{
private:
    ComPtr<ID3D11Buffer>                m_SB; // 구조화 버퍼 
    ComPtr<ID3D11ShaderResourceView>    m_SRV;
    ComPtr<ID3D11UnorderedAccessView>   m_UAV; // : Shader 에서 수정될 수 있는 경우 UAV 를 통해 레지스터에 전달

    D3D11_BUFFER_DESC                   m_desc;

    UINT                                m_iElementSize; // 버퍼 단위 ( TileData 구조체 단위 ) 
    UINT                                m_iElementCount;

    SB_TYPE                             m_eType;
    bool                                m_bCpuRead;

public:
    UINT GetBufferSize() { return m_iElementSize * m_iElementCount; }

public:
    int Create(UINT _iElementSize, UINT _iElementCount, SB_TYPE _eType, bool _bCpuAccessRead, void* _pInitialData);


public:
    CLONE_DISABLE(CStructuredBuffer);

public:
    CStructuredBuffer();
    ~CStructuredBuffer();
};

