#pragma once

class CScene;
class CGameObject;

class CSceneSaveLoad
{
public:
	// Save
	static void SaveScene(CScene* _pScene, const wstring& _strSceneFilePath);
	static void SaveGameObject(CGameObject* _pObj, FILE* _pFile);

	// Load
	static CScene* LoadScene(const wstring& _strSceneFilePath);
	static CGameObject* LoadGameObject(FILE* _pFile);
};

