#include "pch.h"
#include "SceneOutlinerTool.h"

#include <Engine/CSceneMgr.h>
#include <Engine/CEventMgr.h>


#include <Engine/CScene.h>
#include <Engine/CLayer.h>
#include <Engine/CGameObject.h>

#include "TreeUI.h"
#include "CImGuiMgr.h"
#include "InspectorUI.h"

#include <Engine/CResMgr.h>
#include <Engine/CMesh.h>
#include <Engine/CTransform.h>
#include <Engine/CMeshRender.h>


#include <Engine/CFileMgr.h>



SceneOutlinerTool::SceneOutlinerTool()
	: UI("SceneOutlinerTool")
	, m_pSelectedScene(nullptr)
	, m_pSelectedLayer(nullptr)
	, m_pSelectedGameObject(nullptr)
	, m_RecentClickedType(OBJECT_TYPE::NONE)
{
	m_TreeUI = new TreeUI(true);
	m_TreeUI->SetTitle("SceneOutlinerTool");
	m_TreeUI->UseFrame(false);
	m_TreeUI->ShowDummyRoot(false);

	AddChild(m_TreeUI);

	// Clicked Delegate 등록
	m_TreeUI->SetClickedDelegate(this, (CLICKED)&SceneOutlinerTool::ObjectClicked);

	// Delete Pressed 등록 
	m_TreeUI->SetKeyBinding(KEY::DEL, this, (CLICKED)&SceneOutlinerTool::PressDelete);


	Reset();
}

SceneOutlinerTool::~SceneOutlinerTool()
{
}



void SceneOutlinerTool::update()
{

	if (CEventMgr::GetInst()->HasOccurObjEvent())
	{
		Reset();

	}
	UI::update();


}

void SceneOutlinerTool::render_update()
{
	FileMgrTool_SubFunc();
	

	if (ImGui::Button("New GameObject", ImVec2(120, 0)))
		ImGui::OpenPopup("Create New Obj");

		bool unused_open = true;
		if (ImGui::BeginPopupModal("Create New Obj", &unused_open))
		{
			ImGui::Text("Set New GameObject Name!\n\n");


			if (nullptr != m_pSelectedScene)
			{
				ImGui::Text("select Scene - ");
				ImGui::SameLine();
				string sName = string(m_pSelectedScene->GetName().begin(), m_pSelectedScene->GetName().end());
				ImGui::TextColored(ImVec4(0.0f, 0.5f, 0.5f, 1.f), sName.c_str());

			}
			else
				ImGui::TextColored(ImVec4(1.f, 0.0f, 0.0f, 1.f), "YOU MUST SELECT SCENE!");

			if (nullptr != m_pSelectedLayer)
			{
				ImGui::Text("select Layer - ");
				ImGui::SameLine();
				string sName = string(m_pSelectedLayer->GetName().begin(), m_pSelectedLayer->GetName().end());
				ImGui::TextColored(ImVec4(0.5f, 0.5f, 0.8f, 1.f),sName.c_str());
				ImGui::Text("Add This Obj to this Layer");

			}
			else
				ImGui::TextColored(ImVec4(1.f, 0.0f, 0.0f, 1.f), "YOU MUST SELECT LAYER!");



			static char buf[512];
			ImGui::InputText("SetName", buf, IM_ARRAYSIZE(buf), ImGuiInputTextFlags_None);

			if (ImGui::Button("Complete"))
			{
				string name = buf;
				wstring newName = wstring(name.begin(), name.end());

				CGameObject* NewObj = new CGameObject;
				NewObj->SetName(newName);
				
				// Create GameObject
				if (nullptr != m_pSelectedScene && nullptr != m_pSelectedLayer)
				{
					// 기본적으로 CTransform 을 갖는다. 
					NewObj->AddComponent(new CTransform);
					
					NewObj->Transform()->SetRelativePos(0.f, 0.f, 100.f);
					NewObj->Transform()->SetRelativeScale(200.f, 200.f, 1.f);

					
					m_pSelectedScene->AddObject(NewObj, m_pSelectedLayer->GetName());
					// TReeUI 에 추가하기 위해서 Reset() 
					Reset();

				}
				

				ImGui::CloseCurrentPopup();
				
			}
			ImGui::EndPopup();
		}
	

}

void SceneOutlinerTool::Reset()
{
	m_TreeUI->Clear();


	// 현재 Scene 을 가져온다.
	CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();

	// fisrt Tree Node - Scene 
	TreeNode* pSceneNode = PushSceneToTree(pCurScene, m_TreeUI->GetDummyNode());

	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		CLayer* pLayer = pCurScene->GetLayer(i);
		if (pLayer->GetName() == L"")
			continue;

		// second Tree Node - Layer 
		TreeNode* pLayerNode = PushLayerToTree(pLayer, pSceneNode);
		
		// Third Tree Node - GameObject 
		vector<CGameObject*>& vecRoots = pLayer->GetRootObjects();
		for (size_t i = 0; i < vecRoots.size(); ++i)
		{
			PushGameObjectToTree(vecRoots[i], pLayerNode);
		}
	}


}

void SceneOutlinerTool::ObjectClicked(DWORD_PTR _dw)
{
	TreeNode* pNode = (TreeNode*)_dw;

	string strKey = pNode->GetName();
	OBJECT_TYPE _ObjType = pNode->GetObjType();
	DWORD_PTR data = pNode->GetData();
	m_RecentClickedType = _ObjType;


	switch (_ObjType)
	{
	case OBJECT_TYPE::NONE:
		break;
	case OBJECT_TYPE::SCENE:
	{
		m_pSelectedScene = (CScene*)data;
		m_pSelectedGameObject = nullptr;
		m_pSelectedLayer = nullptr;

		InspectorUI* pInspectorUI = (InspectorUI*)CImGuiMgr::GetInst()->FindUI("Inspector");
		pInspectorUI->SetTargetObject(nullptr);

	}
		break;
	case OBJECT_TYPE::LAYER:
	{
		m_pSelectedLayer = (CLayer*)data;
		m_pSelectedGameObject = nullptr;

		InspectorUI* pInspectorUI = (InspectorUI*)CImGuiMgr::GetInst()->FindUI("Inspector");
		pInspectorUI->SetTargetObject(nullptr);
	}
		break;
	case OBJECT_TYPE::GAME_OBJECT:
	{
		CGameObject* pObject = (CGameObject*)data;
		m_pSelectedGameObject = pObject;

		// InspectorUI TargetObject setting 
		InspectorUI* pInspectorUI = (InspectorUI*)CImGuiMgr::GetInst()->FindUI("Inspector");
		pInspectorUI->SetTargetObject(pObject);
	}
		break;
	default:
		break;
	}
	

	
}

TreeNode* SceneOutlinerTool::PushSceneToTree(CScene* _pScene, TreeNode* _pDestNode)
{
	TreeNode* pNode = m_TreeUI->AddTreeNode(_pDestNode
		, "Scene - " + string(_pScene->GetName().begin(), _pScene->GetName().end())
		, (DWORD_PTR)_pScene);
	pNode->SetObjType(OBJECT_TYPE::SCENE);


	return pNode;

}

TreeNode* SceneOutlinerTool::PushLayerToTree(CLayer* _pLayer, TreeNode* _pDestNode)
{
	TreeNode* pNode = m_TreeUI->AddTreeNode(_pDestNode
		, "Layer - " + string(_pLayer->GetName().begin(), _pLayer->GetName().end())
		, (DWORD_PTR)_pLayer);
	pNode->SetObjType(OBJECT_TYPE::LAYER);


	return pNode;

}

void SceneOutlinerTool::PushGameObjectToTree(CGameObject* _pObject, TreeNode* _pDestNode)
{
	TreeNode* pNode = m_TreeUI->AddTreeNode(_pDestNode
		, string(_pObject->GetName().begin(), _pObject->GetName().end())
		, (DWORD_PTR)_pObject);
	pNode->SetObjType(OBJECT_TYPE::GAME_OBJECT);


	const vector<CGameObject*>& vecChild = _pObject->GetChild();

	for (size_t i = 0; i < vecChild.size(); ++i)
	{
		PushGameObjectToTree(vecChild[i], pNode);
	}
}




void SceneOutlinerTool::PressDelete(DWORD_PTR _dw)
{
	TreeNode* pNode = (TreeNode*)_dw;

	if (nullptr == pNode)
		return;

	CGameObject* pTargetObj = (CGameObject*)pNode->GetData();
	pTargetObj->Destroy();

	// InspectorUI 를 찾아서 Object 를 nullptr 로 세팅한다.

	InspectorUI* pInspectorUI = (InspectorUI*)CImGuiMgr::GetInst()->FindUI("Inspector");
	pInspectorUI->SetTargetObject(nullptr);
}








void SceneOutlinerTool::FileMgrTool_SubFunc()
{

	if (ImGui::BeginChild("FileMgr", ImVec2(230.f, 100.f), true, ImGuiWindowFlags_HorizontalScrollbar))
	{
		string strCurObjectType;
		if (m_RecentClickedType == OBJECT_TYPE::NONE)
			strCurObjectType = "None - ";
		if (m_RecentClickedType == OBJECT_TYPE::SCENE)
			strCurObjectType = "Scene - ";
		if (m_RecentClickedType == OBJECT_TYPE::LAYER)
			strCurObjectType = "Layer - ";
		if (m_RecentClickedType == OBJECT_TYPE::GAME_OBJECT)
			strCurObjectType = "GameObject - ";

		ImGui::Text(strCurObjectType.c_str());
		ImGui::SameLine(100);
		// 현재 TargetObject 를 파일에 저장한다. 
		if (ImGui::Button("Save to File"))
		{
			switch (m_RecentClickedType)
			{
			case OBJECT_TYPE::NONE:
				break;
			case OBJECT_TYPE::SCENE:
			{
				CFileMgr::GetInst()->SaveToFile<CScene>((DWORD_PTR)m_pSelectedScene);

			}
				break;
			case OBJECT_TYPE::LAYER:
			{
				CFileMgr::GetInst()->SaveToFile<CLayer>((DWORD_PTR)m_pSelectedLayer);

			}
				break;
			case OBJECT_TYPE::GAME_OBJECT:
			{
				CFileMgr::GetInst()->SaveToFile<CGameObject>((DWORD_PTR)m_pSelectedGameObject);

			}
				break;

			}
		}

		ImGui::Text(strCurObjectType.c_str());
		ImGui::SameLine(100);
		if (ImGui::Button("Load from File"))
		{
			CGameObject* pNewObj = (CGameObject*)CFileMgr::GetInst()->LoadFromFile<CGameObject>((DWORD_PTR)m_pSelectedGameObject);

			CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();

			// 몇번째 Layer에 저장할 것인지 정한다 
			CLayer* pArrLayer = pCurScene->GetAllLayer();

		}

		
		ImGui::EndChild();

	}

}