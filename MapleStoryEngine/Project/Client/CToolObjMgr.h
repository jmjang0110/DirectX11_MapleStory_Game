#pragma once

class CGameObjectEx;

class CToolObjMgr
	: public CSingleton<CToolObjMgr>
{
	SINGLE(CToolObjMgr);
private:
	vector<CGameObjectEx*>	m_vecObj;

public:
	void init();
	void progress();
};

