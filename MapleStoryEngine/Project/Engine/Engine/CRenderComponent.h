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
    Ptr<CMaterial> GetDynamicMaterial(); // 해당 Mtrl 을 건내주고 복사된 Mtrl의 원본(Master)임을 적어둔다. 
    // 자신만 Material 을 잠시 바꾸고 싶을 때 이걸 건내받는다.
    // 만약에 그냥 sharedMtrl을 건내받고 고친다면 해당 Mtrl을 
    // 가지고 있는 모든 REnderCompoent들이 다 바뀌는 불상사가 일어나낟. 



public:
    virtual void render() = 0;


    virtual void SaveToScene(FILE* _pFile) override;
    virtual void LoadFromScene(FILE* _pFile) override;



public:
    CRenderComponent(COMPONENT_TYPE _type);
    CRenderComponent(const CRenderComponent& _origin);
    ~CRenderComponent();
};

