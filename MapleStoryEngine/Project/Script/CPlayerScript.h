#pragma once

#include <Engine/CScript.h>


enum class PLAYER_DIRECTION
{
    UP,
    DOWN,
    RIGHT,
    LEFT,

    END,

};


enum class PLAYER_STATE
{
    IDLE,
    WALK,
    JUMP,
    ALERT,
    ATTACK,
    DEAD,

    END,

};

enum class PLAYER_ATTACK_STATE
{
    NORMAL_ATT_1,
    NORMAL_ATT_2,
    NORMAL_ATT_3,

    SKILL_ATT_1,
    // ......

    END,

};

enum class PLAYER_JOB
{
    ARCHER,
    END,
};


class CPlayerScript :
    public CScript
{
    // for state
private:
    PLAYER_DIRECTION    m_eDir;
    PLAYER_DIRECTION    m_ePrevDir;

    PLAYER_DIRECTION    m_eMoveDir; // LEFT RIGHT ∏∏ ¿˙¿Â 
    
    PLAYER_STATE        m_eCurState;
    PLAYER_STATE        m_ePrevState;


private:
    //Ptr<CPrefab>    m_pMissilePrefab;
    float           m_fSpeed;
    bool            m_bOnGround;

private:
    int             m_iLevel;

    float           m_fHP;
    float           m_fMP;
    float           m_fEXP;

    Vec3            m_vPrevPos;


public:
    Vec3 GetPrevPos() { return m_vPrevPos; }
    void SetOnGround(bool _b) { m_bOnGround = _b; }
    bool GetOnGround() { return m_bOnGround; }
    void SetCurDir(PLAYER_DIRECTION _eDir) { m_eDir = _eDir; }

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


    CLONE(CPlayerScript)

public:
    CPlayerScript();
    CPlayerScript(const CPlayerScript& _origin);
    ~CPlayerScript();

};

