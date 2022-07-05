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


// =========================
// Std2D Shader
// g_tex_0 : Output Texture
// Rasterizer : CULL_NONE
// BlendState : Default
// DepthStencilState : LESS
// DOMAIN : Masked
#define IsHorizontal_Flip g_int_0
#define IsVertical_Flip g_int_1

// =========================
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
    
    // Animation 정보가 있는 경우
    if (g_useAnim2D)
    {
        float2 vUV = _in.vUV * g_vBackgroundSize;
        
        if (IsHorizontal_Flip == 0 && IsVertical_Flip == 0) // 수평 뒤집기 X 수직 뒤집기 X 
        {
            vUV = vUV - (g_vBackgroundSize - g_vSlice) / 2.f + g_vLT - g_vOffset;
            
            if (vUV.x < g_vLT.x || g_vLT.x + g_vSlice.x < vUV.x
            || vUV.y < g_vLT.y || g_vLT.y + g_vSlice.y < vUV.y)
            {
                discard;
            }
            
        }
        else if (IsHorizontal_Flip == 1 && IsVertical_Flip == 0) // 수평 뒤집기 O 수직 뒤집기 X 
        {
            float2 slice = g_vSlice;
            slice.x *= -1;
            
            float2 lt = g_vLT;
            lt.x -= slice.x;
            
            // lt.x 가 자꾸 BackGround 까지해서 시작점으로 착각했네... 
            // float StartUV_x =  lt.x -  ((g_vBackgroundSize.x - slice.x) / 2.f); !! 
            
            float R_x = lt.x + slice.x + ((g_vBackgroundSize.x - slice.x) / 2.f);
            float width = R_x - (lt.x - ((g_vBackgroundSize.x - slice.x) / 2.f));
            float M_x = lt.x -  ((g_vBackgroundSize.x - slice.x) / 2.f) + (width / 2.f);
           
            vUV = vUV - (g_vBackgroundSize - slice) / 2.f + lt - g_vOffset;
            
            float dist = M_x - vUV.x;
            vUV.x = M_x + dist;
        
            if (vUV.x < lt.x || lt.x + slice.x < vUV.x
            || vUV.y < lt.y || lt.y + slice.y < vUV.y)
            {
                discard;
            }
           
        }
        else if (IsHorizontal_Flip == 0 && IsVertical_Flip == 1) // 수평 뒤집기 X 수직 뒤집기 O 
        { 
            float2 slice = g_vSlice;
            slice.y *= -1;
            
            float2 lt = g_vLT;
            lt.y -= slice.y;
            
            // lt.x 가 자꾸 BackGround 까지해서 시작점으로 착각했네... 
            // float StartUV_x =  lt.x -  ((g_vBackgroundSize.x - slice.x) / 2.f); !! 
            
            float R_y = lt.y + slice.y + ((g_vBackgroundSize.y - slice.y) / 2.f);
            float height = R_y - (lt.y - ((g_vBackgroundSize.y - slice.y) / 2.f));
            float M_y = lt.y - ((g_vBackgroundSize.y - slice.y) / 2.f) + (height / 2.f);
           
            vUV = vUV - (g_vBackgroundSize - slice) / 2.f + lt - g_vOffset;
            
            float dist = M_y - vUV.y;
            vUV.y = M_y + dist;
        
            if (vUV.x < lt.x || lt.x + slice.x < vUV.x
            || vUV.y < lt.y || lt.y + slice.y < vUV.y)
            {
                discard;
            }
            
           
        }
        else if (IsHorizontal_Flip == 1 && IsVertical_Flip == 1) // 수평 뒤집기 O 수직 뒤집기 O 
        {
            
            float2 slice = g_vSlice;
            slice *= -1;
            
            float2 lt = g_vLT;
            lt -= slice;

            
            float R_x = lt.x + slice.x + ((g_vBackgroundSize.x - slice.x) / 2.f);
            float width = R_x - (lt.x - ((g_vBackgroundSize.x - slice.x) / 2.f));
            float M_x = lt.x - ((g_vBackgroundSize.x - slice.x) / 2.f) + (width / 2.f);
            
            float R_y = lt.y + slice.y + ((g_vBackgroundSize.y - slice.y) / 2.f);
            float height = R_y - (lt.y - ((g_vBackgroundSize.y - slice.y) / 2.f));
            float M_y = lt.y - ((g_vBackgroundSize.y - slice.y) / 2.f) + (height / 2.f);
            
            vUV = vUV - (g_vBackgroundSize - slice) / 2.f + lt - g_vOffset;
    
            
            float dist = M_x - vUV.x;
            vUV.x = M_x + dist;
            
            dist = M_y - vUV.y;
            vUV.y = M_y + dist;
            
            if (vUV.x < lt.x || lt.x + slice.x < vUV.x
            || vUV.y < lt.y || lt.y + slice.y < vUV.y)
            {
                discard;
            }
            
            //float2 lt = g_vLT;
            //lt -= slice;
          
            //// offset 구현안함 아직
            //vUV.x = g_vLT.x + (g_vBackgroundSize.x - slice.x) / 2.f - vUV.x;
            //vUV.y = g_vLT.y + (g_vBackgroundSize.y - g_vSlice.y * -1) / 2.f - vUV.y;
        }
            
        //if (vUV.x < g_vLT.x || g_vLT.x + g_vSlice.x < vUV.x
        //    || vUV.y < g_vLT.y || g_vLT.y + g_vSlice.y < vUV.y)
        //{            
        //    discard;
        //}
                
        vOutColor = g_Atlas.Sample(g_sam_1, vUV);     
        

        
    }
    else
    {
        //if(IsBind(g_tex_0))
        if(g_btex_0)
        {
            vOutColor = g_tex_0.Sample(g_sam_0, _in.vUV);
        }
        else
        {
            vOutColor = float4(1.f, 0.f, 1.f, 1.f);
        }        
    }
    
    
    if (vOutColor.a <= 0.1f)
    {
        vOutColor = float4(1.f, 0.f, 1.f, 1.f);
        //discard;
    }
   
    return vOutColor;
}

// ========================
// Std2DAlphaBlend
// BlendState           : Alpha Blend
// DepthStencilState    : No_Write
// DOMAIN               : OPAQUE
//=========================
VTX_OUT VS_Std2DAlpha(VTX_IN _in)
{
    VTX_OUT output = (VTX_OUT) 0.f;
    
    output.vPosition = mul(float4(_in.vPos, 1.f), g_matWVP);
    output.vUV = _in.vUV;
    
    return output;
}

float4 PS_Std2DAlpha(VTX_OUT _in) : SV_Target
{
    float4 vOutColor = (float4) 0.f;
    
    vOutColor = g_tex_0.Sample(g_sam_0, _in.vUV);
   
    return vOutColor;
}




// ==================
// Collider2D Shader
// g_int_0 : Collision
// ==================
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
       
    if(g_int_0)
    {
        vOutColor = float4(1.f, 0.f, 0.f, 1.f);
    }
    else
    {
        vOutColor = float4(0.f, 1.f, 0.f, 1.f);
    }
        
    return vOutColor;
}




// ================
// Empty2D Shader
// - for Render Nothing
// ================

VTX_OUT VS_Empty2D(VTX_IN _in)
{
    VTX_OUT output = (VTX_OUT) 0.f;
    
    return output;
}


float4 PS_Empty2D(VTX_OUT _in) : SV_Target
{
    float4 vOutColor = (float4) 0.f;
    
    discard;
        
    return vOutColor;
}












#endif
