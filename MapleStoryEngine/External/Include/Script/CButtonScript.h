#pragma once
#include <Engine/CScript.h>


class CButtonScript :
    public CScript
{

private:
    bool    m_bEnable;

public:
    void UpdateButton(bool _bEnable);
    void SetEnable(bool _B) { m_bEnable = _B; }
public:
    virtual void start();
    virtual void update();
    virtual void lateupdate();

    virtual void OnCollisionEnter(CGameObject* _OtherObject) override;
    virtual void OnCollision(CGameObject* _OtherObject) override;
    virtual void OnCollisionExit(CGameObject* _OtherObject) override;


    CLONE(CButtonScript);


public:
    CButtonScript();
    CButtonScript(const CButtonScript& _origin);
    ~CButtonScript();


};

