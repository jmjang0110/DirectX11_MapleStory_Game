#pragma once
#include <Engine/CScript.h>

/*
        * Only For "Near" Range Skill *
*/

class CGameObject;
class CPrefab;



class CSkillnearScript :
    public CScript
{
private:
    bool                m_bDelete;
    KEY                 m_eKey;

    CGameObject*        m_SkillObj;
    CGameObject*        m_pSkillUser; // Skill 을 사용하는 User

public:
    void SetUser(CGameObject* _pUser) { m_pSkillUser = _pUser; }
    void SetKey(KEY _key) { m_eKey = _key; }
    void SetSkillObjByPrefab(CPrefab* _Pref = nullptr, wstring _SkillName = L"defaultSkill");


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


    CLONE(CSkillnearScript)


public:
    CSkillnearScript();
    CSkillnearScript(const CSkillnearScript& _origin);
    ~CSkillnearScript();


};

