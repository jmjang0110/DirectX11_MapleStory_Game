#pragma once
#include <Engine/CScript.h>


class CAnimOnceScript :
    public CScript
{

public:
    virtual void start();
    virtual void update();
    virtual void lateupdate();

    virtual void OnCollisionEnter(CGameObject* _OtherObject) override;
    virtual void OnCollision(CGameObject* _OtherObject) override;
    virtual void OnCollisionExit(CGameObject* _OtherObject) override;



public:
    virtual void SaveToScene(FILE* _pFile) override;
    virtual void LoadFromScene(FILE* _pFile) override;


    CLONE(CAnimOnceScript)


public:
    CAnimOnceScript();
    CAnimOnceScript(const CAnimOnceScript& _origin);
    ~CAnimOnceScript();

};

