#pragma once
#include "CComputeShader.h"

class CParticleShader :
    public CComputeShader
{
private:
    CStructuredBuffer* m_ParticleBuffer;

public:
    void SetParticleBuffer(CStructuredBuffer* _buffer){m_ParticleBuffer = _buffer;}


public:
    virtual void UpdateData() override;
    virtual void Clear() override;
    
public:
    CParticleShader();
    ~CParticleShader();
};

