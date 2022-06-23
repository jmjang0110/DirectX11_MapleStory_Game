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

    CLONE(CCursorScript);
public:
    CCursorScript();
    ~CCursorScript();

};

