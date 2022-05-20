#include "pch.h"
#include "CParticleSystem.h"


#include "CTimeMgr.h"

#include "CTransform.h"

#include "CResMgr.h"


CParticleSystem::CParticleSystem()
	: CRenderComponent(COMPONENT_TYPE::PARTICLESYSTEM)
	, m_iMaxCount(1000)
	, m_bPosInherit(0)
	, m_iAliveCount(1)
	, m_fAccTime(0.f)
	, m_fMinLifeTime(0.5f)
	, m_fMaxLifeTime(1.5f)
	, m_fStartSpeed(100.f)
	, m_fEndSpeed(10.f)
	, m_vStartColor(Vec4(255.f / 255.f, 255.f / 255.f, 51.f / 255.f, 1.f))
	, m_vEndColor(Vec4(255.f / 255.f, 0.f / 255.f, 0.f / 255.f, 1.f))
	, m_vStartScale(Vec3(30.f, 30.f, 1.f))
	, m_vEndScale(Vec3(1.f, 1.f, 1.f))
	, m_fParticleCreateDistance(40.f)
	, m_fParticleCreateTerm(0.01f)

	, m_ParticleType(PARTICLE_TYPE::BOMB)

{
	/*
		점 하나 짜리 메쉬를 GeoMetryShdaer 를 이용해서 4개 정점을 생성해 사각형으로 만들것임
	*/
	SetParticleType(m_ParticleType);


	m_ParticleBuffer = new CStructuredBuffer();
	m_ParticleBuffer->Create(sizeof(tParticle), m_iMaxCount, SB_TYPE::READ_WRITE, true, nullptr);

	m_DataBuffer = new CStructuredBuffer;
	m_DataBuffer->Create(sizeof(tParticleData), 1, SB_TYPE::READ_WRITE, true, nullptr);

}

CParticleSystem::CParticleSystem(const CParticleSystem& _origin)
	: CRenderComponent(_origin)
{
}

CParticleSystem::~CParticleSystem()
{
	SAFE_DELETE(m_ParticleBuffer);
	SAFE_DELETE(m_DataBuffer);
}



void CParticleSystem::finalupdate()
{

	m_fAccTime += DT;
	if (m_fParticleCreateTerm < m_fAccTime)
	{
		m_fAccTime = 0.f;
	

		tParticleData data = {};
		data.iAliveCount = m_iAliveCount;
		m_DataBuffer->SetData(&data, 1);

	
		
		

	}


	// StructuredBuffer ReadWrite Test
	/*
	tParticleData data = {};
	data.iAliveCount = 1;
	m_DataBuffer->SetData(&data, 1);

	tParticleData data1 = {};
	m_DataBuffer->GetData(&data1);
	*/

	if (m_ParticleType == PARTICLE_TYPE::BOMB)
	{
		static float acctime = 0.f;
		acctime += DT * 5.f;

		Vec3 pos = Transform()->GetRelativePos();
		
		float a = 140.f, b = 100.f;


		pos.x = (a - b) * cos(acctime) + b * cos(((a / b - 1) * acctime));
		pos.y = (a - b) * sin(acctime) - b * sin(((a / b - 1) * acctime));

		Transform()->SetRelativePos(pos);
	}

	m_CS->SetParticleCreateDistance(m_fParticleCreateDistance);
	

	m_CS->SetParticleBuffer(m_ParticleBuffer);
	m_CS->SetParticleDataBuffer(m_DataBuffer);

	m_CS->SetParticleMinMaxTime(m_fMinLifeTime, m_fMaxLifeTime);
	m_CS->SetStartEndSpeed(m_fStartSpeed, m_fEndSpeed);
	m_CS->SetStartEndColor(m_vStartColor, m_vEndColor);
	m_CS->SetStartEndScale(m_vStartScale, m_vEndScale);

	m_CS->SetObjectWorldPos(Transform()->GetWorldPos());

	m_CS->Excute();
}
void CParticleSystem::render()
{

	Transform()->UpdateData(); // cbuffer TRANSFORM : register(b0)


	// particlerender.fx : StructuredBuffer<tParticle> ParticleBuffer : register(t16);
	m_ParticleBuffer->UpdateData(PIPELINE_STAGE::GS | PIPELINE_STAGE::PS, 16);

	//GetMaterial()->SetScalarParam(SCALAR_PARAM::INT_0, &i);
	GetMaterial()->SetScalarParam(SCALAR_PARAM::INT_1, &m_bPosInherit);

	//================Todo
	GetMaterial()->SetScalarParam(SCALAR_PARAM::INT_2, &m_ParticleTypeNum);
	// ===================


	GetMaterial()->UpdateData();
	GetMesh()->render_particle(m_iMaxCount);

	m_ParticleBuffer->Clear();
}




void CParticleSystem::SetParticleType(PARTICLE_TYPE _tType)
{
	m_ParticleType = _tType;
	m_ParticleTypeNum = (int)_tType;
	SetParticleFactors(m_ParticleType);


	switch (m_ParticleType)
	{

	case PARTICLE_TYPE::BOMB:
		SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"PointMesh"));
		SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"ParticleRenderMtrl"));
		GetMaterial()->SetTexParam(TEX_PARAM::TEX_0, CResMgr::GetInst()->Load<CTexture>(L"Particle_01", L"texture\\particle\\AlphaCircle.png"));
		// ParticleUpdateShader : particle.fx
		m_CS = (CParticleUpdateShader*)CResMgr::GetInst()->FindRes<CComputeShader>(L"ParticleUpdateShader_Bomb").Get();

		break;

	case PARTICLE_TYPE::FIRECRACKER:
		SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"PointMesh"));
		SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"ParticleRenderMtrl"));
		GetMaterial()->SetTexParam(TEX_PARAM::TEX_1, CResMgr::GetInst()->Load<CTexture>(L"Particle_02", L"texture\\particle\\Snow50px.png"));

		m_CS = (CParticleUpdateShader*)CResMgr::GetInst()->FindRes<CComputeShader>(L"ParticleUpdateShader_FireCracker").Get();

		break;

	case PARTICLE_TYPE::MAGIC_CIRCLE:
		SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"PointMesh"));
		SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"ParticleRenderMtrl"));
		GetMaterial()->SetTexParam(TEX_PARAM::TEX_2, CResMgr::GetInst()->Load<CTexture>(L"Particle_03", L"texture\\particle\\Ice.png"));

		m_CS = (CParticleUpdateShader*)CResMgr::GetInst()->FindRes<CComputeShader>(L"ParticleUpdateShader_MagicCircle").Get();

		break;

	case PARTICLE_TYPE::BUFF_EFFECT:

		SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"PointMesh"));
		SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"ParticleRenderMtrl2"));
		GetMaterial()->SetTexParam(TEX_PARAM::TEX_0, CResMgr::GetInst()->Load<CTexture>(L"Particle_1", L"texture\\particle\\Thunderbolt.png"));
		GetMaterial()->SetTexParam(TEX_PARAM::TEX_1, CResMgr::GetInst()->Load<CTexture>(L"Particle_2", L"texture\\particle\\IceSpike.png"));
		GetMaterial()->SetTexParam(TEX_PARAM::TEX_2, CResMgr::GetInst()->Load<CTexture>(L"Particle_3", L"texture\\particle\\manyLight.png"));
		GetMaterial()->SetTexParam(TEX_PARAM::TEX_3, CResMgr::GetInst()->Load<CTexture>(L"Particle_4", L"texture\\particle\\IceSnow.png"));

		m_CS = (CParticleUpdateShader*)CResMgr::GetInst()->FindRes<CComputeShader>(L"ParticleUpdateShader_BuffEffect").Get();

		break;
	case PARTICLE_TYPE::ICE_SPIKE:

		SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"PointMesh"));
		SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"ParticleRenderMtrl2"));
		
		m_CS = (CParticleUpdateShader*)CResMgr::GetInst()->FindRes<CComputeShader>(L"ParticleUpdateShader_BuffEffect").Get();

		break;

	case PARTICLE_TYPE::ICE_SPRING:

		SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"PointMesh"));
		SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"ParticleRenderMtrl2"));
		
		m_CS = (CParticleUpdateShader*)CResMgr::GetInst()->FindRes<CComputeShader>(L"ParticleUpdateShader_BuffEffect").Get();

		break;

	case PARTICLE_TYPE::HYPO_STAR:

		SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"PointMesh"));
		SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"ParticleRenderMtrl2"));
		GetMaterial()->SetTexParam(TEX_PARAM::TEX_0, CResMgr::GetInst()->Load<CTexture>(L"Particle_010", L"texture\\particle\\Bubbles50px.png"));

		m_CS = (CParticleUpdateShader*)CResMgr::GetInst()->FindRes<CComputeShader>(L"ParticleUpdateShader_HypoStar").Get();

		break;


	case PARTICLE_TYPE::END:
		break;
	default:
		break;
	}

}

void CParticleSystem::SetParticleFactors(PARTICLE_TYPE _tType)
{

	switch (m_ParticleType)
	{
	case PARTICLE_TYPE::BOMB:
		m_iMaxCount					= (1000);
		m_bPosInherit				= (0);
		m_iAliveCount				= (1);
		m_fAccTime					= (0.f);
		m_fMinLifeTime				= (0.5f);
		m_fMaxLifeTime				= (1.5f);
		m_fStartSpeed				= (50.f);
		m_fEndSpeed					= (10.f);
		m_vStartColor				= (Vec4(255.f / 255.f, 255.f / 255.f, 51.f / 255.f, 1.f));
		m_vEndColor					= (Vec4(255.f / 255.f, 0.f / 255.f, 0.f / 255.f, 1.f));
		m_vStartScale				= (Vec3(30.f, 30.f, 1.f));
		m_vEndScale					= (Vec3(1.f, 1.f, 1.f));
		m_fParticleCreateDistance	= (40.f);
		m_fParticleCreateTerm		= (0.01f);

		break;
	case PARTICLE_TYPE::FIRECRACKER:
		m_iMaxCount					= (1000);
		m_bPosInherit				= (0);
		m_iAliveCount				= (3);
		m_fAccTime					= (0.f);
		m_fMinLifeTime				= (0.5f);
		m_fMaxLifeTime				= (3.f);
		m_fStartSpeed				= (100.f);
		m_fEndSpeed					= (10.f);
		m_vStartColor				= (Vec4(153.f / 255.f, 255.f / 255.f, 255.f / 255.f, 1.f));
		m_vEndColor					= (Vec4(153.f / 255.f, 204.f / 255.f, 255.f / 255.f, 1.f));
		m_vStartScale				= (Vec3(30.f, 30.f, 1.f));
		m_vEndScale					= (Vec3(1.f, 1.f, 1.f));
		m_fParticleCreateDistance	= (80.f);
		m_fParticleCreateTerm		= (0.002f);

		break;
	case PARTICLE_TYPE::MAGIC_CIRCLE:
		m_iMaxCount					= (401);
		m_bPosInherit				= (0);
		m_iAliveCount				= (401);
		m_fAccTime					= (0.f);
		m_fMinLifeTime				= (0.5f);
		m_fMaxLifeTime				= (10.f);
		m_fStartSpeed				= (50.f);
		m_fEndSpeed					= (50.f);
		m_vStartColor				= (Vec4(153.f / 255.f, 255.f / 255.f, 255.f / 255.f, 1.f));
		m_vEndColor					= (Vec4(153.f / 255.f, 204.f / 255.f, 255.f / 255.f, 1.f));
		m_vStartScale				= (Vec3(10.f, 10.f, 1.f));
		m_vEndScale					= (Vec3(10.f, 10.f, 1.f));
		m_fParticleCreateDistance	= (0.f);
		m_fParticleCreateTerm		= (0.2f);

		break;

	case PARTICLE_TYPE::BUFF_EFFECT:
		m_iMaxCount					= (1000);
		m_bPosInherit				= (0);
		m_iAliveCount				= (900);
		m_fAccTime					= (0.f);
		m_fMinLifeTime				= (0.f);// 어떤 형태 파티클인지 지정 
		m_fMaxLifeTime				= (4.f);
		m_fStartSpeed				= (400.f);
		m_fEndSpeed					= (100.f);
		m_vStartColor				= (Vec4(102.f / 255.f, 102.f / 255.f, 102.f / 255.f, 1.f));
		m_vEndColor					= (Vec4(051.f / 255.f, 051.f / 255.f, 051.f / 255.f, 1.f));
		m_vStartScale				= (Vec3(100.f, 100.f, 1.f));
		m_vEndScale					= (Vec3(100.f, 100.f, 1.f));
		m_fParticleCreateDistance	= (0.f);
		m_fParticleCreateTerm		= (4.f);
		break;

	case PARTICLE_TYPE::ICE_SPIKE:
		m_iMaxCount					= (1000);
		m_bPosInherit				= (0);
		m_iAliveCount				= (1);
		m_fAccTime					= (0.f);
		m_fMinLifeTime				= (1.f); // 어떤 형태 파티클인지 지정 
		m_fMaxLifeTime				= (4.f);
		m_fStartSpeed				= (1.f);
		m_fEndSpeed					= (1.f);
		m_vStartColor				= (Vec4(204.f / 255.f, 204.f / 255.f, 204.f / 255.f, 1.f));
		m_vEndColor					= (Vec4(255.f / 255.f, 255.f / 255.f, 255.f / 255.f, 1.f));
		m_vStartScale				= (Vec3(60.f, 120.f, 1.f));
		m_vEndScale					= (Vec3(400.f, 800.f, 1.f));
		m_fParticleCreateDistance	= (0.f);
		m_fParticleCreateTerm		= (4.f);
		break;

		case PARTICLE_TYPE::ICE_SPRING:
		m_iMaxCount					= (1000);
		m_bPosInherit				= (0);
		m_iAliveCount				= (30);
		m_fAccTime					= (0.f);
		m_fMinLifeTime				= (2.f); // 어떤 형태 파티클인지 지정 
		m_fMaxLifeTime				= (4.f);
		m_fStartSpeed				= (200.f);
		m_fEndSpeed					= (100.f);
		m_vStartColor				= (Vec4(204.f / 255.f, 255.f / 255.f, 255.f / 255.f, 1.f));
		m_vEndColor					= (Vec4(204.f / 255.f, 255.f / 255.f, 255.f / 255.f, 1.f));
		m_vStartScale				= (Vec3(60.f, 120.f, 1.f));
		m_vEndScale					= (Vec3(400.f, 800.f, 1.f));
		m_fParticleCreateDistance	= (500.f);
		m_fParticleCreateTerm		= (4.f);
		break;

		case PARTICLE_TYPE::HYPO_STAR:
		m_iMaxCount					= (1000);
		m_bPosInherit				= (0);
		m_iAliveCount				= (1);
		m_fAccTime					= (0.f);
		m_fMinLifeTime				= (0.5f);
		m_fMaxLifeTime				= (8.f);
		m_fStartSpeed				= (20.f);
		m_fEndSpeed					= (20.f);
		m_vStartColor				= (Vec4(204.f / 255.f, 255.f / 255.f, 255.f / 255.f, 1.f));
		m_vEndColor					= (Vec4(204.f / 255.f, 255.f / 255.f, 255.f / 255.f, 1.f));
		m_vStartScale				= (Vec3(30.f, 30.f, 1.f));
		m_vEndScale					= (Vec3(10.f, 10.f, 1.f));
		m_fParticleCreateDistance	= (300.f);
		m_fParticleCreateTerm		= (0.1f);

		break;

	case PARTICLE_TYPE::END:
		break;
	default:
		break;
	}

}

