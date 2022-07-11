#pragma once
#include "CRenderComponent.h"

#include "CStructuredBuffer.h"
#include "CDamageParticleShader.h"


class CDamageParticle :
    public CRenderComponent
{

private:
    Ptr<CDamageParticleShader>      m_CS;           // 파티클 업데이트 쉐이더

    CStructuredBuffer* m_ParticleBuffer;   // 파티클 구조화 버퍼
    CStructuredBuffer* m_DataBuffer;       // 추가 데이터 버퍼(파티클 AliveCount)
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

    Vec2                            m_vStartLT_UV;
    Vec2                            m_vSlice_UV;
    int                             m_DamageNum;

    float                            m_fDeltaTime;
    float                           m_fTimer;

    Vec2                            m_hitEffect_StartUV;
    Vec2                            m_hitEffect_SliceUV;

public:
    void SetDamageNum(int _damage) { m_DamageNum = _damage; }
    void SetStartLT_UV(Vec2 _LTuv) { m_vStartLT_UV = _LTuv;}
    void setSliceUV(Vec2 _sliceUV) { m_vSlice_UV = _sliceUV; }
    void SetHitEffectStartUV(Vec2 _uv) { m_hitEffect_StartUV = _uv; }
    void SetHitEffectSliceUV(Vec2 _uv) { m_hitEffect_SliceUV = _uv; }

public:
    // MaxCnt에 다라 ParticleBuffer크기를 갱신한다. 
    void SetMaxParticleCount(UINT _iMax);


public:
    virtual void finalupdate() override;
    virtual void render()   override;

public:
    virtual void SaveToScene(FILE* _pFile) override;
    virtual void LoadFromScene(FILE* _pFile) override;
    CLONE(CDamageParticle);




public:
    // Get 
    UINT    GetMaxCount() { return m_iMaxCount; }
    int     GetPosInherit() { return m_bPosInherit; }

    int     GetAliveCount() { return m_iAliveCount; }
    float   GetMinLifeTime() { return m_fMinLifeTime; }
    float   GetMaxLifeTime() { return m_fMaxLifeTime; }
    float   GetStartSpeed() { return m_fStartSpeed; }
    float   GetEndSpeed() { return m_fEndSpeed; }
    Vec4    GetStartColor() { return m_vStartColor; }
    Vec4    GetEndColor() { return m_vEndColor; }
    Vec3    GetStartScale() { return m_vStartScale; }
    Vec3    GetEndScale() { return m_vEndScale; }

    float   GetParticleCreateDIstance() { return  m_fParticleCreateDistance; }
    float   GetParticleCreateTerm() { return  m_fParticleCreateTerm; }


public:
    // Set 
    void    SetMaxCount(UINT _maxCnt) { m_iMaxCount = _maxCnt; }
    void    SetPosInherit(int _posInherit) { m_bPosInherit = _posInherit; }

    void    SetAliveCount(int _AliveCnt) { m_iAliveCount = _AliveCnt; }
    void    SetMinLifeTime(float _MinLifeTime) { m_fMinLifeTime = _MinLifeTime; }
    void    SetMaxLifeTime(float _MaxLifeTime) { m_fMaxLifeTime = _MaxLifeTime; }
    void    SetStartSpeed(float _StartSpeed) { m_fStartSpeed = _StartSpeed; }
    void    SetEndSpeed(float _EndSpeed) { m_fEndSpeed = _EndSpeed; }
    void    SetStartColor(Vec4 _StartColor) { m_vStartColor = _StartColor; }
    void    SetEndColor(Vec4 _EndColor) { m_vEndColor = _EndColor; }
    void    SetStartScale(Vec3 _StartScale) { m_vStartScale = _StartScale; }
    void    SetEndScale(Vec3 _EndScale) { m_vEndScale = _EndScale; }

    void    SetParticleCreateDIstance(float _dist) { m_fParticleCreateDistance = _dist; }
    void    SetParticleCreateTerm(float _term) { m_fParticleCreateTerm = _term; }


public:
    CDamageParticle();
    CDamageParticle(const CDamageParticle& _origin);
    ~CDamageParticle();
};

