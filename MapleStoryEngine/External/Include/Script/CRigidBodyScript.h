#pragma once
#include <Engine/CScript.h>


// F = M * A  <->  A = F / M

class CRigidBodyScript :
    public CScript
{
private:

    Vec3            m_vForce;       // 크기 , 방향 
    Vec3            m_vAccel;       // 가속도  
    Vec3            m_vVelocity;    // 속도 ( 크기 : 속력 , 방향 )

    float           m_fMass;        // 질량 
    float           m_fMaxSpeed;    // 최대 속력 
    
    float           m_fFricCoeff;   // 마찰 계수 


public:
    virtual void start();
    virtual void update();
    virtual void lateupdate();

    virtual void OnCollisionEnter(CGameObject* _OtherObject) override;

public:
    virtual void SaveToScene(FILE* _pFile) override;
    virtual void LoadFromScene(FILE* _pFile) override;


public:
    void AddForce(Vec3 _vF) { m_vForce += _vF; }
    void AddVelocity(Vec3 _v) { m_vVelocity += _v; }

public:
    void SetForce(Vec3 _vForce) { m_vForce = _vForce; }
    void SetVelocity(Vec3 _v) { m_vVelocity = _v; }
    void SetMass(float _fMass) { m_fMass = _fMass; }
    void SetMaxSpeed(float _v) { m_fMaxSpeed = _v; }
    void SetFricCoeff(float _fFricCoeff) { m_fFricCoeff = _fFricCoeff; }

    Vec3 GetForce() { return m_vForce; }
    Vec3 GetVeclocity() { return m_vVelocity; }
    float GetMass() { return m_fMass; }
    float GetMaxSpeed() { return m_fMaxSpeed; }
    float GetFricCoeff() { return m_fFricCoeff; }



public:
    CLONE(CRigidBodyScript)

public:
    CRigidBodyScript();
    ~CRigidBodyScript();


};

