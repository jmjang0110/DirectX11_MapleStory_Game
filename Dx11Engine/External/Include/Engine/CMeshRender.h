#pragma once
#include "CComponent.h"

#include "CMesh.h"
#include "CMaterial.h"


class CMeshRender :
    public CComponent
{
private:
    Ptr<CMesh>      m_pMesh;

    Ptr<CMaterial>  m_pMtrl;            // 현재 사용중인 재질 
    Ptr<CMaterial>  m_pSharedMtrl;      // 공유 재질 
    Ptr<CMaterial>  m_pDynamicMtrl;     // 별도로 기지는 재질 


public:
    void SetMesh(Ptr<CMesh> _pMesh) { m_pMesh = _pMesh; }
    void SetSharedMaterial(Ptr<CMaterial> _pMtrl);


    Ptr<CMesh> GetMesh() { return m_pMesh; }
    Ptr<CMaterial> GetSharedMaterial();
    Ptr<CMaterial> GetDynamicMaterial();



public:
    virtual void finalupdate() override;
    virtual void render() override;

    CLONE(CMeshRender)

public:
    CMeshRender();
    CMeshRender(const CMeshRender& _meshrender);
    ~CMeshRender();
};

