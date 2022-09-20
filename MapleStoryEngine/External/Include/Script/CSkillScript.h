#pragma once
#include <Engine/CScript.h>
#include "CBasicBallScript.h"


/*

        * Only For "Far" Range Skill *

*/
class CGameObject;
class CPrefab;

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
    Ptr<CSound>         m_pPrepareBgm;
    Ptr<CSound>         m_pKeyDownBgm;
    Ptr<CSound>         m_pEndBgm;

    bool m_bPrepareBgmStart;
    bool m_bEndBgmStart;


private:
    bool                m_bOffsetY;
    float               m_fArrowTimer;
    float               m_fMaxArrowTimer;
    wstring             m_wstrBallPrefabName;

    float               m_fTimer;



    KEY                 m_eKey;
    SKILL_STATE         m_eState;

    // Skill prefab
    CGameObject*        m_PrepareObj;
    CGameObject*        m_KeyDownObj;
    CGameObject*        m_EndObj;

    CGameObject*        m_pSkillUser; // Skill 을 사용하는 User 

    CPrefab*            m_ArrowPrefab;

    BALLMOVE_TYPE       m_eBallMoveType;


public:
    void SetKey(KEY _key) { m_eKey = _key; }
    void SetSkillObjByPrefab(CPrefab* _prepare = nullptr
        , CPrefab* _keydown = nullptr, CPrefab* _end = nullptr);
    void SetUser(CGameObject* _pUser) { m_pSkillUser = _pUser; }
    void SetBallName(wstring _name) { m_wstrBallPrefabName = _name; }
    void SetBallMoveType(BALLMOVE_TYPE _type) { m_eBallMoveType = _type; }
    void SetOffsetY(bool _b) { m_bOffsetY = _b; }

    void UpdatePos();
    void AttackArrow();


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

