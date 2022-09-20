#include "pch.h"
#include "CPlayerScript.h"

#include "CMissileScript.h"

#include "CScriptMgr.h"


#include <Engine/CAnimator2D.h>
#include <Engine/CLight2D.h>
#include <Engine/CCollider2D.h>
#include <Engine/CScene.h>
#include <Engine/CLayer.h>
#include <Engine/CSceneMgr.h>
#include <Engine/CPrefab.h>
#include <Engine/CAnimation2D.h>

#include "CRigidBodyScript.h"
#include "CBasicBallScript.h"
#include "CDoubleJumpScript.h"
#include "CSkillScript.h"
#include "CSkillnearScript.h"
#include "CInventoryScript.h"

#include "CSceneSaveLoad.h"
#include "CSceneStartScript.h"
#include "CDamageScript.h"
#include "CLaunchSkillScript.h"
#include "CDieNoticeScript.h"



CPlayerScript::CPlayerScript()
	: CScript((int)SCRIPT_TYPE::PLAYERSCRIPT)
	//, m_pMissilePrefab(nullptr)
	, m_fSpeed(10.f)
	, m_eDir(PLAYER_DIRECTION::RIGHT)
	, m_eCurState(PLAYER_STATE::IDLE)
	, m_ePrevState(PLAYER_STATE::IDLE)
	, m_bOnGround(false)
	, m_iLevel(100)
	, m_fHP(0.f)
	, m_fMP(0.f)
	, m_fEXP(0.f)
	, m_fMaxMP(20000.f)
	, m_fMaxHP(70000.f)
	, m_fMaxEXP(20000.f)

	, m_fMinAttack(10000.f)
	, m_fMaxAttack(20000.f)

{

	SetName(CScriptMgr::GetScriptName(this));


	ClearScriptParam();
	AddScriptParam("PlayerSpeed", SCRIPTPARAM_TYPE::FLOAT, &m_fSpeed);
	AddScriptParam("Direction", SCRIPTPARAM_TYPE::INT, &m_eDir);
	AddScriptParam("PlayerOnGround", SCRIPTPARAM_TYPE::INT, &m_bOnGround);

}

CPlayerScript::CPlayerScript(const CPlayerScript& _origin)
	: CScript((int)SCRIPT_TYPE::PLAYERSCRIPT)
	, m_fSpeed(_origin.m_fSpeed)
	, m_eDir(_origin.m_eDir)
	, m_eCurState(PLAYER_STATE::IDLE)
	, m_ePrevState(PLAYER_STATE::IDLE)
	, m_bOnGround(_origin.m_bOnGround)
	, m_iLevel(_origin.m_iLevel)
	, m_fHP(_origin.m_fHP)
	, m_fMP(_origin.m_fMP)
	, m_fEXP(_origin.m_fEXP)
	, m_fMaxHP(_origin.m_fMaxHP)
	, m_fMaxMP(_origin.m_fMaxMP)
	, m_fMaxEXP(_origin.m_fMaxEXP)

	, m_fMinAttack(_origin.m_fMinAttack)
	, m_fMaxAttack(_origin.m_fMaxAttack)


{
	SetName(CScriptMgr::GetScriptName(this));


	ClearScriptParam();
	AddScriptParam("PlayerSpeed", SCRIPTPARAM_TYPE::FLOAT, &m_fSpeed);
	AddScriptParam("Direction", SCRIPTPARAM_TYPE::INT, &m_eDir);
	AddScriptParam("PlayerOnGround", SCRIPTPARAM_TYPE::INT, &m_bOnGround);

}

CPlayerScript::~CPlayerScript()
{
	CSceneSaveLoad::pMainPlayer = nullptr;

}

void CPlayerScript::Revival()
{
	m_bDie = false;
	m_eCurState = PLAYER_STATE::IDLE;
	m_ePrevState = PLAYER_STATE::IDLE;

	m_fHP = m_fMaxHP;
	m_fMP = m_fMaxMP;


}

void CPlayerScript::start()
{
	//m_pMissilePrefab = CResMgr::GetInst()->FindRes<CPrefab>(L"MissilePrefab");
	m_eDir = PLAYER_DIRECTION::RIGHT;
	m_eCurState = PLAYER_STATE::IDLE;
	m_ePrevState = PLAYER_STATE::IDLE;

	m_bOnGround = false;
	Collider2D()->Reset();

	ClearScriptParam();
	AddScriptParam("PlayerSpeed", SCRIPTPARAM_TYPE::FLOAT, &m_fSpeed);
	AddScriptParam("Direction", SCRIPTPARAM_TYPE::INT, &m_eDir);
	AddScriptParam("PlayerOnGround", SCRIPTPARAM_TYPE::INT, &m_bOnGround);

	m_iHitCnt = 0;
	m_fTimer = 0;
	m_pJumpBgm = CResMgr::GetInst()->Load<CSound>(L"sound\\Player\\Jump.mp3", L"sound\\Player\\Jump.mp3");
	m_pDieBgm = CResMgr::GetInst()->Load<CSound>(L"sound\\Tombstone.mp3", L"sound\\Tombstone.mp3");

	
	CSceneSaveLoad::pMainPlayer = GetOwner();


	if (CSceneMgr::GetInst()->GetCurScene()->GetName() == L"ShipToToyCastle")
	{
		Vec3 vPos = Transform()->GetRelativePos();
		vPos.x = -99.f;
		vPos.y = -365.f;
		Transform()->SetRelativePos(vPos);

	}

	if (CSceneMgr::GetInst()->GetCurScene()->GetName() == L"MapleLoad")
	{

		Vec3 vPos = Transform()->GetRelativePos();
		vPos.x = -600.f;
		vPos.y = -200.f;
		Transform()->SetRelativePos(vPos);

	}



	if (CSceneMgr::GetInst()->GetCurScene()->GetName() == L"MapleLoad")
	{
		m_fHP *= 0.5f;
	}

}

void CPlayerScript::update()
{

	if (m_bD_SkillStart == true)
		m_bD_SkillColltime += DT;

	if (m_bD_SkillColltime >= 3.f)
		m_bD_SkillStart = false;

	m_fMaxMP = 70000.f;

	m_fTimer += DT;
	if (m_fTimer >= 2.f)
	{
		m_fTimer = 0.f;
		m_iHitCnt = 0;
	}

	m_vPrevPos = GetOwner()->Transform()->GetRelativePos();

	Update_Skill();

	Update_State();
	Update_Move();
	Update_Gravity();
	Update_Animation();
	Update_Inventory();

	Update_Bgm();


	m_ePrevState = m_eCurState;
	m_ePrevDir = m_eDir;
	m_ePrevAttState = m_eCurAttState;

}




void CPlayerScript::lateupdate()
{

}

void CPlayerScript::OnCollisionEnter(CGameObject* _OtherObject)
{
	if (m_bDie == true)
		return;

	// Player <-> Monster Attack (Ball)
	 CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();
	CBasicBallScript* pBallScript = (CBasicBallScript*)_OtherObject->GetScriptByName(L"CBasicBallScript");
	if (pBallScript != nullptr)
	{
		m_iHitCnt++;
		float OffSetY = 52.f;
		if (m_iHitCnt >= 1)
			OffSetY = 32.f;

		pBallScript->SetHit(true); // Next Frame - Delete Ball(Arrow)
		// Damage 
		int MaxAttack = pBallScript->GetMaxAttack();
		int MinAttack = pBallScript->GetMinAttack();

		int randNum = rand() % (MaxAttack - MinAttack);
		int damage = MinAttack + randNum;

		SubHP(damage);
		if (m_fHP <= 0.f)
		{
			m_bDie = true;
			m_eCurState = PLAYER_STATE::DEAD;
			Vec3 vPos = GetOwner()->Transform()->GetRelativePos();
			vPos.y += 300.f;
			GetOwner()->Transform()->SetRelativePos(vPos);
			m_pDieBgm->Play(1, 0.7f, true);



			// Die notice 경고창 
			CPrefab* pPrefab = CSceneSaveLoad::pSceneMgrScript->GetPrefab(L"DieNotice");
			CGameObject* pObj = pPrefab->Instantiate();
			CDieNoticeScript* pDieNotice = (CDieNoticeScript*)pObj->GetScriptByName(L"CDieNoticeScript");
			pDieNotice->start();

			pCurScene->AddObject(pObj, L"UI");


		}

		// Damage 
		CPrefab* pPrefab = CSceneSaveLoad::pSceneMgrScript->GetPrefab(L"Damage");

		CGameObject* pDamageObj = pPrefab->Instantiate();
		Vec3 vPos = GetOwner()->Transform()->GetRelativePos();
		vPos.y += (m_iHitCnt * OffSetY);

		pDamageObj->Transform()->SetRelativePos(vPos);
		CDamageScript* pDamageScript = (CDamageScript*)CScriptMgr::GetScript(L"CDamageScript");
		pDamageObj->AddComponent((CComponent*)pDamageScript);
		pDamageScript->Init(DAMAGE_TYPE::MISS, damage, 2.f);
		pCurScene->AddObject(pDamageObj, L"Damage");

		// Hit Animation (Ball) 
		if(_OtherObject->GetName() == L"GiganticBall")
			pPrefab = CSceneSaveLoad::pSceneMgrScript->GetPrefab(L"GiganticBallHit");
		if(_OtherObject->GetName() == L"TanatosBall")
			pPrefab = CSceneSaveLoad::pSceneMgrScript->GetPrefab(L"TanatosBallHit");

		// CLaunchSkillScript : 시간 없어서 그냥 여기다가 응용해서 만들었음.. 
		CGameObject* pHitObj = pPrefab->Instantiate();
		CLaunchSkillScript* pLaunchSkillScript = (CLaunchSkillScript*)CScriptMgr::GetScript(L"CLaunchSkillScript");
		pLaunchSkillScript->SetHit(true);

		pHitObj->AddComponent((CComponent*)pLaunchSkillScript);
		vPos = GetOwner()->Transform()->GetRelativePos();
		vPos.z -= 10.f;
		pHitObj->Transform()->SetRelativePos(vPos);


		pCurScene->AddObject(pHitObj,L"SubSkill_1");


	}


	// Player <-> BossMob
	if (_OtherObject->GetName() == L"PapulatusSkillHit")
	{
		m_iHitCnt++;
		float OffSetY = 52.f;
		if (m_iHitCnt >= 1)
			OffSetY = 32.f;

		// Damage 
		int MaxAttack = 1000.f;
		int MinAttack = 2000.f;

		int randNum = rand() % (MaxAttack - MinAttack);
		int damage = MinAttack + randNum;


		SubHP(damage);
		if (m_fHP <= 0.f)
		{
			m_bDie = true;
			m_eCurState = PLAYER_STATE::DEAD;
			Vec3 vPos = GetOwner()->Transform()->GetRelativePos();
			vPos.y += 300.f;
			GetOwner()->Transform()->SetRelativePos(vPos);



			// Die notice 경고창 
			CPrefab* pPrefab = CSceneSaveLoad::pSceneMgrScript->GetPrefab(L"DieNotice");
			CGameObject* pObj = pPrefab->Instantiate();
			CDieNoticeScript* pDieNotice = (CDieNoticeScript*)pObj->GetScriptByName(L"CDieNoticeScript");
			pDieNotice->start();

			pCurScene->AddObject(pObj, L"UI");


		}
		// Damage 
		CPrefab* pPrefab = CSceneSaveLoad::pSceneMgrScript->GetPrefab(L"Damage");

		CGameObject* pDamageObj = pPrefab->Instantiate();
		Vec3 vPos = GetOwner()->Transform()->GetRelativePos();
		vPos.y += (m_iHitCnt * OffSetY);

		pDamageObj->Transform()->SetRelativePos(vPos);
		CDamageScript* pDamageScript = (CDamageScript*)CScriptMgr::GetScript(L"CDamageScript");
		pDamageObj->AddComponent((CComponent*)pDamageScript);
		pDamageScript->Init(DAMAGE_TYPE::CRITICAL_3, damage, 2.f);
		pCurScene->AddObject(pDamageObj, L"Damage");

	}


}

void CPlayerScript::OnCollision(CGameObject* _OtherObject)
{
	if (m_bDie == true)
		return;
	// Boss <-> Player 
	 CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();
	CLaunchSkillScript* pLaunchScript = (CLaunchSkillScript*)_OtherObject->GetScriptByName(L"CLaunchSkillScript");
	if (pLaunchScript != nullptr)
	{
		if (_OtherObject->GetName() == L"TimeOutSkill")
		{
			if (pLaunchScript->GetTime() >= 1.9f)
			{
				if (pLaunchScript->GetATtackSuccess() == false)
				{
					m_iHitCnt++;
					float OffSetY = 52.f;
					if (m_iHitCnt >= 1)
						OffSetY = 32.f;

					// Damage 
					int MaxAttack = 30000.f;
					int MinAttack = 25000.f;

					int randNum = rand() % (MaxAttack - MinAttack);
					int damage = MinAttack + randNum;


					SubHP(damage);
					if (m_fHP <= 0.f)
					{
						m_bDie = true;
						m_eCurState = PLAYER_STATE::DEAD;
						Vec3 vPos = GetOwner()->Transform()->GetRelativePos();
						vPos.y += 300.f;
						GetOwner()->Transform()->SetRelativePos(vPos);


						// Die notice 경고창 
						CPrefab* pPrefab = CSceneSaveLoad::pSceneMgrScript->GetPrefab(L"DieNotice");
						CGameObject* pObj = pPrefab->Instantiate();
						CDieNoticeScript* pDieNotice = (CDieNoticeScript*)pObj->GetScriptByName(L"CDieNoticeScript");
						pDieNotice->start();

						pCurScene->AddObject(pObj, L"UI");


					}
					// Damage 
					CPrefab* pPrefab = CSceneSaveLoad::pSceneMgrScript->GetPrefab(L"Damage");

					CGameObject* pDamageObj = pPrefab->Instantiate();
					Vec3 vPos = GetOwner()->Transform()->GetRelativePos();
					vPos.y += (m_iHitCnt * OffSetY);

					pDamageObj->Transform()->SetRelativePos(vPos);
					CDamageScript* pDamageScript = (CDamageScript*)CScriptMgr::GetScript(L"CDamageScript");
					pDamageObj->AddComponent((CComponent*)pDamageScript);
					pDamageScript->Init(DAMAGE_TYPE::CRITICAL_3, damage, 2.f);
					pCurScene->AddObject(pDamageObj, L"Damage");
					
					pLaunchScript->SetAttackSuccess(true);

				}
				
			}
		}
	}


}

void CPlayerScript::OnCollisionExit(CGameObject* _OtherObject)
{
}




void CPlayerScript::Update_Skill()
{
	if (m_bDie == true)
		return;

	/*
	* Skill Test
	*/
	if (KEY_TAP(KEY::A))
	{
		CGameObject* ptest = new CGameObject;
		ptest->SetName(L"SkillTest");
		ptest->AddComponent(new CTransform);
		ptest->Transform()->SetRelativePos(GetOwner()->Transform()->GetRelativePos());


		CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();

		CSkillScript* pScript = (CSkillScript*)CScriptMgr::GetScript(L"CSkillScript");
		ptest->AddComponent((CComponent*)pScript);

		CPrefab* prepare = CSceneSaveLoad::pSceneMgrScript->GetPrefab(L"StormPrepare");
		CPrefab* KeyDown = CSceneSaveLoad::pSceneMgrScript->GetPrefab(L"Storm");
		CPrefab* End = CSceneSaveLoad::pSceneMgrScript->GetPrefab(L"StormEnd");


		pScript->SetKey(KEY::A);
		pScript->SetSkillObjByPrefab(prepare, KeyDown, End);
		pScript->SetName(L"stormSkill");
		pScript->SetUser(GetOwner());
		pScript->SetBallName(L"prefab\\BasicBall.pref");
		pScript->SetBallMoveType(BALLMOVE_TYPE::LINEAR);
		pScript->SetOffsetY(true);
		pScript->start();


		SubMP(50.f);

		pCurScene = CSceneMgr::GetInst()->GetCurScene();
		pCurScene->AddObject(ptest, L"Skill");


	}

	if (KEY_TAP(KEY::S))
	{
		CGameObject* ptest = new CGameObject;
		ptest->SetName(L"ArrowFlatter");
		ptest->AddComponent(new CTransform);
		ptest->Transform()->SetRelativePos(GetOwner()->Transform()->GetRelativePos());


		CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();

		CSkillScript* pScript = (CSkillScript*)CScriptMgr::GetScript(L"CSkillScript");
		ptest->AddComponent((CComponent*)pScript);


		CPrefab* prepare = CSceneSaveLoad::pSceneMgrScript->GetPrefab(L"ArrowFlatterPrepare");
		CPrefab* KeyDown = CSceneSaveLoad::pSceneMgrScript->GetPrefab(L"ArrowFlatter");
		CPrefab* End = CSceneSaveLoad::pSceneMgrScript->GetPrefab(L"ArrowFlatterEnd");


		pScript->SetKey(KEY::S);
		pScript->SetSkillObjByPrefab(prepare, KeyDown, End);
		pScript->SetName(L"ArrowFlatterSkill");
		pScript->SetUser(GetOwner());
		pScript->SetBallName(L"prefab\\ArrowFlatterBall.pref");
		pScript->SetBallMoveType(BALLMOVE_TYPE::DIAGONAL);
		pScript->SetOffsetY(false);
		pScript->start();

		SubMP(50.f);

		pCurScene = CSceneMgr::GetInst()->GetCurScene();
		pCurScene->AddObject(ptest, L"Skill");



	}
	if (KEY_TAP(KEY::LCTRL))
	{

		CPrefab* Skillpref = CSceneSaveLoad::pSceneMgrScript->GetPrefab(L"FinalBlow");


		CGameObject* pFinalBlow = Skillpref->Instantiate();
		pFinalBlow->Transform()->SetRelativePos(GetOwner()->Transform()->GetRelativePos());
		pFinalBlow->SetName(L"FinalBlow");

		CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();

		CSkillnearScript* pScript = (CSkillnearScript*)CScriptMgr::GetScript(L"CSkillnearScript");
		pScript->start();
		pScript->SetHitMaxMonsterCnt(1);
		pFinalBlow->AddComponent((CComponent*)pScript);

		pScript->SetKey(KEY::LCTRL);
		//pScript->SetSkillObjByPrefab(m_prefFinalBlow, L"FinalBlow");
		pScript->SetUser(GetOwner());
		SubMP(30.f);

		pCurScene = CSceneMgr::GetInst()->GetCurScene();
		pCurScene->AddObject(pFinalBlow, L"SubSkill_1");



	}

	if (KEY_TAP(KEY::D))
	{
		if (m_bD_SkillStart == false)
		{
			m_bD_SkillStart = true;
			m_bD_SkillColltime = 0.f;


			CPrefab* Skillpref = CSceneSaveLoad::pSceneMgrScript->GetPrefab(L"LargeSkill");


			CGameObject* pLargeSkill = Skillpref->Instantiate();
			Vec3 vPos = GetOwner()->Transform()->GetRelativePos();
			vPos.y += 137.f;
			pLargeSkill->Transform()->SetRelativePos(vPos);
			pLargeSkill->SetName(L"LargeSkill");

			CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();

			CSkillnearScript* pScript = (CSkillnearScript*)CScriptMgr::GetScript(L"CSkillnearScript");
			pScript->start();
			pScript->SetHitMaxMonsterCnt(6);


			pLargeSkill->AddComponent((CComponent*)pScript);

			pScript->SetKey(KEY::D);
			//pScript->SetSkillObjByPrefab(m_prefFinalBlow, L"FinalBlow");
			pScript->SetUser(GetOwner());
			SubMP(50.f);

			pCurScene = CSceneMgr::GetInst()->GetCurScene();
			pCurScene->AddObject(pLargeSkill, L"SubSkill_1");
		}
		

	}




	if (KEY_PRESSED(KEY::A) || KEY_PRESSED(KEY::S))
	{
		SubMP(3.f);

	}

}



void CPlayerScript::Update_State()
{
	if (m_bDie == true)
		return;


	CRigidBodyScript* pRigid = (CRigidBodyScript*)GetOwner()->GetScriptByName(L"CRigidBodyScript");



	if (nullptr != pRigid)
	{
		float fSpeed = pRigid->GetVelocity().Length();
		

		if (CSceneMgr::GetInst()->GetCurScene()->GetName() != L"ClockTowerOriginFloor")
		{
	

			if (m_bOnGround)
			{

				Vec3 MaxVelocity = pRigid->GetMaxVelocity();
				MaxVelocity.x = 150.f;
				pRigid->SetMaxVelocity(MaxVelocity);

				m_eCurState = PLAYER_STATE::IDLE;



			}

			if (0.f == fSpeed && m_bOnGround
				&& m_ePrevState != PLAYER_STATE::ATTACK
				&& m_ePrevState != PLAYER_STATE::ALERT)
			{
				m_eCurState = PLAYER_STATE::IDLE;
				m_eDir = m_ePrevDir;


			}

			if (m_ePrevState == PLAYER_STATE::JUMP)
			{
				if (m_bOnGround == TRUE)
				{
					m_eCurState = PLAYER_STATE::IDLE;
					m_eDir = m_ePrevDir; // TEST 


				}
			}
		}
		else
		{
			if ((m_bOnGround && m_ePrevState == PLAYER_STATE::DOUBLE_JUMP) ||
			(m_bOnGround && m_ePrevState == PLAYER_STATE::ROPE_MOVE_DOWN) ||
			(m_bOnGround && m_ePrevState == PLAYER_STATE::ROPE_MOVE_UP) ||
			(m_bOnGround && m_ePrevState == PLAYER_STATE::ROPE_STOP))
			{

				Vec3 MaxVelocity = pRigid->GetMaxVelocity();
				MaxVelocity.x = 150.f;
				pRigid->SetMaxVelocity(MaxVelocity);

				m_eCurState = PLAYER_STATE::IDLE;



			}

			if (0.f == fSpeed && m_bOnGround
				&& m_ePrevState != PLAYER_STATE::ATTACK
				&& m_ePrevState != PLAYER_STATE::ALERT)
			{
				m_eCurState = PLAYER_STATE::IDLE;
				m_eDir = m_ePrevDir;


			}

			if (m_ePrevState == PLAYER_STATE::JUMP)
			{
				if (m_bOnGround == TRUE)
				{
					m_eCurState = PLAYER_STATE::IDLE;
					m_eDir = m_ePrevDir; // TEST 


				}
			}
		}

	}


	if (KEY_TAP(KEY::SPACE))
	{
		if (m_eCurState != PLAYER_STATE::JUMP && m_eCurState != PLAYER_STATE::DOUBLE_JUMP)
		{
			m_eCurState = PLAYER_STATE::JUMP;
		}
		else if (m_eCurState == PLAYER_STATE::JUMP)
		{
			m_eCurState = PLAYER_STATE::DOUBLE_JUMP;

			// Double Jump - Add Child Obj ( Eff )

			RegisterDoubleJumpEff();
			m_fMP -= 100.f;
		}
		

		if (m_ePrevState == PLAYER_STATE::PRONE)
		{

			Vec3 vPos = GetOwner()->Transform()->GetRelativePos();
			vPos.y -= 10.f;
			GetOwner()->Transform()->SetRelativePos(vPos);
			m_eCurState = PLAYER_STATE::JUMP;
		
		}

	}
	if (m_eCurState == PLAYER_STATE::JUMP || m_eCurState == PLAYER_STATE::DOUBLE_JUMP)
		return;


	if (m_eCurState != PLAYER_STATE::JUMP && m_eCurState != PLAYER_STATE::DOUBLE_JUMP &&
		m_eCurState != PLAYER_STATE::ROPE_MOVE_DOWN && m_eCurState != PLAYER_STATE::ROPE_MOVE_UP 
		&& m_eCurState != PLAYER_STATE::ROPE_STOP)
	{
		if (KEY_TAP(KEY::LEFT))
		{
			m_eCurState = PLAYER_STATE::WALK;
			m_eDir = PLAYER_DIRECTION::LEFT;
		}
		if (KEY_TAP(KEY::RIGHT))
		{
			m_eCurState = PLAYER_STATE::WALK;
			m_eDir = PLAYER_DIRECTION::RIGHT;
		}
		if (KEY_TAP(KEY::DOWN))
		{
			m_eCurState = PLAYER_STATE::PRONE;

		}
	}




	// KEY_PRESSED 
	if (m_ePrevState == PLAYER_STATE::JUMP || m_ePrevState == PLAYER_STATE::DOUBLE_JUMP )
	{
		if (KEY_PRESSED(KEY::LEFT))
		{
			if (m_bOnGround)
				m_eCurState = PLAYER_STATE::WALK;

				m_eDir = PLAYER_DIRECTION::LEFT;


		}

		if (KEY_PRESSED(KEY::RIGHT))
		{
			if (m_bOnGround)
				m_eCurState = PLAYER_STATE::WALK;

			m_eDir = PLAYER_DIRECTION::RIGHT;
	

		}
	}


	if (m_eCurState == PLAYER_STATE::ROPE_MOVE_UP ||
		m_eCurState == PLAYER_STATE::ROPE_MOVE_DOWN ||
		m_eCurState == PLAYER_STATE::ROPE_STOP)
	{
		if (KEY_PRESSED(KEY::LEFT))
		{
			if (m_bOnGround)
				m_eCurState = PLAYER_STATE::WALK;

			m_eDir = PLAYER_DIRECTION::LEFT;
			if (KEY_TAP(KEY::SPACE))
				m_eCurState = PLAYER_STATE::JUMP;
 
		}

		if (KEY_PRESSED(KEY::RIGHT))
		{
			if (m_bOnGround)
				m_eCurState = PLAYER_STATE::WALK;

			m_eDir = PLAYER_DIRECTION::RIGHT;

			if (KEY_TAP(KEY::SPACE))
				m_eCurState = PLAYER_STATE::JUMP;

		}

	}

	if ((m_ePrevState == PLAYER_STATE::IDLE
		|| m_ePrevState == PLAYER_STATE::ATTACK
		|| m_ePrevState == PLAYER_STATE::WALK
		|| m_ePrevState == PLAYER_STATE::PRONE)
		&& (m_eCurState != PLAYER_STATE::ROPE_MOVE_DOWN
			&& m_eCurState != PLAYER_STATE::ROPE_MOVE_UP
			&& m_eCurState != PLAYER_STATE::ROPE_STOP))
	{
		if (m_eCurState != PLAYER_STATE::JUMP)
		{
			if (KEY_PRESSED(KEY::DOWN))
			{
  				m_eCurState = PLAYER_STATE::PRONE;
			}

		}
		
	}

	Vec3 vObjPos = GetOwner()->Collider2D()->GetWorldPos();
	Vec2 vObjScale = GetOwner()->Collider2D()->GetOffsetScale();
	float vObj_SouthLine_y = vObjPos.y - (vObjScale.y / 2);

	if ((m_fGroundNorthY - 10 > vObj_SouthLine_y) && m_bOnAngleGround == false)
	{
		if (m_ePrevState == PLAYER_STATE::PRONE)
		{
			m_eCurState = PLAYER_STATE::JUMP;
			m_bOnGround = false;


		}
	/*	if(m_eCurState != PLAYER_STATE::ROPE_MOVE_DOWN 
			&& m_eCurState != PLAYER_STATE::IDLE
			&& m_eCurState != PLAYER_STATE::ROPE_STOP
			&& m_eCurState != PLAYER_STATE::ROPE_MOVE_UP)
	*/
	}



	// KEY_AWAY
	if (m_eCurState == PLAYER_STATE::WALK)
	{
		if (KEY_AWAY(KEY::LEFT))
		{
			m_eCurState = PLAYER_STATE::IDLE;
		}
		if (KEY_AWAY(KEY::RIGHT))
		{
			m_eCurState = PLAYER_STATE::IDLE;
		}

	}
	if (m_eCurState == PLAYER_STATE::PRONE)
	{
		if (KEY_AWAY(KEY::DOWN))
		{
			m_eCurState = PLAYER_STATE::IDLE;

		}
	}



	if (m_eCurState == PLAYER_STATE::ATTACK)
	{

		vector<CGameObject*> vecChild = GetOwner()->GetChild();
		for (int i = 0; i < vecChild.size(); ++i)
		{
			if (vecChild[i]->GetName() == L"Body")
			{
				CAnimator2D* pAnimator2D = vecChild[i]->Animator2D();
				CAnimation2D* pAnim = pAnimator2D->GetCurAnim();
				if (pAnim->IsFinish() == true)
				{
					m_eCurState = PLAYER_STATE::ALERT;
					ResetAnim(PLAYER_STATE::ATTACK, GetOwner());

				}
				break;
			}
		}
	}


	if (m_eCurState != PLAYER_STATE::JUMP && m_eCurState != PLAYER_STATE::DOUBLE_JUMP)
	{
		if (KEY_TAP(KEY::LCTRL) || KEY_PRESSED(KEY::LCTRL))
		{
			m_eCurState = PLAYER_STATE::ATTACK;
			m_eCurAttState = PLAYER_ATTACK_STATE::NORMAL_ATT_1;
		}
		if (KEY_TAP(KEY::A))
		{
			m_eCurState = PLAYER_STATE::ATTACK;
			m_eCurAttState = PLAYER_ATTACK_STATE::SKILL_ATT_1;
		}
		if (KEY_TAP(KEY::S))
		{
			m_eCurState = PLAYER_STATE::ATTACK;
			m_eCurAttState = PLAYER_ATTACK_STATE::SKILL_ATT_2;

		}

		if (KEY_AWAY(KEY::LCTRL))
		{
			m_eCurState = PLAYER_STATE::ALERT;

		}
		if (KEY_AWAY(KEY::A))
		{
			m_eCurState = PLAYER_STATE::ALERT;
		}
		if (KEY_AWAY(KEY::S))
		{
			m_eCurState = PLAYER_STATE::ALERT;

		}

	}


	if ((m_eCurState != PLAYER_STATE::JUMP && m_eCurState != PLAYER_STATE::DOUBLE_JUMP)
		|| m_eCurState == PLAYER_STATE::ALERT && m_eCurState != PLAYER_STATE::ATTACK
		|| m_eCurState == PLAYER_STATE::IDLE && m_eCurState != PLAYER_STATE::ATTACK
		|| m_eCurState == PLAYER_STATE::WALK && m_eCurState != PLAYER_STATE::ATTACK)
	{
		if (m_eCurState != PLAYER_STATE::ROPE_MOVE_DOWN && m_eCurState != PLAYER_STATE::ROPE_MOVE_UP
			&& m_eCurState != PLAYER_STATE::ROPE_STOP)
		{
			if (KEY_PRESSED(KEY::RIGHT) || KEY_PRESSED(KEY::LEFT))
			{
				m_eCurState = PLAYER_STATE::WALK;
			}

		}
		
	}



	if (m_eCurState != PLAYER_STATE::JUMP && m_eCurState != PLAYER_STATE::DOUBLE_JUMP)
	{
		if (KEY_PRESSED(KEY::A))
		{
			m_eCurState = PLAYER_STATE::ATTACK;
			m_eCurAttState = PLAYER_ATTACK_STATE::SKILL_ATT_1;

		}

		if (KEY_PRESSED(KEY::S))
		{
			m_eCurState = PLAYER_STATE::ATTACK;
			m_eCurAttState = PLAYER_ATTACK_STATE::SKILL_ATT_2;

		}

	}

	//if (m_eCurState == PLAYER_STATE::PRONE)
	//{
	//	if (KEY_PRESSED(KEY::DOWN) && KEY_TAP(KEY::SPACE))
	//	{
	//		m_eCurState = PLAYER_STATE::JUMP;

	//		CRigidBodyScript* pRigid = (CRigidBodyScript*)GetOwner()->GetScriptByName(L"CRigidBodyScript");
	//		pRigid->AddVelocity(Vec3(0.f, -100.f, 0.f)); // 앞으로 확감
	//	}
	//}


}

void CPlayerScript::Update_Move()
{


	if (KEY_TAP(KEY::ENTER))
	{
		Vec3 vPos = GetOwner()->Transform()->GetRelativePos();
		GetOwner()->Transform()->SetRelativePos(Vec3(0.f, 0.f, vPos.z));
	}





	CRigidBodyScript* pRigid = (CRigidBodyScript*)GetOwner()->GetScriptByName(L"CRigidBodyScript");

	if (m_bDie == true)
	{
		Vec3 MaxVelocity = pRigid->GetMaxVelocity();
		MaxVelocity.x = 150.f;
		pRigid->SetMaxVelocity(MaxVelocity);		
		return;
	}


	if (nullptr != pRigid)
	{
		// KEY_TAP
		if (KEY_TAP(KEY::LEFT))
		{
			m_eDir = PLAYER_DIRECTION::LEFT;
			if (m_eCurState != PLAYER_STATE::JUMP && m_eCurState != PLAYER_STATE::DOUBLE_JUMP)
				pRigid->AddVelocity(Vec3(-120.f, 0.f, 0.f));		// 속도 즉시 증가 

		}
		if (KEY_TAP(KEY::RIGHT))
		{
			m_eDir = PLAYER_DIRECTION::RIGHT;
			if (m_eCurState != PLAYER_STATE::JUMP && m_eCurState != PLAYER_STATE::DOUBLE_JUMP)
				pRigid->AddVelocity(Vec3(120.f, 0.f, 0.f));

		}


		if (KEY_TAP(KEY::SPACE))
		{
			if (nullptr != pRigid)
			{
				if (m_ePrevState == PLAYER_STATE::ROPE_STOP ||
					m_ePrevState == PLAYER_STATE::ROPE_MOVE_UP ||
					m_ePrevState == PLAYER_STATE::ROPE_MOVE_DOWN)
				{
					if (m_eDir == PLAYER_DIRECTION::RIGHT)
					{
						pRigid->AddVelocity(Vec3(200.f, 100.f, 0.f)); // 앞으로 확감
						Vec3 MaxVelocity = pRigid->GetMaxVelocity();
						MaxVelocity.x = 200.f;
						pRigid->SetMaxVelocity(MaxVelocity);
					}
					else if (m_eDir == PLAYER_DIRECTION::LEFT)
					{
						pRigid->AddVelocity(Vec3(-200.f, 100.f, 0.f)); // 앞으로 확감
						Vec3 MaxVelocity = pRigid->GetMaxVelocity();
						MaxVelocity.x = -200.f;
						pRigid->SetMaxVelocity(MaxVelocity);
					}
				}
				if (m_eCurState == PLAYER_STATE::JUMP)
				{
					if(m_ePrevState == PLAYER_STATE::PRONE)
 						pRigid->AddVelocity(Vec3(0.f, -400.f, 0.f));
					else 
						pRigid->AddVelocity(Vec3(0.f, 400.f, 0.f));

				}
				else if (m_eCurState == PLAYER_STATE::DOUBLE_JUMP &&
					m_ePrevState != PLAYER_STATE::DOUBLE_JUMP)
				{
					if (m_eDir == PLAYER_DIRECTION::RIGHT)
					{
						pRigid->AddVelocity(Vec3(400.f, 100.f, 0.f)); // 앞으로 확감
						Vec3 MaxVelocity = pRigid->GetMaxVelocity();
						MaxVelocity.x = 400.f;
						pRigid->SetMaxVelocity(MaxVelocity);
					}
					else if (m_eDir == PLAYER_DIRECTION::LEFT)
					{
						pRigid->AddVelocity(Vec3(-400.f, 100.f, 0.f)); // 앞으로 확감
						Vec3 MaxVelocity = pRigid->GetMaxVelocity();
						MaxVelocity.x = -400.f;
						pRigid->SetMaxVelocity(MaxVelocity);
					}

				}
			}
		}

		if(m_eCurState != PLAYER_STATE::WALK
			&& m_eCurState != PLAYER_STATE::JUMP
			&& m_eCurState != PLAYER_STATE::DOUBLE_JUMP)
		{
			if (KEY_TAP(KEY::DOWN))
				pRigid->SetVelocity(Vec3(0.f, 0.f, 0.f));

		}

		// KEY_PRESSED 
		if (m_eCurState != PLAYER_STATE::JUMP && m_eCurState != PLAYER_STATE::DOUBLE_JUMP)
		{
			if (KEY_PRESSED(KEY::LEFT))
			{
				pRigid->AddForce(Vec3(-250.f, 0.f, 0.f));
			}

			if (KEY_PRESSED(KEY::RIGHT))
			{
				pRigid->AddForce(Vec3(250.f, 0.f, 0.f));
			}
		}


	}



}

void CPlayerScript::Update_Animation()
{
	CGameObject* pOwner = GetOwner();
	CAnimator2D* pAnimator2D = pOwner->Animator2D();
	CAnimation2D* pAnim = pAnimator2D->GetCurAnim();

	switch (m_eCurState)
	{
	case PLAYER_STATE::IDLE:
	{
		if (PLAYER_DIRECTION::LEFT == m_eDir)
		{
			pAnimator2D->Play(L"STAND1_LEFT", true);
		}
		else if (PLAYER_DIRECTION::RIGHT == m_eDir)
		{
			pAnimator2D->Play(L"STAND1_RIGHT", true);
		}
	}
	break;
	case PLAYER_STATE::WALK:
	{
		if (PLAYER_DIRECTION::LEFT == m_eDir)
		{
			pAnimator2D->Play(L"WALK1_LEFT", true);
		}
		else if (PLAYER_DIRECTION::RIGHT == m_eDir)
		{
			pAnimator2D->Play(L"WALK1_RIGHT", true);
		}
	}
	break;
	case PLAYER_STATE::JUMP:
	{
		if (PLAYER_DIRECTION::LEFT == m_eDir)
		{
			pAnimator2D->Play(L"JUMP_LEFT", true);
		}
		else if (PLAYER_DIRECTION::RIGHT == m_eDir)
		{
			pAnimator2D->Play(L"JUMP_RIGHT", true);
		}
	}

	break;
	case PLAYER_STATE::PRONE:
	{
		if (PLAYER_DIRECTION::LEFT == m_eDir)
		{
			pAnimator2D->Play(L"PRONE_LEFT", true);
		}
		else if (PLAYER_DIRECTION::RIGHT == m_eDir)
		{
			pAnimator2D->Play(L"PRONE_RIGHT", true);
		}
	}
	break;

	case PLAYER_STATE::ATTACK:
	{
		if (m_eCurAttState == PLAYER_ATTACK_STATE::NORMAL_ATT_1)
		{
			if (PLAYER_DIRECTION::LEFT == m_eDir)
			{
				pAnimator2D->Play(L"SWINGT1_LEFT", FALSE);
				if (pAnimator2D->GetCurAnim()->IsFinish() == true)
					pAnimator2D->Play(L"SWINGT1_LEFT", true);

			}
			else if (PLAYER_DIRECTION::RIGHT == m_eDir)
			{
				pAnimator2D->Play(L"SWINGT1_RIGHT", true);
			}
		}

		if (m_eCurAttState == PLAYER_ATTACK_STATE::SKILL_ATT_1)
		{
			if (PLAYER_DIRECTION::LEFT == m_eDir)
			{
				pAnimator2D->Play(L"SHOOT1_LEFT", true);
			}
			else if (PLAYER_DIRECTION::RIGHT == m_eDir)
			{
				pAnimator2D->Play(L"SHOOT1_RIGHT", true);
			}
		}


		if (m_eCurAttState == PLAYER_ATTACK_STATE::SKILL_ATT_2)
		{
			if (PLAYER_DIRECTION::LEFT == m_eDir)
			{
				pAnimator2D->Play(L"SHOOTF_LEFT", true);
			}
			else if (PLAYER_DIRECTION::RIGHT == m_eDir)
			{
				pAnimator2D->Play(L"SHOOTF_RIGHT", true);
			}
		}
	}
	break;
	case PLAYER_STATE::ALERT:
	{
		if (PLAYER_DIRECTION::LEFT == m_eDir)
		{

			pAnimator2D->Play(L"ALERT_LEFT", true);
		}
		else if (PLAYER_DIRECTION::RIGHT == m_eDir)
		{
			pAnimator2D->Play(L"ALERT_RIGHT", true);
		}
	}
	break;
	case PLAYER_STATE::ROPE_MOVE_UP:
	case PLAYER_STATE::ROPE_MOVE_DOWN:
	{
		if (PLAYER_DIRECTION::LEFT == m_eDir)
		{

			pAnimator2D->Play(L"ROPE", true);
		}
		else if (PLAYER_DIRECTION::RIGHT == m_eDir)
		{
			pAnimator2D->Play(L"ROPE", true);
		}
	}
		break;
	case PLAYER_STATE::ROPE_STOP:

	{
		if (PLAYER_DIRECTION::LEFT == m_eDir)
		{

			pAnimator2D->Play(L"ROPE_STOP", true);
		}
		else if (PLAYER_DIRECTION::RIGHT == m_eDir)
		{
			pAnimator2D->Play(L"ROPE_STOP", true);
		}
	}
	break;



	case PLAYER_STATE::DEAD:
	{
		if (PLAYER_DIRECTION::LEFT == m_eDir)
		{

			pAnimator2D->Play(L"DIE_LEFT", true);
		}
		else if (PLAYER_DIRECTION::RIGHT == m_eDir)
		{
			pAnimator2D->Play(L"DIE_RIGHT", true);
		}
	}

		break;


	}

	//vector<CGameObject*> vecChild = GetOwner()->GetChild();
	//for (int i = 0; i < vecChild.size(); ++i)
	//{
	//	if (vecChild[i]->Animator2D() == nullptr)
	//		continue;
	//	CAnimator2D* pAnimator2D = vecChild[i]->Animator2D();

	//	Update_Animation(vecChild[i], L"Head");
	//	Update_Animation(vecChild[i], L"Body");
	//	Update_Animation(vecChild[i], L"Arm");

	//}
}



void CPlayerScript::RegisterDoubleJumpEff()
{
	if (m_bDie == true)
		return;

	Ptr<CSound> pDragBgm = CResMgr::GetInst()->Load<CSound>(L"sound\\DoubleJump.mp3", L"sound\\DoubleJump.mp3");
	pDragBgm->Play(1, 1.f, true);


	// ** m_pDbJumpPrefab ** 
	// Prefab 파일에 저장된 gameObject 를 읽어서 해당 Layer 에 포함한다. 
	CPrefab* DoubleJumpPref = CSceneSaveLoad::pSceneMgrScript->GetPrefab(L"DoubleJumpEffect");

	CGameObject* NewObj = DoubleJumpPref->Instantiate();
	CDoubleJumpScript* pScript = (CDoubleJumpScript*)CScriptMgr::GetScript(L"CDoubleJumpScript");

	if (m_eDir == PLAYER_DIRECTION::LEFT)
	{
		NewObj->Animator2D()->Play(L"Move_Left", false);

	}
	else if (m_eDir == PLAYER_DIRECTION::RIGHT)
	{
		NewObj->Animator2D()->Play(L"Move_Right", false);
	}

	NewObj->AddComponent((CComponent*)pScript);
	CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();
	pCurScene->AddObject(NewObj, L"Player");
	CSceneMgr::GetInst()->AddChild(GetOwner(), NewObj);


	// ** m_pDbJumpSubPrefab ** 
	// Prefab 파일에 저장된 gameObject 를 읽어서 해당 Layer 에 포함한다. 
	CPrefab* DoubleJumpSubPref = CSceneSaveLoad::pSceneMgrScript->GetPrefab(L"DoubleJumpSubEffect");
	NewObj = DoubleJumpSubPref->Instantiate();

	Vec3 vPos = GetOwner()->Transform()->GetRelativePos();

	pScript = (CDoubleJumpScript*)CScriptMgr::GetScript(L"CDoubleJumpScript");

	if (m_eDir == PLAYER_DIRECTION::LEFT)
	{
		vPos.x += 50.f;
		vPos.y += 20.f;
		NewObj->Animator2D()->Play(L"Move_Left", false);

	}
	else if (m_eDir == PLAYER_DIRECTION::RIGHT)
	{
		vPos.y += 20.f;
		vPos.x -= 50.f;
		NewObj->Animator2D()->Play(L"Move_Right", false);
	}
	NewObj->Transform()->SetRelativePos(vPos);
	NewObj->AddComponent((CComponent*)pScript);
	pCurScene = CSceneMgr::GetInst()->GetCurScene();
	pCurScene->AddObject(NewObj, L"Skill");

}

void CPlayerScript::ResetAnim(PLAYER_STATE _eState, CGameObject* _pObj)
{
	if (_eState != PLAYER_STATE::ATTACK)
		return;

	vector<CGameObject*> vecChild = _pObj->GetChild();
	for (int i = 0; i < vecChild.size(); ++i)
	{
		if (vecChild[i]->Animator2D() == nullptr)
			continue;
		CAnimator2D* pAnimator2D = vecChild[i]->Animator2D();
		CAnimation2D* pAnim = pAnimator2D->GetCurAnim();
		if (pAnim->IsFinish() == true)
		{
			pAnim->Reset();

		}
		if (vecChild[i]->GetChild().empty() == false)
		{
			ResetAnim(PLAYER_STATE::ATTACK, vecChild[i]);
		}
	}

}

void CPlayerScript::Update_Animation(CGameObject* _pObj, wstring _name)
{
	if (_pObj->GetName() != _name)
		return;

	m_eDir = m_eDir;
	CAnimator2D* pAnimator2D = _pObj->Animator2D();


	if (_pObj->GetName() == _name)
	{
		switch (m_eCurState)
		{
		case PLAYER_STATE::IDLE:
		{

			if (PLAYER_DIRECTION::LEFT == m_eDir)
			{
				pAnimator2D->Play(L"STAND1_LEFT", true);
			}
			else if (PLAYER_DIRECTION::RIGHT == m_eDir)
			{
				pAnimator2D->Play(L"STAND1_RIGHT", true);

			}

		}
		break;
		case PLAYER_STATE::WALK:
		{
			if (PLAYER_DIRECTION::LEFT == m_eDir)
			{
				pAnimator2D->Play(L"WALK1_LEFT", true);
			}
			else if (PLAYER_DIRECTION::RIGHT == m_eDir)
			{
				pAnimator2D->Play(L"WALK1_RIGHT", true);

			}
		}
		break;
		case PLAYER_STATE::JUMP:
		{
			if (PLAYER_DIRECTION::LEFT == m_eDir)
			{

				pAnimator2D->Play(L"JUMP_LEFT", true);


			}
			else if (PLAYER_DIRECTION::RIGHT == m_eDir)
			{

				pAnimator2D->Play(L"JUMP_RIGHT", true);

			}
		}

		break;

		case PLAYER_STATE::ATTACK:
		{
			if (m_eCurAttState == PLAYER_ATTACK_STATE::NORMAL_ATT_1)
			{
				if (PLAYER_DIRECTION::LEFT == m_eDir)
				{
					pAnimator2D->Play(L"SWINGO1_LEFT", false);
				}
				else if (PLAYER_DIRECTION::RIGHT == m_eDir)
				{
					pAnimator2D->Play(L"SWINGO1_RIGHT", true);
				}
			}

			if (m_eCurAttState == PLAYER_ATTACK_STATE::SKILL_ATT_1)
			{
				if (PLAYER_DIRECTION::LEFT == m_eDir)
				{
					pAnimator2D->Play(L"SHOOT1_LEFT", false);
				}
				else if (PLAYER_DIRECTION::RIGHT == m_eDir)
				{
					pAnimator2D->Play(L"SHOOT1_RIGHT", true);
				}
			}


			if (m_eCurAttState == PLAYER_ATTACK_STATE::SKILL_ATT_2)
			{
				if (PLAYER_DIRECTION::LEFT == m_eDir)
				{
					pAnimator2D->Play(L"SHOOTF_LEFT", false);
				}
				else if (PLAYER_DIRECTION::RIGHT == m_eDir)
				{
					pAnimator2D->Play(L"SHOOTF_RIGHT", true);
				}
			}

		}
		break;
		case PLAYER_STATE::ALERT:
		{
			if (PLAYER_DIRECTION::LEFT == m_eDir)
			{
				pAnimator2D->Play(L"ALERT_LEFT", true);
			}
			else if (PLAYER_DIRECTION::RIGHT == m_eDir)
			{
				pAnimator2D->Play(L"ALERT_RIGHT", true);
			}
		}

		break;

		case PLAYER_STATE::ROPE_MOVE_UP:
		case PLAYER_STATE::ROPE_MOVE_DOWN:
		{

			if (PLAYER_DIRECTION::LEFT == m_eDir)
			{
				pAnimator2D->Play(L"ROPE", true);
			}
			else if (PLAYER_DIRECTION::RIGHT == m_eDir)
			{
				pAnimator2D->Play(L"ROPE", true);
			}
		}

		break;
		case PLAYER_STATE::ROPE_STOP:
		{
			if (PLAYER_DIRECTION::LEFT == m_eDir)
			{
				pAnimator2D->Play(L"ROPE_STOP", true);
			}
			else if (PLAYER_DIRECTION::RIGHT == m_eDir)
			{
				pAnimator2D->Play(L"ROPE_STOP", true);
			}
		}
		break;


		case PLAYER_STATE::DEAD:

			break;
		}

		vector<CGameObject*> ChildChildObj = _pObj->GetChild();
		if (ChildChildObj.empty() == false)
		{
			for (int i = 0; i < ChildChildObj.size(); ++i)
			{
				Update_Animation(ChildChildObj[i], L"Eye");
			}
		}

	}


}

void CPlayerScript::Update_Gravity()
{
	if (m_bDie == true)
		return;
}





void CPlayerScript::SaveToScene(FILE* _pFile)
{

	CScript::SaveToScene(_pFile);

	fwrite(&m_fSpeed, sizeof(float), 1, _pFile);
	fwrite(&m_bOnGround, sizeof(bool), 1, _pFile);
	
	fwrite(&m_fDiff_Between_Ground, sizeof(float), 1, _pFile);
	fwrite(&m_iLevel, sizeof(int), 1, _pFile);
	
	fwrite(&m_fHP, sizeof(float), 1, _pFile);
	fwrite(&m_fMP, sizeof(float), 1, _pFile);
	fwrite(&m_fEXP, sizeof(float), 1, _pFile);

	fwrite(&m_fMaxHP, sizeof(float), 1, _pFile);
	fwrite(&m_fMaxMP, sizeof(float), 1, _pFile);
	fwrite(&m_fMaxEXP, sizeof(float), 1, _pFile);


	fwrite(&m_fMinAttack, sizeof(float), 1, _pFile);
	fwrite(&m_fMaxAttack, sizeof(float), 1, _pFile);

	fwrite(&m_iLevel, sizeof(int), 1, _pFile);

	fwrite(&m_STR, sizeof(float), 1, _pFile);
	fwrite(&m_DEX, sizeof(float), 1, _pFile);
	fwrite(&m_INT, sizeof(float), 1, _pFile);
	fwrite(&m_LUK, sizeof(float), 1, _pFile);
}

void CPlayerScript::LoadFromScene(FILE* _pFile)
{
	CScript::LoadFromScene(_pFile);


	fread(&m_fSpeed, sizeof(float), 1, _pFile);
	fread(&m_bOnGround, sizeof(bool), 1, _pFile);
	
	fread(&m_fDiff_Between_Ground, sizeof(float), 1, _pFile);
	fread(&m_iLevel, sizeof(int), 1, _pFile);

	fread(&m_fHP, sizeof(float), 1, _pFile);
	fread(&m_fMP, sizeof(float), 1, _pFile);
	fread(&m_fEXP, sizeof(float), 1, _pFile);
	
	fread(&m_fMaxHP, sizeof(float), 1, _pFile);
	fread(&m_fMaxMP, sizeof(float), 1, _pFile);
	fread(&m_fMaxEXP, sizeof(float), 1, _pFile);

	fread(&m_fMinAttack, sizeof(float), 1, _pFile);
	fread(&m_fMaxAttack, sizeof(float), 1, _pFile);

	fread(&m_iLevel, sizeof(int), 1, _pFile);


	fread(&m_STR, sizeof(float), 1, _pFile);
	fread(&m_DEX, sizeof(float), 1, _pFile);
	fread(&m_INT, sizeof(float), 1, _pFile);
	fread(&m_LUK, sizeof(float), 1, _pFile);
}

void CPlayerScript::Update_Inventory()
{

	CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();
	CLayer* pLayer = pCurScene->GetLayer(L"Inventory");

	CGameObject* pObj = pLayer->FindObj(L"Inventory");
	CInventoryScript* pInvenScript = (CInventoryScript*)pObj->GetScriptByName(L"CInventoryScript");

	if (pInvenScript == nullptr)
		return;

	if (KEY_TAP(KEY::I))
	{
		bool active = pInvenScript->GetActivate();
		pInvenScript->Show(!active);
	}


}

void CPlayerScript::Update_Bgm()
{
	if (m_bDie == true)
		return;
	if (KEY_TAP(KEY::SPACE))
	{
		if (m_eCurState != PLAYER_STATE::DOUBLE_JUMP)
		{
			m_pJumpBgm->Stop();
			m_pJumpBgm->Play(1, 1.f, true);
		}
	}


}



//
//void CPlayerScript::update()
//{
//	/*if (KEY_PRESSED(KEY::Z))
//	{
//		Vec3 vRot = Transform()->GetRelativeRotation();
//		vRot.z += DT * XM_2PI;
//		Transform()->SetRelativeRotation(vRot);
//	}
//
//	if (KEY_TAP(KEY::SPACE))
//	{
//		GetOqwner()->Destroy();
//		GetOwner()->GetChild().at(0)->Destroy();
//
//		GetOwner()->GetChild().at(0)->Destroy();
//		GetOwner()->Destroy();
//
//		GetOwner()->GetChild().at(0)->Destroy();
//		GetOwner()->Destroy();
//
//		if (nullptr != m_pMissilePrefab)
//		{
//			CGameObject* pMissileObject = m_pMissilePrefab->Instantiate();
//
//			Vec3 vMissilePos = Transform()->GetRelativePos();
//			vMissilePos.y += Transform()->GetRelativeScale().y / 2.f;
//
//			CSceneMgr::GetInst()->SpawnObject(pMissileObject, vMissilePos, L"Missile", 0);
//		}		
//	}
//
//	if (KEY_TAP(KEY::B))
//	{
//		m_bBurn = true;
//		Ptr<CMaterial> pMtrl = MeshRender()->GetMaterial();
//		Vec4 vColor(1.f, 0.75f, 0.5f, 0.f);
//		pMtrl->SetScalarParam(SCALAR_PARAM::VEC4_0, &vColor);
//	}
//
//	Burnning();
//	*/
//
//
//	// ===== Todo ========
//	// 이동 방향에 따라 CLight2D 의 Dir 을 바꾼다. 
//	vector<CGameObject*> vecChild = GetOwner()->GetChild();
//	for (int i = 0; i < vecChild.size(); ++i)
//	{
//		if (nullptr != vecChild[i]->GetComponent(COMPONENT_TYPE::LIGHT2D))
//		{
//			Vec3 vDir = GetOwner()->Transform()->GetRelativePos() - PrevPos;
//			if(vDir != Vec3(0.f, 0.f ,0.f))
//				vecChild[i]->Light2D()->SetLightDir(vDir);
//
//		}
//	}
//	PrevPos = GetOwner()->Transform()->GetRelativePos();
//
//}