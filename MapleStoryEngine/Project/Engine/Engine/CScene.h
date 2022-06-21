#pragma once
#include "CEntity.h"


#include "CSceneFile.h"


class CLayer;
class CGameObject;

class CScene :
    public CEntity
{
private:
    CLayer*         m_arrLayer[MAX_LAYER];
    SCENE_STATE     m_eSceneState;
    wstring         m_strResKey;


public:
    void start();
    void update();
    void lateupdate();
    void finalupdate();


public:
    void SetLayerName(int _iLayerIdx, const wstring& _strName);
    void AddObject(CGameObject* _pRootObj, const wstring& _strLayerName);
    void AddObject(CGameObject* _pRootObj, int _iLayerIdx);
    CLayer* GetLayer(int _iIdx) { assert(!(_iIdx < 0 || MAX_LAYER <= _iIdx));  return m_arrLayer[_iIdx]; }
    CLayer* GetLayer(const wstring& _strLayerName);


    void SetResKey(const wstring& _strKey) { m_strResKey = _strKey; }
    Ptr<CSceneFile> GetSceneFile();


    SCENE_STATE GetSceneState() { return m_eSceneState; }
    void SetSceneState(SCENE_STATE _eState);


    // ============= Todo ===============
    CLayer** GetAllLayer() { return m_arrLayer; }
    // ============= Todo ===============

    
private:
    int GetLayerIdxFromName(const wstring& _strName);

    CLONE(CScene)

public:
    CScene();
    ~CScene();
};

