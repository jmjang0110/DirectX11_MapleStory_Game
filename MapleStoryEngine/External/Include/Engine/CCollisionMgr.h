#pragma once

class CGameObject;
class CCollider2D;

union COLLIDER_ID
{
	struct {
		UINT iLeftID;
		UINT iRightID;
	};
	long long id;
};

class CCollisionMgr
	: public CSingleton<CCollisionMgr>
{
	SINGLE(CCollisionMgr);
private:
	UINT					m_arrCheck[MAX_LAYER];
	map<long long, bool>	m_mapColInfo;	// 충돌 조합 고유 키


public:
	void update();

public:
	void CollisionCheck(const wstring& _strLeftName, const wstring& _strRightName);
	void CollisionOff(const wstring& _strLeftName, const wstring& _strRightName);	

	void CollisionCheck(int _iLayerLeftIdx, int _iLayerRightIdx);
	void CollisionOff(int _iLayerLeftIdx, int _iLayerRightIdx);

private:
	void CollisionBetweenLayer(const vector<CGameObject*>& _left, const vector<CGameObject*>& _right);
	bool IsCollision(CCollider2D* _pLeftCol, CCollider2D* _pRightCol);
	bool IsCollision_Box(CCollider2D* _pLeftCol, CCollider2D* _pRightCol);
	bool IsCollision_Circle(CCollider2D* _pLeftCol, CCollider2D* _pRightCol);

	// ==== todo ====
public:
	const UINT* GetColArrCheck() { return m_arrCheck; };
	void EraseColInfo(CCollider2D* _LeftCol, CCollider2D* _RightCol);
	void SetColChcek(UINT* _ArrCheck) { for (int i = 0; i < MAX_LAYER; ++i) m_arrCheck[i] = _ArrCheck[i]; m_mapColInfo.clear(); }

	void SaveToFile(FILE* _pFile);
	void LoadFromFile(FILE* _pFile);
	bool IsCollisionCheck(CCollider2D* _pLeftCol, CCollider2D* _pRightCol) { return IsCollision(_pLeftCol, _pRightCol); }
	void ClearCollisionInfo() { for (int i = 0; i < MAX_LAYER; ++i) m_arrCheck[i] = 0; m_mapColInfo.clear(); }
	// ==============

};

