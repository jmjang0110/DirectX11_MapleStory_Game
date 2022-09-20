#include "pch.h"
#include "CNumberScript.h"


#include <Engine/CTimeMgr.h>
#include <Engine/CTileMap.h>
#include <Engine/CTransform.h>

#include <Engine/CEventMgr.h>


CNumberScript::CNumberScript()
	: CScript((int)SCRIPT_TYPE::NUMBERSCRIPT)
	, m_fTimer(0.f)
	, m_iNum(0)
	, m_vStartLT(0.f, 0.f)
	, m_vSlice(0.f, 0.f)

{
	SetName(CScriptMgr::GetScriptName(this));
	//Init(Vec2(0.f, 0.f), Vec2(8.f, 11.f), NUMBER_TYPE::ITEM);

	//Init(Vec2(0.f, 0.f), Vec2(7.f, 10.f), NUMBER_TYPE::MESO);

}

CNumberScript::CNumberScript(const CNumberScript& _origin)
	: CScript((int)SCRIPT_TYPE::NUMBERSCRIPT)
	, m_fTimer(0.f)
	, m_iNum(_origin.m_iNum)
	, m_vStartLT(_origin.m_vStartLT)
	, m_vSlice(_origin.m_vSlice)
	, m_eType(_origin.m_eType)

{
	SetName(CScriptMgr::GetScriptName(this));
}

CNumberScript::~CNumberScript()
{
	int i = 0;


}


void CNumberScript::Init(NUMBER_TYPE _type, Vec2 _startLT, Vec2 _slice)
{
	m_vStartLT = _startLT;
	m_vSlice = _slice;
	m_eType = _type;

	switch (_type)
	{
	case NUMBER_TYPE::ITEM:
	{
		m_pTex = CResMgr::GetInst()->Load<CTexture>(L"texture\\UI\\Number.png", L"texture\\UI\\Number.png");
		m_vStartLT = Vec2(0.f, 0.f);
		m_vSlice = Vec2(8.f, 11.f);

	}
	break;
	case NUMBER_TYPE::MESO:
	{
		m_pTex = CResMgr::GetInst()->Load<CTexture>(L"texture\\UI\\HpMpLevel\\LvNum.png", L"texture\\UI\\HpMpLevel\\LvNum.png");
		m_vStartLT = Vec2(0.f, 0.f);
		m_vSlice = Vec2(7.f, 10.f);

	}


	break;
	case NUMBER_TYPE::BLACK:
	{
		m_pTex = CResMgr::GetInst()->Load<CTexture>(L"texture\\Inventory\\BlackNumber.png", L"texture\\Inventory\\BlackNumber.png");
		m_vStartLT = Vec2(0.f, 0.f);
		m_vSlice = Vec2(5.f, 7.f);
	}
		break;
	case NUMBER_TYPE::SHIP_TIMER:
	{
		m_pTex = CResMgr::GetInst()->Load<CTexture>(L"texture\\UI\\TimerNum.png", L"texture\\UI\\TimerNum.png");

		m_vStartLT = Vec2(0.f, 0.f);
		m_vSlice = Vec2(28.f, 47.f);
	}
		break;
	}

	GetOwner()->TileMap()->SetAtlasTex(m_pTex);
	GetOwner()->TileMap()->SetTileSize(m_vSlice);
	GetOwner()->Transform()->SetIgnoreParentScale(true);
	if (NUMBER_TYPE::ITEM == m_eType)
	{
		GetOwner()->Transform()->SetRelativePos(Vec3(-9.f, -9.f, -1.f));
	}
	//start();

}


void CNumberScript::start()
{

	//if (GetOwner()->TileMap() == nullptr && m_pTex != nullptr)
	//{
	//	CGameObject* ChildObj = new CGameObject;
	//	ChildObj->SetName(L"Number");
	//	ChildObj->AddComponent(new CTransform);
	//	ChildObj->AddComponent(new CTileMap);
	//	ChildObj->TileMap()->SetAtlasTex(m_pTex);
	//	ChildObj->TileMap()->SetTileSize(m_vSlice);
	//	ChildObj->TileMap()->SetTileMapCount(1, 1);

	//	ChildObj->Transform()->SetRelativeScale(m_vSlice.x * 1, m_vSlice.y * 1, 1.f);
	//	if (NUMBER_TYPE::ITEM == m_eType)
	//	{
	//		ChildObj->Transform()->SetRelativePos(Vec3(-9.f, -9.f, -1.f));
	//	}
	//	ChildObj->Transform()->SetIgnoreParentScale(true);
	//	ChildObj->TileMap()->SetTileData(0, 1);

	//	ChildObj->UpdateLayerIdx(GetOwner()->GetLayerIndex());
	//	GetOwner()->AddChild(ChildObj);

	//	CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();

	//	CGameObject* test = GetOwner();
	//	int i = 0;

	//	//pCurScene->AddObject(ChildObj, GetOwner()->GetLayerIndex());

	//	//CSceneMgr::GetInst()->AddChild(GetOwner(), ChildObj);

	//}



}

void CNumberScript::UpdateNumber(UINT _num)
{
	if (_num == 0)
		int i = 0;

	m_iNum = _num;

	vector<UINT> vTemp;
	m_vecNum.clear();
	vTemp.swap(m_vecNum);

	int num = _num;
	while (num > 0)
	{
		m_vecNum.push_back(num % 10); // 123 -> 3 2 1 로 저장 -> 사용할땐 거꾸로 
		num /= 10;
	}

	int size = m_vecNum.size();
	if (_num == 0)
		size = 1;

	CGameObject* NumObj = GetOwner();// ->FindChildObj(L"Number");
	NumObj->TileMap()->SetTileMapCount(size, 1);
	NumObj->Transform()->SetRelativeScale(m_vSlice.x * size, m_vSlice.y * 1, 1.f);

	int idx = 0;

	if(_num == 0)
		NumObj->TileMap()->SetTileData(idx, 0);


	for (int i = m_vecNum.size() - 1; i >= 0; --i)
	{
		NumObj->TileMap()->SetTileData(idx, m_vecNum[i]);
		idx++;

	}
}



void CNumberScript::update()
{
	m_fTimer += DT;

	/*if (m_fTimer >= 1.f)
	{
		CGameObject* test = GetOwner();
		CGameObject* NumObj = GetOwner()->FindChildObj(L"Number");

		m_iNum += 1;
		UpdateNumbber(m_iNum);

		m_fTimer = 0.f;

	}*/

}

void CNumberScript::lateupdate()
{
}

void CNumberScript::OnCollisionEnter(CGameObject* _OtherObject)
{
}

void CNumberScript::OnCollision(CGameObject* _OtherObject)
{
}

void CNumberScript::OnCollisionExit(CGameObject* _OtherObject)
{
}

