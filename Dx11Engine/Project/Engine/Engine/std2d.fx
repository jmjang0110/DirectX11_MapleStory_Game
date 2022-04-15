#ifndef _STD2D
#define _STD2D

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


// ============
// Std2D Shader
// g_float_0            : Mask Limit / limit 기준 alpha 값 세팅할때 사용 
// g_tex_0              : Output Texture
// Rasterizer           : CULL_NONE
// BlendState           : Default
// DepthStencilState    : LESS 
// ============
VTX_OUT VS_Std2D(VTX_IN _in)
{
    VTX_OUT output = (VTX_OUT) 0.f;
    
    output.vPosition = mul(float4(_in.vPos, 1.f), g_matWVP);
    output.vUV = _in.vUV;
    
    return output;
}

float4 PS_Std2D(VTX_OUT _in) : SV_Target
{
    float4 vOutColor = (float4) 0.f;
    
    vOutColor = g_tex_0.Sample(g_sam_0, _in.vUV);
    
    if (vOutColor.a <= g_float_0)
    {
        discard;
    }
    
    return vOutColor;
}

// ======================
// STD 2D AlphaBlend
// BlendState           :   Alpha blend 
// DepthStencil State   :   NO_Write // 깊이 텍스처를 남기지 않는다. ( 반투명 ) 
// DOMAIN               :   OPAQUE 
// ======================

VTX_OUT VS_Std2DAlphaBlend(VTX_IN _in)
{
    VTX_OUT output = (VTX_OUT) 0.f;
    
    output.vPosition = mul(float4(_in.vPos, 1.f), g_matWVP);
    output.vUV = _in.vUV;
    
    return output;
}

float4 PS_Std2DAlphaBlend(VTX_OUT _in) : SV_Target
{
    float4 vOutColor = (float4) 0.f;
    
    vOutColor = g_tex_0.Sample(g_sam_0, _in.vUV);

    return vOutColor;
}


// =================
// Collider2D Shader
// g_int_0 : Collision
// =================
VTX_OUT VS_Collider2D(VTX_IN _in)
{
    VTX_OUT output = (VTX_OUT) 0.f;
    
    output.vPosition = mul(float4(_in.vPos, 1.f), g_matWVP);
    output.vUV = _in.vUV;
    
    return output;
}


float4 PS_Collider2D(VTX_OUT _in) : SV_Target
{
    float4 vOutColor = (float4) 0.f;
       
    if (g_int_0)
    {
        vOutColor = float4(1.f, 0.f, 0.f, 1.f);
    }
    else
    {
        vOutColor = float4(0.f, 1.f, 0.f, 1.f);
    }
    
    
    return vOutColor;
}







#endif
