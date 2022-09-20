#pragma once
#include <Engine/CScript.h>


class CTimerScript :
    public CScript
{
private:
    float       m_fTimer;
    float       m_fEndTime;

    bool        m_bEnd;


private:
    int         m_iCurNum;
    int         m_iPrevNum;


public:
    void SetEndtime(float _fTime) { m_fEndTime = _fTime; }
    void SavePlayerToPrefab();


    bool GetEnd() { return m_bEnd; }
    void UpdateUIpos();

public:
    virtual void start();
    virtual void update();
    virtual void lateupdate();

    virtual void OnCollisionEnter(CGameObject* _OtherObject) override;
    virtual void OnCollision(CGameObject* _OtherObject) override;
    virtual void OnCollisionExit(CGameObject* _OtherObject) override;


    CLONE(CTimerScript);


public:
    CTimerScript();
    CTimerScript(const CTimerScript& _origin);
    ~CTimerScript();



};

