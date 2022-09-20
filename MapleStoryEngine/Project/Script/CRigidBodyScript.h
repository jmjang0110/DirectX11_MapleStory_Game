#pragma once
#include <Engine/CScript.h>

// F : Force 
// M : Mass
// A : Accel


// F = M * A  <->  A = F / M

class CRigidBodyScript :
    public CScript
{
private:

    Vec3            m_vForce;       // 크기 , 방향 
    Vec3            m_vAccel;       // 가속도  
    Vec3            m_vAccelA;      // 추가 가속도 
    Vec3            m_vVelocity;    // 속도 ( 크기 : 속력 , 방향 )
    Vec3            m_vMaxVelocity;    // 최대 속도  

    float           m_fMass;        // 질량 
    
    float           m_fFricCoeff;   // 마찰 계수 



    // todo
    float m_vAccelRatio; // Accel 비율을 줄이거나 늘려서 줄이고 싶은 애들은 줄인다. Default : 1.f 

public:
    void SetAccelRatio(float _F) { m_vAccelRatio = _F; }



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


public:
    void AddForce(Vec3 _vF) { m_vForce += _vF; }
    void AddVelocity(Vec3 _v) { m_vVelocity += _v; }
    void AddAccelAlpha(Vec3 _vAccelA) { m_vAccelA += _vAccelA; }


public:
    void SetForce(Vec3 _vForce) { m_vForce = _vForce; }
    void SetVelocity(Vec3 _v) { m_vVelocity = _v; }
    void SetMass(float _fMass) { m_fMass = _fMass; }
    void SetMaxVelocity(Vec3 _v) { m_vMaxVelocity = _v; }
    void SetFricCoeff(float _fFricCoeff) { m_fFricCoeff = _fFricCoeff; }
    void SetAccelAlpha(Vec3 _vAccelA) { m_vAccelA = _vAccelA; }


    Vec3 GetForce() { return m_vForce; }
    Vec3 GetVelocity() { return m_vVelocity; }
    Vec3 GetAccelAlpha() { return m_vAccelA; }
    Vec3 GetMaxVelocity() { return m_vMaxVelocity; }
    float GetMass() { return m_fMass; }
    float GetFricCoeff() { return m_fFricCoeff; }



public:
    CLONE(CRigidBodyScript)

public:
    CRigidBodyScript();
    CRigidBodyScript(const CRigidBodyScript& _origin);
    ~CRigidBodyScript();


};

