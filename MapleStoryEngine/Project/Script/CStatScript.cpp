#include "pch.h"
#include "CStatScript.h"
#include "CNumberScript.h"
#include "CSceneSaveLoad.h"
#include "CSceneStartScript.h"
#include "CPlayerScript.h"


CStatScript::CStatScript()
	: CScript((int)SCRIPT_TYPE::STATSCRIPT)

{
	SetName(CScriptMgr::GetScriptName(this));

}


CStatScript::CStatScript(const CStatScript& _origin)
	: CScript((int)SCRIPT_TYPE::STATSCRIPT)


{
	SetName(CScriptMgr::GetScriptName(this));

}

CStatScript::~CStatScript()
{
}

void CStatScript::InsertNumber(CGameObject* _pObj, int num)
{
	if (_pObj == nullptr)
		return;

	CNumberScript* pNumScript = (CNumberScript*)CScriptMgr::GetScript(L"CNumberScript");
	_pObj->AddComponent((CComponent*)pNumScript);
	pNumScript->Init(NUMBER_TYPE::BLACK); // Black Number 
	pNumScript->UpdateNumber(num);

	
}

void CStatScript::start()
{

	if (CSceneSaveLoad::pMainPlayer != nullptr)
	{
		CPlayerScript* pPlayerScript = (CPlayerScript*)CSceneSaveLoad::pMainPlayer->GetScriptByName(L"CPlayerScript");
		
		if (pPlayerScript == nullptr)
			return;


		// HP
		m_HP = GetOwner()->FindChildObj(L"HP");
		InsertNumber(m_HP, pPlayerScript->GetHp());

		// MP
		m_MP = GetOwner()->FindChildObj(L"MP");
		InsertNumber(m_MP, pPlayerScript->GetMp());

		// ATTACK
		m_ATT_MIN = GetOwner()->FindChildObj(L"MIN_ATTACK");
		InsertNumber(m_ATT_MIN, pPlayerScript->GetMinAttack());

		// ATTACK
		m_ATT_MAX = GetOwner()->FindChildObj(L"MAX_ATTACK");
		InsertNumber(m_ATT_MAX, pPlayerScript->GetMaxAttack());

		
		// STR
		m_STR = GetOwner()->FindChildObj(L"STR");
		InsertNumber(m_STR, 1000);
		// DEX 
		m_DEX = GetOwner()->FindChildObj(L"DEX");
		InsertNumber(m_DEX, 1000);
		// INT
		m_INT = GetOwner()->FindChildObj(L"INT");
		InsertNumber(m_INT, 1000);
		// LUK
		m_LUK = GetOwner()->FindChildObj(L"LUK");
		InsertNumber(m_LUK, 1000);

	}
	


	
}

void CStatScript::update()
{
	CPlayerScript* pPlayerScript = (CPlayerScript*)CSceneSaveLoad::pMainPlayer->GetScriptByName(L"CPlayerScript");
	// HP
	InsertNumber(m_HP, pPlayerScript->GetHp());
	// MP 
	InsertNumber(m_MP, pPlayerScript->GetMp());
	// ATTACK
	InsertNumber(m_ATT_MIN, pPlayerScript->GetMinAttack());
	InsertNumber(m_ATT_MAX, pPlayerScript->GetMaxAttack());

	// STR
	InsertNumber(m_STR, pPlayerScript->GetSTR());
	// DEX 
	InsertNumber(m_DEX, pPlayerScript->GetDEX());
	// INT
	InsertNumber(m_INT, pPlayerScript->GetINT());
	// LUK
	InsertNumber(m_LUK, pPlayerScript->GetLUK());


}

void CStatScript::lateupdate()
{
}

void CStatScript::OnCollisionEnter(CGameObject* _OtherObject)
{
}

void CStatScript::OnCollision(CGameObject* _OtherObject)
{
	

	// find Camera Obj !!  
	CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();
	CLayer* pLayer = pCurScene->GetLayer(L"Default");
	CGameObject* pCursorObj = nullptr;
	pCursorObj = pLayer->FindObj(L"Cursor");
	m_Cursor_CurPos = pCursorObj->Transform()->GetRelativePos();
	CGameObject* pCamera = pLayer->FindObj(L"MainCamera");
	Vec3 CameraPos = pCamera->Transform()->GetRelativePos();


	if (KEY_TAP(KEY::LBTN) && m_bMove == true)
	{
		m_Cursor_StartPos.x = m_Cursor_CurPos.x;
		m_Cursor_StartPos.y = m_Cursor_CurPos.y;

		m_Cursor_PrevDiff = m_Cursor_Diff;
		m_Cursor_Diff.x = 0.f;
		m_Cursor_Diff.y = 0.f;


	}

	if (KEY_PRESSED(KEY::LBTN) && m_bMove == true)
	{

		m_Cursor_Diff.x = m_Cursor_CurPos.x - m_Cursor_StartPos.x;
		m_Cursor_Diff.y = m_Cursor_CurPos.y - m_Cursor_StartPos.y;

	}

	if (KEY_AWAY(KEY::LBTN) && m_bMove == true)
	{
		m_Cursor_Diff.x = m_Cursor_Diff.x + m_Cursor_PrevDiff.x;
		m_Cursor_Diff.y = m_Cursor_Diff.y + m_Cursor_PrevDiff.y;

		m_Cursor_PrevDiff.x = 0.f;
		m_Cursor_PrevDiff.y = 0.f;


	}


}

void CStatScript::OnCollisionExit(CGameObject* _OtherObject)
{
}

void CStatScript::SaveToScene(FILE* _pFile)
{
}

void CStatScript::LoadFromScene(FILE* _pFile)
{
}
