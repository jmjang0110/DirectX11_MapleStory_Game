#ifndef _PARTICLERENDER
#define _PARTICLERENDER

#include "value.fx"
#include "struct.fx"

// ====================
// Damage ParticleRenderShader
// Mesh     : PointMesh
// Domain   : Opaque
// Blend    : AlphaBlend
// DepthStencil : Less
// Rasterizer : CULL_NONE


// Parameter
#define PARTICLE_INDEX  g_int_1
#define POS_INHERIT     g_int_0

#define StartLT     g_vec2_0
#define SliceSizeUV g_vec2_1

#define HitEffect_StartLT g_vec2_2
#define HitEffect_SliceUV g_vec2_3


#define DamageNum   g_int_2
#define DeltaTime    g_float_0


// ====================

StructuredBuffer<tParticle> ParticleBuffer : register(t16);

struct VTX_IN
{
    uint InstID : SV_InstanceID;
   
};

struct VTX_OUT
{
    uint InstID : SV_InstanceID;
};


VTX_OUT VS_DamageParticleRender(VTX_IN _in)
{
    VTX_OUT output = (VTX_OUT) 0.f;
   
    output.InstID = _in.InstID;
    
    return output;
}

struct GS_OUT
{
    float4 vPosition : SV_Position;
    float2 vUV : TEXCOORD;
    uint InstID : SV_InstanceID;
};

// 1 개의 정점을 6개로 분할
[maxvertexcount(6)]
void GS_DamageParticleRender(point VTX_OUT _in[1], inout TriangleStream<GS_OUT> _output)
{
    uint ID = _in[0].InstID;
    
    // 해당 인스턴스가 비활성화면, 출력 정점이 없다 ==> 아무일도 없다
    if (0 == ParticleBuffer[ID].Alive)
        return;
    
     // 파티클 정점의 월드위치 구하기
    float3 vParticleWorldPos = (float3) 0.f;
    
    
    if (POS_INHERIT)
    {
        vParticleWorldPos = g_matWorld._41_42_43 + ParticleBuffer[ID].vPos;
    }
    else
    {
        vParticleWorldPos = ParticleBuffer[ID].vPos;
    }
    
    // World 좌표를 View, Proj 으로 변환
    float4 vViewPos = mul(float4(vParticleWorldPos, 1.f), g_matView);
    float3 vScale = ParticleBuffer[ID].vScale;
    
    // ViewSpace 에서 Point 를 Rect 로 분할
    // 0 --- 1
    // |  \  |
    // 3 --- 2
    float3 arrRect[4] =
    {
        float3(vViewPos.x - vScale.x / 2.f, vViewPos.y + vScale.y / 2.f, vViewPos.z),
        float3(vViewPos.x + vScale.x / 2.f, vViewPos.y + vScale.y / 2.f, vViewPos.z),
        float3(vViewPos.x + vScale.x / 2.f, vViewPos.y - vScale.y / 2.f, vViewPos.z),
        float3(vViewPos.x - vScale.x / 2.f, vViewPos.y - vScale.y / 2.f, vViewPos.z)
    };
    
    GS_OUT output[4] = { (GS_OUT) 0.f, (GS_OUT) 0.f, (GS_OUT) 0.f, (GS_OUT) 0.f };
    
    for (int i = 0; i < 4; ++i)
    {
        output[i].vPosition = mul(float4(arrRect[i], 1.f), g_matProj);
        output[i].InstID = _in[0].InstID;
    }
    
    
    float2 imgLT = StartLT;
    float2 imgSlice = SliceSizeUV;

    int digit = 0;
    int num[100];
    int damage_num = DamageNum;
    
    for (int k = 0; k < 25; ++k)
    {

        if(damage_num != 0)
        {
            num[k] = damage_num % 10;
            damage_num = damage_num / 10;
            digit++;
        }
        else
        {
            num[k] = 1;
        }
    }

    float2 Offset = float2(0.f, 0.f);
    if (digit - ID >= 0)
        Offset = imgSlice * num[digit - ID];


    if(ID == 0) // 마지막
    {
        output[0].vUV = float2(HitEffect_StartLT.x, HitEffect_StartLT.y);
        output[1].vUV = float2(HitEffect_StartLT.x + HitEffect_SliceUV.x, HitEffect_StartLT.y);
        output[2].vUV = float2(HitEffect_StartLT.x + HitEffect_SliceUV.x, HitEffect_StartLT.y + HitEffect_SliceUV.y);
        output[3].vUV = float2(HitEffect_StartLT.x, HitEffect_StartLT.y + HitEffect_SliceUV.y);
    }
    else
    {
        output[0].vUV = float2(imgLT.x + Offset.x, imgLT.y);
        output[1].vUV = float2(imgLT.x + Offset.x + imgSlice.x, imgLT.y);
        output[2].vUV = float2(imgLT.x + Offset.x + imgSlice.x, imgLT.y + imgSlice.y);
        output[3].vUV = float2(imgLT.x + Offset.x, imgLT.y + imgSlice.y);
    
    }
            
    _output.Append(output[0]);
    _output.Append(output[2]);
    _output.Append(output[3]);
    _output.RestartStrip();
    
    _output.Append(output[2]);
    _output.Append(output[0]);
    _output.Append(output[1]);
    _output.RestartStrip();
}


float4 PS_DamageParticleRender(GS_OUT _in) : SV_Target
{
    float4 vOutColor = (float4) 0.f;
    
    if (g_btex_0)
    {
        vOutColor = g_tex_0.Sample(g_sam_0, _in.vUV);// * ParticleBuffer[_in.InstID].vColor;
    }
    else
    {
        vOutColor = ParticleBuffer[_in.InstID].vColor;
    }
    
    if(vOutColor.a <= 0.0f)
        discard;
    else
        vOutColor.a -= DeltaTime * 0.25f;
    
    return vOutColor;
}

#endif