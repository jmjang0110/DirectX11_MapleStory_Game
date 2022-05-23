#pragma once
#include "CEntity.h"

class CConstBuffer :
    public CEntity
{
private:
    ComPtr<ID3D11Buffer>		m_CB;
    D3D11_BUFFER_DESC           m_Desc;

    CB_TYPE                     m_eCBType; // 상수버퍼를 바인딩 할 레지스터 번호

public:
    virtual void UpdateData() override;
    void UpdateData_CS();
    int Create(UINT _iBufferSize);

    void SetData(void* _pData, UINT _iSize);
        
    virtual CConstBuffer* Clone() { return nullptr; }

public:
    CConstBuffer(CB_TYPE _eType);
    ~CConstBuffer();
};

