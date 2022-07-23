#pragma once
#include <Engine/CScript.h>

enum class BALLMOVE_TYPE
{
    LINEAR,
    DIAGONAL,
    PARABOLA,
    BALLISTIC,

    END,

};

enum class BALL_DIRECTION
{
    // 직선 
    LEFT,         
    RIGHT,      
    
    // 대각선 - 직선 
    LEFT_DIAGONAL,       
    RIGHT_DIAGONAL,

    // 포물선 
    LEFT_PARABOLA,
    RIGHT_PARABOLA,

    // 탄도 
    LEFT_BALLISTIC,
    RIGHT_BALLISTIC,


    END,

};


class CBasicBallScript :
    public CScript
{

private:
    bool                m_bStart;
    float               m_fSpeed;
    float               m_fRadian; // degree 

    BALL_DIRECTION      m_eDir;
    BALLMOVE_TYPE       m_eMoveType;

    float               m_fTimer;
    float               m_fMaxTime;

    Vec3                m_vStartPos;
    Vec3                m_vPrevPos;


private:
    bool                m_bHit;

    float               m_fMAxAttack;
    float               m_fMinAttack;



private:
    void LinearMove();
    void DiagonalMove();
    void ParabolaMove();
    void BallisticMove();

public:
    void SetBallMoveType(BALLMOVE_TYPE _type) { m_eMoveType = _type; }
    void SetMaxAttack(float _f) { m_fMAxAttack = _f; }
    void SetMinAttack(float _f) { m_fMinAttack = _f; }
    float GetMaxAttack() { return m_fMAxAttack; }
    float GetMinAttack() { return m_fMinAttack; }

    void Init(Vec3 _startpos);

public:
    virtual void start();
    virtual void update();
    virtual void lateupdate();

    virtual void OnCollisionEnter(CGameObject* _OtherObject) override;
    virtual void OnCollision(CGameObject* _OtherObject) override;
    virtual void OnCollisionExit(CGameObject* _OtherObject) override;
    void SetDir(BALL_DIRECTION _dir) { m_eDir = _dir; }
    void SetMaxTime(float _maxTime) { m_fMaxTime = _maxTime; }
    void SetSpeed(float _fSpeed) { m_fSpeed = _fSpeed; }
    void SetHit(bool _b) { m_bHit = _b; }


public:
    virtual void SaveToScene(FILE* _pFile) override;
    virtual void LoadFromScene(FILE* _pFile) override;


    CLONE(CBasicBallScript)


public:
    CBasicBallScript();
    CBasicBallScript(const CBasicBallScript& _origin);
    ~CBasicBallScript();

};

