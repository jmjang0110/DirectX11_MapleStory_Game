#pragma once
#include <Engine/CScript.h>

class CPaperBurnScript :
    public CScript
{
private:
    float           m_fBurnStrength;
    bool            m_bBurn;



private:
    void Burnning();


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


    CLONE(CPaperBurnScript)


public:
    CPaperBurnScript();
    CPaperBurnScript(const CPaperBurnScript& _origin);
    ~CPaperBurnScript();

};

