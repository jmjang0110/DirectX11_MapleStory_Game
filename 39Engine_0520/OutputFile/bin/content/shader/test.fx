#ifndef _TEST
#define _TEST

#include "value.fx"

// Vertex Shader
struct VTX_IN
{
    float3 vPos     : POSITION; // semantic    
    float4 vColor   : COLOR;
    float2 vUV      : TEXCOORD;
};

struct VTX_OUT
{
    float4 vPosition    : SV_Position;
    float4 vColor       : COLOR;
    float2 vUV          : TEXCOORD;
};

VTX_OUT VS_Test(VTX_IN _in)
{
    VTX_OUT output = (VTX_OUT) 0.f;   
    
    // Local Mesh 의 좌표를 월드로 배치
    float4 vWorldPos = mul(float4(_in.vPos, 1.f), g_matWorld);    
    
    // World 좌표를 카메라 스페이스로 이동
    float4 vViewPos = mul(vWorldPos, g_matView);
    
    // 카메라 스페이스 좌표를 투영좌표로 이동
    float4 vProjPos = mul(vViewPos, g_matProj);
        
    output.vPosition = vProjPos;
    output.vColor = _in.vColor;    
    output.vUV = _in.vUV;
    
    return output;
}


// Rasterizer
// 정점이 만드는 도형(Topology) 안에 들어오는 픽셀을 검출 (픽셀 쉐이더 후보)
// 해당 픽셀들 마다 픽셀 쉐이더 호출


float4 PS_Test(VTX_OUT _in): SV_Target
{
    float4 vOutColor = (float4) 0.f;
    
    //vOutColor = _in.vColor;
    
    //if (g_int_0)
    //    vOutColor = float4(1.f, 0.f, 0.f, 1.f);
    //else
    //    vOutColor = float4(0.f, 0.f, 1.f, 1.f);
        
    vOutColor = g_tex_0.Sample(g_sam_0, _in.vUV);   
    
    return vOutColor;
}











#endif