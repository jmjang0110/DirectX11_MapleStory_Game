#pragma once
#include "UI.h"

#include <Engine/CGameObject.h>

class ScriptUI :
    public UI
{
private:
    CGameObject*    m_pTargetObject;
    CScript*        m_pTargetScript;
    COMPONENT_TYPE  m_eComType;

    bool            m_bActive;

public:
    void SetTargetObject(CGameObject* _pTarget) { m_pTargetObject = _pTarget; }
    CGameObject* GetTargetObject() { return m_pTargetObject; }

    void SetTargetScript(CScript* _pScript) { m_pTargetScript = _pScript; }
    CScript* GetTargetScript() { return m_pTargetScript; }

protected:
    bool IsComponentActive() { return m_bActive; }

public:
    virtual void update() override;
    virtual void render_update() override;


public:
    ScriptUI();
    ~ScriptUI();
};

