#pragma once

#include <Engine/CScript.h>


class CPlayerScript :
    public CScript
{
private:
   // Ptr<CPrefab>    m_pMissilePrefab;
    float           m_fSpeed;

    float           m_fBurnStrength;
    bool            m_bBurn;

public:
    virtual void start();
    virtual void update();
    virtual void lateupdate();   

    virtual void OnCollisionEnter(CGameObject* _OtherObject) override;


    // ======== File ============
public:
    virtual void SaveToFile(FILE* _pFile);
    virtual void LoadFromFile(FILE* _pFile);

    // ======== File ============

private:
    void Burnning();



    CLONE(CPlayerScript)

public:
    CPlayerScript();
    ~CPlayerScript();

};

