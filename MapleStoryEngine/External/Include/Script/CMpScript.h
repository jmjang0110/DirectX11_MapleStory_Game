#pragma once
#include <Engine/CScript.h>


class CMpScript :
    public CScript
{
private:


    CGameObject* m_pPlayer;
    float       m_fMp;
    float       m_fMaxMp;


    float       m_fDest;

public:
    virtual void start();
    virtual void update();
    virtual void lateupdate();

    virtual void OnCollisionEnter(CGameObject* _OtherObject) override;
    virtual void OnCollision(CGameObject* _OtherObject) override;
    virtual void OnCollisionExit(CGameObject* _OtherObject) override;


    CLONE(CMpScript);


public:
    CMpScript();
    CMpScript(const CMpScript& _origin);
    ~CMpScript();


    friend class CPlayerScript;

};

