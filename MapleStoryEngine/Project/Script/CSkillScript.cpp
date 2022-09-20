#include "pch.h"
#include "CSkillScript.h"

#include <Engine/CGameObject.h>
#include <Engine/CAnimation2D.h>
#include <Engine/CAnimator2D.h>
#include <Engine/CSceneMgr.h>
#include <Engine/CScene.h>
#include <Engine/CLayer.h>

#include <Engine/CPrefab.h>

#include "CScriptMgr.h"
#include "CPlayerScript.h"





CSkillScript::CSkillScript()
	: CScript((int)SCRIPT_TYPE::SKILLSCRIPT)
	, m_PrepareObj(nullptr)
	, m_KeyDownObj(nullptr)
	, m_EndObj(nullptr)
	, m_fArrowTimer(0.f)
	, m_fMaxArrowTimer(0.1f)
	, m_eBallMoveType(BALLMOVE_TYPE::LINEAR)
	, m_bOffsetY(false)


{
	SetName(CScriptMgr::GetScriptName(this));

}

CSkillScript::CSkillScript(const CSkillScript& _origin)
	: CScript((int)SCRIPT_TYPE::SKILLSCRIPT)
	, m_PrepareObj(nullptr)
	, m_KeyDownObj(nullptr)
	, m_EndObj(nullptr)
	, m_fArrowTimer(0.f)
	, m_fMaxArrowTimer(0.1f)
	, m_eBallMoveType(BALLMOVE_TYPE::LINEAR)
	, m_bOffsetY(false)


{
	SetName(CScriptMgr::GetScriptName(this));

}

CSkillScript::~CSkillScript()
{
	SAFE_DELETE(m_ArrowPrefab);
	
	m_pEndBgm->Stop();
	m_pPrepareBgm->Stop();
	m_pKeyDownBgm->Stop();

}


void CSkillScript::start()
{
	m_eState = SKILL_STATE::PREPARE;
	m_fArrowTimer = 0.f;
	m_fMaxArrowTimer = 0.1f;


	if (GetName() == L"stormSkill")
	{
		m_pPrepareBgm = CResMgr::GetInst()->Load<CSound>(L"sound\\Storm\\Loop.mp3", L"sound\\Storm\\Loop.mp3");
		m_pKeyDownBgm = CResMgr::GetInst()->Load<CSound>(L"sound\\Storm\\Use.mp3", L"sound\\Storm\\Use.mp3");
		m_pEndBgm = CResMgr::GetInst()->Load<CSound>(L"sound\\Storm\\End.mp3", L"sound\\Storm\\End.mp3");

	}
	else if (GetName() == L"ArrowFlatterSkill")
	{
		m_pPrepareBgm = CResMgr::GetInst()->Load<CSound>(L"sound\\ArrowFlatter\\Loop.mp3", L"sound\\ArrowFlatter\\Loop.mp3");
		m_pKeyDownBgm = CResMgr::GetInst()->Load<CSound>(L"sound\\ArrowFlatter\\Use.mp3", L"sound\\Storm\\Use.mp3");
		m_pEndBgm = CResMgr::GetInst()->Load<CSound>(L"sound\\ArrowFlatter\\End.mp3", L"sound\\Storm\\End.mp3");

	}
	
	m_fTimer = 0.f;


}

void CSkillScript::update()
{
	m_fTimer += DT;

	UpdatePos();

	switch (m_eState)
	{
	case SKILL_STATE::PREPARE:
	{
		Prepare();
	


	}
	break;
	case SKILL_STATE::KEYDOWN:
	{
	
		KeyDown();
		AttackArrow();
		if (m_fTimer >=3.f)
			CSound::UpdateFMOD();



	}
	break;
	case SKILL_STATE::SKILLEND:
	{
		SkillEnd();

	}
	break;

	}

	if (m_eState == SKILL_STATE::PREPARE)
	{
		if (KEY_AWAY(m_eKey))
		{
			// Event 
			CGameObject* pDelObj = GetOwner();
			int pDelObj_LayerIdx = GetOwner()->GetLayerIndex();

			CSceneMgr::GetInst()->DeRegisterObjInLayer(pDelObj, pDelObj_LayerIdx);
			GetOwner()->Destroy();

		}
	}


}

void CSkillScript::lateupdate()
{



}


void CSkillScript::Prepare()
{
	// Except 
	if (m_eState != SKILL_STATE::PREPARE)
		return;

	if (m_PrepareObj == nullptr)
	{
		m_eState = SKILL_STATE::KEYDOWN;
		return;
	}

	if (m_PrepareObj->Animator2D() == nullptr)
	{
		CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();
		CLayer* pCurLayer = pCurScene->GetLayer(L"Skill");

		// delete Obj 
		vector<CGameObject*> vecObj = pCurLayer->GetRootObjects();
		for (int i = 0; i < vecObj.size(); ++i)
		{
			if (vecObj[i]->GetName() == m_PrepareObj->GetName())
			{
				CSceneMgr::GetInst()->DeRegisterObjInLayer(m_PrepareObj, m_PrepareObj->GetLayerIndex());
				m_PrepareObj->Destroy();
				m_PrepareObj = nullptr;

				break;

			}
		}

		// Change State 
		m_eState = SKILL_STATE::KEYDOWN;
		
		return;
	}
	// On

	CAnimator2D* pAnimator2D = m_PrepareObj->Animator2D();
	CAnimation2D* pCurAnim = pAnimator2D->GetCurAnim();

	if (m_KeyDownObj != nullptr && m_KeyDownObj->Animator2D() != nullptr)
	{
		CAnimator2D* pAnimator2D_Keydown = m_KeyDownObj->Animator2D();
		pAnimator2D_Keydown->Play(L"EMPTY", false);
	}
	if (m_EndObj != nullptr && m_EndObj->Animator2D() != nullptr)
	{
		CAnimator2D* pAnimator2D_End = m_EndObj->Animator2D();
		pAnimator2D_End->Play(L"EMPTY", false);
	}


	if (KEY_TAP(m_eKey))
	{
		m_pPrepareBgm->Play(1, 0.7f, true);
		CPlayerScript* pPlayerScript = (CPlayerScript*)m_pSkillUser->GetScriptByName(L"CPlayerScript");

		if (pPlayerScript->GetDir() == PLAYER_DIRECTION::LEFT)
		{
			pAnimator2D->Play(L"Move_Left", false);

		}
		else if (pPlayerScript->GetDir() == PLAYER_DIRECTION::RIGHT)
		{
			pAnimator2D->Play(L"Move_Right", false);
		}




	}
	if (pCurAnim->IsFinish() == true)
	{
		// Change State 
		m_eState = SKILL_STATE::KEYDOWN;
		pAnimator2D->Play(L"EMPTY", false);
		pAnimator2D->Deactivate();

	}

	return;

}

void CSkillScript::KeyDown()
{
	// Except 
	if (m_eState != SKILL_STATE::KEYDOWN)
		return;

	if (m_KeyDownObj == nullptr)
	{
		m_eState = SKILL_STATE::SKILLEND;
		return;
	}

	if (m_KeyDownObj->Animator2D() == nullptr)
	{
		CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();
		CLayer* pCurLayer = pCurScene->GetLayer(L"Skill");

		// delete Obj 
		vector<CGameObject*> vecObj = pCurLayer->GetRootObjects();
		for (int i = 0; i < vecObj.size(); ++i)
		{
			if (vecObj[i]->GetName() == m_KeyDownObj->GetName())
			{
				CSceneMgr::GetInst()->DeRegisterObjInLayer(m_KeyDownObj, m_KeyDownObj->GetLayerIndex());
				m_KeyDownObj->Destroy();
				m_KeyDownObj = nullptr;

				break;

			}
		}

		// Change State 
		m_eState = SKILL_STATE::SKILLEND;
		return;
	}
	// On

	CAnimator2D* pAnimator2D = m_KeyDownObj->Animator2D();
	CAnimation2D* pCurAnim = pAnimator2D->GetCurAnim();

	if (m_EndObj != nullptr && m_EndObj->Animator2D() != nullptr)
	{
		CAnimator2D* pAnimator2D_End = m_EndObj->Animator2D();
		pAnimator2D_End->Play(L"EMPTY", false);
	}



	if (KEY_PRESSED(m_eKey))
	{
		CPlayerScript* pPlayerScript = (CPlayerScript*)m_pSkillUser->GetScriptByName(L"CPlayerScript");

		if (pPlayerScript->GetDir() == PLAYER_DIRECTION::LEFT)
		{
			pAnimator2D->Play(L"Move_Left", true);

		}
		else if (pPlayerScript->GetDir() == PLAYER_DIRECTION::RIGHT)
		{
			pAnimator2D->Play(L"Move_Right", true);
		}

	}
	if (KEY_AWAY(m_eKey))
	{
		// Change State 
		m_eState = SKILL_STATE::SKILLEND;
		pAnimator2D->Play(L"EMPTY", false);
		pAnimator2D->Deactivate();

		m_fTimer = 0.f;
		m_pEndBgm->Play(1, 0.8f, true);
	}

	return;

}

void CSkillScript::SkillEnd()
{
	// Except 
	if (m_eState != SKILL_STATE::SKILLEND)
		return;

	if (m_EndObj == nullptr )
	{
		CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();
		CLayer* pCurLayer = pCurScene->GetLayer(L"Skill");

		CGameObject* pDelObj = GetOwner();
		CSceneMgr::GetInst()->DeRegisterObjInLayer(pDelObj, pDelObj->GetLayerIndex());
		pDelObj->Destroy();

		return;
	}

	if (m_EndObj->Animator2D() == nullptr)
	{
		CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();
		CLayer* pCurLayer = pCurScene->GetLayer(L"Skill");

		// delete Obj 
		vector<CGameObject*> vecObj = pCurLayer->GetRootObjects();
		for (int i = 0; i < vecObj.size(); ++i)
		{
			if (vecObj[i]->GetName() == m_EndObj->GetName())
			{
				CSceneMgr::GetInst()->DeRegisterObjInLayer(m_EndObj, m_EndObj->GetLayerIndex());
				m_EndObj->Destroy();
				m_EndObj = nullptr;

				break;

			}
		}

		// Change State 
		m_eState = SKILL_STATE::SKILLEND;
		return;
	}
	// On

	CAnimator2D* pAnimator2D = m_EndObj->Animator2D();
	CPlayerScript* pPlayerScript = (CPlayerScript*)m_pSkillUser->GetScriptByName(L"CPlayerScript");

	if (pPlayerScript->GetDir() == PLAYER_DIRECTION::LEFT)
	{
		pAnimator2D->Play(L"Move_Left", false);

	}
	else if (pPlayerScript->GetDir() == PLAYER_DIRECTION::RIGHT)
	{
		pAnimator2D->Play(L"Move_Right", false);
	}
	CAnimation2D* pCurAnim = pAnimator2D->GetCurAnim();
	if (pCurAnim->IsFinish() == true && m_fTimer >= 1.2f)
	{
		CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();
		CLayer* pCurLayer = pCurScene->GetLayer(L"Skill");

		CGameObject* pDelObj = GetOwner();
		CSceneMgr::GetInst()->DeRegisterObjInLayer(pDelObj, pDelObj->GetLayerIndex());
		pDelObj->Destroy();
	}


	return;
}

// �Ű������� ���� Prefab�� �ܺο��� �˾Ƽ� �� ���쵵��.. 
void CSkillScript::SetSkillObjByPrefab(CPrefab* _prepare
	, CPrefab* _keydown, CPrefab* _end)
{
	CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();
	CLayer* pCurLayer = pCurScene->GetLayer(L"Skill");

	if (_prepare != nullptr)
	{
		m_PrepareObj = _prepare->Instantiate();
		m_PrepareObj->SetName(L"prepare_test");
		m_PrepareObj->Transform()->SetRelativePos(Vec3(0.f, 0.f, 0.f));

		GetOwner()->AddChild(m_PrepareObj);
	}
	if (_keydown != nullptr)
	{
		m_KeyDownObj = _keydown->Instantiate();
		m_KeyDownObj->SetName(L"Keydown_test");
		m_KeyDownObj->Transform()->SetRelativePos(Vec3(0.f, 0.f, 0.f));


		GetOwner()->AddChild(m_KeyDownObj);
	}
	if (_end != nullptr)
	{
		m_EndObj = _end->Instantiate();
		m_EndObj->SetName(L"End_test");
		m_KeyDownObj->Transform()->SetRelativePos(Vec3(0.f, 0.f, 0.f));

		GetOwner()->AddChild(m_EndObj);

	}
}

void CSkillScript::UpdatePos()
{
	CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();
	CLayer* pLayer = pCurScene->GetLayer(L"Player");
	vector<CGameObject*> vecObjs = pLayer->GetRootObjects();
	Vec3 vPlayerPos = Vec3(0.f, 0.f, 0.f);

	// Get Player Pos 
	for (int i = 0; i < vecObjs.size(); ++i)
	{
		if (vecObjs[i]->GetName() == L"player")
		{
			vPlayerPos = vecObjs[i]->Transform()->GetRelativePos();
			break;
		}
	}


	// Update Pos
	GetOwner()->Transform()->SetRelativePos(vPlayerPos);


}

void CSkillScript::AttackArrow()
{
	m_fArrowTimer += DT;

	// 1�ʿ� 10�� ȭ�� 
	if (m_fArrowTimer < m_fMaxArrowTimer)
		return;
	else
		m_fArrowTimer = 0.f;



	m_pKeyDownBgm->Play(1, 0.25f, true);

	CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();
	CLayer* pLayer = pCurScene->GetLayer(L"SubSkill_1");



	if (m_ArrowPrefab == nullptr)
	{
		wstring strPrefabKey = m_wstrBallPrefabName;
		wstring strContent = CPathMgr::GetInst()->GetContentPath();
		wstring FullPath = strContent + strPrefabKey;

		m_ArrowPrefab = new CPrefab;
		m_ArrowPrefab->Load(FullPath);
		assert(m_ArrowPrefab);
	}


	// Prefab ���Ͽ� ����� gameObject �� �о �ش� Layer �� �����Ѵ�. 
	CGameObject* NewObj = m_ArrowPrefab->Instantiate();
	Vec3 vPos = GetOwner()->Transform()->GetRelativePos();

	if (m_bOffsetY)
	{
		int Offset_y = rand() % 20;
		if (Offset_y % 2 == 1)
			Offset_y *= -1;
		vPos.y += Offset_y;
	}





	CBasicBallScript* pScript = (CBasicBallScript*)CScriptMgr::GetScript(L"CBasicBallScript");
	CPlayerScript* pPlayerScript = (CPlayerScript*)m_pSkillUser->GetScriptByName(L"CPlayerScript");
	pScript->SetBallMoveType(m_eBallMoveType);

	if (pPlayerScript->GetDir() == PLAYER_DIRECTION::LEFT)
	{
		pScript->SetDir(BALL_DIRECTION::LEFT);
		NewObj->Animator2D()->Play(L"Move_Left", true);
		vPos.x -= 80.f;

	}
	else if (pPlayerScript->GetDir() == PLAYER_DIRECTION::RIGHT)
	{

		pScript->SetDir(BALL_DIRECTION::RIGHT);
		NewObj->Animator2D()->Play(L"Move_Right", true);
		vPos.x += 80.f;

	}

	NewObj->Transform()->SetRelativePos(vPos);
	NewObj->AddComponent((CComponent*)pScript);

	pCurScene->AddObject(NewObj, pLayer->GetName());
	pScript->Init(vPos);
	pScript->SetMaxAttack(pPlayerScript->GetMaxAttack());
	pScript->SetMinAttack(pPlayerScript->GetMinAttack());

	if (GetName() == L"stormSkill")
	{
		pScript->SetHitBgm(L"sound\\Storm\\Hit.mp3");
	}
	else if (GetName() == L"ArrowFlatterSkill")
	{
		pScript->SetHitBgm(L"sound\\ArrowFlatter\\Hit.mp3");
	}



	


}


void CSkillScript::OnCollisionEnter(CGameObject* _OtherObject)
{
}

void CSkillScript::OnCollision(CGameObject* _OtherObject)
{
}

void CSkillScript::OnCollisionExit(CGameObject* _OtherObject)
{
}

void CSkillScript::SaveToScene(FILE* _pFile)
{
}

void CSkillScript::LoadFromScene(FILE* _pFile)
{
}

