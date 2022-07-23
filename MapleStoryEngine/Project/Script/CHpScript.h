#pragma once
#include <Engine/CScript.h>


class CHpScript :
    public CScript
{
private:

    CGameObject* m_pPlayer;
    float       m_fHp;
    float       m_fMaxHp;

    float       m_fDest;

public:
    virtual void start();
    virtual void update();
    virtual void lateupdate();

    virtual void OnCollisionEnter(CGameObject* _OtherObject) override;
    virtual void OnCollision(CGameObject* _OtherObject) override;
    virtual void OnCollisionExit(CGameObject* _OtherObject) override;


    CLONE(CHpScript);


public:
    CHpScript();
    CHpScript(const CHpScript& _origin);
    ~CHpScript();
    friend class CPlayerScript;

};

