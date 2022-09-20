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
    ROPE_MOVE_UP,
    ROPE_MOVE_DOWN,
    ROPE_STOP,

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

    float           m_fMaxHP;
    float           m_fMaxMP;
    float           m_fMaxEXP;

    bool            m_bDie;


    float           m_fMinAttack;
    float           m_fMaxAttack;

    float           m_fTimer;
    Vec3            m_vPrevPos;
    int             m_iHitCnt;


private:
    Ptr<CSound>     m_pJumpBgm;
    Ptr<CSound>     m_pDieBgm;

private:
    float           m_STR;
    float           m_DEX;
    float           m_INT;
    float           m_LUK;

    float           m_fGroundNorthY;
    bool            m_bOnAngleGround;

private:
    float           m_bD_SkillColltime;
    bool            m_bD_SkillStart;





public:
    void SetGroundNorthY(float _f) { m_fGroundNorthY = _f; }
    void SetOnGroundAngle(bool _b) { m_bOnAngleGround = _b; }
    
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
    float GetMaxHp() { return m_fMaxHP; }
    float GetMaxMp() { return m_fMaxMP; }


    float GetSTR() { return m_STR; }
    float GetDEX() { return m_DEX; }
    float GetINT() { return m_INT; }
    float GetLUK() { return m_LUK; }



    void LevelUp() { m_iLevel++; }


public:
    void AddHp(float _fhp) {
        m_fHP += _fhp; if (m_fHP >= m_fMaxHP) m_fHP = m_fMaxHP;

        int i = 0; 

    }
    void AddMp(float _fmp) { m_fMP += _fmp;if (m_fMP >= m_fMaxMP) m_fHP = m_fMaxHP; }
    void AddExp(float _fexp) { m_fEXP += _fexp;  }

    void SubHP(float _fhp) { m_fHP -= _fhp; if (m_fHP <= 0.f) m_fHP = 0.f; }
    void SubMP(float _fmp) { m_fMP -= _fmp; if (m_fMP <= 0.f) m_fHP = 0.f; }

    void SetMP(float _F) { m_fMP = _F; }
    void SetHP(float _F) { m_fHP = _F; }
    void SetExp(float _f) { m_fEXP = _f; }

    int GetLevel() { return m_iLevel; }



    void SetState(PLAYER_STATE _eState) { m_ePrevState = m_eCurState; m_eCurState = _eState; }
    void SetPrevState(PLAYER_STATE _eState) { m_ePrevState = _eState; }

    PLAYER_STATE GetState() { return m_eCurState; }


    void Revival();

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

    void Update_Bgm();



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
    friend class CStatScript;

};

