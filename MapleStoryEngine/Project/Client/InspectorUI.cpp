#include "pch.h"
#include "InspectorUI.h"

#include "TransformUI.h"
#include "MeshRenderUI.h"
#include "CameraUI.h"

#include "ScriptUI.h"
#include "MaterialUI.h"
#include "TextureUI.h"
#include "Animator2DUI.h"
#include "GraphicsShaderUI.h"
#include "ComputeShaderUI.h"
#include "MeshUI.h"
#include "ParticleSystemUI.h"
#include "Collider2DUI.h"
#include "ListUI.h"
#include "TileMapUI.h"

#include "CImGuiMgr.h"

#include <Engine/CFileMgr.h>
#include <Engine/CSceneMgr.h>
#include <Engine/CLayer.h>
#include <Engine/CResMgr.h>
#include <Engine/CKeyMgr.h>

// COMPONENT-TYPE
#include <Engine/CTransform.h>
#include <Engine/CCamera.h>
#include <Engine/CCollider2D.h>
#include <Engine/CAnimator2D.h>
#include <Engine/CMeshRender.h>
#include <Engine/CTileMap.h>
#include <Engine/CParticleSystem.h>





InspectorUI::InspectorUI()
	: UI("Inspector")
	, m_pTargetObject(nullptr)
	, m_pTargetRes(nullptr)
	, m_arrComUI{}
	, m_arrResUI{}
{
	// ComponentUI 생성   
	ComponentUI* pComUI = nullptr;

	pComUI = new TransformUI;
	AddChild(pComUI);
	m_arrComUI[(UINT)COMPONENT_TYPE::TRANSFORM] = pComUI;

	pComUI = new MeshRenderUI;
	AddChild(pComUI);
	m_arrComUI[(UINT)COMPONENT_TYPE::MESHRENDER] = pComUI;

	pComUI = new CameraUI;
	AddChild(pComUI);
	m_arrComUI[(UINT)COMPONENT_TYPE::CAMERA] = pComUI;

	// Animator2D UI
	pComUI = new Animator2DUI;
	AddChild(pComUI);
	m_arrComUI[(UINT)COMPONENT_TYPE::ANIMATOR2D] = pComUI;

	// PArticle SYstem UI
	pComUI = new ParticleSystemUI;
	AddChild(pComUI);
	m_arrComUI[(UINT)COMPONENT_TYPE::PARTICLESYSTEM] = pComUI;

	//Collider2D SYstem UI
	pComUI = new Collider2DUI;
	AddChild(pComUI);
	m_arrComUI[(UINT)COMPONENT_TYPE::COLLIDER2D] = pComUI;

	// TileMapUI
	pComUI = new TileMapUI;
	AddChild(pComUI);
	m_arrComUI[(UINT)COMPONENT_TYPE::TILEMAP] = pComUI;


	// ==============
	// ResInfoUI 생성
	// ==============
	ResInfoUI* pResInfoUI = nullptr;

	// MaterialUI
	pResInfoUI = new MaterialUI;
	AddChild(pResInfoUI);
	m_arrResUI[(UINT)RES_TYPE::MATERIAL] = pResInfoUI;


	// MeshUI
	pResInfoUI = new MeshUI;
	AddChild(pResInfoUI);
	m_arrResUI[(UINT)RES_TYPE::MESH] = pResInfoUI;


	// TextureUI
	pResInfoUI = new TextureUI;
	AddChild(pResInfoUI);
	m_arrResUI[(UINT)RES_TYPE::TEXTURE] = pResInfoUI;

	// GraphicsShaderUI
	pResInfoUI = new GraphicsShaderUI;
	AddChild(pResInfoUI);
	m_arrResUI[(UINT)RES_TYPE::GRAPHICS_SHADER] = pResInfoUI;

	// ComputeShaderUI
	pResInfoUI = new ComputeShaderUI;
	AddChild(pResInfoUI);
	m_arrResUI[(UINT)RES_TYPE::COMPUTE_SHADER] = pResInfoUI;



}

InspectorUI::~InspectorUI()
{
}


void InspectorUI::update()
{
	/*if (KEY_TAP(KEY::I))
	{
		if (IsActive())
			Deactivate();
		else
			Activate();
	}*/



	//ImGui::End();


}

void InspectorUI::render_update()
{

	GameObjectTool_SubFunc();
	ImGui::Separator();
}


void InspectorUI::SetTargetObject(CGameObject* _pTarget)
{
	// ======== Todo =================
	/*
		내가 만든 SceneOutlinerTool 에서는 SCene Layer 가 눌리는 상황도 있으므로 
		Scene Layer 가 눌렸을 때는 nullptr 가 들어가게 만들었다. 예외처리 하자 
	*/
	if (nullptr == _pTarget)
		return;

	// ==============================

	m_pTargetObject = _pTarget;

	for (int i = 0; i < (int)COMPONENT_TYPE::END; ++i)
	{
		if (nullptr != m_arrComUI[i])
		{
			// Object 가 nullptr 인 경우
			if (nullptr == m_pTargetObject)
				m_arrComUI[i]->Deactivate();
			else if (m_pTargetObject->GetComponent((COMPONENT_TYPE)i))
			{
				m_arrComUI[i]->Activate();
				m_arrComUI[i]->SetTargetObject(m_pTargetObject);
			}
			else
			{
				m_arrComUI[i]->Deactivate();
			}
		}
	}

	const vector<CScript*>& vecScripts = m_pTargetObject->GetScripts();
	ScriptUI* pScriptUI = nullptr;

	for (size_t i = 0; i < vecScripts.size(); ++i)
	{
		if (m_vecScriptUI.size() <= i)
			pScriptUI = AddScriptUI();
		else
			pScriptUI = m_vecScriptUI[i];

		pScriptUI->SetTargetObject(m_pTargetObject);
		pScriptUI->SetTargetScript(vecScripts[i]);
		pScriptUI->Activate();
	}


	// ResInfoUI 비활성화
	for (int i = 0; i < (int)RES_TYPE::END; ++i)
	{
		if (nullptr != m_arrResUI[i] && m_arrResUI[i]->IsActive())
			m_arrResUI[i]->Deactivate();
	}
}
void InspectorUI::SetTargetResource(CRes* _pTargetRes)
{

	m_pTargetRes = _pTargetRes;

	// ComponentUI 전부 비활성화
	for (int i = 0; i < (int)COMPONENT_TYPE::END; ++i)
	{
		if (nullptr != m_arrComUI[i] && m_arrComUI[i]->IsActive())
			m_arrComUI[i]->Deactivate();
	}


	// ScriptUI 전부 비활성화
	for (size_t i = 0; i < m_vecScriptUI.size(); ++i)
	{
		m_vecScriptUI[i]->Deactivate();
	}


	// 활성화 시킬 RES_TYPE 을 알아냄
	RES_TYPE type = _pTargetRes->GetResType();

	// 해당 리소스 ResInfoUI 활성화
	for (int i = 0; i < (int)RES_TYPE::END; ++i)
	{
		if (nullptr != m_arrResUI[(int)i])
		{
			if (i == (int)type)
			{
				m_arrResUI[i]->Activate();
				m_arrResUI[i]->SetTargetRes(_pTargetRes);
			}
			else
				m_arrResUI[i]->Deactivate();
		}
	}

}


ScriptUI* InspectorUI::AddScriptUI()
{
	ScriptUI* pScriptUI = new ScriptUI;
	pScriptUI->Deactivate();
	AddChild(pScriptUI);

	m_vecScriptUI.push_back(pScriptUI);

	return pScriptUI;
}

// ============= TOdo =====================================================
//  AddComponent 버튼에서 Component 를  눌렸을 때 일어날 함수 

void InspectorUI::AddComponent(DWORD_PTR _param)
{
	string strComType= (char*)_param;

	// 들어온 컴퍼넌트 이름을 통해서 TargetObject에 component 를 추가하자.

	for (UINT i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
	{
		if (strComType == ToString((COMPONENT_TYPE(i))))
		{
			COMPONENT_TYPE ComType = (COMPONENT_TYPE)i;
			if (nullptr != m_pTargetObject)
			{
				switch (ComType)
				{
				case COMPONENT_TYPE::TRANSFORM:
				{
					m_pTargetObject->AddComponent(new CTransform);
				}
					break;
				case COMPONENT_TYPE::CAMERA:
				{
					m_pTargetObject->AddComponent(new CCamera);
					m_pTargetObject->Camera()->SetProjType(PROJ_TYPE::ORTHOGRAPHIC);
					m_pTargetObject->Camera()->CheckLayerMaskAll();


				}
					break;
				case COMPONENT_TYPE::COLLIDER2D:
				{
					m_pTargetObject->AddComponent(new CCollider2D);
				}
					break;

				case COMPONENT_TYPE::ANIMATOR2D:
				{
					m_pTargetObject->AddComponent(new CAnimator2D);
				}
					break;

				case COMPONENT_TYPE::MESHRENDER:
				{
					m_pTargetObject->AddComponent(new CMeshRender);
					m_pTargetObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
					m_pTargetObject->MeshRender()->SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std2DMtrl"));
				}
					break;
				case COMPONENT_TYPE::TILEMAP:
				{
					m_pTargetObject->AddComponent(new CTileMap);
				}
					break;
				case COMPONENT_TYPE::PARTICLESYSTEM:
				{
					m_pTargetObject->AddComponent(new CParticleSystem);
				}
					break;

				case COMPONENT_TYPE::COLLIDER3D:
				case COMPONENT_TYPE::ANIMATOR3D:
				case COMPONENT_TYPE::BOUNDINGBOX:
				case COMPONENT_TYPE::LANDSCAPE:
				case COMPONENT_TYPE::DECAL:
				case COMPONENT_TYPE::SCRIPT:
				default:
					return;

				}


				if (m_pTargetObject->GetComponent(ComType))
				{
					m_arrComUI[i]->Activate();
					m_arrComUI[i]->SetTargetObject(m_pTargetObject);
				}
			}
		}


	}



}



// =======================================================					=======================================================
// =======================================================		SUB Func	=======================================================
// =======================================================					=======================================================


void InspectorUI::GameObjectTool_SubFunc()
{

	if (nullptr != m_pTargetObject)
	{
		ImGui::BeginChild("AddComponentToTargetObject", ImVec2(200.f, 100.f), true, ImGuiWindowFlags_HorizontalScrollbar);
		

			if (ImGui::Button("Add Component"))
			{

				// ListUI 활성화한다.
				//const map<wstring, CRes*>& mapRes = CResMgr::GetInst()->GetResList(RES_TYPE::COMPONENT);

				ListUI* pListUI = (ListUI*)CImGuiMgr::GetInst()->FindUI("##ListUI");
				pListUI->Clear();
				pListUI->SetTitle("Component List");

				for (int i = 0; i < (int)COMPONENT_TYPE::END; ++i)
				{
					if (nullptr == m_pTargetObject->GetComponent((COMPONENT_TYPE)i))
					{
						pListUI->AddList(ToString((COMPONENT_TYPE)i));

					}

				}

				pListUI->Activate();
				// TODO - 선택된 Component 를 TargetObjecct 에 AddComponent 한다 . 
				pListUI->SetDBCEvent(this, (DBCLKED)&InspectorUI::AddComponent);

			}

		ImGui::EndChild();
		
	}

}

