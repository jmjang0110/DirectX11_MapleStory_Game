#pragma once
#include <Engine/CScript.h>
#include "CStateScript.h"

enum class MONSTER_DIRECTION
{
    UP,
    DOWN,
    RIGHT,
    LEFT,

    END,

};

struct tMonsterInfo
{
    float      fSpeed;           // 속도
    float      fHP;              // 체력 

    float      fRecogRange;      // 인식 범위 
    float      fAttRange;        // 공격 범위
    float      fAtt;             // 공격력
    float      fDef;             // 방어력 


};

class CMonsterScript :
    public CScript
{
private:
    class CMobGroundScript* m_pMobGround; // 자신이 속한 GroundScript 
    class CAIScript* m_pAI;
    tMonsterInfo        m_tInfo;


    // cur
    MONSTER_STATE       m_eCurStateType;
    MONSTER_DIRECTION   m_eDir;
    Vec3                m_vCurPos;

    // Prev 
    MONSTER_STATE       m_ePrevStateType;
    MONSTER_DIRECTION   m_ePrevDir;
    Vec3                m_vPrevPos;

  
private:
    int             m_iHitCnt;
    bool            m_bAttackEnd;
    bool            m_bDie;


    float           m_fDiff_Between_Ground;
 
public:
    float               GetSpeed() { return m_tInfo.fSpeed; }
    MONSTER_DIRECTION   GetDir() { return m_eDir; }
    tMonsterInfo        GetMonsterInfo() { return m_tInfo; }
    bool                GetAttackEnd() { return m_bAttackEnd; }
    void                SetPrevPos(Vec3 _prevPos) { m_vPrevPos = _prevPos; }
    void                SetMobGroundScript(CMobGroundScript* _MGScript) { m_pMobGround = _MGScript; }
public:
    void SetDiffBetweenGround(float _fDiff) { m_fDiff_Between_Ground = _fDiff; }
    float GetDiffBetweenGround() { return m_fDiff_Between_Ground; }

public:
    void    SetSpeed(float _f) { m_tInfo.fSpeed = _f; }
    void    SetAIScript(class CAIScript* _pAI) { m_pAI = _pAI; }
    void    SetMonsterInfo(const tMonsterInfo& _info) { m_tInfo = _info; }
    void    SetDir(MONSTER_DIRECTION _dir) { m_eDir = _dir; }


public:

    Vec3 GetPrevPos() { return m_vPrevPos; }

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


    CLONE(CMonsterScript)


public:
    CMonsterScript();
    CMonsterScript(const CMonsterScript& _origin);
    ~CMonsterScript();


    friend class CSceneStartScript;

};

