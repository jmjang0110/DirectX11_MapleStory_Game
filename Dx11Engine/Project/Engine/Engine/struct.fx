#ifndef _STRUCT
#define _STRUCT

#include "value.fx"

// ±¸Á¶È­ ¹öÆÛ : Texture ÇÈ¼¿ ÇÑÄ­À» Ä¿½ºÅÒ ÇÏ´Â °³³ä 

struct tTileData
{

    int         iImgIdx;
    float2      vLTUV;
    int         iPadding;

};


struct tParticle
{
    float3      vPos;
    float3      vScale;
    float3      vDir;
    float4      vColor;

    int         Alive;
    float2      Padding;
};



#endif