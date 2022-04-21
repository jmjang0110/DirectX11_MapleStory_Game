#pragma once

class CScene;
class CGameObject;

class CSceneMgr
	: public CSingleton<CSceneMgr>
{	
	friend class CSingleton<CSceneMgr>;
private:
	CSceneMgr();
	~CSceneMgr();


private:
	CScene*		m_pCurScene;

public:
	CScene* GetCurScene() { return m_pCurScene; }

	void SpawnObject(CGameObject* _pSpawnObject, Vec3 _vWorldPos, wstring _strName, UINT _iLayerIdx);
	void SpawnObject(CGameObject* _pSpawnObject, UINT _iLayerIdx);

	void AddChild(CGameObject* _pParentObject, CGameObject* _pChildObject);
	void ClearLayer();


public:
	void init();
	void progress();
	void render();


public:
	// ----- LOAD TEXTURES -----
	void LoadTextures();

	// ----- MAKE RENDER OBJECT -----
	void AddMainPlayerObj();
	void AddMonsterObj();

	void AddTauromacisObj();



	// ----- MAKE TOOL OBJECT ------
	void AddCameraObj();


	// ----- MAKE PREFAB -----
	void AddMissilePrefab();




};

