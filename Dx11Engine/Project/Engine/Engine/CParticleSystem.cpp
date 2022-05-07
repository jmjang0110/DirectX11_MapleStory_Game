#include "pch.h"
#include "CParticleSystem.h"


#include "CTransform.h"

#include "CResMgr.h"

const int iWidth = 10;
const int iHeight = 10;

CParticleSystem::CParticleSystem()
	: CRenderComponent(COMPONENT_TYPE::PARTICLESYSTEM)
	, m_iMaxCount(iWidth* iHeight)
	, m_bPosInherit(0)
{
	/*
		점 하나 짜리 메쉬를 GeoMetryShdaer 를 이용해서 4개 정점을 생성해 사각형으로 만들것임
	*/
	SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"PointMesh"));
	SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"ParticleRenderMtrl"));

	GetMaterial()->SetTexParam(TEX_PARAM::TEX_0, CResMgr::GetInst()->Load<CTexture>(L"Particle_01", L"texture\\particle\\smokeparticle.png"));


	m_CS = (CParticleUpdateShader*)CResMgr::GetInst()->FindRes<CComputeShader>(L"ParticleUpdateShader").Get();


	tParticle arrParticle[iWidth * iHeight] = {};

	Vec2 vResolution = CDevice::GetInst()->GetRenderResolution();


	for (int i = 0; i < iHeight; ++i)
	{
		for (int j = 0; j < iWidth; ++j)
		{
			arrParticle[iWidth * i + j].vPos =
				Vec3((vResolution.x / iWidth) * (float)j - vResolution.x / 2.f
					, (vResolution.y / iHeight) * (float)i - vResolution.y / 2.f
					, 500.f);

			arrParticle[iWidth * i + j].vPos.x = arrParticle[iWidth * i + j].vPos.x / 10.f;
			arrParticle[iWidth * i + j].vPos.y = arrParticle[iWidth * i + j].vPos.y / 10.f;

			arrParticle[iWidth * i + j].vScale = Vec3(50.f, 50.f, 1.f);
			arrParticle[iWidth * i + j].vColor = Vec4(0.f, 0.f, 1.f, 1.f);

			arrParticle[iWidth * i + j].Alive = 1;

			/*if( (iHeight * iWidth) / 2 < iWidth * i + j)
				arrParticle[iWidth * i + j].Alive = 0;
			else
				arrParticle[iWidth * i + j].Alive = 1;*/
		}
	}

	m_ParticleBuffer = new CStructuredBuffer();
	m_ParticleBuffer->Create(sizeof(tParticle), m_iMaxCount, SB_TYPE::READ_WRITE, false, arrParticle);
}

CParticleSystem::CParticleSystem(const CParticleSystem& _origin)
	: CRenderComponent(_origin)
{
}

CParticleSystem::~CParticleSystem()
{
	SAFE_DELETE(m_ParticleBuffer);
}


void CParticleSystem::finalupdate()
{
	m_CS->SetParticleBuffer(m_ParticleBuffer);
	m_CS->Excute();
}

void CParticleSystem::render()
{

	Transform()->UpdateData();

	m_ParticleBuffer->UpdateData(PIPELINE_STAGE::GS | PIPELINE_STAGE::PS, 16);

	//GetMaterial()->SetScalarParam(SCALAR_PARAM::INT_0, &i);
	GetMaterial()->SetScalarParam(SCALAR_PARAM::INT_1, &m_bPosInherit);
	GetMaterial()->UpdateData();
	GetMesh()->render_particle(m_iMaxCount);

	m_ParticleBuffer->Clear();
}