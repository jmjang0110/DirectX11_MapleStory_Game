#pragma once
#include "CRenderComponent.h"

#include "CStructuredBuffer.h"
#include "CParticleUpdateShader.h"

class CParticleSystem :
    public CRenderComponent
{
private:
    Ptr<CParticleUpdateShader>      m_CS;           // 파티클 업데이트 쉐이더

    CStructuredBuffer*              m_ParticleBuffer;   // 파티클 구조화 버퍼
    CStructuredBuffer*              m_DataBuffer;       // 추가 데이터 버퍼(파티클 AliveCount)
    UINT                            m_iMaxCount;        // 파티클 최대 개수

    int                             m_bPosInherit;      // 오브젝트 좌표에 영향
    int                             m_iAliveCount;      // 이번 프레임에 생성(활성화) 될 파티클 수
    
    float                           m_fMinLifeTime;     // 파티클의 최소 생명 시간
    float                           m_fMaxLifeTime;     // 파티클의 최대 생명 시간

    float                           m_fStartSpeed;      // 파티클의 최소 속력
    float                           m_fEndSpeed;        // 파티클의 최대 속력

    Vec4                            m_vStartColor;      // 파티클 초기 색상
    Vec4                            m_vEndColor;        // 파티클 최종 색상

    Vec3                            m_vStartScale;      // 파티클 초기 크기
    Vec3                            m_vEndScale;        // 파티클 최종 크기


    float                           m_fParticleCreateDistance;  // 파티클 생성 범위
    float                           m_fParticleCreateTerm;      // 파티클 생성 간격
    float                           m_fAccTime;
    
    


public:
    virtual void finalupdate() override;
    virtual void render()   override;

    CLONE(CParticleSystem);
public:
    CParticleSystem();
    CParticleSystem(const CParticleSystem& _origin);
    ~CParticleSystem();
};

