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

    Vec3            m_vForce;       // ũ�� , ���� 
    Vec3            m_vAccel;       // ���ӵ�  
    Vec3            m_vAccelA;      // �߰� ���ӵ� 
    Vec3            m_vVelocity;    // �ӵ� ( ũ�� : �ӷ� , ���� )
    Vec3            m_vMaxVelocity;    // �ִ� �ӵ�  

    float           m_fMass;        // ���� 
    
    float           m_fFricCoeff;   // ���� ��� 



    // todo
    float m_vAccelRatio; // Accel ������ ���̰ų� �÷��� ���̰� ���� �ֵ��� ���δ�. Default : 1.f 

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

