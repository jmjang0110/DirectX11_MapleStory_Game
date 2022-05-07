#ifndef _PARTICLE
#define _PARTICLE

#include "value.fx"
#include "struct.fx"


// =======================
// Particle Update Shader
// =======================

RWStructuredBuffer<tParticle> ParticleBuffer : register(u0);

[numthreads(32, 1, 1)]
void CS_Particle(int3 _id : SV_DispatchThreadID)
{
    if (ParticleBuffer[_id.x].Alive)
    {
        ParticleBuffer[_id.x].vColor = float4(0.f, 1.f, 1.f, 1.f);
        
        float3 vDir = float3(ParticleBuffer[_id.x].vPos.xy, 0.f);
        vDir = normalize(vDir);
    
        ParticleBuffer[_id.x].vPos += vDir * fDT * 50.f;
    }
}

#endif