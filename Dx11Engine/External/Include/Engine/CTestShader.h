#pragma once
#include "CComputeShader.h"


// 특정 pixel 을 지정된 Color 로 색칠하는 쉐이더 
// -> Compute shader 와 Thread 를 이용해서 pixel  색 지정을 
//    한번에 해결  
class CTestShader :
    public CComputeShader
{
private:
    Ptr<CTexture>   m_pOutputTex;



public:
    void SetOutputTexture(Ptr<CTexture> _pOutTex)
    {
        m_pOutputTex = _pOutTex;
        m_Param.iArr[0] = (int)m_pOutputTex->Width();
        m_Param.iArr[1] = (int)m_pOutputTex->Height();
    }

    void SetColor(Vec4 _vColor) { m_Param.v4Arr[0] = _vColor; }


    virtual void UpdateData();
    virtual void Clear();


public:
    CTestShader();
    ~CTestShader();
};

