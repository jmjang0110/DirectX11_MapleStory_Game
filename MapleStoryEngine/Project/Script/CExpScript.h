#pragma once
#include <Engine/CScript.h>


class CExpScript
    : public CScript
{

private:
	float			m_fMaxExp;
	float			m_fExp;

	int				m_iLevel;
	CGameObject*	m_pPlayer;


	float			m_fDest;
	Ptr<CSound>     m_pLevelUpBgm;
	
	float m_fBgmstartTimer;
	bool m_bLEvelUp;



public:
	void SetMaxExp(float _f) { m_fMaxExp = _f; }
	void SetPlayer(CGameObject* _pPlayer) { m_pPlayer = _pPlayer; }

	void AddExp(float _f) { m_fMaxExp += _f; }
	bool GetMaxExp() { return m_fMaxExp; }


public:
	virtual void start();
	virtual void update();
	virtual void lateupdate();

	virtual void OnCollisionEnter(CGameObject* _OtherObject) override;
	virtual void OnCollision(CGameObject* _OtherObject) override;
	virtual void OnCollisionExit(CGameObject* _OtherObject) override;


	CLONE(CExpScript)

public:
	CExpScript();
	CExpScript(const CExpScript& _origin);
	virtual ~CExpScript();

	friend class CPlayerScript;


};

