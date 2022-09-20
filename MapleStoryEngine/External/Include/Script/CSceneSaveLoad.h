#pragma once

class CScene;
class CLayer;
class CGameObject;
class CPrefab;

class CSceneSaveLoad
{
public:
	// Scene/Default/SceneManager ( CSceneStartSCript 에서 초기화 ) 
	static CGameObject* pSceneManagerObj;
	static class CSceneStartScript* pSceneMgrScript;
	static CGameObject* pMainPlayer;
	static CGameObject* pMainCamera;


	static wstring LoadSceneName;


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

	// == todo === 
	static void SaveLayer(CLayer* _pLayer, const wstring& _strLayerFilePath);
	static CLayer* LoadLayer(const wstring& _strLayerFIlePath);

	// == == == ==


public:
	// Load 된 Scene 에 Player를 Load 한 후 Player Layer 에 등록 
	static void LoadMainPlayer(CScene* _pScene);
	static void LoadInventory(CScene* _pScene);
	static void LoadMainBar(CScene* _pScene);


};

