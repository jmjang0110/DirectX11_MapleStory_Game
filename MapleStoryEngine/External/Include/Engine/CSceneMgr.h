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
	void ChangeScene(CScene* _pNextScene);

	void SpawnObject(CGameObject* _pSpawnObject, Vec3 _vWorldPos, wstring _strName, UINT _iLayerIdx);
	void SpawnObject(CGameObject* _pSpawnObject, UINT _iLayerIdx);
	// ===== Todo =====
	void DestroyObject(CGameObject* _pDestroyObject);
	void DeRegisterObjInLayer(CGameObject* _pDeObj, int _LayerIdx);
	void RegisterObhInLayer(CGameObject* _pObj, int _LayerIdx);
	void SwapLayer(int _Lidx, int _Ridx);
	//===== ===== =====
	void AddChild(CGameObject* _pParentObject, CGameObject* _pChildObject);	
	void DisconnectParent(CGameObject* _pObject);
	void ClearLayer();

	CGameObject* FindObjectByName(const wstring& _strName);
	void FindObjectsByName(wstring& _strName, vector<CGameObject*>& _vecOut);

public:
	void init();
	void progress();	


private:
	void AddPlayer();
	void Add_MapleStory_Player();



};

