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

}

void SceneOutlinerTool::Reset()
{
	// 현재 Scene 을 가져온다.
	CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();

	TreeNode* pSceneNode = PushSceneToTree(pCurScene, m_TreeUI->GetDummyNode());

	// GameObject 추가
	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		CLayer* pLayer = pCurScene->GetLayer(i);
		if (pLayer->GetName() == L"")
			continue;

		TreeNode* pLayerNode = PushLayerToTree(pLayer, pSceneNode);
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
	//TreeNode* pNode = (TreeNode*)_dw;

	//string strKey = pNode->GetName();
	//CGameObject* pObject = (CGameObject*)pNode->GetData();

	//assert(pObject);

	//// InspectorUI 를 얻어옴
	//InspectorUI* pInspectorUI = (InspectorUI*)CImGuiMgr::GetInst()->FindUI("Inspector");
	//pInspectorUI->SetTargetObject(pObject);
}

void SceneOutlinerTool::PushGameObjectToTree(CGameObject* _pObject, TreeNode* _pDestNode)
{
	TreeNode* pNode = m_TreeUI->AddTreeNode(_pDestNode
		, string(_pObject->GetName().begin(), _pObject->GetName().end())
		, (DWORD_PTR)_pObject);

	const vector<CGameObject*>& vecChild = _pObject->GetChild();

	for (size_t i = 0; i < vecChild.size(); ++i)
	{
		PushGameObjectToTree(vecChild[i], pNode);
	}
}

TreeNode* SceneOutlinerTool::PushSceneToTree(CScene* _pScene, TreeNode* _pDestNode)
{
	TreeNode* pNode = m_TreeUI->AddTreeNode(_pDestNode
		, string(_pScene->GetName().begin(), _pScene->GetName().end())
		, (DWORD_PTR)_pScene);

	return pNode;

}

TreeNode* SceneOutlinerTool::PushLayerToTree(CLayer* _pLayer, TreeNode* _pDestNode)
{
	TreeNode* pNode = m_TreeUI->AddTreeNode(_pDestNode
		, string(_pLayer->GetName().begin(), _pLayer->GetName().end())
		, (DWORD_PTR)_pLayer);

	return pNode;

}
