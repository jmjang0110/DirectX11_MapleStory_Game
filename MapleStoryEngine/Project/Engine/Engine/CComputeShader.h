#pragma once
#include "CShader.h"

#include "CDevice.h"

#include "CTexture.h"
#include "CStructuredBuffer.h"


class CComputeShader :
    public CShader
{
private:
    ComPtr<ID3DBlob>                m_CSBlob;
    ComPtr<ID3D11ComputeShader>     m_CS;

protected:
    // 그룹 개수 
    UINT                            m_iGroupX;
    UINT                            m_iGroupY;
    UINT                            m_iGroupZ;

    // 그룹 당 쓰레드 개수 
    UINT                            m_iGroupPerThreadCountX;
    UINT                            m_iGroupPerThreadCountY;
    UINT                            m_iGroupPerThreadCountZ;

    tScalarParam                    m_Param;


public:
    

public:    
    void Excute();
    void Excute(UINT _GroupX, UINT _GroupY, UINT _GroupZ);


protected:
    virtual void UpdateData() = 0;
    virtual void Clear() = 0;

public:
    int CreateComputeShader(const wstring& _strRelativePath, const string& _strFunc);

    // ========== Todo ===============
    UINT GetGroupPerThreadCountX() { return m_iGroupPerThreadCountX; }
    UINT GetGroupPerThreadCountY() { return m_iGroupPerThreadCountY; }
    UINT GetGroupPerThreadCountZ() { return m_iGroupPerThreadCountZ; }

    UINT GetGroupCountX() { return m_iGroupX; }
    UINT GetGroupCountY() { return m_iGroupY; }
    UINT GetGroupCountZ() { return m_iGroupZ; }

    tScalarParam    GetScalarParam() { return m_Param; }


public:
    CComputeShader();
    CComputeShader(UINT _iGroupPerThreadX, UINT _iGroupPerThreadY, UINT _iGroupPerThreadZ);
    ~CComputeShader();
};

