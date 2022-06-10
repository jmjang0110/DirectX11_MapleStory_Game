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

