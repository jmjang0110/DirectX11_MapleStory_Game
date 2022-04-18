#include "pch.h"
#include "CCollisionMgr.h"

#include "CResMgr.h"

#include "CSceneMgr.h"
#include "CScene.h"
#include "CLayer.h"

#include "CGameObject.h"
#include "CCollider2D.h"


CCollisionMgr::CCollisionMgr()
	: m_arrCheck{}
{
}

CCollisionMgr::~CCollisionMgr()
{
}

void CCollisionMgr::update()
{
	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		for (UINT j = i; j < MAX_LAYER; ++j)
		{
			if (m_arrCheck[i] & (1 << j))
			{
				// i 에 해당하는 EOBJ_TYPE 오브젝트들과, j 에 해당하는EOBJ_TYPE 오브젝트들 끼리 충돌 진행
				CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();

				CollisionBetweenLayer(pCurScene->GetLayer(i)->GetObjects()
					, pCurScene->GetLayer(j)->GetObjects());
			}
		}
	}
}

void CCollisionMgr::CollisionBetweenLayer(const vector<CGameObject*>& _left, const vector<CGameObject*>& _right)
{
	CCollider2D* pLeftCol = nullptr;
	CCollider2D* pRightCol = nullptr;

	for (size_t i = 0; i < _left.size(); ++i)
	{
		pLeftCol = _left[i]->Collider2D();

		if (nullptr == pLeftCol)
			continue;

		for (size_t j = 0; j < _right.size(); ++j)
		{
			pRightCol = _right[j]->Collider2D();

			if (nullptr == pRightCol)
				continue;


			// 두 충돌체가 이전에 충돌했었는지 확인
			// 두 충돌체의 조합 아이디 생성
			COLLIDER_ID ID;
			ID.iLeftID = pLeftCol->GetID();
			ID.iRightID = pRightCol->GetID();
			map<long long, bool>::iterator iter = m_mapColInfo.find(ID.id);

			// 두 충돌체는 처음 검사를 진행하고 있다.
			if (iter == m_mapColInfo.end())
			{
				m_mapColInfo.insert(make_pair(ID.id, false));
				iter = m_mapColInfo.find(ID.id);
			}

			// 두 충돌체중 하나라도 데드 상태인지 
			bool bDead = pLeftCol->GetOwner()->IsDead() || pRightCol->GetOwner()->IsDead();

			// 충돌 검사
			// 충돌 중이다.
			if (IsCollision(pLeftCol, pRightCol))
			{
				// 이전 프레임에도 충돌 중이었다.
				if (iter->second)
				{
					// 충돌 중이다
					if (bDead)
					{
						// 둘중 하나라도 삭제 예정이라면(삭제를 통한 충돌 해제가 발생하는 것으로 본다)
						pLeftCol->OnCollisionExit(pRightCol);
						pRightCol->OnCollisionExit(pLeftCol);
						m_mapColInfo.erase(iter);
					}
					else
					{
						pLeftCol->OnCollision(pRightCol);
						pRightCol->OnCollision(pLeftCol);
					}
				}

				// 이전에는 충돌하지 않고 있었다.
				else
				{
					// 이번에 막 충돌 진입했다.
					if (!bDead)
					{
						// 둘 중 하나라도 삭제 예정이라면 충돌 시작 자체를 하지 않게 한다.
						pLeftCol->OnCollisionEnter(pRightCol);
						pRightCol->OnCollisionEnter(pLeftCol);
						iter->second = true;
					}
				}
			}

			// 충돌하지 않고 있다.
			else
			{
				// 이전 프레임에서는 충돌 중이었다.
				if (iter->second)
				{
					pLeftCol->OnCollisionExit(pRightCol);
					pRightCol->OnCollisionExit(pLeftCol);
					iter->second = false;
				}
			}
		}
	}
}


bool CCollisionMgr::IsCollision(CCollider2D* _pLeftCol, CCollider2D* _pRightCol)
{
	if (_pLeftCol->GetCollider2DType() == COLLIDER2D_TYPE::BOX
		&& _pRightCol->GetCollider2DType() == COLLIDER2D_TYPE::BOX)
	{
		return IsCollision_Box(_pLeftCol, _pRightCol);
	}
	else if (_pLeftCol->GetCollider2DType() == COLLIDER2D_TYPE::CIRCLE
		&& _pRightCol->GetCollider2DType() == COLLIDER2D_TYPE::CIRCLE)
	{
		return IsCollision_Circle(_pLeftCol, _pRightCol);
	}
	else
	{
		return false;
	}
}

bool CCollisionMgr::IsCollision_Box(CCollider2D* _pLeftCol, CCollider2D* _pRightCol)
{
	// 충돌체가 사용하는 기본 도형(사각형) 로컬 정점위치를 알아낸다.
	// 0 -- 1
	// | \  |
	// 3 -- 2	
	static CMesh* pRectMesh = CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh").Get();
	static Vtx* pVtx = pRectMesh->GetVtxSysMem();
	static Vec3 vLocalPos[4] = { pVtx[0].vPos, pVtx[1].vPos, pVtx[2].vPos, pVtx[3].vPos };

	Matrix matLeft = _pLeftCol->GetWorldMat();
	Matrix matRight = _pRightCol->GetWorldMat();

	// Local 스페이스의 네개의 정점을 각 충돌체 월드 위치로 보낸다.
	Vec3 vAsix[4] = {};

	// (Vector3, 0.f) X Matirx(4x4)
	// XMVector3TransformNormal();

	// (Vector3, 1.f) X Matirx(4x4)
	// 월드로 보낸 정점을 통해서 각 투영 축 이면서 투영시킬 벡터 성분 4개를 구한다.
	vAsix[0] = XMVector3TransformCoord(vLocalPos[1], matLeft) - XMVector3TransformCoord(vLocalPos[0], matLeft);
	vAsix[1] = XMVector3TransformCoord(vLocalPos[3], matLeft) - XMVector3TransformCoord(vLocalPos[0], matLeft);
	vAsix[2] = XMVector3TransformCoord(vLocalPos[1], matRight) - XMVector3TransformCoord(vLocalPos[0], matRight);
	vAsix[3] = XMVector3TransformCoord(vLocalPos[3], matRight) - XMVector3TransformCoord(vLocalPos[0], matRight);

	// 월드에 배치된 두 충돌체의 중심을 이은 벡터
	//Vec3 vCenter = XMVector3TransformCoord(Vec3::Zero, matRight) - XMVector3TransformCoord(Vec3::Zero, matLeft);	
	Vec3 vCenter = _pRightCol->GetWorldPos() - _pLeftCol->GetWorldPos();

	for (int i = 0; i < 4; ++i)
	{
		vAsix[i].z = 0.f;
		Vec3 vProj = vAsix[i];
		vProj.Normalize();

		float fDist = 0.f;

		for (int j = 0; j < 4; ++j)
		{
			// vProj 에 vAsix[j] 를 투영시킨 길이		
			fDist += abs(vAsix[j].Dot(vProj));
		}
		fDist *= 0.5f;
		float fCenterDist = abs(vCenter.Dot(vProj));

		if (fDist < fCenterDist)
			return false;
	}

	return true;
}

bool CCollisionMgr::IsCollision_Circle(CCollider2D* _pLeftCol, CCollider2D* _pRightCol)
{
	Vec3 vCenter = _pLeftCol->GetWorldPos() - _pRightCol->GetWorldPos();
	float fDist = vCenter.Length();
	float fRadius = fabsf(_pLeftCol->GetWorldScale().x) * 0.5f + fabsf(_pRightCol->GetWorldScale().x) * 0.5f;

	if (fRadius < fDist)
	{
		return false;
	}

	return true;
}


void CCollisionMgr::CollisionCheck(int _iLayerLeftIdx, int _iLayerRightIdx)
{
	UINT iCol = 0, iRow = 0;

	if ((UINT)_iLayerLeftIdx > (UINT)_iLayerRightIdx)
	{
		iCol = (UINT)_iLayerLeftIdx;
		iRow = (UINT)_iLayerRightIdx;
	}
	else
	{
		iCol = (UINT)_iLayerRightIdx;
		iRow = (UINT)_iLayerLeftIdx;
	}

	m_arrCheck[iRow] |= (1 << iCol);
}

void CCollisionMgr::CollisionOff(int _iLayerLeftIdx, int _iLayerRightIdx)
{
	UINT iCol = 0, iRow = 0;

	if ((UINT)_iLayerLeftIdx > (UINT)_iLayerRightIdx)
	{
		iCol = (UINT)_iLayerLeftIdx;
		iRow = (UINT)_iLayerRightIdx;
	}
	else
	{
		iCol = (UINT)_iLayerRightIdx;
		iRow = (UINT)_iLayerLeftIdx;
	}

	m_arrCheck[iRow] &= ~(1 << iCol);
}

void CCollisionMgr::CollisionCheck(const wstring& _strLeftName, const wstring& _strRightName)
{
	CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();
	CLayer* pLeftLayer = pCurScene->GetLayer(_strLeftName);
	CLayer* pRightLayer = pCurScene->GetLayer(_strRightName);

	if (pLeftLayer && pRightLayer)
		CollisionCheck(pLeftLayer->GetLayerIdx(), pRightLayer->GetLayerIdx());
}

void CCollisionMgr::CollisionOff(const wstring& _strLeftName, const wstring& _strRightName)
{
	CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();
	CLayer* pLeftLayer = pCurScene->GetLayer(_strLeftName);
	CLayer* pRightLayer = pCurScene->GetLayer(_strRightName);

	if (pLeftLayer && pRightLayer)
		CollisionOff(pLeftLayer->GetLayerIdx(), pRightLayer->GetLayerIdx());
}
