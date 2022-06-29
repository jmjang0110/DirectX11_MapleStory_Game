#include "pch.h"
#include "CScript.h"




CScript::CScript(int _iScriptType)
	: CComponent(COMPONENT_TYPE::SCRIPT)
	, m_iScriptID(_iScriptType)
{
}

CScript::~CScript()
{
}


void CScript::SaveToScene(FILE* _pFile)
{

}

void CScript::LoadFromScene(FILE* _pFile)
{


}