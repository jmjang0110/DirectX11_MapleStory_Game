#include "pch.h"
#include "CScriptMgr.h"

#include "CAIScript.h"
#include "CameraPlayerMoveScript.h"
#include "CBackGroundScript.h"
#include "CCameraMoveScript.h"
#include "CCursorScript.h"
#include "CDamageScript.h"
#include "CGravityScript.h"
#include "CGroundScript.h"
#include "CIdleStateScript.h"
#include "CMissileScript.h"
#include "CMonsterFactoryScript.h"
#include "CMonsterScript.h"
#include "CPaperBurnScript.h"
#include "CPlayerScript.h"
#include "CRigidBodyScript.h"
#include "CStateScript.h"
#include "CTraceStateScript.h"

void CScriptMgr::GetScriptInfo(vector<wstring>& _vec)
{
	_vec.push_back(L"CAIScript");
	_vec.push_back(L"CameraPlayerMoveScript");
	_vec.push_back(L"CBackGroundScript");
	_vec.push_back(L"CCameraMoveScript");
	_vec.push_back(L"CCursorScript");
	_vec.push_back(L"CDamageScript");
	_vec.push_back(L"CGravityScript");
	_vec.push_back(L"CGroundScript");
	_vec.push_back(L"CIdleStateScript");
	_vec.push_back(L"CMissileScript");
	_vec.push_back(L"CMonsterFactoryScript");
	_vec.push_back(L"CMonsterScript");
	_vec.push_back(L"CPaperBurnScript");
	_vec.push_back(L"CPlayerScript");
	_vec.push_back(L"CRigidBodyScript");
	_vec.push_back(L"CStateScript");
	_vec.push_back(L"CTraceStateScript");
}

CScript * CScriptMgr::GetScript(const wstring& _strScriptName)
{
	if (L"CAIScript" == _strScriptName)
		return new CAIScript;
	if (L"CameraPlayerMoveScript" == _strScriptName)
		return new CameraPlayerMoveScript;
	if (L"CBackGroundScript" == _strScriptName)
		return new CBackGroundScript;
	if (L"CCameraMoveScript" == _strScriptName)
		return new CCameraMoveScript;
	if (L"CCursorScript" == _strScriptName)
		return new CCursorScript;
	if (L"CDamageScript" == _strScriptName)
		return new CDamageScript;
	if (L"CGravityScript" == _strScriptName)
		return new CGravityScript;
	if (L"CGroundScript" == _strScriptName)
		return new CGroundScript;
	if (L"CIdleStateScript" == _strScriptName)
		return new CIdleStateScript;
	if (L"CMissileScript" == _strScriptName)
		return new CMissileScript;
	if (L"CMonsterFactoryScript" == _strScriptName)
		return new CMonsterFactoryScript;
	if (L"CMonsterScript" == _strScriptName)
		return new CMonsterScript;
	if (L"CPaperBurnScript" == _strScriptName)
		return new CPaperBurnScript;
	if (L"CPlayerScript" == _strScriptName)
		return new CPlayerScript;
	if (L"CRigidBodyScript" == _strScriptName)
		return new CRigidBodyScript;
	if (L"CStateScript" == _strScriptName)
		return new CStateScript;
	if (L"CTraceStateScript" == _strScriptName)
		return new CTraceStateScript;
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
	case (UINT)SCRIPT_TYPE::BACKGROUNDSCRIPT:
		return new CBackGroundScript;
		break;
	case (UINT)SCRIPT_TYPE::CAMERAMOVESCRIPT:
		return new CCameraMoveScript;
		break;
	case (UINT)SCRIPT_TYPE::CURSORSCRIPT:
		return new CCursorScript;
		break;
	case (UINT)SCRIPT_TYPE::DAMAGESCRIPT:
		return new CDamageScript;
		break;
	case (UINT)SCRIPT_TYPE::GRAVITYSCRIPT:
		return new CGravityScript;
		break;
	case (UINT)SCRIPT_TYPE::GROUNDSCRIPT:
		return new CGroundScript;
		break;
	case (UINT)SCRIPT_TYPE::IDLESTATESCRIPT:
		return new CIdleStateScript;
		break;
	case (UINT)SCRIPT_TYPE::MISSILESCRIPT:
		return new CMissileScript;
		break;
	case (UINT)SCRIPT_TYPE::MONSTERFACTORYSCRIPT:
		return new CMonsterFactoryScript;
		break;
	case (UINT)SCRIPT_TYPE::MONSTERSCRIPT:
		return new CMonsterScript;
		break;
	case (UINT)SCRIPT_TYPE::PAPERBURNSCRIPT:
		return new CPaperBurnScript;
		break;
	case (UINT)SCRIPT_TYPE::PLAYERSCRIPT:
		return new CPlayerScript;
		break;
	case (UINT)SCRIPT_TYPE::RIGIDBODYSCRIPT:
		return new CRigidBodyScript;
		break;
	case (UINT)SCRIPT_TYPE::STATESCRIPT:
		return new CStateScript;
		break;
	case (UINT)SCRIPT_TYPE::TRACESTATESCRIPT:
		return new CTraceStateScript;
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

	case SCRIPT_TYPE::BACKGROUNDSCRIPT:
		return L"CBackGroundScript";
		break;

	case SCRIPT_TYPE::CAMERAMOVESCRIPT:
		return L"CCameraMoveScript";
		break;

	case SCRIPT_TYPE::CURSORSCRIPT:
		return L"CCursorScript";
		break;

	case SCRIPT_TYPE::DAMAGESCRIPT:
		return L"CDamageScript";
		break;

	case SCRIPT_TYPE::GRAVITYSCRIPT:
		return L"CGravityScript";
		break;

	case SCRIPT_TYPE::GROUNDSCRIPT:
		return L"CGroundScript";
		break;

	case SCRIPT_TYPE::IDLESTATESCRIPT:
		return L"CIdleStateScript";
		break;

	case SCRIPT_TYPE::MISSILESCRIPT:
		return L"CMissileScript";
		break;

	case SCRIPT_TYPE::MONSTERFACTORYSCRIPT:
		return L"CMonsterFactoryScript";
		break;

	case SCRIPT_TYPE::MONSTERSCRIPT:
		return L"CMonsterScript";
		break;

	case SCRIPT_TYPE::PAPERBURNSCRIPT:
		return L"CPaperBurnScript";
		break;

	case SCRIPT_TYPE::PLAYERSCRIPT:
		return L"CPlayerScript";
		break;

	case SCRIPT_TYPE::RIGIDBODYSCRIPT:
		return L"CRigidBodyScript";
		break;

	case SCRIPT_TYPE::STATESCRIPT:
		return L"CStateScript";
		break;

	case SCRIPT_TYPE::TRACESTATESCRIPT:
		return L"CTraceStateScript";
		break;

	}
	return nullptr;
}