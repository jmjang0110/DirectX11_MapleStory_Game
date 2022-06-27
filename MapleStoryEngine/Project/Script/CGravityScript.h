#pragma once
#include <Engine/CScript.h>

/*


*/
// G = M * A 
// - 지구의 질량이 너무 커서 물체의 질량은 영향을 못미친다, 
// -> 고정적인 가속도 9.8 m/s 

// 아래방향 - 일정한 가속도 
// 프레임 마다 아래 방향으로 힘 작용 
class CGravityScript :
    public CScript
{
private:




public:
    virtual void start();
    virtual void update();
    virtual void lateupdate();

    virtual void OnCollisionEnter(CGameObject* _OtherObject) override;



public:
    virtual void SaveToScene(FILE* _pFile) override;
    virtual void LoadFromScene(FILE* _pFile) override;


    CLONE(CGravityScript)


public:
    CGravityScript();
    ~CGravityScript();

};

