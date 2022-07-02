#pragma once

class CScene;
class CGameObject;
class CPrefab;

class CSceneSaveLoad
{
public:
	// Save
	static void SaveScene(CScene* _pScene, const wstring& _strSceneFilePath);
	static void SaveGameObject(CGameObject* _pObj, FILE* _pFile);
	//static void SavePrefab(CGameObject* _pProtoObj, CPrefab* _pPrefab, FILE* _pFile);

	static void SavePrefab(CPrefab* _Prefab, const wstring& _strFilePath);


	// Load
	static CScene* LoadScene(const wstring& _strSceneFilePath);
	static CGameObject* LoadGameObject(FILE* _pFile);
	//static CPrefab* LoadPrefab(FILE* _pFile);

	static int LoadPrefab(CPrefab* _Prefab, const wstring& _strFilePath);



};

