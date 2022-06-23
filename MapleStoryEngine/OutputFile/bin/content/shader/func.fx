#ifndef _FUNC
#define _FUNC

#include "value.fx"

static float GaussianFilter[5][5] =
{
    0.003f , 0.0133f, 0.0219f, 0.0133f, 0.003f,
    0.0133f, 0.0596f, 0.0983f, 0.0596f, 0.0133f,
    0.0219f, 0.0983f, 0.1621f, 0.0983f, 0.0219f,
    0.0133f, 0.0596f, 0.0983f, 0.0596f, 0.0133f,
    0.003f , 0.0133f, 0.0219f, 0.0133f, 0.003f,
};


int IsBind(in Texture2D _tex)
{
    uint width = 0;
    uint height = 0;
    _tex.GetDimensions(width, height);
    
    if(0 != width || 0 != height)
        return 1;
   
    return 0;
}

float4 GaussianSample(in Texture2D _noisetex, float2 _vUV)
{
    float4 vOutColor = (float4) 0.f;
    
    if(0.f < _vUV.x)
    {
        _vUV.x = frac(_vUV.x);
    }
    
    if (0.f < _vUV.y)
    {
        _vUV.y = frac(_vUV.y);
    }
    
    
    
    // NoiseTexture 해상도를 이용해서 픽셀 인덱스(정수) 를 알아낸다.
    int2 iPixelIdx = (int2) (vNoiseResolution * _vUV);
    iPixelIdx -= int2(2, 2);
    
    
    // 해당 픽셀을 중심으로 가우시안 필터를 적용해서 색상의 평균치를 계산한다.
    for (int i = 0; i < 5; ++i)
    {
        for (int j = 0; j < 5; ++j)
        {
            int2 idx = int2(iPixelIdx.x + j, iPixelIdx.y + i);
            vOutColor += _noisetex[idx] * GaussianFilter[j][i];
        }
    }
    
    return vOutColor;
}




float3 CalculateLight2D(float3 _vWorldPos)
{
  
    // 광원 적용    
    //iLight2DCount;
    // Dir 0
    // Point 1
    // Spot 2
    //g_Light2DBuffer[0].iLightType;
    
    float3 vLightColor = (float3) 0.f; //LightColor 에 누적해서 최종빛의 세기를 구한다. 
    
    
    for (int i = 0; i < iLight2DCount; ++i)
    {
        if (0 == g_Light2DBuffer[i].iLightType)
        {
            vLightColor *= g_Light2DBuffer[i].color.vDiff; // 광원의 색상을 내 원래 색상에 곱한다. 
        }
        else if (1 == g_Light2DBuffer[i].iLightType)
        {
        // pixel worldpos <--> Light World Pos
            //float fDistance = distance(g_Light2DBuffer[i].vWorldPos.xy, _vWorldPos.xy);
            //if (fDistance <= g_Light2DBuffer[i].fRange)
            //{
            //    vLightColor += g_Light2DBuffer[i].color.vDiff;
            //}
            //else
            //{
            //    vLightColor = 0.f;
 
            //}
        
             // pixel worldpos <--> Light World Pos
            float fDistance = distance(g_Light2DBuffer[i].vWorldPos.xy, _vWorldPos.xy);
            float fRatio = 1.f - saturate(fDistance / g_Light2DBuffer[i].fRange);
            vLightColor += g_Light2DBuffer[i].color.vDiff.rgb * fRatio;
            
        }
        else if (2 == g_Light2DBuffer[i].iLightType)
        {
             // pixel worldpos <--> Light World Pos
            float fDistance = distance(g_Light2DBuffer[i].vWorldPos.xy, _vWorldPos.xy);
            if (fDistance <= g_Light2DBuffer[i].fRange)
            {
            
                // 사이각을 구한다. 
                float2 pxVector = _vWorldPos.xy - g_Light2DBuffer[i].vWorldPos.xy;
                float2 LightDir = normalize(g_Light2DBuffer[i].vLightDir.xy);
                pxVector = normalize(pxVector);
                
                // 카메랑 방향 벡터와 광원 위치와 픽셀 위치로의 벡터로 내적 ( 사이각 ) 을 구한다. 
                float fInnerProduct = pxVector.x * LightDir.x + pxVector.y * LightDir.y;
                fInnerProduct = acos(fInnerProduct);
        
                fInnerProduct *= (180.f / 3.141592f); // Radian to Degree 
        
                if (fInnerProduct <= g_Light2DBuffer[i].fAngle)
                {
                    float fRatio = 1.f - saturate(fDistance / g_Light2DBuffer[i].fRange);
                    vLightColor += g_Light2DBuffer[i].color.vDiff.rgb * fRatio;
                }
                else
                {
                    vLightColor = 0.f;
                }

            }
   
        
        }
    
    }
        
        
    return vLightColor;

}

#endif