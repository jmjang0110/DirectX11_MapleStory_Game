#pragma once
#include <Engine/CScript.h>
class CBackGroundScript :
    public CScript
{
private:

    float           m_fSpeed;

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


    CLONE(CBackGroundScript)

public:
    CBackGroundScript();
    CBackGroundScript(const CBackGroundScript& _origin);
    ~CBackGroundScript();

};

