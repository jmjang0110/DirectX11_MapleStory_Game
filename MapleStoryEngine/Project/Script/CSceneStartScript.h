#pragma once
#include <Engine/CScript.h>

/*
    Default Layer 에서 Scene 이 시작됐을 때 시작할 일들을 
    Script 로 만듭니다. 
    예를들어 Scene 이 시작되면  Layer 에 저장된 Player 를 Load 할 수 있습니다. 
*/
class CSceneStartScript :
    public CScript
{



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


    CLONE(CSceneStartScript)


public:
    CSceneStartScript();
    CSceneStartScript(const CSceneStartScript& _origin);
    ~CSceneStartScript();




};

