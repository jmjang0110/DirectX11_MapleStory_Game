#pragma once
#include <Engine/CScript.h>

/*
    Default Layer ���� Scene �� ���۵��� �� ������ �ϵ��� 
    Script �� ����ϴ�. 
    ������� Scene �� ���۵Ǹ�  Layer �� ����� Player �� Load �� �� �ֽ��ϴ�. 
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

