#pragma once
#include "CComponent.h"

#include "CMesh.h"
#include "CMaterial.h"


enum class COLLIDER2D_TYPE
{
    BOX,
    CIRCLE,
    END,

};

class CCollider2D :
    public CComponent
{
private:
    Matrix              m_matColWorld;

    COLLIDER2D_TYPE     m_eColliderType;

    Vec2                m_vOffsetPos;       // 오브젝트로 부터의 충돌체의 상대 거리
    Vec2                m_vOffsetScale;     // 오브젝트 크기로 부터 충돌체의 배율

    Ptr<CMesh>          m_pMesh;            // 충돌체 모양
    Ptr<CMaterial>      m_pMtrl;            // 충돌체 랜더링 재질

    int                 m_iCollisionCount;  // 충돌 횟수


public:
    void SetCollider2DType(COLLIDER2D_TYPE _type);

    void SetOffsetPos(Vec2 _vOffsetPos) { m_vOffsetPos = _vOffsetPos; }
    Vec2 GetOffsetPos() { return m_vOffsetPos; }
    void SetOffsetPos(float _x, float _y) { m_vOffsetPos = Vec2(_x, _y); }


    void SetOffsetScale(Vec2 _vOffsetScale);
    void SetOffsetScale(float _x, float _y);
    Vec2 GetOffsetScale() { return m_vOffsetScale; }


    COLLIDER2D_TYPE GetCollider2DType() { return m_eColliderType; }
    Vec3 GetWorldPos() { return m_matColWorld.Translation(); }
    Vec3 GetWorldScale() { return Vec3(m_vOffsetScale); }

    Matrix GetWorldMat() { return m_matColWorld; }

    int GetCollisionCount() { return m_iCollisionCount; }

public:
    virtual void finalupdate() override;
    virtual void UpdateData() override;
    void render();

    // ======== File ============
public:
    virtual void SaveToFile(FILE* _pFile);
    virtual void LoadFromFile(FILE* _pFile, bool IsPrevRead = false);

    // ======== File ============




public:

    // 이전 프레임에는 충돌 X, 이번 프레임에 충돌 시작일 때
    void OnCollisionEnter(CCollider2D* _Other);

    // 이전 프레임에도 충돌 중, 지금도 충돌 중일 때
    void OnCollision(CCollider2D* _Other);

    // 이전 프레임에 충돌 중, 지금은 충돌 X 일때
    void OnCollisionExit(CCollider2D* _Other);


    CLONE(CCollider2D)
public:
    CCollider2D();
    CCollider2D(const CCollider2D& _Origin);
    ~CCollider2D();
};

