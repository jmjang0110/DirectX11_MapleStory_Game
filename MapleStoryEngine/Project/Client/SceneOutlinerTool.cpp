#include "pch.h"
#include "SceneOutlinerTool.h"

#include <Engine/CSceneMgr.h>
#include <Engine/CScene.h>
#include <Engine/CLayer.h>
#include <Engine/CGameObject.h>

#include "TreeUI.h"
#include "CImGuiMgr.h"
#include "InspectorUI.h"

#include <Engine/CLayer.h>
#include <Engine/CScene.h>


SceneOutlinerTool::SceneOutlinerTool()
	: UI("SceneOutlinerTool")
	, m_pSelectedScene(nullptr)
	, m_pSelectedLayer(nullptr)
	, m_pSelectedGameObject(nullptr)
{
	m_TreeUI = new TreeUI(true);
	m_TreeUI->SetTitle("SceneOutlinerTool");
	m_TreeUI->UseFrame(false);
	m_TreeUI->ShowDummyRoot(false);

	AddChild(m_TreeUI);

	// Clicked Delegate 등록
	m_TreeUI->SetClickedDelegate(this, (CLICKED)&SceneOutlinerTool::ObjectClicked);

	Reset();
}

SceneOutlinerTool::~SceneOutlinerTool()
{
}



void SceneOutlinerTool::update()
{
	UI::update();


}

void SceneOutlinerTool::render_update()
{

	if (ImGui::Button("New GameObject", ImVec2(120, 0)))
		ImGui::OpenPopup("Create New Obj");

		bool unused_open = true;
		if (ImGui::BeginPopupModal("Create New Obj", &unused_open))
		{
			ImGui::Text("Set New GameObject Name!\n\n");

			static char buf[512];
			ImGui::InputText("SetName", buf, IM_ARRAYSIZE(buf), ImGuiInputTextFlags_None);

			if (ImGui::Button("Complete"))
			{
				string name = buf;
				wstring newName = wstring(name.begin(), name.end());

				CGameObject* NewObj = new CGameObject;
				NewObj->SetName(newName);
				
				if (nullptr != m_pSelectedScene && nullptr != m_pSelectedLayer)
				{
					// 삭제하기 전에 해당 m_TreeUI 를 UI:: vecChild 에서 빼야한다. 
					DeleteChild(m_TreeUI);

					m_TreeUI = new TreeUI(true);
					m_TreeUI->SetTitle("SceneOutlinerTool");
					m_TreeUI->UseFrame(false);
					m_TreeUI->ShowDummyRoot(false);

					AddChild(m_TreeUI);

					// Clicked Delegate 등록
					m_TreeUI->SetClickedDelegate(this, (CLICKED)&SceneOutlinerTool::ObjectClicked);
				
					m_pSelectedScene->AddObject(NewObj, m_pSelectedLayer->GetName());
					
					Reset();

				}
				

				ImGui::CloseCurrentPopup();
				
			}
			ImGui::EndPopup();
		}
	

}

void SceneOutlinerTool::Reset()
{
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
	int i = 0;


}

void SceneOutlinerTool::ObjectClicked(DWORD_PTR _dw)
{
	TreeNode* pNode = (TreeNode*)_dw;

	string strKey = pNode->GetName();
	OBJECT_TYPE _ObjType = pNode->GetObjType();
	DWORD_PTR data = pNode->GetData();

	switch (_ObjType)
	{
	case OBJECT_TYPE::NONE:
		break;
	case OBJECT_TYPE::SCENE:
	{
		m_pSelectedScene = (CScene*)data;

	}
		break;
	case OBJECT_TYPE::LAYER:
	{
		m_pSelectedLayer = (CLayer*)data;

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




