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
#include "SceneOutlinerTool.h"

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
#include <Engine/CCollisionMgr.h>
#include <Engine/CScript.h>


#include <ResourceUI.h>

#include <Script/CSceneSaveLoad.h>
#include <Script/CScriptMgr.h>



InspectorUI::InspectorUI()
	: UI("Inspector")
	, m_pTargetObject(nullptr)
	, m_pTargetLayer(nullptr)
	, m_pTargetScene(nullptr)
	, m_pColLeftLayer(nullptr)
	, m_pColRightLayer(nullptr)
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
	if (nullptr == m_pTargetObject)
	{
		SceneTool_subFunc();
		LayerTool_subFunc();
		SetCollisionCheck();
		ShowCollisionLayer();

	}

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
			
			// == todo ==
			// 삭제시키기 위해서 title로 이름 저장 
			wstring ScriptName = CScriptMgr::GetScriptName(vecScripts[i]);
			pScriptUI->SetTitle(string(ScriptName.begin(), ScriptName.end()));
			// == == == ==

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


	// UI 갱신 
	const vector<CScript*>& vecScripts = m_pTargetObject->GetScripts();
	ScriptUI* pScriptUI = nullptr;

	for (size_t i = 0; i < vecScripts.size(); ++i)
	{
		if (m_vecScriptUI.size() <= i)
			pScriptUI = AddScriptUI();
		else
			pScriptUI = m_vecScriptUI[i];

		// == todo ==
		// 삭제시키기 위해서 title로 이름 저장 
		wstring ScriptName = CScriptMgr::GetScriptName(vecScripts[i]);
		pScriptUI->SetTitle(string(ScriptName.begin(), ScriptName.end()));
		// == == == ==

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

// _param : CSCript*
void InspectorUI::DeleteScript(DWORD_PTR _param)
{
	CScript* pScript = (CScript*)_param;
	
	// 해당 Script 삭제 
	wstring wstrScriptName = CScriptMgr::GetScriptName(pScript);
	string strScriptName = string(wstrScriptName.begin(), wstrScriptName.end());
	m_pTargetObject->DeleteScript(wstrScriptName);

	// UI 갱신 
	for (int i = 0; i < m_vecScriptUI.size(); ++i)
	{
		if (m_vecScriptUI[i]->GetTitle() == strScriptName)
		{
			DeleteChild(m_vecScriptUI[i]);
			m_vecScriptUI.erase(m_vecScriptUI.begin() + i);
			break;
		}
	}
	
}


void InspectorUI::GameObjectTool_SubFunc()
{
	if (nullptr == m_pTargetObject)
		return;

	ImGui::BeginChild("GameObject tool", ImVec2(200.f, 70.f), true, ImGuiWindowFlags_HorizontalScrollbar);

	// Add Component Button 
	if (nullptr != m_pTargetObject)
	{
		if (ImGui::Button("Add Component", ImVec2(150.f, 20.f)))
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
		if (ImGui::Button("Add Script", ImVec2(150.f, 20.f)))
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


	// REgister  Prefab Button
	if (nullptr != m_pTargetObject)
	{
		if (ImGui::Button("Register Prefab", ImVec2(150.f, 20.f)))
		{
			// Prefab 하려는 GameObject 가 이미 존재한다면 
			if (nullptr != CResMgr::GetInst()->FindRes<CPrefab>(m_pTargetObject->GetName()))
				return;

			// RelativePath 저장은 CPRefab Save 에서 해주고 있음 
			wstring strContent = CPathMgr::GetInst()->GetContentPath();
			wstring wstrResKey = L"prefab\\" + m_pTargetObject->GetName() + L".pref";
			wstring FullPath = strContent + wstrResKey;

			CPrefab* pPref = new CPrefab;;// (pProtoObj);
			pPref->SetProto(m_pTargetObject->Clone());

			// Prefab 추가
			if (nullptr == CResMgr::GetInst()->FindRes<CPrefab>(wstrResKey))
			{
				// Prefab 추가 
				CResMgr::GetInst()->AddRes<CPrefab>(wstrResKey, pPref);

			}
			// Prefab 이 이미 있을 경우 
			else
			{
				RES_TYPE pResType = CResMgr::GetInst()->FindRes<CPrefab>(wstrResKey)->GetResType();
				if (RES_TYPE::PREFAB == pResType)
				{
					// 기존 Prefab 을 지우고 갱신한다 
					CResMgr::GetInst()->UpdatePrefabRes<CPrefab>(wstrResKey, pPref);

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

void InspectorUI::SceneTool_subFunc()
{
	if (nullptr == m_pTargetScene)
		return;

	wstring wstrSceneName = m_pTargetScene->GetName();
	string strSceneName = string(wstrSceneName.begin(), wstrSceneName.end());


	ImGui::PushID(0);
	ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(0.8f, 0.f, 0.5f));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(1.f, 0.2f, 0.8f));
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(0.f, 0.8f, 0.8f));
	ImGui::Button(strSceneName.c_str());
	ImGui::PopStyleColor(3);
	ImGui::PopID();

	ImGui::SameLine();
	bool unused_open = true;
	if (ImGui::Button("Change Scene Name"))
		ImGui::OpenPopup("Change Scene Name");

	ImVec2 center = ImGui::GetMainViewport()->GetCenter();
	ImGui::SetNextWindowPos(center, ImGuiCond_Always, ImVec2(0.5f, 0.5f));
	if (ImGui::BeginPopupModal("Change Scene Name", &unused_open))
	{
		ImGui::Text("Change Scene Name\n\n");

		static char buf[512];
		ImGui::InputText("name", buf, IM_ARRAYSIZE(buf), ImGuiInputTextFlags_None);

		if (ImGui::Button("Change Scene Name"))
		{
			string strName = buf;
			wstring wstrName = wstring(strName.begin(), strName.end());

			// File 저장 
			m_pTargetScene->SetName(wstrName);
			wstring SceneResKey = L"scene\\" + wstrName + L".scene";
			m_pTargetScene->SetResKey(SceneResKey);
			wstring strSceneFilePath = CPathMgr::GetInst()->GetContentPath();
			CSceneSaveLoad::SaveScene(m_pTargetScene, strSceneFilePath + SceneResKey);

			// CImGuiMgr 에 Delegate 등록 
			tUIDelegate tDeleteCom;
			tDeleteCom.dwParam = (DWORD_PTR)nullptr;
			tDeleteCom.pFunc = (PARAM_1)&SceneOutlinerTool::Reset;
			tDeleteCom.pInst = CImGuiMgr::GetInst()->FindUI("SceneOutlinerTool");

			CImGuiMgr::GetInst()->AddDelegate(tDeleteCom);
			ImGui::CloseCurrentPopup();

			buf[0] = '\0';
			ImGui::CloseCurrentPopup();
			SetModalUI(false);
		}
		ImGui::EndPopup();
	}
}

void InspectorUI::LayerTool_subFunc()
{
	if (nullptr == m_pTargetLayer)
		return;

	wstring wstrLayerName = m_pTargetLayer->GetName();
	string strLayerName = string(wstrLayerName.begin(), wstrLayerName.end());


	ImGui::PushID(0);
	ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(0.8f, 0.f, 0.5f));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(1.f, 0.2f, 0.8f));
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(0.f, 0.8f, 0.8f));
	ImGui::Button(strLayerName.c_str());
	ImGui::PopStyleColor(3);
	ImGui::PopID();

	ImGui::SameLine();
	bool unused_open = true;
	if (ImGui::Button("Change Layer Name"))
		ImGui::OpenPopup("Change Layer Name");

	ImVec2 center = ImGui::GetMainViewport()->GetCenter();
	ImGui::SetNextWindowPos(center, ImGuiCond_Always, ImVec2(0.5f, 0.5f));
		if (ImGui::BeginPopupModal("Change Layer Name", &unused_open))
		{
			ImGui::Text("Change Layer Name\n\n");

			static char buf[512];
			ImGui::InputText("name", buf, IM_ARRAYSIZE(buf), ImGuiInputTextFlags_None);

			if (ImGui::Button("Change Layer Name"))
			{
				string strName = buf;
				wstring wstrName = wstring(strName.begin(), strName.end());

				m_pTargetLayer->SetName(wstrName);

				// CImGuiMgr 에 Delegate 등록 
				tUIDelegate tDeleteCom;
				tDeleteCom.dwParam = (DWORD_PTR)nullptr;
				tDeleteCom.pFunc = (PARAM_1)&SceneOutlinerTool::Reset;
				tDeleteCom.pInst = CImGuiMgr::GetInst()->FindUI("SceneOutlinerTool");

				CImGuiMgr::GetInst()->AddDelegate(tDeleteCom);
				ImGui::CloseCurrentPopup();

				buf[0] = '\0';
				ImGui::CloseCurrentPopup();
				SetModalUI(false);
			}
			ImGui::EndPopup();
		}

}




void InspectorUI::SetCollisionCheck()
{
	if (m_pTargetScene == nullptr)
		return;
	ImGui::Separator();

	static string LeftLayerName = "Left Layer";
	static string RIghtLayerName = "Right Layer";

	// Fill Layer Info
	vector<CLayer*> vecLayer;
	for (int i = 0; i < MAX_LAYER; ++i)
	{
		if (L"" == m_pTargetScene->GetLayer(i)->GetName())
			continue;

		vecLayer.push_back(m_pTargetScene->GetLayer(i));
	}

	// Left Collide Layer Setting
	ImGui::PushItemWidth(100.f);
	if (ImGui::BeginCombo("##Left Layer", LeftLayerName.c_str(), ImGuiComboFlags_NoArrowButton))
	{
		for (int n = 0; n < vecLayer.size(); ++n)
		{
			string LayerName = string(vecLayer[n]->GetName().begin(), vecLayer[n]->GetName().end());
			if (ImGui::Selectable(LayerName.c_str()))
			{
				LeftLayerName = LayerName;
				m_pColLeftLayer = vecLayer[n];
			}
		}
		ImGui::EndCombo();
	}
	ImGui::PopItemWidth();

	ImGui::SameLine();
	ImGui::Text("--");
	ImGui::SameLine();
	
	// Collide Setting Button 
	if (ImGui::Button("Collide"))
	{
		if (nullptr != m_pColLeftLayer && nullptr != m_pColRightLayer)
		{
			CCollisionMgr::GetInst()->CollisionCheck(m_pColLeftLayer->GetName(), m_pColRightLayer->GetName());

			LeftLayerName = "Left Layer";
			RIghtLayerName = "Right Layer";
			m_pColLeftLayer = nullptr;
			m_pColRightLayer = nullptr;

		}
	}
	
	
	ImGui::SameLine();
	ImGui::Text("--");
	ImGui::SameLine();

	ImGui::PushItemWidth(100.f);

	// Right Collide Layer Setting 
	if (ImGui::BeginCombo("##Right Layer", RIghtLayerName.c_str(), ImGuiComboFlags_NoArrowButton))
	{
		for (int n = 0; n < vecLayer.size(); ++n)
		{
			string LayerName = string(vecLayer[n]->GetName().begin(), vecLayer[n]->GetName().end());
			if (ImGui::Selectable(LayerName.c_str()))
			{
				RIghtLayerName = LayerName;
				m_pColRightLayer = vecLayer[n];
			}
		}
		ImGui::EndCombo();
	}
	ImGui::PopItemWidth();

}

void InspectorUI::ShowCollisionLayer()
{
	if (m_pTargetScene == nullptr)
		return;
	const UINT* arrCol = CCollisionMgr::GetInst()->GetColArrCheck();
	
	ImGui::Separator();

	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		for (UINT j = i; j < MAX_LAYER; ++j)
		{
			if (arrCol[i] & (1 << j))
			{
				CLayer* LeftLayer = m_pTargetScene->GetLayer(i);
				CLayer* RightLayer = m_pTargetScene->GetLayer(j);

				string LeftName = string(LeftLayer->GetName().begin(), LeftLayer->GetName().end());
				string RIghtName = string(RightLayer->GetName().begin(), RightLayer->GetName().end());
				
				string ButtonNum = std::to_string(i * MAX_LAYER + j);
				string ButtonName = ButtonNum + string("_Off");
				if (ImGui::Button(ButtonName.c_str()))
				{
					
					CCollisionMgr::GetInst()->CollisionOff(i, j);

					// Left Layer 
					vector<CGameObject*> LeftvecObjs;
					vector<CGameObject*> RightvecObjs;

					LeftvecObjs = LeftLayer->GetRootObjects();
					RightvecObjs = RightLayer->GetRootObjects();

					for (int i = 0; i < LeftvecObjs.size(); ++i)
					{
						for (int k = 0; k < RightvecObjs.size(); ++k)
						{
							CCollider2D* LeftCol = LeftvecObjs[i]->Collider2D();
							CCollider2D* RightCol = RightvecObjs[k]->Collider2D();

							if (nullptr == LeftCol || nullptr == RightCol)
								continue;
							if (LeftCol->GetID() == RightCol->GetID())
								continue;


							if (IsCollision(LeftCol, RightCol))
							{
								if (LeftCol->GetCollisionCount() > 0 &&
									RightCol->GetCollisionCount() > 0)
								{
									LeftCol->OnCollisionExit(RightCol);
									RightCol->OnCollisionExit(LeftCol);

									// 같은 레이어끼리 충돌체크 했을 때 (i = j) / (j = i) 
									// 두번 충돌체크 하기에 없앨 때도 한번 더 처리 한다. 
									if (LeftLayer == RightLayer)
									{
										LeftCol->OnCollisionExit(RightCol);
										RightCol->OnCollisionExit(LeftCol);
									}


								}
							}

							CCollisionMgr::GetInst()->EraseColInfo(LeftCol, RightCol);
						}
					}
				}
				ImGui::SameLine();
				ImGui::Text(LeftName.c_str());
				ImGui::SameLine();
				ImGui::TextColored(ImVec4(0.f, 0.9f, 0.5f, 1.f), "Collide Checking");
				ImGui::SameLine();
				ImGui::Text(RIghtName.c_str());
			}
		}
	}
	

	ImGui::Separator();

}



bool InspectorUI::IsCollision(CCollider2D* _pLeftCol, CCollider2D* _pRightCol)
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

bool InspectorUI::IsCollision_Box(CCollider2D* _pLeftCol, CCollider2D* _pRightCol)
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

	// 월드로 보낸 정점을 통해서 각 투영 축 이면서 투영시킬 벡터 성분 4개를 구한다.
	vAsix[0] = XMVector3TransformCoord(vLocalPos[1], matLeft) - XMVector3TransformCoord(vLocalPos[0], matLeft);
	vAsix[1] = XMVector3TransformCoord(vLocalPos[3], matLeft) - XMVector3TransformCoord(vLocalPos[0], matLeft);
	vAsix[2] = XMVector3TransformCoord(vLocalPos[1], matRight) - XMVector3TransformCoord(vLocalPos[0], matRight);
	vAsix[3] = XMVector3TransformCoord(vLocalPos[3], matRight) - XMVector3TransformCoord(vLocalPos[0], matRight);

	// 월드에 배치된 두 충돌체의 중심을 이은 벡터
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

bool InspectorUI::IsCollision_Circle(CCollider2D* _pLeftCol, CCollider2D* _pRightCol)
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
