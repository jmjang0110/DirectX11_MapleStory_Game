#include "pch.h"
#include "CTestShader.h"

CTestShader::CTestShader()
	: CComputeShader(32, 32, 1)
{
}

CTestShader::~CTestShader()
{
}

void CTestShader::UpdateData()
{
	m_pOutputTex->UpdateData_CS(0, false);

	// 호출 그룹 개수 계산
	m_iGroupPerThreadCountX;
	m_iGroupPerThreadCountY;

	m_iGroupX = (UINT)m_pOutputTex->Width() / m_iGroupPerThreadCountX + !!((UINT)m_pOutputTex->Width() % m_iGroupPerThreadCountX);
	m_iGroupY = (UINT)m_pOutputTex->Height() / m_iGroupPerThreadCountY + !!((UINT)m_pOutputTex->Height() % m_iGroupPerThreadCountY);
	m_iGroupZ = 1;
}

void CTestShader::Clear()
{
	CTexture::ClearCS(0);
}
