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
    PRONE,
    JUMP,
    DOUBLE_JUMP,
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
    SKILL_ATT_2,
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
    PLAYER_ATTACK_STATE m_eCurAttState;
    PLAYER_ATTACK_STATE m_ePrevAttState;



private:
    //Ptr<CPrefab>    m_pMissilePrefab;
    float           m_fSpeed;
    bool            m_bOnGround;
    float           m_fDiff_Between_Ground;

private:
    int             m_iLevel;

    float           m_fHP;
    float           m_fMP;
    float           m_fEXP;

    float           m_fMinAttack;
    float           m_fMaxAttack;


    Vec3            m_vPrevPos;



public:
    Vec3 GetPrevPos() { return m_vPrevPos; }
    PLAYER_DIRECTION GetDir() { return m_eDir; }
    void SetOnGround(bool _b) { m_bOnGround = _b; }
    bool GetOnGround() { return m_bOnGround; }
    void SetCurDir(PLAYER_DIRECTION _eDir) { m_eDir = _eDir; }
    void SetDiffBetweenGround(float _fDiff) { m_fDiff_Between_Ground = _fDiff; }
    float GetDiffBetweenGround() { return m_fDiff_Between_Ground; }


    float GetMaxAttack() { return m_fMaxAttack; }
    float GetMinAttack() { return m_fMinAttack; }

    float GetExp() { return m_fEXP; }
    float GetHp() { return m_fHP; }
    float GetMp() { return m_fMP; }

public:
    void AddHp(float _fhp) { m_fHP += _fhp; }
    void AddMp(float _fmp) { m_fMP += _fmp; }
    void AddExp(float _fexp) { m_fEXP += _fexp; }



public:
    virtual void start();
    virtual void update();
    virtual void lateupdate();   

    virtual void OnCollisionEnter(CGameObject* _OtherObject) override;
    virtual void OnCollision(CGameObject* _OtherObject) override;
    virtual void OnCollisionExit(CGameObject* _OtherObject) override;

public:
    void Update_Skill();

    void Update_State();
    void Update_Move();
    void Update_Gravity();
    void Update_Animation();

    void Update_Inventory();


public:
    void RegisterDoubleJumpEff();
    void ResetAnim(PLAYER_STATE _eState, CGameObject* _pObj);


private:
    void Update_Animation(CGameObject* _pObj, wstring _name);
  

public:
    virtual void SaveToScene(FILE* _pFile) override;
    virtual void LoadFromScene(FILE* _pFile) override;


    CLONE(CPlayerScript)

public:
    CPlayerScript();
    CPlayerScript(const CPlayerScript& _origin);
    ~CPlayerScript();



    friend class CExpScript;
    friend class CHpScript;
    friend class CMpScript;
    friend class CItemScript;

};

