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
    class CAIScript*    m_pAI;
    tMonsterInfo        m_tInfo;
    MONSTER_DIRECTION   m_eDir;
    MONSTER_DIRECTION   m_ePrevDir;

    MONSTER_STATE       m_ePrevStateType;
    MONSTER_STATE       m_eCurStateType;

  
private:
    int             m_iHitCnt;

    float           m_fDiff_Between_Ground;
    Vec3            m_vPrevPos;



public:
    float               GetSpeed() { return m_tInfo.fSpeed; }
    MONSTER_DIRECTION   GetDir() { return m_eDir; }
    tMonsterInfo        GetMonsterInfo() { return m_tInfo; }

private:
    void    SetSpeed(float _f) { m_tInfo.fSpeed = _f; }
    void    SetAIScript(class CAIScript* _pAI) { m_pAI = _pAI; }
    void    SetMonsterInfo(const tMonsterInfo& _info) { m_tInfo = _info; }
    void    SetDir(MONSTER_DIRECTION _dir) { m_eDir = _dir; }


public:

    Vec3 GetPrevPos() { return m_vPrevPos; }


public:
    void SetDiffBetweenGround(float _fDiff) { m_fDiff_Between_Ground = _fDiff; }
    float GetDiffBetweenGround() { return m_fDiff_Between_Ground; }

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



};

