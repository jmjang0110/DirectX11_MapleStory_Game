#pragma once
#include <Engine/CScript.h>

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

    END,

};


class CBasicBallScript :
    public CScript
{

private:
    float               m_fSpeed;
    float               m_fAngle; // degree 

    BALL_DIRECTION      m_eDir;

    float m_fTimer;
    float m_fMaxTime;


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


public:
    virtual void SaveToScene(FILE* _pFile) override;
    virtual void LoadFromScene(FILE* _pFile) override;


    CLONE(CBasicBallScript)


public:
    CBasicBallScript();
    CBasicBallScript(const CBasicBallScript& _origin);
    ~CBasicBallScript();

};

