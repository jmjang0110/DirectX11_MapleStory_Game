#include "pch.h"
#include "CScriptMgr.h"

#include "CAIScript.h"
#include "CameraPlayerMoveScript.h"
#include "CAttackStateScript.h"
#include "CBackGroundScript.h"
#include "CBasicBallScript.h"
#include "CBossMonsterScript.h"
#include "CButtonScript.h"
#include "CCameraMoveScript.h"
#include "CCloudScript.h"
#include "CConsumeScript.h"
#include "CCursorScript.h"
#include "CDamageScript.h"
#include "CDeadStateScript.h"
#include "CDoubleJumpScript.h"
#include "CExpScript.h"
#include "CGravityScript.h"
#include "CGroundScript.h"
#include "CHpScript.h"
#include "CIdleStateScript.h"
#include "CInventoryScript.h"
#include "CItemScript.h"
#include "CLaunchSkillScript.h"
#include "CMainBarScript.h"
#include "CMissileScript.h"
#include "CMobGroundScript.h"
#include "CMonsterFactoryScript.h"
#include "CMonsterScript.h"
#include "CMpScript.h"
#include "CNumberScript.h"
#include "CPaperBurnScript.h"
#include "CPatrolStateScript.h"
#include "CPlayerScript.h"
#include "CRigidBodyScript.h"
#include "CSkillnearScript.h"
#include "CSkillScript.h"
#include "CStateScript.h"
#include "CTraceStateScript.h"
#include "CWallScript.h"

void CScriptMgr::GetScriptInfo(vector<wstring>& _vec)
{
	_vec.push_back(L"CAIScript");
	_vec.push_back(L"CameraPlayerMoveScript");
	_vec.push_back(L"CAttackStateScript");
	_vec.push_back(L"CBackGroundScript");
	_vec.push_back(L"CBasicBallScript");
	_vec.push_back(L"CBossMonsterScript");
	_vec.push_back(L"CButtonScript");
	_vec.push_back(L"CCameraMoveScript");
	_vec.push_back(L"CCloudScript");
	_vec.push_back(L"CConsumeScript");
	_vec.push_back(L"CCursorScript");
	_vec.push_back(L"CDamageScript");
	_vec.push_back(L"CDeadStateScript");
	_vec.push_back(L"CDoubleJumpScript");
	_vec.push_back(L"CExpScript");
	_vec.push_back(L"CGravityScript");
	_vec.push_back(L"CGroundScript");
	_vec.push_back(L"CHpScript");
	_vec.push_back(L"CIdleStateScript");
	_vec.push_back(L"CInventoryScript");
	_vec.push_back(L"CItemScript");
	_vec.push_back(L"CLaunchSkillScript");
	_vec.push_back(L"CMainBarScript");
	_vec.push_back(L"CMissileScript");
	_vec.push_back(L"CMobGroundScript");
	_vec.push_back(L"CMonsterFactoryScript");
	_vec.push_back(L"CMonsterScript");
	_vec.push_back(L"CMpScript");
	_vec.push_back(L"CNumberScript");
	_vec.push_back(L"CPaperBurnScript");
	_vec.push_back(L"CPatrolStateScript");
	_vec.push_back(L"CPlayerScript");
	_vec.push_back(L"CRigidBodyScript");
	_vec.push_back(L"CSkillnearScript");
	_vec.push_back(L"CSkillScript");
	_vec.push_back(L"CStateScript");
	_vec.push_back(L"CTraceStateScript");
	_vec.push_back(L"CWallScript");
}

CScript * CScriptMgr::GetScript(const wstring& _strScriptName)
{
	if (L"CAIScript" == _strScriptName)
		return new CAIScript;
	if (L"CameraPlayerMoveScript" == _strScriptName)
		return new CameraPlayerMoveScript;
	if (L"CAttackStateScript" == _strScriptName)
		return new CAttackStateScript;
	if (L"CBackGroundScript" == _strScriptName)
		return new CBackGroundScript;
	if (L"CBasicBallScript" == _strScriptName)
		return new CBasicBallScript;
	if (L"CBossMonsterScript" == _strScriptName)
		return new CBossMonsterScript;
	if (L"CButtonScript" == _strScriptName)
		return new CButtonScript;
	if (L"CCameraMoveScript" == _strScriptName)
		return new CCameraMoveScript;
	if (L"CCloudScript" == _strScriptName)
		return new CCloudScript;
	if (L"CConsumeScript" == _strScriptName)
		return new CConsumeScript;
	if (L"CCursorScript" == _strScriptName)
		return new CCursorScript;
	if (L"CDamageScript" == _strScriptName)
		return new CDamageScript;
	if (L"CDeadStateScript" == _strScriptName)
		return new CDeadStateScript;
	if (L"CDoubleJumpScript" == _strScriptName)
		return new CDoubleJumpScript;
	if (L"CExpScript" == _strScriptName)
		return new CExpScript;
	if (L"CGravityScript" == _strScriptName)
		return new CGravityScript;
	if (L"CGroundScript" == _strScriptName)
		return new CGroundScript;
	if (L"CHpScript" == _strScriptName)
		return new CHpScript;
	if (L"CIdleStateScript" == _strScriptName)
		return new CIdleStateScript;
	if (L"CInventoryScript" == _strScriptName)
		return new CInventoryScript;
	if (L"CItemScript" == _strScriptName)
		return new CItemScript;
	if (L"CLaunchSkillScript" == _strScriptName)
		return new CLaunchSkillScript;
	if (L"CMainBarScript" == _strScriptName)
		return new CMainBarScript;
	if (L"CMissileScript" == _strScriptName)
		return new CMissileScript;
	if (L"CMobGroundScript" == _strScriptName)
		return new CMobGroundScript;
	if (L"CMonsterFactoryScript" == _strScriptName)
		return new CMonsterFactoryScript;
	if (L"CMonsterScript" == _strScriptName)
		return new CMonsterScript;
	if (L"CMpScript" == _strScriptName)
		return new CMpScript;
	if (L"CNumberScript" == _strScriptName)
		return new CNumberScript;
	if (L"CPaperBurnScript" == _strScriptName)
		return new CPaperBurnScript;
	if (L"CPatrolStateScript" == _strScriptName)
		return new CPatrolStateScript;
	if (L"CPlayerScript" == _strScriptName)
		return new CPlayerScript;
	if (L"CRigidBodyScript" == _strScriptName)
		return new CRigidBodyScript;
	if (L"CSkillnearScript" == _strScriptName)
		return new CSkillnearScript;
	if (L"CSkillScript" == _strScriptName)
		return new CSkillScript;
	if (L"CStateScript" == _strScriptName)
		return new CStateScript;
	if (L"CTraceStateScript" == _strScriptName)
		return new CTraceStateScript;
	if (L"CWallScript" == _strScriptName)
		return new CWallScript;
	return nullptr;
}

CScript * CScriptMgr::GetScript(UINT _iScriptType)
{
	switch (_iScriptType)
	{
	case (UINT)SCRIPT_TYPE::AISCRIPT:
		return new CAIScript;
		break;
	case (UINT)SCRIPT_TYPE::AMERAPLAYERMOVESCRIPT:
		return new CameraPlayerMoveScript;
		break;
	case (UINT)SCRIPT_TYPE::ATTACKSTATESCRIPT:
		return new CAttackStateScript;
		break;
	case (UINT)SCRIPT_TYPE::BACKGROUNDSCRIPT:
		return new CBackGroundScript;
		break;
	case (UINT)SCRIPT_TYPE::BASICBALLSCRIPT:
		return new CBasicBallScript;
		break;
	case (UINT)SCRIPT_TYPE::BOSSMONSTERSCRIPT:
		return new CBossMonsterScript;
		break;
	case (UINT)SCRIPT_TYPE::BUTTONSCRIPT:
		return new CButtonScript;
		break;
	case (UINT)SCRIPT_TYPE::CAMERAMOVESCRIPT:
		return new CCameraMoveScript;
		break;
	case (UINT)SCRIPT_TYPE::CLOUDSCRIPT:
		return new CCloudScript;
		break;
	case (UINT)SCRIPT_TYPE::CONSUMESCRIPT:
		return new CConsumeScript;
		break;
	case (UINT)SCRIPT_TYPE::CURSORSCRIPT:
		return new CCursorScript;
		break;
	case (UINT)SCRIPT_TYPE::DAMAGESCRIPT:
		return new CDamageScript;
		break;
	case (UINT)SCRIPT_TYPE::DEADSTATESCRIPT:
		return new CDeadStateScript;
		break;
	case (UINT)SCRIPT_TYPE::DOUBLEJUMPSCRIPT:
		return new CDoubleJumpScript;
		break;
	case (UINT)SCRIPT_TYPE::EXPSCRIPT:
		return new CExpScript;
		break;
	case (UINT)SCRIPT_TYPE::GRAVITYSCRIPT:
		return new CGravityScript;
		break;
	case (UINT)SCRIPT_TYPE::GROUNDSCRIPT:
		return new CGroundScript;
		break;
	case (UINT)SCRIPT_TYPE::HPSCRIPT:
		return new CHpScript;
		break;
	case (UINT)SCRIPT_TYPE::IDLESTATESCRIPT:
		return new CIdleStateScript;
		break;
	case (UINT)SCRIPT_TYPE::INVENTORYSCRIPT:
		return new CInventoryScript;
		break;
	case (UINT)SCRIPT_TYPE::ITEMSCRIPT:
		return new CItemScript;
		break;
	case (UINT)SCRIPT_TYPE::LAUNCHSKILLSCRIPT:
		return new CLaunchSkillScript;
		break;
	case (UINT)SCRIPT_TYPE::MAINBARSCRIPT:
		return new CMainBarScript;
		break;
	case (UINT)SCRIPT_TYPE::MISSILESCRIPT:
		return new CMissileScript;
		break;
	case (UINT)SCRIPT_TYPE::MOBGROUNDSCRIPT:
		return new CMobGroundScript;
		break;
	case (UINT)SCRIPT_TYPE::MONSTERFACTORYSCRIPT:
		return new CMonsterFactoryScript;
		break;
	case (UINT)SCRIPT_TYPE::MONSTERSCRIPT:
		return new CMonsterScript;
		break;
	case (UINT)SCRIPT_TYPE::MPSCRIPT:
		return new CMpScript;
		break;
	case (UINT)SCRIPT_TYPE::NUMBERSCRIPT:
		return new CNumberScript;
		break;
	case (UINT)SCRIPT_TYPE::PAPERBURNSCRIPT:
		return new CPaperBurnScript;
		break;
	case (UINT)SCRIPT_TYPE::PATROLSTATESCRIPT:
		return new CPatrolStateScript;
		break;
	case (UINT)SCRIPT_TYPE::PLAYERSCRIPT:
		return new CPlayerScript;
		break;
	case (UINT)SCRIPT_TYPE::RIGIDBODYSCRIPT:
		return new CRigidBodyScript;
		break;
	case (UINT)SCRIPT_TYPE::SKILLNEARSCRIPT:
		return new CSkillnearScript;
		break;
	case (UINT)SCRIPT_TYPE::SKILLSCRIPT:
		return new CSkillScript;
		break;
	case (UINT)SCRIPT_TYPE::STATESCRIPT:
		return new CStateScript;
		break;
	case (UINT)SCRIPT_TYPE::TRACESTATESCRIPT:
		return new CTraceStateScript;
		break;
	case (UINT)SCRIPT_TYPE::WALLSCRIPT:
		return new CWallScript;
		break;
	}
	return nullptr;
}

const wchar_t * CScriptMgr::GetScriptName(CScript * _pScript)
{
	switch ((SCRIPT_TYPE)_pScript->GetScriptType())
	{
	case SCRIPT_TYPE::AISCRIPT:
		return L"CAIScript";
		break;

	case SCRIPT_TYPE::AMERAPLAYERMOVESCRIPT:
		return L"CameraPlayerMoveScript";
		break;

	case SCRIPT_TYPE::ATTACKSTATESCRIPT:
		return L"CAttackStateScript";
		break;

	case SCRIPT_TYPE::BACKGROUNDSCRIPT:
		return L"CBackGroundScript";
		break;

	case SCRIPT_TYPE::BASICBALLSCRIPT:
		return L"CBasicBallScript";
		break;

	case SCRIPT_TYPE::BOSSMONSTERSCRIPT:
		return L"CBossMonsterScript";
		break;

	case SCRIPT_TYPE::BUTTONSCRIPT:
		return L"CButtonScript";
		break;

	case SCRIPT_TYPE::CAMERAMOVESCRIPT:
		return L"CCameraMoveScript";
		break;

	case SCRIPT_TYPE::CLOUDSCRIPT:
		return L"CCloudScript";
		break;

	case SCRIPT_TYPE::CONSUMESCRIPT:
		return L"CConsumeScript";
		break;

	case SCRIPT_TYPE::CURSORSCRIPT:
		return L"CCursorScript";
		break;

	case SCRIPT_TYPE::DAMAGESCRIPT:
		return L"CDamageScript";
		break;

	case SCRIPT_TYPE::DEADSTATESCRIPT:
		return L"CDeadStateScript";
		break;

	case SCRIPT_TYPE::DOUBLEJUMPSCRIPT:
		return L"CDoubleJumpScript";
		break;

	case SCRIPT_TYPE::EXPSCRIPT:
		return L"CExpScript";
		break;

	case SCRIPT_TYPE::GRAVITYSCRIPT:
		return L"CGravityScript";
		break;

	case SCRIPT_TYPE::GROUNDSCRIPT:
		return L"CGroundScript";
		break;

	case SCRIPT_TYPE::HPSCRIPT:
		return L"CHpScript";
		break;

	case SCRIPT_TYPE::IDLESTATESCRIPT:
		return L"CIdleStateScript";
		break;

	case SCRIPT_TYPE::INVENTORYSCRIPT:
		return L"CInventoryScript";
		break;

	case SCRIPT_TYPE::ITEMSCRIPT:
		return L"CItemScript";
		break;

	case SCRIPT_TYPE::LAUNCHSKILLSCRIPT:
		return L"CLaunchSkillScript";
		break;

	case SCRIPT_TYPE::MAINBARSCRIPT:
		return L"CMainBarScript";
		break;

	case SCRIPT_TYPE::MISSILESCRIPT:
		return L"CMissileScript";
		break;

	case SCRIPT_TYPE::MOBGROUNDSCRIPT:
		return L"CMobGroundScript";
		break;

	case SCRIPT_TYPE::MONSTERFACTORYSCRIPT:
		return L"CMonsterFactoryScript";
		break;

	case SCRIPT_TYPE::MONSTERSCRIPT:
		return L"CMonsterScript";
		break;

	case SCRIPT_TYPE::MPSCRIPT:
		return L"CMpScript";
		break;

	case SCRIPT_TYPE::NUMBERSCRIPT:
		return L"CNumberScript";
		break;

	case SCRIPT_TYPE::PAPERBURNSCRIPT:
		return L"CPaperBurnScript";
		break;

	case SCRIPT_TYPE::PATROLSTATESCRIPT:
		return L"CPatrolStateScript";
		break;

	case SCRIPT_TYPE::PLAYERSCRIPT:
		return L"CPlayerScript";
		break;

	case SCRIPT_TYPE::RIGIDBODYSCRIPT:
		return L"CRigidBodyScript";
		break;

	case SCRIPT_TYPE::SKILLNEARSCRIPT:
		return L"CSkillnearScript";
		break;

	case SCRIPT_TYPE::SKILLSCRIPT:
		return L"CSkillScript";
		break;

	case SCRIPT_TYPE::STATESCRIPT:
		return L"CStateScript";
		break;

	case SCRIPT_TYPE::TRACESTATESCRIPT:
		return L"CTraceStateScript";
		break;

	case SCRIPT_TYPE::WALLSCRIPT:
		return L"CWallScript";
		break;

	}
	return nullptr;
}