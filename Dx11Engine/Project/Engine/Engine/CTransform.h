#pragma once
#include "CComponent.h"

class CTransform :
    public CComponent
{
private:
    Vec3    m_vRelativePos;
    Vec3    m_vRelativeScale;
    Vec3    m_vRelativeRot;     // 축별 회전량

    Vec3    m_arrRelativeDir[(UINT)DIR_TYPE::END];   // Local
    Vec3    m_arrWorldDir[(UINT)DIR_TYPE::END];      // World 
    


    Matrix  m_matWorld; // 위치변환 정보 행렬

    bool    m_bIgnoreParentScale;

public:
    void SetPos(float _x, float _y, float _z) { m_vRelativePos = Vec3(_x, _y, _z); }
    void SetScale(float _x, float _y, float _z) { m_vRelativeScale = Vec3(_x, _y, _z); }
    void SetRotation(float _x, float _y, float _z) { m_vRelativeRot = Vec3(_x, _y, _z); }

    void SetPos(const Vec3& _vPos) { m_vRelativePos = _vPos; }
    void SetScale(const Vec3& _vScale) { m_vRelativeScale = _vScale; }
    void SetRotation(const Vec3 _vRot) { m_vRelativeRot = _vRot; }

    const Vec3& GetRelativePos() { return m_vRelativePos; }
    const Vec3& GetRelativeScale() { return m_vRelativeScale; }
    const Vec3& GetRelativeRotation() { return m_vRelativeRot; };

    const Vec3& GetRelativeDir(DIR_TYPE _eType) { return m_arrRelativeDir[(UINT)_eType]; };

    const Vec3& GetRelativeRIghtDir() { return m_arrRelativeDir[(UINT)DIR_TYPE::RIGHT]; };
    const Vec3& GetRelativeUpDir() { return m_arrRelativeDir[(UINT)DIR_TYPE::UP]; };
    const Vec3& GetRelativeFrontDir() { return m_arrRelativeDir[(UINT)DIR_TYPE::FRONT]; };

    const Vec3& GetWorldDir(DIR_TYPE _eType) { return m_arrWorldDir[(UINT)_eType]; };
    const Vec3& GetWorldRightDir() { return m_arrWorldDir[(UINT)DIR_TYPE::RIGHT]; };
    const Vec3& GetWorldUpDir() { return m_arrWorldDir[(UINT)DIR_TYPE::UP]; };
    const Vec3& GetWorldFrontDir() { return m_arrWorldDir[(UINT)DIR_TYPE::FRONT]; };




    Vec3 GetWorldPos() { return m_matWorld.Translation(); }
    Vec3 GetWorldScale();
    const Matrix& GetWorldMat() { return m_matWorld; }


    void SetIgnoreParentScale(bool _bSet){ m_bIgnoreParentScale = _bSet;}

    virtual void UpdateData() override;
    virtual void finalupdate() override;


    CLONE(CTransform)

public:
    CTransform();
    ~CTransform();
};

