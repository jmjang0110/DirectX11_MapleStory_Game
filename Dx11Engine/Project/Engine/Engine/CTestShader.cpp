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

	/*
		!!((UINT)m_pOutputTex->Width() % m_iGroupPerThreadCountX);
		
		->> 만약에 나머지가 있으면 !! 해주면 1 이므로 그룹을 하나 더 지정 O [ ex (26!! -> 1 ) ] 
			만약에 나머지가 없으면 !! 해주면 0 이므로 구룹을 하나 더 지정 X [ ex (0 !! -> 0 ) ] 
	*/
	m_iGroupX = (UINT)m_pOutputTex->Width() / m_iGroupPerThreadCountX + !!((UINT)m_pOutputTex->Width() % m_iGroupPerThreadCountX);
	m_iGroupY = (UINT)m_pOutputTex->Height() / m_iGroupPerThreadCountY + !!((UINT)m_pOutputTex->Height() % m_iGroupPerThreadCountY);
	m_iGroupZ = 1;
}
 
void CTestShader::Clear()
{
	CTexture::ClearCS(0);
}
