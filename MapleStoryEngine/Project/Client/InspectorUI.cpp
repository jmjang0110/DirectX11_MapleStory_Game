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
#include "Light2DUI.h"

#include "DamageParticleUI.h"



#include "CImGuiMgr.h"

#include <Engine/CFileMgr.h>
#include <Engine/CSceneMgr.h>
#include <Engine/CLayer.h>
#include <Engine/CResMgr.h>
#include <Engine/CKeyMgr.h>

#include <Engine/CRes.h>
#include <Engine/CPrefab.h>
#include <Engine/CRenderMgr.h>

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
#include <Engine/CLight2D.h>

#include <Engine/CDamageParticle.h>

#include <ResourceUI.h>

#include <Script/CSceneSaveLoad.h>
#include <Script/CScriptMgr.h>

#include <Engine/CDamageParticle.h>




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
	// ComponentUI ����   
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

	// Daamage PArticle SYstem UI
	pComUI = new DamageParticleUI;
	AddChild(pComUI);
	m_arrComUI[(UINT)COMPONENT_TYPE::DAMAGE_PARTICLESYSTEM] = pComUI;


	//Collider2D SYstem UI
	pComUI = new Collider2DUI;
	AddChild(pComUI);
	m_arrComUI[(UINT)COMPONENT_TYPE::COLLIDER2D] = pComUI;

	// TileMapUI
	pComUI = new TileMapUI;
	AddChild(pComUI);
	m_arrComUI[(UINT)COMPONENT_TYPE::TILEMAP] = pComUI;

	// Light2DUI
	pComUI = new Light2DUI;
	AddChild(pComUI);
	m_arrComUI[(UINT)COMPONENT_TYPE::LIGHT2D] = pComUI;

	// ==============
	// ResInfoUI ����
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
			// Object �� nullptr �� ���
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
	// ������Ʈ�� null �̸�
	if (nullptr == m_pTargetObject)
	{
		// ��� ��ũ��ƮUI ��Ȱ��ȭ
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
			// ������Ű�� ���ؼ� title�� �̸� ���� 
			wstring ScriptName = CScriptMgr::GetScriptName(vecScripts[i]);
			pScriptUI->SetTitle(string(ScriptName.begin(), ScriptName.end()));
			// == == == ==

			pScriptUI->SetTargetObject(m_pTargetObject);
			pScriptUI->SetTargetScript(vecScripts[i]);
			pScriptUI->Activate();
		}

		// ScriptUI �� �� ���� ������
		if (vecScripts.size() < m_vecScriptUI.size())
		{
			// �����ϴ� UI �� ������ ������ ScriptUI ���� ��Ȱ��ȭ �Ѵ�.��
			for (int i = vecScripts.size(); i < m_vecScriptUI.size(); ++i)
			{
				m_vecScriptUI[i]->Deactivate();
			}
		}
	}


	// ResInfoUI ��Ȱ��ȭ
	for (int i = 0; i < (int)RES_TYPE::END; ++i)
	{
		if (nullptr != m_arrResUI[i] && m_arrResUI[i]->IsActive())
			m_arrResUI[i]->Deactivate();
	}
}

void InspectorUI::SetTargetResource(CRes* _pTargetRes)
{
	// ComponentUI ���� ��Ȱ��ȭ
	for (int i = 0; i < (int)COMPONENT_TYPE::END; ++i)
	{
		if (nullptr != m_arrComUI[i] && m_arrComUI[i]->IsActive())
			m_arrComUI[i]->Deactivate();
	}

	// ScriptUI ���� ��Ȱ��ȭ
	for (size_t i = 0; i < m_vecScriptUI.size(); ++i)
	{
		m_vecScriptUI[i]->Deactivate();
	}

	// Ȱ��ȭ ��ų RES_TYPE �� �˾Ƴ�
	RES_TYPE type = _pTargetRes->GetResType();

	// �ش� ���ҽ� ResInfoUI Ȱ��ȭ
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
//  AddComponent ��ư���� Component ��  ������ �� �Ͼ �Լ� 
// - �⺻���� �̸� �����س��´�. 

void InspectorUI::AddComponent(DWORD_PTR _param)
{
	string strComType = (char*)_param;

	// ���� ���۳�Ʈ �̸��� ���ؼ� TargetObject�� component �� �߰�����.

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
					m_pTargetObject->Camera()->CheckLayerMaskAll(); // iLayerMask ���° ���̾� �׸����� �˷��ش�.


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

				case COMPONENT_TYPE::DAMAGE_PARTICLESYSTEM:
				{
					if (nullptr == m_pTargetObject->GetRenderComponent())
					{
						m_pTargetObject->AddComponent(new CDamageParticle);
						Ptr<CTexture> pParticleTex = CResMgr::GetInst()->Load<CTexture>(L"texture\\hit\\damageSkin.png", L"texture\\hit\\damageSkin.png");
						Vec2 sliceUV = Vec2(37.f / pParticleTex->Width(), 39.f / pParticleTex->Height());
						Vec2 StartUV = Vec2(0.f / pParticleTex->Width(), 100.f / pParticleTex->Height());
						// TEST 
						m_pTargetObject->DamageParticle()->GetMaterial()->SetTexParam(TEX_PARAM::TEX_0, pParticleTex);
						m_pTargetObject->DamageParticle()->SetDamageNum(123456789);
						m_pTargetObject->DamageParticle()->SetStartLT_UV(StartUV);
						m_pTargetObject->DamageParticle()->setSliceUV(sliceUV);

						m_pTargetObject->DamageParticle()->SetMaxParticleCount(9);
						m_pTargetObject->DamageParticle()->SetAliveCount(9);


					}

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

				// New Component �� UI���� Ȱ��ȭ 
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
	if (m_pTargetObject == nullptr)
		return;

	string strScriptType = (char*)_param;
	wstring wstrSCriptType = wstring(strScriptType.begin(), strScriptType.end());

	m_pTargetObject->AddComponent((CComponent*)CScriptMgr::GetScript(wstrSCriptType));
	CScript* pScript = m_pTargetObject->GetScriptByName(wstrSCriptType);
	if (pScript != nullptr)
		pScript->start();


	// UI ���� 
	const vector<CScript*>& vecScripts = m_pTargetObject->GetScripts();
	ScriptUI* pScriptUI = nullptr;

	for (size_t i = 0; i < vecScripts.size(); ++i)
	{
		if (m_vecScriptUI.size() <= i)
			pScriptUI = AddScriptUI();
		else
			pScriptUI = m_vecScriptUI[i];

		// == todo ==
		// ������Ű�� ���ؼ� title�� �̸� ���� 
		wstring ScriptName = CScriptMgr::GetScriptName(vecScripts[i]);
		pScriptUI->SetTitle(string(ScriptName.begin(), ScriptName.end()));
		// == == == ==

		pScriptUI->SetTargetObject(m_pTargetObject);
		pScriptUI->SetTargetScript(vecScripts[i]);
		pScriptUI->Activate();
	}

	// ScriptUI �� �� ���� ������
	if (vecScripts.size() < m_vecScriptUI.size())
	{
		// �����ϴ� UI �� ������ ������ ScriptUI ���� ��Ȱ��ȭ �Ѵ�.��
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
	// Target Object ���� �ش� Component �� ���� 
	m_pTargetObject->DeleteComponent(eComType);

	// UI ���� 
	m_arrComUI[(UINT)eComType]->Deactivate();
	m_arrComUI[(UINT)eComType]->SetTargetObject(nullptr);
}

// _param : CSCript*
void InspectorUI::DeleteScript(DWORD_PTR _param)
{
	CScript* pScript = (CScript*)_param;

	// �ش� Script ���� 
	wstring wstrScriptName = CScriptMgr::GetScriptName(pScript);
	string strScriptName = string(wstrScriptName.begin(), wstrScriptName.end());
	m_pTargetObject->DeleteScript(wstrScriptName);

	// UI ���� 
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

	ImVec2 ToolBoxSize = ImVec2(200.f, 80.f);

	ImGui::BeginChild("GameObject tool", ToolBoxSize, true, ImGuiWindowFlags_HorizontalScrollbar);

	// Add Component Button 
	if (nullptr != m_pTargetObject)
	{
		if (ImGui::Button("Add Component", ImVec2(150.f, 20.f)))
		{
			// ListUI Ȱ��ȭ�Ѵ�.
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
			// TODO - ���õ� Component �� TargetObjecct �� AddComponent �Ѵ� . 
			pListUI->SetDBCEvent(this, (DBCLKED)&InspectorUI::AddComponent);
		}
	}

	// Add Script Button 
	if (nullptr != m_pTargetObject)
	{
		if (ImGui::Button("Add Script", ImVec2(150.f, 20.f)))
		{
			// ListUI Ȱ��ȭ�Ѵ�.
			ListUI* pListUI = (ListUI*)CImGuiMgr::GetInst()->FindUI("##ListUI");
			pListUI->Clear();
			pListUI->SetTitle("Script List");


			vector<wstring> vecScriptInfo;
			CScriptMgr::GetScriptInfo(vecScriptInfo);

			for (int i = 0; i < vecScriptInfo.size(); ++i)
			{
				// Except Script List 
				if (vecScriptInfo[i] == L"CIdleStateScript" ||
					vecScriptInfo[i] == L"CStateScript" ||
					vecScriptInfo[i] == L"CTraceStateScript")
					continue;

				if (nullptr == m_pTargetObject->GetScriptByName(vecScriptInfo[i]))
					pListUI->AddList(string(vecScriptInfo[i].begin(), vecScriptInfo[i].end()));
			}

			pListUI->Activate();
			// TODO - ���õ� Component �� TargetObjecct �� AddComponent �Ѵ� . 
			pListUI->SetDBCEvent(this, (DBCLKED)&InspectorUI::AddScript);
		}
	}


	// REgister  Prefab Button
	if (nullptr != m_pTargetObject)
	{
		if (ImGui::Button("Register Prefab", ImVec2(150.f, 20.f)))
		{
			// Prefab �Ϸ��� GameObject �� �̹� �����Ѵٸ� 
			if (nullptr != CResMgr::GetInst()->FindRes<CPrefab>(m_pTargetObject->GetName()))
				return;

			// RelativePath ������ CPRefab Save ���� ���ְ� ���� 
			wstring strContent = CPathMgr::GetInst()->GetContentPath();
			wstring wstrResKey = L"prefab\\" + m_pTargetObject->GetName() + L".pref";
			wstring FullPath = strContent + wstrResKey;

			CPrefab* pPref = new CPrefab;;// (pProtoObj);
			CGameObject* pCloneObj = m_pTargetObject->Clone();

			pPref->SetProto(pCloneObj);

			// Prefab �߰�
			if (nullptr == CResMgr::GetInst()->FindRes<CPrefab>(wstrResKey))
			{
				// Prefab �߰� 
				CResMgr::GetInst()->AddRes<CPrefab>(wstrResKey, pPref);

			}
			// Prefab �� �̹� ���� ��� 
			else
			{
				RES_TYPE pResType = CResMgr::GetInst()->FindRes<CPrefab>(wstrResKey)->GetResType();
				if (RES_TYPE::PREFAB == pResType)
				{
					// ���� Prefab �� ����� �����Ѵ� 
					CResMgr::GetInst()->UpdatePrefabRes<CPrefab>(wstrResKey, pPref);

				}

			}


			// CImGuiMgr �� Delegate ��� 
			tUIDelegate tInfo;
			tInfo.dwParam = (DWORD_PTR)nullptr;
			tInfo.pFunc = (PARAM_1)&ResourceUI::Reset;
			tInfo.pInst = CImGuiMgr::GetInst()->FindUI("Resource");

			CImGuiMgr::GetInst()->AddDelegate(tInfo);
		}
	}
	ImGui::EndChild();

	ImGui::SameLine();

	ImGui::BeginChild("GameObject Delete tool", ToolBoxSize, true, ImGuiWindowFlags_HorizontalScrollbar);

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
					CSceneMgr::GetInst()->DestroyObject(m_pTargetObject); // ���������ӿ� ������ 
					// CImGuiMgr �� Delegate ��� 
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


	bool unused_open = true;
	if (ImGui::Button("Change Object Name"))
		ImGui::OpenPopup("Change Object Name");

	ImVec2 center = ImGui::GetMainViewport()->GetCenter();
	ImGui::SetNextWindowPos(center, ImGuiCond_Always, ImVec2(0.5f, 0.5f));
	if (ImGui::BeginPopupModal("Change Object Name", &unused_open))
	{
		ImGui::Text("Change Object Name\n\n");

		static char buf[512];
		ImGui::InputText("name", buf, IM_ARRAYSIZE(buf), ImGuiInputTextFlags_None);

		if (ImGui::Button("Complete"))
		{
			string strName = buf;
			wstring wstrName = wstring(strName.begin(), strName.end());

			// Change GameObject Name 
			if (m_pTargetObject != nullptr)
			{
				m_pTargetObject->SetName(wstrName);
			}


			// CImGuiMgr �� Delegate ��� 
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

	ImGui::SameLine(150.f);
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

		if (ImGui::Button("Complete"))
		{
			string strName = buf;
			wstring wstrName = wstring(strName.begin(), strName.end());

			// File ���� 
			m_pTargetScene->SetName(wstrName);
			wstring SceneResKey = L"scene\\" + wstrName + L".scene";
			m_pTargetScene->SetResKey(SceneResKey);
			wstring strSceneFilePath = CPathMgr::GetInst()->GetContentPath();
			CSceneSaveLoad::SaveScene(m_pTargetScene, strSceneFilePath + SceneResKey);

			// CImGuiMgr �� Delegate ��� 
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

	ImGui::SameLine(150.f);
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

		if (ImGui::Button("Complete"))
		{
			string strName = buf;
			wstring wstrName = wstring(strName.begin(), strName.end());

			m_pTargetLayer->SetName(wstrName);

			// CImGuiMgr �� Delegate ��� 
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

					for (int t = 0; t < LeftvecObjs.size(); ++t)
					{
						for (int k = 0; k < RightvecObjs.size(); ++k)
						{
							CCollider2D* LeftCol = LeftvecObjs[t]->Collider2D();
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

									// ���� ���̾�� �浹üũ ���� �� (i = j) / (j = i) 
									// �ι� �浹üũ �ϱ⿡ ���� ���� �ѹ� �� ó�� �Ѵ�. 
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







void InspectorUI::CollisionOffAll(CScene* _pScene)
{
	const UINT* arrCol = CCollisionMgr::GetInst()->GetColArrCheck();

	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		for (UINT j = i; j < MAX_LAYER; ++j)
		{
			if (arrCol[i] & (1 << j))
			{
				CLayer* LeftLayer = _pScene->GetLayer(i);
				CLayer* RightLayer = _pScene->GetLayer(j);

				string LeftName = string(LeftLayer->GetName().begin(), LeftLayer->GetName().end());
				string RIghtName = string(RightLayer->GetName().begin(), RightLayer->GetName().end());


				CCollisionMgr::GetInst()->CollisionOff(i, j);

				// Left Layer 
				vector<CGameObject*> LeftvecObjs;
				vector<CGameObject*> RightvecObjs;

				LeftvecObjs = LeftLayer->GetRootObjects();
				RightvecObjs = RightLayer->GetRootObjects();

				for (int t = 0; t < LeftvecObjs.size(); ++t)
				{
					for (int k = 0; k < RightvecObjs.size(); ++k)
					{
						CCollider2D* LeftCol = LeftvecObjs[t]->Collider2D();
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

								// ���� ���̾�� �浹üũ ���� �� (i = j) / (j = i) 
								// �ι� �浹üũ �ϱ⿡ ���� ���� �ѹ� �� ó�� �Ѵ�. 
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
		}
	}
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
	// �浹ü�� ����ϴ� �⺻ ����(�簢��) ���� ������ġ�� �˾Ƴ���.
	// 0 -- 1
	// | \  |
	// 3 -- 2	
	static CMesh* pRectMesh = CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh").Get();
	static Vtx* pVtx = pRectMesh->GetVtxSysMem();
	static Vec3 vLocalPos[4] = { pVtx[0].vPos, pVtx[1].vPos, pVtx[2].vPos, pVtx[3].vPos };

	Matrix matLeft = _pLeftCol->GetWorldMat();
	Matrix matRight = _pRightCol->GetWorldMat();

	// Local �����̽��� �װ��� ������ �� �浹ü ���� ��ġ�� ������.
	Vec3 vAsix[4] = {};

	// ����� ���� ������ ���ؼ� �� ���� �� �̸鼭 ������ų ���� ���� 4���� ���Ѵ�.
	vAsix[0] = XMVector3TransformCoord(vLocalPos[1], matLeft) - XMVector3TransformCoord(vLocalPos[0], matLeft);
	vAsix[1] = XMVector3TransformCoord(vLocalPos[3], matLeft) - XMVector3TransformCoord(vLocalPos[0], matLeft);
	vAsix[2] = XMVector3TransformCoord(vLocalPos[1], matRight) - XMVector3TransformCoord(vLocalPos[0], matRight);
	vAsix[3] = XMVector3TransformCoord(vLocalPos[3], matRight) - XMVector3TransformCoord(vLocalPos[0], matRight);

	// ���忡 ��ġ�� �� �浹ü�� �߽��� ���� ����
	Vec3 vCenter = _pRightCol->GetWorldPos() - _pLeftCol->GetWorldPos();

	for (int i = 0; i < 4; ++i)
	{
		vAsix[i].z = 0.f;
		Vec3 vProj = vAsix[i];
		vProj.Normalize();

		float fDist = 0.f;

		for (int j = 0; j < 4; ++j)
		{
			// vProj �� vAsix[j] �� ������Ų ����		
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
