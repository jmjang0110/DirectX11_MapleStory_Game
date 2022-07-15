#pragma once
#include <Engine/CScript.h>


class CDoubleJumpScript :
    public CScript
{

public:
	virtual void Enter();
	virtual void Exit();


	virtual void start();
	virtual void update();
	virtual void lateupdate();

	virtual void OnCollisionEnter(CGameObject* _OtherObject) override;
	virtual void OnCollision(CGameObject* _OtherObject) override;
	virtual void OnCollisionExit(CGameObject* _OtherObject) override;


	CLONE(CDoubleJumpScript)

public:
	CDoubleJumpScript();
	CDoubleJumpScript(const CDoubleJumpScript& _origin);
	virtual ~CDoubleJumpScript();

};

