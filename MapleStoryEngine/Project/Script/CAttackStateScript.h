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
    int                 m_Skill1_Cnt;


private:
    bool                m_bAttackEnd; // Attack End 
    bool                m_bAddSubSkill;
    bool                m_bHitPlayer;


public:
    void SetAttackEnd(bool _b) { m_bAttackEnd = _b; }
    void AddPapulatusSkill(BOSS_ATTACK_STATE _eState);
    void ShootPlayer(BOSS_ATTACK_STATE _eState);


private:
    void UpdateBossMobAttack();
    void UpdateMonsterAttack();

private:
    void ShootGiganticBall();
    void ShootTanatosBall();


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

