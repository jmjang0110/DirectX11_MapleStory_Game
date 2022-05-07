#include "pch.h"
#include "CParticleUpdateShader.h"


CParticleUpdateShader::CParticleUpdateShader()
	: CComputeShader(32, 1, 1) // X 축 : 32개  32 묶음 짜리 그룹
	, m_ParticleBuffer(nullptr)
{
}

CParticleUpdateShader::~CParticleUpdateShader()
{
}

void CParticleUpdateShader::UpdateData()
{
	m_ParticleBuffer->UpdateData_CS(0, false); // register u 에  bind 하겠다

	// 32 묶음 씩 몇개의 그룹이 있으면
	// 얘네들을 다 커버 할 수 있을 만큼의 thread 가 붙냐 ?
	m_iGroupX = m_ParticleBuffer->GetElementCount() / m_iGroupPerThreadCountX + !!(m_ParticleBuffer->GetElementCount() % m_iGroupPerThreadCountX);
	m_iGroupY = 1;
	m_iGroupZ = 1;
}

void CParticleUpdateShader::Clear()
{
	m_ParticleBuffer->Clear();
}