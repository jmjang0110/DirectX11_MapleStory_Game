#include "pch.h"
#include "CTransform.h"

#include "CDevice.h"
#include "CConstBuffer.h"



CTransform::CTransform()
	: CComponent(COMPONENT_TYPE::TRANSFORM)
	, m_vRelativeScale(Vec3(1.f, 1.f, 1.f))
	, m_arrRelativeDir{}
	, m_arrWorldDir{}
	, m_bIgnoreParentScale(false)
{
}

CTransform::~CTransform()
{
}

void CTransform::finalupdate()
{
	// 크기 x 회전(자전) x 이동
	Matrix matScale = XMMatrixScaling(m_vRelativeScale.x, m_vRelativeScale.y, m_vRelativeScale.z);
	Matrix matTranslation = XMMatrixTranslation(m_vRelativePos.x, m_vRelativePos.y, m_vRelativePos.z);

	Matrix matRotX = XMMatrixRotationX(m_vRelativeRot.x);
	Matrix matRotY = XMMatrixRotationY(m_vRelativeRot.y);
	Matrix matRotZ = XMMatrixRotationZ(m_vRelativeRot.z);
	Matrix matRotation = matRotX * matRotY * matRotZ;

	// World 행렬 만들기
	m_matWorld = matScale * matRotation * matTranslation;

	// RelativeDir 구하기
	static Vec3 vAxis[(UINT)DIR_TYPE::END] = { Vec3::Right, Vec3::Up, Vec3::Front };

	for (int i = 0; i < (int)DIR_TYPE::END; ++i)
	{	
		//vAxis[i](회전하지 않았을 때의 기저축) x 회전행렬
		m_arrWorldDir[i] = m_arrRelativeDir[i] = XMVector3TransformNormal(vAxis[i], matRotation);
	}


	if (GetOwner()->GetParent())
	{		
		Matrix matParentWorld = GetOwner()->GetParent()->Transform()->GetWorldMat();

		if (m_bIgnoreParentScale)
		{
			// 부모 오브젝트의 크기를 무시한다. 
			Vec3 vParentWorldScale = GetOwner()->GetParent()->Transform()->GetWorldScale();
			Matrix matParentScale = XMMatrixScaling(vParentWorldScale.x, vParentWorldScale.y, vParentWorldScale.z);
			Matrix matParentScaleInv = XMMatrixInverse(nullptr, matParentScale);

			m_matWorld = m_matWorld * matParentScaleInv * matParentWorld;
		}
		else
		{
			// 부모 오브젝트의 크기에 영향을 받는다. 
			m_matWorld *= matParentWorld;
		}

		// World Dir 구하기
		for (int i = 0; i < (int)DIR_TYPE::END; ++i)
		{
			m_arrWorldDir[i] = XMVector3TransformNormal(m_arrRelativeDir[i], matParentWorld);
			m_arrWorldDir[i].Normalize();
		}
	}
}

void CTransform::active()
{
	CComponent::active();
	assert(nullptr);
}

void CTransform::deactive()
{
	CComponent::deactive();
	assert(nullptr);
}



Vec3 CTransform::GetWorldScale()
{
	Vec3 vWorldScale = m_vRelativeScale;

	CGameObject* pParent = GetOwner()->GetParent();
	if (m_bIgnoreParentScale)
		pParent = nullptr;

	while (pParent)
	{
		vWorldScale *= pParent->Transform()->GetRelativeScale();

		bool bIgnoreParentScale = pParent->Transform()->m_bIgnoreParentScale;
		pParent = pParent->GetParent();

		if (bIgnoreParentScale)
			pParent = nullptr;
	}

	return vWorldScale;
}

void CTransform::UpdateData()
{
	g_transform.matWorld = m_matWorld;
	g_transform.matWV = g_transform.matWorld * g_transform.matView;
	g_transform.matWVP = g_transform.matWV * g_transform.matProj;


	CConstBuffer* pBuffer = CDevice::GetInst()->GetCB(CB_TYPE::TRANSFORM);
	pBuffer->SetData(&g_transform, sizeof(tTransform));
	pBuffer->UpdateData();
}

void CTransform::SaveToFile(FILE* _pFile)
{
	CComponent::SaveToFile(_pFile);

	fwrite(&m_vRelativePos, sizeof(Vec3), 1, _pFile);
	fwrite(&m_vRelativeScale, sizeof(Vec3), 1, _pFile);
	fwrite(&m_vRelativeRot, sizeof(Vec3), 1, _pFile);

	for (UINT i = 0; i < (UINT)DIR_TYPE::END; ++i)
	{
		fwrite(&m_arrRelativeDir, sizeof(Vec3), 1, _pFile);
		fwrite(&m_arrWorldDir, sizeof(Vec3), 1, _pFile);

	}

	fwrite(&m_matWorld, sizeof(Matrix), 1, _pFile);
	fwrite(&m_bIgnoreParentScale, sizeof(bool), 1, _pFile);


}

void CTransform::LoadFromFile(FILE* _pFile, bool IsPrevRead)
{
	CComponent::LoadFromFile(_pFile);

	fread(&m_vRelativePos, sizeof(Vec3), 1, _pFile);
	fread(&m_vRelativeScale, sizeof(Vec3), 1, _pFile);
	fread(&m_vRelativeRot, sizeof(Vec3), 1, _pFile);

	for (UINT i = 0; i < (UINT)DIR_TYPE::END; ++i)
	{
		Vec3 RelativeDir;
		Vec3 WorldDir;

		fread(&RelativeDir, sizeof(Vec3), 1, _pFile);
		fread(&WorldDir, sizeof(Vec3), 1, _pFile);

		m_arrRelativeDir[i] = RelativeDir;
		m_arrWorldDir[i] = WorldDir;
		

	}

	fread(&m_matWorld, sizeof(Matrix), 1, _pFile);
	fread(&m_bIgnoreParentScale, sizeof(bool), 1, _pFile);


}
