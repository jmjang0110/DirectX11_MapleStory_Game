#ifndef _PAPERBURN
#define _PAPERBURN

#include "value.fx"


// ================
// PaperBurn Shader
// g_float_0 : Strength
#define BurnColor g_vec4_0.rgb

// g_tex_0 : Output Texture
// Rasterizer : CULL_NONE
// BlendState : Default
// DepthStencilState : LESS
// DOMAIN : Masked
// =========================
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


VTX_OUT VS_PaperBurn(VTX_IN _in)
{
    VTX_OUT output = (VTX_OUT) 0.f;
    
    output.vPosition = mul(float4(_in.vPos, 1.f), g_matWVP);
    output.vUV = _in.vUV;
    
    return output;
}

float4 PS_PaperBurn(VTX_OUT _in) : SV_Target
{
    float4 vOutColor = (float4) 0.f;
    
    // Animation 정보가 있는 경우
    if (g_useAnim2D)
    {
        float2 vUV = _in.vUV * g_vBackgroundSize;
        vUV = vUV - (g_vBackgroundSize - g_vSlice) / 2.f + g_vLT - g_vOffset;
        
        if (vUV.x < g_vLT.x || g_vLT.x + g_vSlice.x < vUV.x
            || vUV.y < g_vLT.y || g_vLT.y + g_vSlice.y < vUV.y)
        {
            discard;
        }
                
        vOutColor = g_Atlas.Sample(g_sam_1, vUV);
    }
    else
    {        
        if (g_btex_0)
        {
            vOutColor = g_tex_0.Sample(g_sam_0, _in.vUV);
        }
        else
        {
            vOutColor = float4(1.f, 0.f, 1.f, 1.f);
        }
    }
    
    float fNoise = g_noise_cloud.Sample(g_sam_0, _in.vUV).g;
    vOutColor.a -= ((fNoise * g_float_0) + g_float_0);
        
    float fRatio = 0.f;
    
    if (0.2f <= vOutColor.a)
    {
        fRatio = 1.f - ((vOutColor.a - 0.2f) / 0.8f);
        vOutColor.rgb = BurnColor * (fRatio) + (vOutColor.rgb * (1.f - fRatio));
    }
    else if (0.05f < vOutColor.a)
    {
        fRatio = 1.f - ((vOutColor.a - 0.05f) / 0.15f);
        vOutColor.rgb = float3(1.f, 1.f, 1.f) * (fRatio) + (BurnColor * (1.f - fRatio));
    }
    else
    {
        fRatio = 1.f - ((vOutColor.a - 0.f) / 0.05f);
        vOutColor.rgb = float3(0.f, 0.f, 0.f) * (fRatio) + (float3(1.f, 1.f, 1.f) * (1.f - fRatio));
    }
      
    if (vOutColor.a <= 0.f)
    {       
        discard;
    }        
   
    return vOutColor;
}






#endif