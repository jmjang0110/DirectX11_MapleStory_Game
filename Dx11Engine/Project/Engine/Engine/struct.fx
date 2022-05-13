#ifndef _STRUCT
#define _STRUCT

#include "value.fx"

// 구조화 버퍼 : Texture 픽셀 한칸을 커스텀 하는 개념 

struct tTileData
{

    int         iImgIdx;
    float2      vLTUV;
    int         iPadding;

};


struct tParticle
{
    float3      vPos;           // 12
    float3      vScale;         // 12
    float3      vDir;           // 12
    float4      vColor;         // 16

    // ===========

    float       fInclination;   // 기울기 
    float3      vTop;           // 꼭지점 
    int         TextureTypeNum;
    
    // ===========
    int         Alive;          // 4
    float       m_fCurTime;     // 4
    float       m_fMaxTime;     // 4
};

struct tParticleData
{
    int iAliveCount;
    float3 vPadding;
    
};




#endif