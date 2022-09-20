#pragma once
#include <Engine/CScript.h>
#include "CStateScript.h"

enum class BOSS_DIRECTION
{
    UP,
    DOWN,
    RIGHT,
    LEFT,
    NONE,

};

enum class BOSS_ATTACK_STATE
{
    NORMAL_ATTACK1,
    NORMAL_ATTACK2,
    NORMAL_ATTACK3,
    NORMAL_ATTACK4,

    SKILL_ATTACK1,
    SKILL_ATTACK2,
    SKILL_ATTACK3,
    SKILL_ATTACK4,
    SKILL_ATTACK5,
    SKILL_ATTACK6,

    
    END,

};

struct tBossMonInfo
{
    float      fSpeed;           // �ӵ�
    float      fHP;              // ü�� 
    float       fMaxHP;


    float      fRecogRange;      // �ν� ���� 
    float      fAttRange;        // ���� ����
    float      fAtt;             // ���ݷ�
    float      fDef;             // ���� 
};


class CBossMonsterScript :
    public CScript
{
private:
    class CAIScript*    m_pAI;
    tBossMonInfo        m_tInfo;


    // cur
    MONSTER_STATE       m_eCurStateType;
    BOSS_DIRECTION      m_eDir;
    BOSS_ATTACK_STATE   m_eAttackState;
    Vec3                m_vCurPos;

    // Prev 
    MONSTER_STATE       m_ePrevStateType;
    BOSS_DIRECTION      m_ePrevDir;
    BOSS_ATTACK_STATE   m_ePrevAttackState;
    Vec3                m_vPrevPos;


private:
    int             m_iHitCnt;
    bool            m_bAttackEnd;
    bool            m_bDie;
    float           m_fTimer;


public:
    bool                GetDie() { return m_bDie; }

    float               GetSpeed() { return m_tInfo.fSpeed; }
    BOSS_DIRECTION      GetDir() { return m_eDir; }
    tBossMonInfo        GetMonsterInfo() { return m_tInfo; }
    bool                GetAttackEnd() { return m_bAttackEnd; }
    void                SetPrevPos(Vec3 _prevPos) { m_vPrevPos = _prevPos; }


    void    SetSpeed(float _f) { m_tInfo.fSpeed = _f; }
    void    SetAIScript(class CAIScript* _pAI) { m_pAI = _pAI; }
    void    SetMonsterInfo(const tBossMonInfo& _info) { m_tInfo = _info; }
    void    SetDir(BOSS_DIRECTION _dir) { m_eDir = _dir; }
    void    SetBossAttState(BOSS_ATTACK_STATE _estate) { m_eAttackState = _estate; }

    void SubHP(float damage) { m_tInfo.fHP -= damage; if (m_tInfo.fHP <= 0.f) { m_eCurStateType = MONSTER_STATE::DEAD;  m_bDie = true; } }


public:
    virtual void start();
    virtual void update();
    virtual void lateupdate();

    virtual void OnCollisionEnter(CGameObject* _OtherObject) override;
    virtual void OnCollision(CGameObject* _OtherObject) override;
    virtual void OnCollisionExit(CGameObject* _OtherObject) override;


public:
    void Update_State();
    void Update_Move();
    void Update_Gravity();
    void Update_Animation();

public:
    virtual void SaveToScene(FILE* _pFile) override;
    virtual void LoadFromScene(FILE* _pFile) override;


    CLONE(CBossMonsterScript)


public:
    CBossMonsterScript();
    CBossMonsterScript(const CBossMonsterScript& _origin);
    ~CBossMonsterScript();

};

