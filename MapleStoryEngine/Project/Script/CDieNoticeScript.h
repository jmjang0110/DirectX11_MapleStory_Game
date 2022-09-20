#pragma once
#include <Engine/CScript.h>
class CDieNoticeScript :
    public CScript
{
private:
	class CGameObject* m_pBtn;
	class CButtonScript* m_pBtnScript;


public:
	// CButtonScript 에 넘겨줄 함수 ( 
	void BtnClickedEvent(DWORD_PTR _param);


public:
	virtual void start();
	virtual void update();
	virtual void lateupdate();

	virtual void OnCollisionEnter(CGameObject* _OtherObject) override;
	virtual void OnCollision(CGameObject* _OtherObject) override;
	virtual void OnCollisionExit(CGameObject* _OtherObject) override;


	CLONE(CDieNoticeScript)

public:
	CDieNoticeScript();
	CDieNoticeScript(const CDieNoticeScript& _origin);
	virtual ~CDieNoticeScript();
};

