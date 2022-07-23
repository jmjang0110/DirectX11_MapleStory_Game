#pragma once
#include <Engine/CScript.h>
class CConsumeScript :
    public CScript
{

public:
    virtual void start();
    virtual void update();
    virtual void lateupdate();

    virtual void OnCollisionEnter(CGameObject* _OtherObject) override;
    virtual void OnCollision(CGameObject* _OtherObject) override;
    virtual void OnCollisionExit(CGameObject* _OtherObject) override;


    CLONE(CConsumeScript);


public:
    CConsumeScript();
    CConsumeScript(const CConsumeScript& _origin);
    ~CConsumeScript();


};

