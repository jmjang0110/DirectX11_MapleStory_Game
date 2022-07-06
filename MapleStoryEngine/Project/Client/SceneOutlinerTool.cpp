#include "pch.h"
#include "SceneOutlinerTool.h"

#include <Engine/CSceneMgr.h>
#include <Engine/CEventMgr.h>
#include <Engine/CCollisionMgr.h>


#include <Engine/CScene.h>
#include <Engine/CLayer.h>
#include <Engine/CGameObject.h>

#include "TreeUI.h"
#include "CImGuiMgr.h"
#include "InspectorUI.h"
#include "MenuUI.h"


#include <Engine/CResMgr.h>
#include <Engine/CMesh.h>
#include <Engine/CTransform.h>
#include <Engine/CMeshRender.h>
#include <Engine/CCamera.h>
#include <Engine/CCollider2D.h>

#include "CImGuiMgr.h"
#include "ListUI.h"
#include "PrefabUI.h"
#include "InspectorUI.h"

#include <Script/CSceneSaveLoad.h>



SceneOutlinerTool::SceneOutlinerTool()
	: UI("SceneOutlinerTool")
	, m_pSelectedScene(nullptr)
	, m_pSelectedLayer(nullptr)
	, m_pSelectedGameObject(nullptr)
	, m_RecentClickedType(ENGINE_TYPE::NONE)
	, m_NewLayerIdx(0)
{
	m_TreeUI = new TreeUI(true);
	m_TreeUI->SetTitle("SceneOutlinerTool");
	m_TreeUI->UseFrame(false);
	m_TreeUI->ShowDummyRoot(false);

	m_TreeUI->UseDragDropSelf(true);
	m_TreeUI->UseDragDropOuter(true);

	AddChild(m_TreeUI);

	// Clicked Delegate 등록
	m_TreeUI->SetClickedDelegate(this, (CLICKED)&SceneOutlinerTool::ObjectClicked);
	m_TreeUI->SetDragAndDropDelegate(this, (DRAG_DROP)&SceneOutlinerTool::DragAndDropDelegate);
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
	ImGui::BeginChild("New Object", ImVec2(150.f, 80.f), true, ImGuiWindowFlags_HorizontalScrollbar);

	NewSceneButton();
	NewLayerButton();
	NewObjectButton();

	ImGui::EndChild();

	ImGui::SameLine();

	ImGui::BeginChild("SaveDelete", ImVec2(150.f, 80.f), true, ImGuiWindowFlags_HorizontalScrollbar);

	SceneSaveButton();
	SaveLayerButton();
	DeleteLayerButton();

	ImGui::EndChild();

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
	// InspectorUI 
	InspectorUI* pInspectorUI = (InspectorUI*)CImGuiMgr::GetInst()->FindUI("Inspector");
	pInspectorUI->SetTargetObject(nullptr);
	pInspectorUI->SetTargetLayer(nullptr);
	pInspectorUI->SetTargetScene(nullptr);


	// SceneOutlinerUI 갱신
	m_pSelectedScene = nullptr;
	m_pSelectedLayer = nullptr;
	m_pSelectedGameObject = nullptr;


}

void SceneOutlinerTool::ObjectClicked(DWORD_PTR _dw)
{
	TreeNode* pNode = (TreeNode*)_dw;

	string strKey = pNode->GetName();
	ENGINE_TYPE _ObjType = pNode->GetObjType();
	DWORD_PTR data = pNode->GetData();
	m_RecentClickedType = _ObjType;


	switch (_ObjType)
	{
	case ENGINE_TYPE::SCENE:
	{
		m_pSelectedScene = (CScene*)data;
		m_pSelectedGameObject = nullptr;
		m_pSelectedLayer = nullptr;

		InspectorUI* pInspectorUI = (InspectorUI*)CImGuiMgr::GetInst()->FindUI("Inspector");
		pInspectorUI->SetTargetScene(m_pSelectedScene);
		pInspectorUI->SetTargetLayer(m_pSelectedLayer);
		pInspectorUI->SetTargetObject(m_pSelectedGameObject);

	}
	break;
	case ENGINE_TYPE::LAYER:
	{
		m_pSelectedLayer = (CLayer*)data;
		m_pSelectedGameObject = nullptr;

		InspectorUI* pInspectorUI = (InspectorUI*)CImGuiMgr::GetInst()->FindUI("Inspector");
		pInspectorUI->SetTargetLayer(m_pSelectedLayer);
		pInspectorUI->SetTargetObject(m_pSelectedGameObject);

	}
	break;
	case ENGINE_TYPE::GAME_OBJECT:
	{
		CGameObject* pObject = (CGameObject*)data;
		m_pSelectedGameObject = pObject;

		InspectorUI* pInspectorUI = (InspectorUI*)CImGuiMgr::GetInst()->FindUI("Inspector");
		pInspectorUI->SetTargetObject(m_pSelectedGameObject);
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
	pNode->SetObjType(ENGINE_TYPE::SCENE);


	return pNode;

}

TreeNode* SceneOutlinerTool::PushLayerToTree(CLayer* _pLayer, TreeNode* _pDestNode)
{
	int layerIdx = _pLayer->GetLayerIdx();
	TreeNode* pNode = m_TreeUI->AddTreeNode(_pDestNode
		, std::to_string(layerIdx) + "Layer - " + string(_pLayer->GetName().begin(), _pLayer->GetName().end())
		, (DWORD_PTR)_pLayer);

	pNode->SetObjType(ENGINE_TYPE::LAYER);
	return pNode;

}

void SceneOutlinerTool::PushGameObjectToTree(CGameObject* _pObject, TreeNode* _pDestNode)
{
	TreeNode* pNode = m_TreeUI->AddTreeNode(_pDestNode
		, string(_pObject->GetName().begin(), _pObject->GetName().end())
		, (DWORD_PTR)_pObject);
	pNode->SetObjType(ENGINE_TYPE::GAME_OBJECT);


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


void SceneOutlinerTool::DragAndDropDelegate(DWORD_PTR _dwDrag, DWORD_PTR _dwDrop)
{
	if (_dwDrag == (DWORD_PTR)nullptr || _dwDrop == (DWORD_PTR)nullptr)
		return;

	// GameObject -> GameObject 
	if (ENGINE_TYPE::GAME_OBJECT == m_TreeUI->GetDragNode()->GetObjType()
		&& ENGINE_TYPE::GAME_OBJECT == m_TreeUI->GetDropNode()->GetObjType())
	{

		CGameObject* pChildObject = (CGameObject*)_dwDrag;
		CGameObject* pDropTargetObject = (CGameObject*)_dwDrop;

		// 드롭 목적지가 제대로 들어 온 경우
		if (nullptr != pDropTargetObject)
		{
			if (pChildObject == pDropTargetObject
				|| pDropTargetObject->IsAncestor(pChildObject))
			{
				return;
			}

			// Todo 
			pDropTargetObject->Transform()->SetIgnoreParentScale(true);
			// == == 
			CSceneMgr::GetInst()->AddChild(pDropTargetObject, pChildObject);
		}

		// 자식 오브젝트의 목적지가 nullptr 인 경우
		else
		{
			// 이미 최상위 부모 오브젝트는 이벤트 처리가 필요없다.
			if (nullptr == pChildObject->GetParent())
			{
				return;
			}

			CSceneMgr::GetInst()->DisconnectParent(pChildObject);
		}

	}

	// GameObject -> Layer 
	if (ENGINE_TYPE::GAME_OBJECT == m_TreeUI->GetDragNode()->GetObjType()
		&& ENGINE_TYPE::LAYER == m_TreeUI->GetDropNode()->GetObjType())
	{

		CGameObject* pObject = (CGameObject*)_dwDrag;
		CLayer* pDropTargetLayer = (CLayer*)_dwDrop;

		// 드롭 목적지가 제대로 들어 온 경우
		if (nullptr != pDropTargetLayer)
		{
			// 최상위 오브젝트로 있는 오브젝트만 받는다. 
			if (pObject != pObject->GetAncestor())
				return;

			// 현재 레이어의 m_vecRoot 에서 지운다. 
			CSceneMgr::GetInst()->DeRegisterObjInLayer(pObject, pObject->GetLayerIndex());
			// 새로운 레이어에 등록한다. ( EVENT 등록 )
			CSceneMgr::GetInst()->SpawnObject(pObject, pDropTargetLayer->GetLayerIdx());

		}

	}

	// Layer -> Layer 
	if (ENGINE_TYPE::LAYER == m_TreeUI->GetDragNode()->GetObjType()
		&& ENGINE_TYPE::LAYER == m_TreeUI->GetDropNode()->GetObjType())
	{

		CLayer* pDragLayer = (CLayer*)_dwDrag;
		CLayer* pDropLayer = (CLayer*)_dwDrop;

		// 드롭 목적지가 제대로 들어 온 경우
		if (nullptr != pDropLayer)
		{
			CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();
			int iDragLayer_idx = pDragLayer->GetLayerIdx();
			int iDropLayer_idx = pDropLayer->GetLayerIdx();

			if (nullptr != pCurScene)
				CSceneMgr::GetInst()->SwapLayer(iDragLayer_idx, iDropLayer_idx);

			InspectorUI* pInspectorUI = (InspectorUI*)CImGuiMgr::GetInst()->FindUI("Inspector");
			pInspectorUI->CollisionOffAll(pCurScene);

			
		}
	}

}



// ====== Todo 
void SceneOutlinerTool::NewObjectButton()
{
	// Layer 를 고른 후 GameObject 생성 가능 
	// Create New GameObject
	if (ENGINE_TYPE::LAYER == m_RecentClickedType || ENGINE_TYPE::GAME_OBJECT == m_RecentClickedType)
	{
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
				ImGui::TextColored(ImVec4(0.5f, 0.5f, 0.8f, 1.f), sName.c_str());
				ImGui::Text("Add This Obj to this Layer");

			}
			else
				ImGui::TextColored(ImVec4(1.f, 0.0f, 0.0f, 1.f), "YOU MUST SELECT LAYER!");


			ImGui::Text("Show Prefab List");
			ImGui::SameLine();
			if (ImGui::Button("##PrefabListbtn", Vec2(15, 15)))
			{
				// ListUI 활성화한다.
				const map<wstring, CRes*>& mapRes = CResMgr::GetInst()->GetResList(RES_TYPE::PREFAB);
				ListUI* pListUI = (ListUI*)CImGuiMgr::GetInst()->FindUI("##ListUI");
				pListUI->Clear();
				pListUI->SetTitle("Prefab List");

				for (const auto& pair : mapRes)
				{
					pListUI->AddList(string(pair.first.begin(), pair.first.end()));
				}

				pListUI->Activate();
				pListUI->SetDBCEvent(this, (DBCLKED) & ::SceneOutlinerTool::PrefabSelect);
			}



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
					ImGui::CloseCurrentPopup();

				}


				ImGui::CloseCurrentPopup();

			}
			ImGui::EndPopup();
		}
	}

}

void SceneOutlinerTool::NewSceneButton()
{

	// 현재 보여지는 SCene 을 저장한 후에 
	// 새로운 SCene 을 생성한다. 
	if (ENGINE_TYPE::NONE == m_RecentClickedType || ENGINE_TYPE::SCENE == m_RecentClickedType)
	{
		if (ImGui::Button("New Scene", ImVec2(120, 0)))
		{
			// 기존 Scene을 저장한다. 
			MenuUI* pCurMenuUI = (MenuUI*)CImGuiMgr::GetInst()->FindUI("Menu");
			pCurMenuUI->SetSaveScene(true);
			ImGui::OpenPopup("Create New Scene");
		}

		bool unused_open = true;
		if (ImGui::BeginPopupModal("Create New Scene", &unused_open))
		{
			ImGui::Text("Set New Scene Name!\n\n");
			static char buf[512];
			ImGui::InputText("SetName", buf, IM_ARRAYSIZE(buf), ImGuiInputTextFlags_None);


			if (ImGui::Button("Complete"))
			{
				string name = buf;
				wstring newName = wstring(name.begin(), name.end());

				CScene* NewScene = new CScene;
				NewScene->SetName(newName);

				// File 저장 
				wstring SceneResKey = L"scene\\" + newName + L".scene";
				NewScene->SetResKey(SceneResKey);
				wstring strSceneFilePath = CPathMgr::GetInst()->GetContentPath();
				CSceneSaveLoad::SaveScene(NewScene, strSceneFilePath + SceneResKey);

				NewScene->SetLayerName(0, L"Default");

				// Camera Object 추가
				CGameObject* pCamObj = new CGameObject;
				pCamObj->SetName(L"MainCamera");
				pCamObj->AddComponent(new CTransform);
				pCamObj->AddComponent(new CCamera);

				pCamObj->Camera()->SetProjType(PROJ_TYPE::ORTHOGRAPHIC);
				pCamObj->Camera()->SetCameraAsMain();
				pCamObj->Camera()->CheckLayerMaskAll();

				NewScene->AddObject(pCamObj, L"Default");

				CSceneMgr::GetInst()->ChangeScene(NewScene);
				// Create Scene
				Reset();
				ImGui::CloseCurrentPopup();

			}


			ImGui::EndPopup();

		}


	}


}

void SceneOutlinerTool::NewLayerButton()
{
	if (ENGINE_TYPE::NONE == m_RecentClickedType || ENGINE_TYPE::SCENE == m_RecentClickedType)
	{
		if (ImGui::Button("New Layer", ImVec2(120, 0)))
			ImGui::OpenPopup("Create New Layer");

		bool unused_open = true;
		if (ImGui::BeginPopupModal("Create New Layer", &unused_open))
		{
			ImGui::Text("Set New Layer Name!\n\n");
			static char buf[512];
			ImGui::InputText("SetName", buf, IM_ARRAYSIZE(buf), ImGuiInputTextFlags_None);

			ImGui::Text("Layer Idx");
			static int layerIdx = 0;
			ImGui::DragInt("##LayerIdx", &layerIdx, 1.f, 0, MAX_LAYER);


			ImGui::Text("Show Layer List");
			ImGui::SameLine();
			if (ImGui::Button("##LayerListBtn", Vec2(15, 15)))
			{
				// ListUI 활성화한다.
				const map<wstring, CRes*>& mapRes = CResMgr::GetInst()->GetResList(RES_TYPE::LAYERFILE);
				ListUI* pListUI = (ListUI*)CImGuiMgr::GetInst()->FindUI("##ListUI");
				pListUI->Clear();
				pListUI->SetTitle("Layer List");

				for (const auto& pair : mapRes)
				{
					pListUI->AddList(string(pair.first.begin(), pair.first.end()));
				}

				pListUI->Activate();
				pListUI->SetDBCEvent(this, (DBCLKED) & ::SceneOutlinerTool::LayerSelect);
				m_NewLayerIdx = layerIdx;

			}

			if (ImGui::Button("Complete"))
			{
				string name = buf;
				wstring newName = wstring(name.begin(), name.end());

				if (nullptr != m_pSelectedScene)
				{
					bool bCheckCreate = true;

					// 해당 레이어 인덱스/이름(이)가 이미 있다면 
					if (L"" != m_pSelectedScene->GetLayer(layerIdx)->GetName()
						|| nullptr != m_pSelectedScene->GetLayer(newName))
					{
						bCheckCreate = false;
					}

					// Layer 생성 
					if (bCheckCreate)
					{
						m_pSelectedScene->SetLayerName(layerIdx, newName);

						Reset();

					}
				}
				ImGui::CloseCurrentPopup();
			}


			ImGui::EndPopup();
		}

	}
}

void SceneOutlinerTool::SceneSaveButton()
{
	if (nullptr == m_pSelectedScene)
		return;
	if (m_pSelectedScene->GetName() == L"")
		return;

	if (ImGui::Button("Save Scene", ImVec2(100.f, 20.f)))
	{

		// File 저장 
		wstring SceneName = m_pSelectedScene->GetName();
		wstring SceneResKey = L"scene\\" + SceneName + L".scene";
		m_pSelectedScene->SetResKey(SceneResKey);
		wstring strContentPath = CPathMgr::GetInst()->GetContentPath();
		CSceneSaveLoad::SaveScene(m_pSelectedScene, strContentPath + SceneResKey);

		ImGui::OpenPopup("Save Scene");
	}

	bool unused_open = true;
	if (ImGui::BeginPopupModal("Save Scene", &unused_open))
	{
		ImGui::Text("Save Scene Complete");

		if (ImGui::Button("Complete"))
		{
			ImGui::CloseCurrentPopup();
		}
		ImGui::EndPopup();
	}

	return;

}

void SceneOutlinerTool::SaveLayerButton()
{
	if (nullptr == m_pSelectedLayer)
		return;
	if (m_pSelectedLayer->GetName() == L"")
		return;

	if (ImGui::Button("Save Layer", ImVec2(100.f, 20.f)))
	{

		// File 저장 
		wstring LayerName = m_pSelectedLayer->GetName();
		wstring LayerResKey = L"layer\\" + LayerName + L".layer";
		m_pSelectedLayer->SetResKey(LayerResKey);
		wstring strContentFilePath = CPathMgr::GetInst()->GetContentPath();
		CSceneSaveLoad::SaveLayer(m_pSelectedLayer, strContentFilePath + LayerResKey);

		ImGui::OpenPopup("Save Layer");
	}

	bool unused_open = true;
	if (ImGui::BeginPopupModal("Save Layer", &unused_open))
	{
		ImGui::Text("Save Layer Complete");

		if (ImGui::Button("Complete"))
		{
			ImGui::CloseCurrentPopup();
		}
		ImGui::EndPopup();
	}

	return;

}

void SceneOutlinerTool::DeleteLayerButton()
{
	if (nullptr == m_pSelectedLayer)
		return;
	if (m_pSelectedLayer->GetName() == L"")
		return;


	ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(0.f, 0.8f, 0.8f));

	if (ImGui::Button("Delete Layer", ImVec2(100.f, 20.f)))
	{
		ImGui::OpenPopup("Delete Layer");
	}

	bool unused_open = true;
	if (ImGui::BeginPopupModal("Delete Layer", &unused_open))
	{
		wstring LayerName = m_pSelectedLayer->GetName();
		string strLayerName = string(LayerName.begin(), LayerName.end());

		ImGui::TextColored(ImVec4(0.f, 0.5f, 0.8f, 1.f), strLayerName.c_str());
		ImGui::Text("Are you sure you delete this layer?");

		if (ImGui::Button("Complete"))
		{
			UINT	LayerIdx = m_pSelectedLayer->GetLayerIdx();

			// CImGuiMgr 에 Delegate 등록 
			tUIDelegate tDeleteCom;
			tDeleteCom.dwParam = (DWORD_PTR)LayerIdx;
			tDeleteCom.pFunc = (PARAM_1)&SceneOutlinerTool::DeleteLayer;
			tDeleteCom.pInst = CImGuiMgr::GetInst()->FindUI("SceneOutlinerTool");

			CImGuiMgr::GetInst()->AddDelegate(tDeleteCom);


			ImGui::CloseCurrentPopup();
		}
		ImGui::EndPopup();
	}
	ImGui::PopStyleColor(1);

	return;

}


// _param : LayerIdx
void SceneOutlinerTool::DeleteLayer(DWORD_PTR _param)
{
	UINT delLayerIdx = (UINT)_param;
	CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();
	CLayer* pLayer = pCurScene->GetLayer(delLayerIdx);

	pLayer->Reset();
	Reset();

}


// _param : Prefab Name
void SceneOutlinerTool::PrefabSelect(DWORD_PTR _param)
{
	string strSelectedName = (char*)_param;
	wstring strPrefabKey = wstring(strSelectedName.begin(), strSelectedName.end());
	wstring strContent = CPathMgr::GetInst()->GetContentPath();
	wstring FullPath = strContent + strPrefabKey;


	CPrefab* pPrefab = new CPrefab;
	pPrefab->Load(FullPath);

	assert(pPrefab);

	if (m_pSelectedScene && m_pSelectedLayer)
	{
		// Prefab 파일에 저장된 gameObject 를 읽어서 해당 Layer 에 포함한다. 
		CGameObject* NewObj = pPrefab->Instantiate();
		m_pSelectedScene->AddObject(NewObj, m_pSelectedLayer->GetName());
		SAFE_DELETE(pPrefab);
		// TReeUI 에 추가하기 위해서 Reset() 
		Reset();
	}

}

// _param : Layer Name ( selected from List )
void SceneOutlinerTool::LayerSelect(DWORD_PTR _param)
{

	string strSelectedName = (char*)_param;
	wstring strLayerName = wstring(strSelectedName.begin(), strSelectedName.end());
	wstring strContent = CPathMgr::GetInst()->GetContentPath();
	wstring FullPath = strContent + strLayerName;


	CLayer* pLayer = nullptr;
	pLayer = CSceneSaveLoad::LoadLayer(FullPath);

	assert(pLayer);



	if (m_pSelectedScene)
	{
		m_pSelectedScene->CopyLayer(pLayer, m_NewLayerIdx);
		SAFE_DELETE(pLayer);
		m_NewLayerIdx = 0;

		// TReeUI 에 추가하기 위해서 Reset() 
		Reset();
	}

}

