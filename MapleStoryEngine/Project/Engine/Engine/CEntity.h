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

	// ======== File ============
public:
	virtual void SaveToFile(FILE* _pFile);
	// 이전에 이름을 읽었으면 false
	virtual void LoadFromFile(FILE* _pFile , bool IsPrevRead = false);

	// ======== File ============

public:
	CEntity();
	CEntity(const CEntity& _origin);
	virtual ~CEntity();
};

