#include "pch.h"
#include "ResourceUI.h"

#include <Engine/CResMgr.h>
#include <Engine/CEventMgr.h>
#include <Engine/CPrefab.h>

#include "CImGuiMgr.h"
#include "InspectorUI.h"
#include "TreeUI.h"

#include <Engine/CSceneMgr.h>
#include <Engine/CScene.h>
#include <Script/CSceneSaveLoad.h>
#include <Script/CSceneStartScript.h>


#include <Engine/CLayerFile.h>


#include "SceneOutlinerTool.h"
#include <experimental/filesystem>



ResourceUI::ResourceUI()
	: UI("Resource")
{
	m_TreeUI = new TreeUI(true);
	m_TreeUI->SetTitle("Resource");
	m_TreeUI->UseFrame(true);
	m_TreeUI->UseDragDropOuter(true);


	AddChild(m_TreeUI);

	// Clicked Delegate 등록
	m_TreeUI->SetClickedDelegate(this, (CLICKED)&ResourceUI::ItemClicked);


	// DoubleClicked Deletage 등록
	m_TreeUI->SetDoubleClickedDelegate(this, (CLICKED)&ResourceUI::ItemDBClicked);


	Reset();
}

ResourceUI::~ResourceUI()
{
}

void ResourceUI::update()
{
	// 리소스 변경상태 저장
	if (KEY_PRESSED(KEY::LCTRL) && KEY_TAP(KEY::S))
	{
		CResMgr::GetInst()->SaveChangedRes();
	}

	UI::update();
}

void ResourceUI::render_update()
{
	if (CEventMgr::GetInst()->HasOccurObjEvent())
	{
		Renew();
	}
}

void ResourceUI::Reset()
{
	// ==== Todo ===== 
	// 새로운 Resource 가 들어왔을 시 TreeUI 가 갱신되기 위해서 조금 수정함 
	m_TreeUI->Clear();
	// ===============
	


	// Content 폴더 및의 리소스 로딩
	Reload();
	Renew();

}

void ResourceUI::ItemClicked(DWORD_PTR _dwNode)
{
	TreeNode* pNode = (TreeNode*)_dwNode;

	string strKey = pNode->GetName();
	CRes* pResource = (CRes*)pNode->GetData();

	// 프레임 노드가 눌렸다면 아무일도 없다.
	if (nullptr == pResource)
		return;

	// InspectorUI 를 얻어옴
	InspectorUI* pInspectorUI = (InspectorUI*)CImGuiMgr::GetInst()->FindUI("Inspector");
	pInspectorUI->SetTargetResource(pResource);
}


void ResourceUI::ItemDBClicked(DWORD_PTR _dwNode)
{
	TreeNode* pNode = (TreeNode*)_dwNode;

	string strKey = pNode->GetName();
	CRes* pResource = (CRes*)pNode->GetData();

	// 프레임 노드가 눌렸다면 아무일도 없다.
	if (nullptr == pResource || pNode->GetParent()->GetName() != "SCENEFILE")
		return;

	// Scene 로딩
	// 현재 Scene 정지
	CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();
	pCurScene->SetSceneState(SCENE_STATE::STOP);

	// 로딩할 Scene 파일의 경로 계산
	wstring strFilePath = CPathMgr::GetInst()->GetContentPath();
	strFilePath += pResource->GetRelativePath();

	CScene* pNewScene = CSceneSaveLoad::LoadScene(strFilePath);
	CSceneMgr::GetInst()->ChangeScene(pNewScene);

	// ======= Todo ==========

	// CImGuiMgr 에 Delegate 등록 
	tUIDelegate tDeleteCom;
	tDeleteCom.dwParam = (DWORD_PTR)nullptr;
	tDeleteCom.pFunc = (PARAM_1)&SceneOutlinerTool::Reset;
	tDeleteCom.pInst = CImGuiMgr::GetInst()->FindUI("SceneOutlinerTool");

	CImGuiMgr::GetInst()->AddDelegate(tDeleteCom);
	// =======================



}



void ResourceUI::Reload()
{
	// Content 폴더 밑으로 모든 파일의 상대경로를 알아낸다.
	m_vecResPath.clear();
	FindFileName(CPathMgr::GetInst()->GetContentPath());

	// 키값 확인
	for (size_t i = 0; i < m_vecResPath.size(); ++i)
	{
		// 1. 파일명을 통해서 리소스의 종류를 알아낸다.
		RES_TYPE eType = GetResTypeFromExt(m_vecResPath[i]);

		// 2. 상대경로를 키값으로 해서 리소스매니저에 로딩한다.
		switch (eType)
		{
		case RES_TYPE::PREFAB:
			CResMgr::GetInst()->Load<CPrefab>(m_vecResPath[i], m_vecResPath[i]);
			break;
		case RES_TYPE::MESHDATA:

			break;
		case RES_TYPE::MATERIAL:
			CResMgr::GetInst()->Load<CMaterial>(m_vecResPath[i], m_vecResPath[i]);
			break;
		case RES_TYPE::MESH:

			break;
		case RES_TYPE::TEXTURE:
			CResMgr::GetInst()->Load<CTexture>(m_vecResPath[i], m_vecResPath[i]);
			break;
		case RES_TYPE::SOUND:

			CResMgr::GetInst()->Load<CSound>(m_vecResPath[i], m_vecResPath[i]);
			break;
		case RES_TYPE::SCENEFILE:
			CResMgr::GetInst()->Load<CSceneFile>(m_vecResPath[i], m_vecResPath[i]);
			break;

		case RES_TYPE::LAYERFILE:
			CResMgr::GetInst()->Load<CLayerFile>(m_vecResPath[i], m_vecResPath[i]);
			break;

		}
	}

	// 3. 로딩되어있는 리소스들의 실제 파일 존재 확인
	for (UINT i = 0; i < (UINT)RES_TYPE::END; ++i)
	{
		wstring strContentPath = CPathMgr::GetInst()->GetContentPath();

		const map<wstring, CRes*>& mapRes = CResMgr::GetInst()->GetResList((RES_TYPE)i);
		for (const auto& pair : mapRes)
		{
			if (pair.second->IsEngineRes())
				continue;

			// File Exist 체크
			if (!filesystem::exists(strContentPath + pair.second->GetRelativePath()))
			{
				// 4. 없으면 리소스매니저에서 메모리 해제
				MessageBox(nullptr, L"원본파일 삭제 됨", L"파일변경 감지", MB_OK);

				if (0 == pair.second->GetRefCount())
				{
					// 삭제
					tEventInfo info;
					info.eType = EVENT_TYPE::DELETE_RES;
					info.lParam = (DWORD_PTR)pair.second;
					CEventMgr::GetInst()->AddEvent(info);

					MessageBox(nullptr, L"리소스 메모리 해제", L"파일변경 감지", MB_OK);
				}

				else
				{
					MessageBox(nullptr, L"사용 중인 리소스\n 메모리 해제 실패", L"파일변경 감지", MB_OK);
				}
			}
		}
	}
}


void ResourceUI::Renew()
{
	m_TreeUI->Clear();

	// 리소스를 Tree 에 표시
	for (int i = 0; i < (int)RES_TYPE::END; ++i)
	{
		// TreeUI 가 DummyRoot 를 사용하기 때문에, 리소스 항목 노드들은 더미 자식으로 들어감
		TreeNode* pResNode = m_TreeUI->AddTreeNode(nullptr, ToString((RES_TYPE)i));

		const map<wstring, CRes*>& mapRes = CResMgr::GetInst()->GetResList((RES_TYPE)i);

		for (const auto& pair : mapRes)
		{
			// 각 리소스 노드들은 해당 리소스 항목 자식으로 들어감
			m_TreeUI->AddTreeNode(pResNode, string(pair.first.begin(), pair.first.end()), (DWORD_PTR)pair.second);
		}
	}
}

void ResourceUI::FindFileName(const wstring& _strFolderPath)
{
	wstring strContent = _strFolderPath + L"*.*";

	WIN32_FIND_DATA FindFileData = {};

	HANDLE hFind = nullptr;

	hFind = FindFirstFile(strContent.c_str(), &FindFileData);

	if (hFind == INVALID_HANDLE_VALUE)
		return;

	while (FindNextFile(hFind, &FindFileData))
	{
		if (FindFileData.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY)
		{
			if (0 == wcscmp(FindFileData.cFileName, L".."))
				continue;

			FindFileName(_strFolderPath + FindFileData.cFileName + L"\\");
			continue;
		}

		wstring strRelativePath = _strFolderPath + FindFileData.cFileName;
		strRelativePath = CPathMgr::GetInst()->GetRelativePath(strRelativePath);

		m_vecResPath.push_back(strRelativePath);
	}

	FindClose(hFind);
}


RES_TYPE ResourceUI::GetResTypeFromExt(const wstring& _strExt)
{
	wchar_t szExt[50] = {};
	_wsplitpath_s(_strExt.c_str(), nullptr, 0, nullptr, 0, nullptr, 0, szExt, 50);

	wstring strExt = szExt;

	if (L".mtrl" == strExt)
		return RES_TYPE::MATERIAL;
	else if (L".png" == strExt || L".jpeg" == strExt || L".bmp" == strExt
		|| L".jpg" == strExt || L".tga" == strExt || L".dds" == strExt)
		return RES_TYPE::TEXTURE;
	else if (L".mp3" == strExt || L".wav" == strExt || L".ogg" == strExt)
		return RES_TYPE::SOUND;
	else if (L".pref" == strExt)
		return RES_TYPE::PREFAB;
	else if (L".mesh" == strExt)
		return RES_TYPE::MESH;
	else if (L".scene" == strExt)
		return RES_TYPE::SCENEFILE;
	else if (L".layer" == strExt)
		return RES_TYPE::LAYERFILE;


	return RES_TYPE::END;
}

