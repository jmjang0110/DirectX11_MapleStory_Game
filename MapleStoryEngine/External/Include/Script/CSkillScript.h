#pragma once
#include <Engine/CScript.h>


class CGameObject;

enum class SKILL_STATE
{
    PREPARE,
    KEYDOWN,
    SKILLEND,

    END, 
};


class CSkillScript :
    public CScript
{
private:
    KEY                 m_eKey;
    SKILL_STATE         m_eState;

    // Skill prefab
    CGameObject*        m_PrepareObj;
    CGameObject*        m_KeyDownObj;
    CGameObject*        m_EndObj;

public:
    void SetKey(KEY _key) { m_eKey = _key; }
    void SetSkillObjByPrefab(CPrefab* _prepare = nullptr
        , CPrefab* _keydown = nullptr, CPrefab* _end = nullptr);
    void UpdatePos();


private:
    void Prepare();
    void KeyDown();
    void SkillEnd();


public:
    virtual void start();
    virtual void update();
    virtual void lateupdate();

    virtual void OnCollisionEnter(CGameObject* _OtherObject) override;
    virtual void OnCollision(CGameObject* _OtherObject) override;
    virtual void OnCollisionExit(CGameObject* _OtherObject) override;



public:
    virtual void SaveToScene(FILE* _pFile) override;
    virtual void LoadFromScene(FILE* _pFile) override;


    CLONE(CSkillScript)


public:
    CSkillScript();
    CSkillScript(const CSkillScript& _origin);
    ~CSkillScript();
};

