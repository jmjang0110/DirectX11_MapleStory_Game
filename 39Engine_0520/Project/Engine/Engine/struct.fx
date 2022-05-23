#ifndef _STRUCT
#define _STRUCT

#include "value.fx"

struct tTileData
{
    int iImgIdx;
    float2 vLTUV;
    int iPadding;
};


struct tParticle
{
    float3  vPos;
    float3  vScale;
    float3  vDir;
    float4  vColor;

    int     Alive;
    float   m_fCurTime;
    float   m_fMaxTime;
};

struct tParticleData
{
    int     iAliveCount;
    float3  vPadding;
};



#endif
