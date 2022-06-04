#pragma once
#include "CComponent.h"

#include "CMaterial.h"
#include "CMesh.h"

class CRenderComponent :
    public CComponent
{
private:
    Ptr<CMesh>      m_pMesh;        // 메쉬
    Ptr<CMaterial>  m_pMtrl;        // 현재 사용중인 재질
    Ptr<CMaterial>  m_pSharedMtrl;  // 공유 재질
    Ptr<CMaterial>  m_pDynamicMtrl; // 별도로 가지는 재질

public:
    void SetMesh(Ptr<CMesh> _pMesh) { m_pMesh = _pMesh; }
    void SetSharedMaterial(Ptr<CMaterial> _pMtrl);

    Ptr<CMesh> GetMesh() { return m_pMesh; }
    Ptr<CMaterial> GetMaterial() { return m_pMtrl; }
    Ptr<CMaterial> GetSharedMaterial();
    Ptr<CMaterial> GetDynamicMaterial();


    // ======== File ============
public:
    virtual void SaveToFile(FILE* _pFile);
    virtual void LoadFromFile(FILE* _pFile, bool IsPrevRead = false);

    // ======== File ============

public:
    virtual void render() = 0;

public:
    CRenderComponent(COMPONENT_TYPE _type);
    CRenderComponent(const CRenderComponent& _origin);
    ~CRenderComponent();
};

