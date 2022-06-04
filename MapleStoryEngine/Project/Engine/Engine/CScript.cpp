#include "pch.h"
#include "CScript.h"


CScript::CScript()
	: CComponent(COMPONENT_TYPE::SCRIPT)
	, m_iScriptID(-1)
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

