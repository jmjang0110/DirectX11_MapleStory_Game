#ifndef _PARTICLE_MAGIC_CIRCLE_
#define _PARTICLE_MAGIC_CIRCLE_

#include "value.fx"
#include "struct.fx"
#include "func.fx"

// =======================
// Particle Update Shader _PARTICLE_MAGIC_CIRCLE_

#define MaxThreadCount      g_int_0
#define MaxParticleCount    g_int_1

#define CreateRange         g_float_0
#define WorldPos            float3(g_float_1, g_float_2, g_float_3)

#define MinLifeTime         g_vec2_0.x
#define MaxLifeTime         g_vec2_0.y

#define MinSpeed            g_vec2_1.x
#define MaxSpeed            g_vec2_1.y

#define StartColor          g_vec4_0
#define EndColor            g_vec4_1
#define StartScale          g_vec4_2
#define EndScale            g_vec4_3

// =======================

RWStructuredBuffer<tParticle> ParticleBuffer : register(u0);
RWStructuredBuffer<tParticleData> DataBuffer : register(u1);

[numthreads(32, 1, 1)]
void CS_Particle(int3 _id : SV_DispatchThreadID)
{
    if (MaxParticleCount < _id.x)
        return;
          
    // 파티클이 비활성화 되어있다면...
    if (0 == ParticleBuffer[_id.x].Alive)
    {
        int iOriginValue = DataBuffer[0].iAliveCount;
        int iInputValue = DataBuffer[0].iAliveCount - 1;
        int iOutValue = 0;
    
        // Alive 기회가 없으면 리턴
        if (iOriginValue <= 0)
            return;
    
        InterlockedExchange(DataBuffer[0].iAliveCount, iInputValue, iOutValue);
    
        // 활성화 성공한 경우
        if (iOriginValue == iOutValue)
        {
            ParticleBuffer[_id.x].Alive = 1;
        
            // 랜덤한 위치 부여
            float2 vSampleUV = float2(((float) _id.x / (float) MaxThreadCount) + fAccTime, fAccTime);
            vSampleUV.y += sin(vSampleUV.x * 3.141592f * 2.f) * 0.5f;
            float3 vOut = GaussianSample(g_noise_cloud, vSampleUV).rgb;
                        
            float3 vRandom =
            {
                GaussianSample(g_noise_01, vSampleUV + float2(0.1f, 0.f)).r
                , GaussianSample(g_noise_01, vSampleUV + float2(0.2f, 0.f)).r
                , GaussianSample(g_noise_01, vSampleUV + float2(0.3f, 0.f)).r
            };
                        
            
            ParticleBuffer[_id.x].vPos = WorldPos + (vOut - 0.5f) * CreateRange; // 파티클 생성 위치 
            ParticleBuffer[_id.x].vScale = float3(10.f, 10.f, 1.f); // 파티클 초기 크기 
            ParticleBuffer[_id.x].vDir = normalize(vRandom - 0.5f); // 파티클 진행 방향 
            
            // =========================
            ParticleBuffer[_id.x].vPos.x = cos(ParticleBuffer[_id.x].m_fCurTime * vRandom.g) + WorldPos.x;
          
            //===========================   
            
            
            ParticleBuffer[_id.x].m_fCurTime = 0.f;
            ParticleBuffer[_id.x].m_fMaxTime = MinLifeTime + (MaxLifeTime - MinLifeTime) * vOut.r;
            
            if (400 == _id.x)
                ParticleBuffer[_id.x].m_fMaxTime *= 3.f;

        }
    }
    
    // 활성화 된 파티클
    else
    {
        // 생존시간 누적
        ParticleBuffer[_id.x].m_fCurTime += fDT;
        
        // 최대 생존시간을 넘어선 경우
        
        if (ParticleBuffer[_id.x].m_fMaxTime <= ParticleBuffer[_id.x].m_fCurTime)
        {
            ParticleBuffer[_id.x].Alive = 0;
            return;
        }
                
      
        
        float fLifeRatio = ParticleBuffer[_id.x].m_fCurTime / ParticleBuffer[_id.x].m_fMaxTime;
        
        float fSpeed = MinSpeed + (MaxSpeed - MinSpeed) * fLifeRatio;
        float4 vColor = StartColor + (EndColor - StartColor) * fLifeRatio;
        float3 vScale = StartScale + (EndScale - StartScale) * fLifeRatio;
     
        //ParticleBuffer[_id.x].vPos += ParticleBuffer[_id.x].vDir * fSpeed * fDT;
        
        if( 0 <= _id.x && _id.x < 100)
        {
            ParticleBuffer[_id.x].vPos.z = sin(ParticleBuffer[_id.x].m_fCurTime * 3.f) * 50.f + WorldPos.z;
            ParticleBuffer[_id.x].vPos.x = cos(ParticleBuffer[_id.x].m_fCurTime * 3.f) * 50.f + WorldPos.x;
            ParticleBuffer[_id.x].vScale = vScale;
            ParticleBuffer[_id.x].vColor = vColor;
            
        }
        else if (100 <= _id.x && _id.x < 200)
        {
            ParticleBuffer[_id.x].vPos.y = sin(ParticleBuffer[_id.x].m_fCurTime * 3.f) * 50.f;
            ParticleBuffer[_id.x].vPos.z = cos(ParticleBuffer[_id.x].m_fCurTime * 3.f) * 50.f + WorldPos.z;
            
            ParticleBuffer[_id.x].vScale = vScale;
            ParticleBuffer[_id.x].vColor = vColor;
        }
        else if (200 <= _id.x && _id.x < 300)
        {
            ParticleBuffer[_id.x].vPos.z = sin(ParticleBuffer[_id.x].m_fCurTime * 3.f) * 50.f + WorldPos.z;
            ParticleBuffer[_id.x].vPos.x = cos(ParticleBuffer[_id.x].m_fCurTime * 3.f) * 50.f + WorldPos.x;
            ParticleBuffer[_id.x].vPos.y = ParticleBuffer[_id.x].vPos.x - WorldPos.x;
            
            ParticleBuffer[_id.x].vScale = vScale;
            ParticleBuffer[_id.x].vColor = vColor;
        }
        else if (300 <= _id.x && _id.x < 400)
        {
            ParticleBuffer[_id.x].vPos.z = sin(ParticleBuffer[_id.x].m_fCurTime * 3.f) * 50.f + WorldPos.z;
            ParticleBuffer[_id.x].vPos.x = cos(ParticleBuffer[_id.x].m_fCurTime * 3.f) * 50.f + WorldPos.x;
            ParticleBuffer[_id.x].vPos.y = (ParticleBuffer[_id.x].vPos.x - WorldPos.x) * -1;
            
            ParticleBuffer[_id.x].vScale = vScale;
            ParticleBuffer[_id.x].vColor = vColor;
        }
        else if (400 == _id.x)
        {
            ParticleBuffer[_id.x].vPos = WorldPos;
            
            //ParticleBuffer[_id.x].vScale += vScale * 0.002f;
            ParticleBuffer[_id.x].vScale = vScale * 10.f; //cos(ParticleBuffer[_id.x].m_fCurTime) * 30.f;
            ParticleBuffer[_id.x].vColor = vColor *  cos(ParticleBuffer[_id.x].m_fCurTime * 5.f);
        }
       
       
        
    }

}

#endif