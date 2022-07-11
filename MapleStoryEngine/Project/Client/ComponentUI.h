#pragma once
#include "UI.h"

#include <Engine/CGameObject.h>

class ComponentUI :
    public UI
{
private:
    CGameObject*        m_pTargetObject;
    COMPONENT_TYPE      m_eComType;


    bool                m_bActive;
    

    // === Todo 
    bool                m_bDel;

public:
    void SetTargetObject(CGameObject* _pTarget) {
        int i = 0;
    m_pTargetObject = _pTarget; }
    CGameObject* GetTargetObject() { return m_pTargetObject; }

    // ===== Todo 
    COMPONENT_TYPE GetComponentType() { return m_eComType; }


protected:
    bool IsComponentActive() { return m_bActive; }

public:
    virtual void update() override;
    virtual void render_update() override;


public:
    ComponentUI(const string& _strName, COMPONENT_TYPE _eComType);
    ~ComponentUI();
};

