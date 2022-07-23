#pragma once
#include "CStateScript.h"
#include "CBossMonsterScript.h"
#include "CMonsterScript.h"


class CAttackStateScript :
    public CStateScript
{
private:
    BOSS_ATTACK_STATE   m_eCurAttState;
    BOSS_ATTACK_STATE   m_ePrevAttState;

    float               m_fTimer;
    CPrefab*            m_pSkillPrefab1;
    int                 m_Skill1_Cnt;


private:
    bool                m_bAttackEnd; // Attack End 

public:
    void SetAttackEnd(bool _b) { m_bAttackEnd = _b; }


private:
    void UpdateBossMobAttack();



public:
    virtual void Enter();
    virtual void Exit();


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


    CLONE(CAttackStateScript)


public:
    CAttackStateScript();
    CAttackStateScript(const CAttackStateScript& _origin);
    virtual ~CAttackStateScript();

};

