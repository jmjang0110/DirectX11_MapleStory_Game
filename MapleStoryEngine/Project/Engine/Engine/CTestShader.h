#pragma once
#include "CComputeShader.h"



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

