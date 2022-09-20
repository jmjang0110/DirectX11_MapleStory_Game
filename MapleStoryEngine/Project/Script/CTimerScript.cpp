#include "pch.h"
#include "CTimerScript.h"

#include	"CNumberScript.h"

#include "CSceneSaveLoad.h"
#include "CSceneStartScript.h"

CTimerScript::CTimerScript()
	: CScript((int)SCRIPT_TYPE::TIMERSCRIPT)


{
	SetName(CScriptMgr::GetScriptName(this));

}

CTimerScript::CTimerScript(const CTimerScript& _origin)
	: CScript((int)SCRIPT_TYPE::TIMERSCRIPT)


{
	SetName(CScriptMgr::GetScriptName(this));

}

CTimerScript::~CTimerScript()
{
}

void CTimerScript::SavePlayerToPrefab()
{
	CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();
	CLayer* pLayer = pCurScene->GetLayer(L"Player");
	CGameObject* pPlayerObj = pLayer->FindObj(L"player");

	// NextScene 에서 Player 의 위치를 미리 조정 
	Vec3 vPos = pPlayerObj->Transform()->GetRelativePos();
	vPos.x = 1140.f; // m_vNextSceneStartPos.x;
	vPos.y = -860.f; // m_vNextSceneStartPos.y;
	pPlayerObj->Transform()->SetRelativePos(vPos);

	Vec3 vCamPos = CSceneSaveLoad::pMainCamera->Transform()->GetRelativePos();
	vCamPos.x = vPos.x;
	vCamPos.y = vPos.y;
	CSceneSaveLoad::pMainCamera->Transform()->SetRelativePos(vCamPos);



	// RelativePath 저장은 CPRefab Save 에서 해주고 있음 
	wstring strContent = CPathMgr::GetInst()->GetContentPath();
	wstring wstrResKey = L"prefab\\" + pPlayerObj->GetName() + L".pref";
	wstring FullPath = strContent + wstrResKey;


	CPrefab* pPref = new CPrefab;;// (pProtoObj);
	CGameObject* pCloneObj = pPlayerObj->Clone();

	pPref->SetProto(pCloneObj);

	// Prefab 추가
	if (nullptr == CResMgr::GetInst()->FindRes<CPrefab>(wstrResKey))
	{
		// Prefab 추가 
		CResMgr::GetInst()->AddRes<CPrefab>(wstrResKey, pPref);

	}
	// Prefab 이 이미 있을 경우 
	else
	{
		RES_TYPE pResType = CResMgr::GetInst()->FindRes<CPrefab>(wstrResKey)->GetResType();
		if (RES_TYPE::PREFAB == pResType)
		{
			// 기존 Prefab 을 지우고 갱신한다 
			CResMgr::GetInst()->UpdatePrefabRes<CPrefab>(wstrResKey, pPref);

		}

	}
}

void CTimerScript::UpdateUIpos()
{
	//CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();
	//CLayer* pLayer = pCurScene->GetLayer(L"Default");
	CGameObject* pCamera = CSceneSaveLoad::pMainCamera; //pLayer->FindObj(L"MainCamera");

	static Vec2 Resolution = CDevice::GetInst()->GetRenderResolution();
	Vec3 CameraPos = pCamera->Transform()->GetRelativePos();

	Vec3 vScale = GetOwner()->Transform()->GetRelativeScale();
	Vec3 vPos = CameraPos;
	vPos.z += 100.f;

	vPos.y = CameraPos.y + (Resolution.y / 2) - (vScale.y / 2) - 40.f;
	GetOwner()->Transform()->SetRelativePos(vPos);
}

void CTimerScript::start()
{


	// Get Child -> Minute / Second 
	CGameObject* SecondNum = GetOwner()->FindChildObj(L"Second");
	CGameObject* MinuteNum = GetOwner()->FindChildObj(L"Minute");

	if (SecondNum->GetScriptByName(L"CNumberScript") != nullptr && MinuteNum->GetScriptByName(L"CNumberScript") != nullptr)
	{
		SecondNum->DeleteScript(L"CNumberScript");
		MinuteNum->DeleteScript(L"CNumberScript");

	}
		


	// Add NumScript
	// Second 
	CNumberScript* NumScript = (CNumberScript*)CScriptMgr::GetScript(L"CNumberScript");
	SecondNum->AddComponent((CComponent*)NumScript);
	NumScript->Init(NUMBER_TYPE::SHIP_TIMER);
	NumScript->UpdateNumber(10);

	// Minute 
	NumScript = (CNumberScript*)CScriptMgr::GetScript(L"CNumberScript");
	MinuteNum->AddComponent((CComponent*)NumScript);
	NumScript->Init(NUMBER_TYPE::SHIP_TIMER);
	NumScript->UpdateNumber(0);

	CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();

	if (pCurScene->GetName() == L"ShipToToyCastle")
	{
		m_fTimer = 15.f;
		m_fEndTime = 0.f;

	}
	else if (pCurScene->GetName() == L"ClockTowerOriginFloor")
	{
		m_fTimer = 1800.f;
		m_fEndTime = 0.f;

	}


	m_iCurNum = m_fEndTime;

}

void CTimerScript::update()
{
	UpdateUIpos();

	if (m_bEnd == true)
	{
		if (CSceneMgr::GetInst()->GetCurScene()->GetName() == L"ShipToToyCastle")
		{
			// Change Scene !!
		// 로딩할 Scene 파일의 경로 계산
			wstring strFilePath = CPathMgr::GetInst()->GetContentPath();
			strFilePath += L"scene\\ToyCastle.scene";;

			CScene* pNextScene = CSceneSaveLoad::LoadScene(strFilePath);
			CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();

			CSceneMgr::GetInst()->SceneChangeEvent(pCurScene, pNextScene);


			SavePlayerToPrefab();
			CSceneSaveLoad::pSceneMgrScript->End();
			
			CSceneMgr::GetInst()->StartSceneEvent(pNextScene);
		}

		return;
	}

	if (m_fTimer >= m_fEndTime)
	{

		m_fTimer -= DT;

	}

	if (m_fTimer <= m_fEndTime)
	{
		m_bEnd = true;
		
	}

	m_iCurNum = m_fTimer;
	if (m_iCurNum != m_iPrevNum)
	{
		m_iPrevNum = m_iCurNum;

		CGameObject* SecondNum = GetOwner()->FindChildObj(L"Second");
		CGameObject* MinuteNum = GetOwner()->FindChildObj(L"Minute");

		// 60 second -> 1 Minute 
		int secondnum = m_iCurNum % 60;
		int minutenum = m_iCurNum / 60;

		CNumberScript* SecondNumScript = (CNumberScript*)SecondNum->GetScriptByName(L"CNumberScript");
		CNumberScript* MinuteNumScript = (CNumberScript*)MinuteNum->GetScriptByName(L"CNumberScript");

		SecondNumScript->UpdateNumber(secondnum);
		MinuteNumScript->UpdateNumber(minutenum);

	}


	

}

void CTimerScript::lateupdate()
{
}

void CTimerScript::OnCollisionEnter(CGameObject* _OtherObject)
{
}

void CTimerScript::OnCollision(CGameObject* _OtherObject)
{
}

void CTimerScript::OnCollisionExit(CGameObject* _OtherObject)
{
}
