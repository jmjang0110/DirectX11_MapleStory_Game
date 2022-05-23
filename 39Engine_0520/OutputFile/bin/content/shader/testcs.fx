#ifndef _TESTCS
#define _TESTCS

#include "value.fx"



// 그룹 스레드 개수
// 최대 제한 1024
// SV_GroupThreadID     : 그룹 내에서의 스레드 인덱스
// SV_GroupIndex        : 그룹 내에서의 스레드 인덱스를 1차원 값으로 변환
// SV_GroupID           : 스레드가 속한 그룹의 인덱스
// SV_DispatchThreadID  : 모든 스레드를 기준으로 했을 때의 인덱스

RWTexture2D<float4> g_RWTex_0 : register(u0);

#define WIDTH   g_int_0
#define HEIGHT  g_int_1
#define Color   g_vec4_0

[numthreads(32, 32, 1)]
void CS_Test(int3 _id : SV_DispatchThreadID)
{
    if (_id.x < 0 || WIDTH <= _id.x || _id.y < 0 || HEIGHT <= _id.y)
        return;    
    
    g_RWTex_0[_id.xy] = Color;
}

#endif
