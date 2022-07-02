#pragma once
#include "UI.h"

#include <Engine/CGameObject.h>

class ComponentUI;
class ResInfoUI;
class CRes;
class ScriptUI;
class CSCene;
class CLayer;


class InspectorUI :
    public UI
{
private:
    CGameObject*        m_pTargetObject;
    CRes*               m_pTargetRes;

    ComponentUI*        m_arrComUI[(UINT)COMPONENT_TYPE::END];
    ResInfoUI*          m_arrResUI[(UINT)RES_TYPE::END];

    vector<ScriptUI*>   m_vecScriptUI;

private:
    // == Todo == 
    CScene* m_pTargetScene;
    CLayer* m_pTargetLayer;

    CLayer* m_pColLeftLayer;
    CLayer* m_pColRightLayer;
    // ==========


public:
    void SetTargetObject(CGameObject* _pTarget);
    void SetTargetResource(CRes* _pTargetRes);


private:
    ScriptUI* AddScriptUI();


public:
    virtual void update() override;
    virtual void render_update() override;



// ============= ToDo ===========================
public:
    const CGameObject* GetTargetObject() { return m_pTargetObject; }
     CRes* GetTargetRes() { return m_pTargetRes; }
     
     void AddComponent(DWORD_PTR _param);
     void AddScript(DWORD_PTR _param);
     
     void DeleteComponent(DWORD_PTR _param);
     void DeleteScript(DWORD_PTR _param);

private:
    void GameObjectTool_SubFunc();
    void SceneTool_subFunc();
    void LayerTool_subFunc();

public:
    void SetTargetScene(CScene* _scene) { m_pTargetScene = _scene; }
    void SetTargetLayer(CLayer* _layer) { m_pTargetLayer = _layer; }

    void SetCollisionCheck();
    void ShowCollisionLayer();

    bool IsCollision(CCollider2D* _pLeftCol, CCollider2D* _pRightCol);
    bool IsCollision_Box(CCollider2D* _pLeftCol, CCollider2D* _pRightCol);
    bool IsCollision_Circle(CCollider2D* _pLeftCol, CCollider2D* _pRightCol);

// ============================================

public:
    InspectorUI();
    ~InspectorUI();
};

