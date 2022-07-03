#pragma once
#include <Engine/Cscript.h>

class CCursorScript :
    public CScript
{
private:

public:
    virtual void start();
    virtual void update();
    virtual void lateupdate();

    virtual void OnCollisionEnter(CGameObject* _OtherObject) override;
    virtual void OnCollision(CGameObject* _OtherObject) override;
    virtual void OnCollisionExit(CGameObject* _OtherObject) override;


    CLONE(CCursorScript);
public:
    CCursorScript();   
    CCursorScript(const CCursorScript& _origin);
    ~CCursorScript();

};

