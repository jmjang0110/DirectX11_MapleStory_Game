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
#include "PrefabUI.h"

#include "CImGuiMgr.h"

#include <Engine/CFileMgr.h>
#include <Engine/CSceneMgr.h>
#include <Engine/CLayer.h>
#include <Engine/CResMgr.h>
#include <Engine/CKeyMgr.h>

#include <Engine/CRes.h>
#include <Engine/CPrefab.h>
// COMPONENT-TYPE
#include <Engine/CTransform.h>
#include <Engine/CCamera.h>
#include <Engine/CCollider2D.h>
#include <Engine/CAnimator2D.h>
#include <Engine/CMeshRender.h>
#include <Engine/CTileMap.h>
#include <Engine/CParticleSystem.h>


#include <ResourceUI.h>

#include <Script/CSceneSaveLoad.h>
#include <Script/CScriptMgr.h>


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

	// PrefabUI
	pResInfoUI = new PrefabUI;
	AddChild(pResInfoUI);
	m_arrResUI[(UINT)RES_TYPE::PREFAB] = pResInfoUI;


}

InspectorUI::~InspectorUI()
{
}



void InspectorUI::update()
{
	//if (KEY_TAP(KEY::I))
	//{
	//	if (IsActive())
	//		Deactivate();
	//	else
	//		Activate();
	//}
}

void InspectorUI::render_update()
{
	GameObjectTool_SubFunc();

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
	// 오브젝트가 null 이면
	if (nullptr == m_pTargetObject)
	{
		// 모든 스크립트UI 비활성화
		for (int i = 0; i < m_vecScriptUI.size(); ++i)
		{
			m_vecScriptUI[i]->Deactivate();
		}
	}
	else
	{
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

		// ScriptUI 가 더 많이 있을때
		if (vecScripts.size() < m_vecScriptUI.size())
		{
			// 대응하는 UI 를 제외한 나머지 ScriptUI 들을 비활성화 한다.ㄴ
			for (int i = vecScripts.size(); i < m_vecScriptUI.size(); ++i)
			{
				m_vecScriptUI[i]->Deactivate();
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
// - 기본값을 미리 설정해놓는다. 

void InspectorUI::AddComponent(DWORD_PTR _param)
{
	string strComType = (char*)_param;

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
					m_pTargetObject->Camera()->CheckLayerMaskAll(); // iLayerMask 몇번째 레이어 그리는지 알려준다.


				}
				break;
				case COMPONENT_TYPE::COLLIDER2D:
				{
					if (nullptr == m_pTargetObject->GetRenderComponent())
					{
						m_pTargetObject->AddComponent(new CMeshRender);
						m_pTargetObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
						m_pTargetObject->MeshRender()->SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"material\\Std2DEmptyMtrl.mtrl"));
						float fLimit = 0.3333f;
						m_pTargetObject->MeshRender()->GetSharedMaterial()->SetScalarParam(SCALAR_PARAM::FLOAT_0, &fLimit);

						m_arrComUI[(UINT)COMPONENT_TYPE::MESHRENDER]->Activate();
						m_arrComUI[(UINT)COMPONENT_TYPE::MESHRENDER]->SetTargetObject(m_pTargetObject);

					}

					m_pTargetObject->AddComponent(new CCollider2D);
					m_pTargetObject->Collider2D()->SetCollider2DType(COLLIDER2D_TYPE::BOX);
					m_pTargetObject->Collider2D()->SetOffsetPos(Vec2(0.f, 0.f));
					m_pTargetObject->Collider2D()->SetOffsetScale(Vec2(100.f, 100.f));
				}
				break;

				case COMPONENT_TYPE::ANIMATOR2D:
				{
					if (nullptr == m_pTargetObject->GetRenderComponent())
					{
						m_pTargetObject->AddComponent(new CMeshRender);
						m_pTargetObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
						m_pTargetObject->MeshRender()->SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"material\\Std2DMtrl.mtrl"));
						float fLimit = 0.3333f;
						m_pTargetObject->MeshRender()->GetSharedMaterial()->SetScalarParam(SCALAR_PARAM::FLOAT_0, &fLimit);

						m_arrComUI[(UINT)COMPONENT_TYPE::MESHRENDER]->Activate();
						m_arrComUI[(UINT)COMPONENT_TYPE::MESHRENDER]->SetTargetObject(m_pTargetObject);

					}

					m_pTargetObject->AddComponent(new CAnimator2D);
				}
				break;

				case COMPONENT_TYPE::MESHRENDER:
				{
					if (nullptr == m_pTargetObject->GetRenderComponent())
					{
						m_pTargetObject->AddComponent(new CMeshRender);
						m_pTargetObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
						m_pTargetObject->MeshRender()->SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"material\\Std2DMtrl.mtrl"));

					}

				}
				break;
				case COMPONENT_TYPE::TILEMAP:
				{
					if (nullptr != m_pTargetObject->GetRenderComponent())
						break;

					m_pTargetObject->AddComponent(new CTileMap);
					Ptr<CTexture> pTileAtlas = CResMgr::GetInst()->Load<CTexture>(L"texture\\tilemap\\YellowToyCastle\\YellowToyCastleTile.png", L"texture\\tilemap\\YellowToyCastle\\YellowToyCastleTile.png");
					m_pTargetObject->TileMap()->SetAtlasTex(pTileAtlas);
					m_pTargetObject->TileMap()->SetTileSize(Vec2(30.f, 30.f));
					m_pTargetObject->TileMap()->SetTileMapCount(12, 20);

					m_pTargetObject->Transform()->SetRelativeScale(12 * 30.f, 20 * 30.f, 1.f);

				}
				break;
				case COMPONENT_TYPE::PARTICLESYSTEM:
				{
					if (nullptr == m_pTargetObject->GetRenderComponent())
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

				// New Component 를 UI에도 활성화 
				if (m_pTargetObject->GetComponent(ComType))
				{
					m_arrComUI[i]->Activate();
					m_arrComUI[i]->SetTargetObject(m_pTargetObject);
				}
			}
		}


	}



}

void InspectorUI::AddScript(DWORD_PTR _param)
{
	string strScriptType = (char*)_param;
	wstring wstrSCriptType = wstring(strScriptType.begin(), strScriptType.end());

	m_pTargetObject->AddComponent((CComponent*)CScriptMgr::GetScript(wstrSCriptType));



}



// =======================================================					=======================================================
// =======================================================		SUB Func	=======================================================
// =======================================================					=======================================================


// _param : COMPONENT_TYPE
void InspectorUI::DeleteComponent(DWORD_PTR _param)
{
	COMPONENT_TYPE eComType = (COMPONENT_TYPE)_param;
	// Target Object 에서 해당 Component 를 삭제 
	m_pTargetObject->DeleteComponent(eComType);

	// UI 갱신 
	m_arrComUI[(UINT)eComType]->Deactivate();
	m_arrComUI[(UINT)eComType]->SetTargetObject(nullptr);
}




void InspectorUI::GameObjectTool_SubFunc()
{
	ImGui::BeginChild("GameObject tool", ImVec2(200.f, 70.f), true, ImGuiWindowFlags_HorizontalScrollbar);

	// Add Component Button 
	if (nullptr != m_pTargetObject)
	{
		if (ImGui::Button("Add Component"))
		{
			// ListUI 활성화한다.
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
	}

	// Add Script Button 
	if (nullptr != m_pTargetObject)
	{
		if (ImGui::Button("Add Script"))
		{
			// ListUI 활성화한다.
			ListUI* pListUI = (ListUI*)CImGuiMgr::GetInst()->FindUI("##ListUI");
			pListUI->Clear();
			pListUI->SetTitle("Script List");


			vector<wstring> vecScriptInfo;
			CScriptMgr::GetScriptInfo(vecScriptInfo);

			for (int i = 0; i < vecScriptInfo.size(); ++i)
			{
				if(nullptr == m_pTargetObject->GetScriptByName(vecScriptInfo[i]))
					pListUI->AddList(string(vecScriptInfo[i].begin(), vecScriptInfo[i].end()));
			}

			pListUI->Activate();
			// TODO - 선택된 Component 를 TargetObjecct 에 AddComponent 한다 . 
			pListUI->SetDBCEvent(this, (DBCLKED)&InspectorUI::AddScript);
		}
	}


	// Change To Prefab Button
	if (nullptr != m_pTargetObject)
	{
		if (ImGui::Button("Register Prefab"))
		{
			// Prefab 하려는 GameObject 가 이미 존재한다면 
			if (nullptr != CResMgr::GetInst()->FindRes<CPrefab>(m_pTargetObject->GetName()))
				return;

			// RelativePath 저장은 CPRefab Save 에서 해주고 있음 
			wstring strContent = CPathMgr::GetInst()->GetContentPath();
			wstring wstrResKey = L"prefab\\" + m_pTargetObject->GetName() + L".pref";
			wstring FullPath = strContent + wstrResKey;

			//CGameObject* pProtoObj = m_pTargetObject->Clone();
			CPrefab* pPref = new CPrefab;;// (pProtoObj);
			pPref->SetProto(m_pTargetObject->Clone());
			//pPref->Save(wstrResKey); 

			// Prefab 추가
			if (nullptr == CResMgr::GetInst()->FindRes<CPrefab>(wstrResKey))
			{
				CResMgr::GetInst()->AddRes<CPrefab>(wstrResKey, pPref);
				pPref->Save(FullPath);

				

			}
			// Prefab 이 이미 있을 경우 
			else
			{
				RES_TYPE pResType = CResMgr::GetInst()->FindRes<CPrefab>(wstrResKey)->GetResType();
				if (RES_TYPE::PREFAB == pResType)
				{
					// 기존 Prefab 을 지우고 갱신한다 
					CResMgr::GetInst()->DeletePrefabRes<CPrefab>(wstrResKey);
					CResMgr::GetInst()->AddRes<CPrefab>(wstrResKey, pPref);

					pPref->Save(FullPath);
					

				}

			}


			// CImGuiMgr 에 Delegate 등록 
			tUIDelegate tInfo;
			tInfo.dwParam = (DWORD_PTR)nullptr;
			tInfo.pFunc = (PARAM_1)&ResourceUI::Reset;
			tInfo.pInst = CImGuiMgr::GetInst()->FindUI("Resource");

			CImGuiMgr::GetInst()->AddDelegate(tInfo);
		}
	}
	ImGui::EndChild();

	ImGui::SameLine();

	ImGui::BeginChild("GameObject Delete tool", ImVec2(200.f, 70.f), true, ImGuiWindowFlags_HorizontalScrollbar);

	// Delete Object Button
	static bool bDelete = false;
	if (nullptr != m_pTargetObject)
	{
		ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(0.f, 0.8f, 0.8f));
		if (ImGui::Button("Delete This Object"))
			bDelete = true;

		if (bDelete)
		{
			ImGui::OpenPopup("Are You Sure?");
			bool unused_open = true;
			if (ImGui::BeginPopupModal("Are You Sure?", &unused_open))
			{

				ImGui::TextColored(ImVec4(1.f, 0.f, 0.f, 1.f), "WARNING\n\nAre You sure you want to delete this Object? \n\n");
				if (ImGui::Button("Yes"))
				{
					bDelete = false;
					CSceneMgr::GetInst()->DestroyObject(m_pTargetObject); // 다음프레임에 삭제됨 
					// CImGuiMgr 에 Delegate 등록 
					tUIDelegate tInfo;
					tInfo.dwParam = (DWORD_PTR)nullptr;
					tInfo.pFunc = (PARAM_1)&InspectorUI::SetTargetObject;
					tInfo.pInst = CImGuiMgr::GetInst()->FindUI("Inspector");

					CImGuiMgr::GetInst()->AddDelegate(tInfo);

					ImGui::CloseCurrentPopup();
				}
				ImGui::SameLine();
				if (ImGui::Button("No"))
				{
					bDelete = false;
					ImGui::CloseCurrentPopup();
				}
				ImGui::EndPopup();
			}

		}

		ImGui::PopStyleColor(1);

	}

	ImGui::EndChild();

}

