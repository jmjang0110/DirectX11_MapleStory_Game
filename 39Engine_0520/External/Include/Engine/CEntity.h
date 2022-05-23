#pragma once


class CEntity
{
private:
	static UINT g_iNextID;

private:
	const UINT	m_iID;
	wstring		m_strName;
	
public:
	void SetName(const wstring& _name){m_strName = _name;}
	const wstring& GetName(){return m_strName;}
	UINT GetID() { return m_iID; }

public:
	virtual void UpdateData() {}

	virtual CEntity* Clone() = 0;

public:
	CEntity();
	CEntity(const CEntity& _origin);
	virtual ~CEntity();
};

