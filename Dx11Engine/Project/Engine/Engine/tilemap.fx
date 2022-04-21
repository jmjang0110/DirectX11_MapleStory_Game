#ifndef _TILEMAP
#define _TILEMAP

#include "value.fx"


struct VTX_IN
{
    float3 vPos : POSITION;
    float2 vUV : TEXCOORD;
};

struct VTX_OUT
{
    float4 vPosition : SV_Position;
    float2 vUV : TEXCOORD;
};

VTX_OUT VS_TileMap(VTX_IN _in)
{
    VTX_OUT output = (VTX_OUT) 0.f;
    
    output.vPosition = mul(float(_in.vPos, 1.f) * g_mapWVP);
    output.vUV = _in.vUV;
    
    return output;
    
}

float4 PS_TileMap(VTX_OUT _in)
{
    float4 vOutColor = (float4) 0.f;
    
 
}