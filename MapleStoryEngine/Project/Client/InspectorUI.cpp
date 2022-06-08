#include "pch.h"
#include "InspectorUI.h"

#include <Engine/CKeyMgr.h>

#include "TransformUI.h"
#include "MeshRenderUI.h"
#include "CameraUI.h"


#include "MaterialUI.h"
#include "TextureUI.h"
#include "Animator2DUI.h"
#include "GraphicsShaderUI.h"
#include "ComputeShaderUI.h"
#include "MeshUI.h"
#include "ParticleSystemUI.h"
#include "Collider2DUI.h"
#include "ListUI.h"



#include "CImGuiMgr.h"



#include <Engine/CFileMgr.h>
#include <Engine/CSceneMgr.h>
#include <Engine/CLayer.h>
#include <Engine/CResMgr.h>




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
	if (ImGui::BeginChild("FileMgr", ImVec2(230.f, 100.f), true, ImGuiWindowFlags_HorizontalScrollbar))
	{
		// 현재 TargetObject 를 파일에 저장한다. 
		if (ImGui::Button("Save to File  "))
			CFileMgr::GetInst()->SaveToFile<CGameObject>((DWORD_PTR)m_pTargetObject);

		if (ImGui::Button("Load from File"))
		{
			CGameObject* pNewObj = (CGameObject*)CFileMgr::GetInst()->LoadFromFile<CGameObject>((DWORD_PTR)m_pTargetObject);

			CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();

			// 몇번째 Layer에 저장할 것인지 정한다 
			CLayer* pArrLayer = pCurScene->GetAllLayer();
			int LayerCnt = 0;

			for (int i = 0; i < MAX_LAYER; ++i)
			{
				if (nullptr != &pArrLayer[i])
				{
					++LayerCnt;
					ImGui::Text("%d", i);


				}
			}
		}

		// 현재 TargetObject 를 파일로부터 로드한 Obj 로 바꾼다. 
		if (ImGui::Button("Load from File to this Object"))
		{
			CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();
			if (nullptr != pCurScene)
			{
				CGameObject* pPrevTargetObj = m_pTargetObject->Clone();
				CLayer* pLayer = pCurScene->GetLayer(m_pTargetObject->GetLayerIdx());

				pLayer->DeregisterObject(m_pTargetObject);

				m_pTargetObject = (CGameObject*)CFileMgr::GetInst()->LoadFromFile<CGameObject>((DWORD_PTR)m_pTargetObject);

				if (m_pTargetObject->GetLayerIdx() < 0)
				{
					m_pTargetObject = pPrevTargetObj;
					pLayer->AddObject(m_pTargetObject);
				}
				else
				{
					SetTargetObject(m_pTargetObject);
					pLayer->AddObject(m_pTargetObject);

				}

			}
		}

		ImGui::EndChild();

	}

	ImGui::SameLine();

	if (nullptr != m_pTargetObject)
	{
		if (ImGui::BeginChild("AddComponentToTargetObject", ImVec2(200.f, 100.f), true, ImGuiWindowFlags_HorizontalScrollbar))
		{

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

	ImGui::Separator();
}


void InspectorUI::SetTargetObject(CGameObject* _pTarget)
{
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



//  AddComponent 버튼에서 Component 를  눌렸을 때 일어날 함수 

void InspectorUI::AddComponent(DWORD_PTR _param)
{
	string strSelectedName = (char*)_param;

	// 들어온 컴퍼넌트 이름을 통해서 TargetObject에 component 를 추가하자. 
}




