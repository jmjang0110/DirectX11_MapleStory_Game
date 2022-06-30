#pragma once

#include <Engine/CScript.h>


enum class PLAYER_DIRECTION
{
    UP,
    DOWN,
    RIGHT,
    LEFT,

};

enum class PLAYER_STATE
{
    IDLE,
    WALK,
    ATTACK,
    JUMP,
    ALERT,
    DEAD,
};



enum class PLAYER_ATTACK_STATE
{
    NORMAL_ATT_1,
    NORMAL_ATT_2,
    NORMAL_ATT_3,

    SKILL_ATT_1,
    // ......

};


class CPlayerScript :
    public CScript
{
    // for state
private:
    PLAYER_DIRECTION    m_eDir;
    
    PLAYER_STATE        m_eCurState;
    PLAYER_STATE        m_ePrevState;


private:
    Ptr<CPrefab>    m_pMissilePrefab;
    float           m_fSpeed;

    float           m_fBurnStrength;
    bool            m_bBurn;


private:
    bool        m_bOnGround;

public:
    void SetOnGround(bool _b) { m_bOnGround = _b; }
    bool GetOnGround() { return m_bOnGround; }


public:
    virtual void start();
    virtual void update();
    virtual void lateupdate();   

    virtual void OnCollisionEnter(CGameObject* _OtherObject) override;

public:
    void Update_Move();
    void Update_Gravity();
    void Update_State();
    void Update_Animation();


private:
    void Burnning();



public:
    virtual void SaveToScene(FILE* _pFile) override;
    virtual void LoadFromScene(FILE* _pFile) override;


    CLONE(CPlayerScript)

public:
    CPlayerScript();
    CPlayerScript(const CPlayerScript& _origin);
    ~CPlayerScript();

};

