#pragma once

#include <vector>
#include <string>

enum class SCRIPT_TYPE
{
	AISCRIPT,
	AMERAPLAYERMOVESCRIPT,
	ATTACKSTATESCRIPT,
	BACKGROUNDSCRIPT,
	BASICBALLSCRIPT,
	CAMERAMOVESCRIPT,
	CURSORSCRIPT,
	DAMAGESCRIPT,
	DEADSTATESCRIPT,
	DOUBLEJUMPSCRIPT,
	GRAVITYSCRIPT,
	GROUNDSCRIPT,
	IDLESTATESCRIPT,
	MISSILESCRIPT,
	MONSTERFACTORYSCRIPT,
	MONSTERSCRIPT,
	PAPERBURNSCRIPT,
	PATROLSTATESCRIPT,
	PLAYERSCRIPT,
	RIGIDBODYSCRIPT,
	STATESCRIPT,
	TRACESTATESCRIPT,
	END,
};

using namespace std;

class CScript;

class CScriptMgr
{
public:
	static void GetScriptInfo(vector<wstring>& _vec);
	static CScript * GetScript(const wstring& _strScriptName);
	static CScript * GetScript(UINT _iScriptType);
	static const wchar_t * GetScriptName(CScript * _pScript);
};
