#pragma once
#include "UI.h"

#include <Engine/CGameObject.h>

class ComponentUI;
class ResInfoUI;
class CRes;

class InspectorUI :
    public UI
{
private:
    CGameObject* m_pTargetObject;
    CRes* m_pTargetRes;

    ComponentUI* m_arrComUI[(UINT)COMPONENT_TYPE::END];
    ResInfoUI* m_arrResUI[(UINT)RES_TYPE::END];


public:
    void SetTargetObject(CGameObject* _pTarget);
    void SetTargetResource(CRes* _pTargetRes);

    // ============= ToDo 
    const CGameObject* GetTargetObject() { return m_pTargetObject; }
     CRes* GetTargetRes() { return m_pTargetRes; }

public:
    virtual void update() override;
    virtual void render_update() override;




public:
    InspectorUI();
    ~InspectorUI();
};

