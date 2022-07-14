#include "pch.h"
#include "CDamageScript.h"

#include <Engine/CTimeMgr.h>
#include <Engine/CTileMap.h>
#include <Engine/CTileMap.h>

#include <Engine/CEventMgr.h>
#include <Engine/CDamageParticle.h>


CDamageScript::CDamageScript()
	: CScript((int)SCRIPT_TYPE::DAMAGESCRIPT)
	, m_eType(DAMAGE_TYPE::BASIC)
	, m_fTimer(0.f)
	, m_fDeltatime(0.f)

{
	SetName(CScriptMgr::GetScriptName(this));
	Init(DAMAGE_TYPE::CRITICAL_1, 10421540, 5);
}


CDamageScript::CDamageScript(const CDamageScript& _origin)
	: CScript((int)SCRIPT_TYPE::DAMAGESCRIPT)

{
	SetName(CScriptMgr::GetScriptName(this));

}

CDamageScript::~CDamageScript()
{
	//Safe_Del_Vec(m_vecDamageObj);
	SAFE_DELETE(m_DamagePrefab);

}


void CDamageScript::Init(DAMAGE_TYPE _eType, int _Damage, float _fMaxTime)
{
	m_eType		= _eType;
	m_iDamage	= _Damage;
	m_fMaxTime	= _fMaxTime;

	int num = m_iDamage;
	while (num > 0)
	{
		m_vecDamageNum.push_back(num % 10); // 123 -> 3 2 1 로 저장 -> 사용할땐 거꾸로 
		num /= 10;
	}

	
}

void CDamageScript::start()
{
	m_fTimer = 0.f;

	CGameObject* pOwner = GetOwner();
	CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();
	int layerIdx = GetOwner()->GetLayerIndex();

	if (pOwner == nullptr)
		return;


	Vec2 vStartLT = Vec2(0.f, 0.f);
	Vec2 vSlice = Vec2(0.f, 0.f);

	switch (m_eType)
	{
	case DAMAGE_TYPE::BASIC:
	{
		vStartLT = Vec2(0.f, 100.f);
		vSlice = Vec2(37.f, 39.f);
	}
	break;
	case DAMAGE_TYPE::MISS:
	{
		vStartLT = Vec2(0.f, 187.f);
		vSlice = Vec2(37.f, 39.f);
	}
	break;
	case DAMAGE_TYPE::CRITICAL_1:
	{
		vStartLT = Vec2(0.f, 0.f);
		vSlice = Vec2(43.f, 48.f);
	}
	break;
	case DAMAGE_TYPE::CRITICAL_2:
	{
		vStartLT = Vec2(0.f, 143.f);
		vSlice = Vec2(39.f, 40.f);
	}
	break;
	case DAMAGE_TYPE::CRITICAL_3:
	{
		vStartLT = Vec2(0.f, 48.f);
		vSlice = Vec2(47.f, 48.f);
	}
	break;
	}



	if (nullptr == pOwner->GetRenderComponent())
	{
		pOwner->AddComponent(new CDamageParticle);
		Ptr<CTexture> pParticleTex = CResMgr::GetInst()->Load<CTexture>(L"texture\\hit\\damageSkin.png", L"texture\\hit\\damageSkin.png");
		Vec2 sliceUV = Vec2(vSlice.x / pParticleTex->Width(), vSlice.y / pParticleTex->Height());
		Vec2 StartUV = Vec2(vStartLT.x / pParticleTex->Width(), vStartLT.y / pParticleTex->Height());

		Vec2 HitStartUV = Vec2(390.f / pParticleTex->Width(), 143.f / pParticleTex->Height());
		Vec2 HitSliceUV = Vec2(62.f / pParticleTex->Width(), 57.f / pParticleTex->Height());

		// TEST 
		pOwner->DamageParticle()->GetMaterial()->SetTexParam(TEX_PARAM::TEX_0, pParticleTex);
		pOwner->DamageParticle()->SetDamageNum(m_iDamage);
		pOwner->DamageParticle()->SetStartLT_UV(StartUV);
		pOwner->DamageParticle()->setSliceUV(sliceUV);

		pOwner->DamageParticle()->SetHitEffectStartUV(HitStartUV);
		pOwner->DamageParticle()->SetHitEffectSliceUV(HitSliceUV);


		pOwner->DamageParticle()->SetMaxParticleCount(m_vecDamageNum.size() + 1);
		pOwner->DamageParticle()->SetAliveCount(m_vecDamageNum.size() + 1);
		
		pOwner->DamageParticle()->SetMaxLifeTime(m_fMaxTime);

	}
}

void CDamageScript::update()
{
	m_fTimer += DT;

	// Delete Obj
	if (m_fTimer >= m_fMaxTime)
	{

		tEventInfo info = {};

		info.eType = EVENT_TYPE::DEREGISTER_OBJ_IN_LAYER;
		info.lParam = (DWORD_PTR)GetOwner();
		info.wParam = (DWORD_PTR)GetOwner()->GetLayerIndex();

		CEventMgr::GetInst()->AddEvent(info);

		GetOwner()->Destroy();
		return;

	}

}

void CDamageScript::lateupdate()
{
	
}





void CDamageScript::OnCollisionEnter(CGameObject* _OtherObject)
{

}

void CDamageScript::OnCollision(CGameObject* _OtherObject)
{

}

void CDamageScript::OnCollisionExit(CGameObject* _OtherObject)
{

}

void CDamageScript::SaveToScene(FILE* _pFile)
{

}

void CDamageScript::LoadFromScene(FILE* _pFile)
{

}

