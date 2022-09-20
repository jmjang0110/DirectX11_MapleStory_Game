#include "pch.h"
#include "CBossHPBarScript.h"

#include "CBossMonsterScript.h"
#include "CSceneSaveLoad.h"
#include "CSceneStartScript.h"


CBossHPBarScript::CBossHPBarScript()
	: CScript((int)SCRIPT_TYPE::BOSSHPBARSCRIPT)
	, m_pSubObj(nullptr)

{
	SetName(CScriptMgr::GetScriptName(this));


}

CBossHPBarScript::CBossHPBarScript(const CBossHPBarScript& _origin)
	: CScript((int)SCRIPT_TYPE::BOSSHPBARSCRIPT)
	, m_pSubObj(nullptr)

{
	SetName(CScriptMgr::GetScriptName(this));


}

CBossHPBarScript::~CBossHPBarScript()
{
}

void CBossHPBarScript::start()
{

	m_fMaxHPsize = 758.f;
	CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();
	CLayer* pCurLayer = pCurScene->GetLayer(L"Monster");
	m_pBoss = pCurLayer->FindObj(L"Papulatus");

}

void CBossHPBarScript::update()
{
	if (m_pBoss == nullptr)
		return;



	// 위치 조정 
	//CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();
	//CLayer* pLayer = pCurScene->GetLayer(L"Default");
	CGameObject* pCamera = CSceneSaveLoad::pMainCamera;
	Vec3 vScale = GetOwner()->Transform()->GetRelativeScale();

	static Vec2 Resolution = CDevice::GetInst()->GetRenderResolution();
	Vec3 CameraPos = pCamera->Transform()->GetRelativePos();
	Vec3 vPos = CameraPos;
	vPos.z += 100.f;

	vPos.y = CameraPos.y + (Resolution.y / 2) - (vScale.y / 2) - 20.f;
	GetOwner()->Transform()->SetRelativePos(vPos);


	



	if (m_pSubObj == nullptr)
	{
		m_pSubObj = GetOwner()->FindChildObj(L"BossHP");
	}

	// Scale 조정 
	CBossMonsterScript* pBossScript = (CBossMonsterScript*)m_pBoss->GetScriptByName(L"CBossMonsterScript");
	tBossMonInfo tBossInfo = pBossScript->GetMonsterInfo();

	m_fCurHP = tBossInfo.fHP;
	float fBossHP = tBossInfo.fHP;
	float fBossMaxHP = tBossInfo.fMaxHP;

	float Ratio = fBossHP / fBossMaxHP;

	Vec3 vBossHPScale = m_pSubObj->Transform()->GetRelativeScale();;

	if (m_fMaxHPsize <= vBossHPScale.x)
		vBossHPScale.x = m_fMaxHPsize;

	vBossHPScale.x = m_fMaxHPsize * Ratio;
	m_pSubObj->Transform()->SetRelativeScale(vBossHPScale);



	if (m_pSubObj != nullptr)
	{
		Vec3 vHPScale = m_pSubObj->Transform()->GetRelativeScale();
	
		Vec3 vPosSub = Vec3(0.f, 0.f, -5.f);
		vPosSub.x = 0.f - (vScale.x / 2) + 39.f + (vHPScale.x / 2);
		vPosSub.y = 10.f;
		m_pSubObj->Transform()->SetRelativePos(vPosSub);
	}


	if (m_fCurHP <= 0.f)
	{
		// Die 
		// 
		// 
		m_bDie = true;
		if (m_pBossNPC != nullptr)
		{
			m_pBossNPC->SetEndBossMonster(m_bDie);
		}
		// Event 
		CGameObject* pDelObj = GetOwner();
		int pDelObj_LayerIdx = GetOwner()->GetLayerIndex();

		CSceneMgr::GetInst()->DeRegisterObjInLayer(pDelObj, pDelObj_LayerIdx);
		GetOwner()->Destroy();
		return;
	}

}

void CBossHPBarScript::lateupdate()
{
}

void CBossHPBarScript::OnCollisionEnter(CGameObject* _OtherObject)
{
}

void CBossHPBarScript::OnCollision(CGameObject* _OtherObject)
{
}

void CBossHPBarScript::OnCollisionExit(CGameObject* _OtherObject)
{
}
