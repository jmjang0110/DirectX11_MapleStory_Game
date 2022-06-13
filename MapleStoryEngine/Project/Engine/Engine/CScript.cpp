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


void CScript::SaveToFile(FILE* _pFile)
{
	CComponent::SaveToFile(_pFile);

	//fwrite(&m_iScriptID, sizeof(int), 1, _pFile);


}

void CScript::LoadFromFile(FILE* _pFile)
{
	CComponent::LoadFromFile(_pFile,true);
	//fread(&m_iScriptID, sizeof(int), 1, _pFile);

}

