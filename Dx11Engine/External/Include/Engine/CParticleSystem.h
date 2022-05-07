#pragma once
#include "CRenderComponent.h"

#include "CStructuredBuffer.h"
#include "CParticleUpdateShader.h"

class CParticleSystem :
    public CRenderComponent
{
private:    
    Ptr<CParticleUpdateShader>      m_CS;               // 파티클 업데이트 쉐이더

    CStructuredBuffer* m_ParticleBuffer;                // 파티클 구조화 버퍼
    UINT                            m_iMaxCount;        // 파티클 최대 개수


    int                             m_bPosInherit;      // 오브젝트 좌표에 영향


public:
    virtual void finalupdate() override;
    virtual void render()   override;

    CLONE(CParticleSystem);
public:
    CParticleSystem();
    CParticleSystem(const CParticleSystem& _origin);
    ~CParticleSystem();
};

