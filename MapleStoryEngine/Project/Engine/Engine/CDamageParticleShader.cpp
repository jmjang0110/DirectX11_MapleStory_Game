#include "pch.h"
#include "CDamageParticleShader.h"


CDamageParticleShader::CDamageParticleShader()
	: CComputeShader(32, 1, 1)
	, m_ParticleBuffer(nullptr)
{
}

CDamageParticleShader::~CDamageParticleShader()
{
}

void CDamageParticleShader::UpdateData()
{
	m_ParticleBuffer->UpdateData_CS(0, false);
	m_ParticleDataBuffer->UpdateData_CS(1, false);

	// 해상도에 맞게 그룹개수가 지정된다.  
	m_iGroupX = m_ParticleBuffer->GetElementCount() / m_iGroupPerThreadCountX
		+ !!(m_ParticleBuffer->GetElementCount() % m_iGroupPerThreadCountX);
	m_iGroupY = 1;
	m_iGroupZ = 1;

	// Max Thread Count
	m_Param.iArr[0] = m_iGroupX * m_iGroupPerThreadCountX;
	m_Param.iArr[1] = m_ParticleBuffer->GetElementCount();
}

void CDamageParticleShader::Clear()
{
	m_ParticleBuffer->Clear();
}