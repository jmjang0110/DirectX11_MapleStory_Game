#pragma once
#include "CComputeShader.h"

class CParticleUpdateShader :
    public CComputeShader
{
private:
    CStructuredBuffer* m_ParticleBuffer;
    CStructuredBuffer* m_ParticleDataBuffer;

public:
    void SetParticleBuffer(CStructuredBuffer* _buffer){m_ParticleBuffer = _buffer;}
    void SetParticleDataBuffer(CStructuredBuffer* _buffer) { m_ParticleDataBuffer = _buffer; }
    void SetObjectWorldPos(Vec3 _vWorldPos) { m_Param.fArr[1] = _vWorldPos.x; m_Param.fArr[2] = _vWorldPos.y; m_Param.fArr[3] = _vWorldPos.z; }
    void SetParticleCreateDistance(float _fDist) { m_Param.fArr[0] = _fDist; }
    void SetParticleMinMaxTime(float _fMin, float _fMax) { m_Param.v2Arr[0] = Vec2(_fMin, _fMax); }    
    void SetStartEndSpeed(float _fMin, float _fMax) { m_Param.v2Arr[1] = Vec2(_fMin, _fMax); }
    void SetStartEndColor(Vec4 _vStart, Vec4 _vEnd) { m_Param.v4Arr[0] = _vStart; m_Param.v4Arr[1] = _vEnd; }
    void SetStartEndScale(Vec3 _vStart, Vec3 _vEnd) { m_Param.v4Arr[2] = _vStart; m_Param.v4Arr[3] = _vEnd; }


public:
    virtual void UpdateData() override;
    virtual void Clear() override;
    
public:
    CParticleUpdateShader();
    ~CParticleUpdateShader();
};

