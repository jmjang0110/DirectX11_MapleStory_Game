#pragma once
#include <Engine/CScript.h>


enum class MONSTER_STATE
{
	IDLE,
	PATROL,
	TRACE,
	ATT,
	RUN,
	DEAD,


	END,

};

class CAIScript;


class CStateScript
	: public CScript
{

private:
	CAIScript* m_pAI;
	MONSTER_STATE		m_eMonsterState;



public:
	CAIScript* GetAI() { return m_pAI; }
	void SetMonsterState(MONSTER_STATE _state) { m_eMonsterState = _state; }
	MONSTER_STATE	GetMonsterStateType() { return m_eMonsterState; }

public:
	virtual void Enter();
	virtual void Exit();


	virtual void start();
	virtual void update();
	virtual void lateupdate();

	virtual void OnCollisionEnter(CGameObject* _OtherObject) override;
	virtual void OnCollision(CGameObject* _OtherObject) override;
	virtual void OnCollisionExit(CGameObject* _OtherObject) override;


	CLONE(CStateScript)

public:
	CStateScript();
	CStateScript(int _type);
	CStateScript(const CStateScript& _origin);
	virtual ~CStateScript();


	friend class CAIScript;
};

