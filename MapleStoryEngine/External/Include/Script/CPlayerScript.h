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


private:
    Vec3            m_vForce;       // 크기 , 방향 
    Vec3            m_vAccel;       // 가속도  
    Vec3            m_vVelocity;    // 속도 ( 크기 : 속력 , 방향 )
 
    float           m_fMass;        // 질량 
    float            m_fMaxSpeed; // 최대 속력 
    float           m_fFricCoeff;   // 마찰 계수 

public:
    void AddForce(Vec3 _vF) { m_vForce += _vF; }
    void SetMass(float _fMass) { m_fMass = _fMass; }
    float GetMass() { return m_fMass; }

    void SetVelocity(Vec3 _v) { m_vVelocity = _v; }
    void SetMaxVelocity(float _v) { m_fMaxSpeed = _v; }


public:
    virtual void start();
    virtual void update();
    virtual void lateupdate();   

    virtual void OnCollisionEnter(CGameObject* _OtherObject) override;

public:
    // for state
    void Update_State();
    void Update_Move();
    void Update_Animation();



private:
    void Burnning();

private:
    void Move();

public:
    virtual void SaveToScene(FILE* _pFile) override;
    virtual void LoadFromScene(FILE* _pFile) override;


    CLONE(CPlayerScript)

public:
    CPlayerScript();
    ~CPlayerScript();

};

