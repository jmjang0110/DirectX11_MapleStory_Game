#include "pch.h"
#include "CFileMgr.h"
#include "CSceneMgr.h"

#include "CEventMgr.h"
#include "CCollisionMgr.h"

#include "CResMgr.h"
#include "CMesh.h"
#include "CGraphicsShader.h"

#include "CDevice.h"
#include "CPathMgr.h"




#include "CTransform.h"
#include "CMeshRender.h"
#include "CCamera.h"
#include "CCollider2D.h"
#include "CAnimator2D.h"
#include "CTileMap.h"
#include "CParticleSystem.h"

#include "CTexture.h"
#include "CPrefab.h"

#include "CTestShader.h"

#include "CCore.h"


CFileMgr::CFileMgr()
{

}

CFileMgr::~CFileMgr()
{

}


void CFileMgr::Init()
{

	m_szName[0] = NULL;


	m_Ofn.lStructSize = sizeof(OPENFILENAME);
	m_Ofn.hwndOwner = CCore::GetInst()->GetMainHwnd();
	m_Ofn.lpstrFile = m_szName;
	m_Ofn.nMaxFile = sizeof(m_szName);
	m_Ofn.lpstrFilter = L"all\0*.*\0Scene\0*.scene\0";
	m_Ofn.nFilterIndex = 0;
	m_Ofn.lpstrFileTitle = nullptr;
	m_Ofn.nMaxFileTitle = 0;

	wstring strTileFolder = CPathMgr::GetInst()->GetContentPath();
	strTileFolder += L"Scene";

	m_Ofn.lpstrInitialDir = strTileFolder.c_str();
	m_Ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;


}






void CFileMgr::SaveGameObject(DWORD_PTR _param)
{
	CGameObject* GameObj = (CGameObject*)_param;
	m_Ofn.lpstrFilter = L"all\0*.*\0GameObject\0*.gameObj\0";
	wstring strTileFolder = CPathMgr::GetInst()->GetContentPath();
	strTileFolder += L"GameObject";
	m_Ofn.lpstrInitialDir = strTileFolder.c_str();

	// Modal
	bool open = false;
	if (open = GetSaveFileName(&m_Ofn))
	{
		m_pFile = nullptr;
		_wfopen_s(&m_pFile, m_szName, L"wb");

		assert(m_pFile);
		GameObj->SaveToFile(m_pFile);
		
		fclose(m_pFile);
	}


}

void CFileMgr::SaveScene(DWORD_PTR _param)
{
	CScene* pScene = (CScene*)_param;
	m_Ofn.lpstrFilter = L"all\0*.*\0Scene\0*.scene\0";
	wstring strTileFolder = CPathMgr::GetInst()->GetContentPath();
	strTileFolder += L"Scene";
	m_Ofn.lpstrInitialDir = strTileFolder.c_str();

	// Modal
	bool open = false;
	if (open = GetSaveFileName(&m_Ofn))
	{
		m_pFile = nullptr;
		_wfopen_s(&m_pFile, m_szName, L"wb");

		assert(m_pFile);
		pScene->SaveToFile(m_pFile);

		fclose(m_pFile);
	}

}

void CFileMgr::SaveLayer(DWORD_PTR _param)
{
	CLayer* pLayer = (CLayer*)_param;
	m_Ofn.lpstrFilter = L"all\0*.*\Layer\0*.layer\0";
	wstring strTileFolder = CPathMgr::GetInst()->GetContentPath();
	strTileFolder += L"Layer";
	m_Ofn.lpstrInitialDir = strTileFolder.c_str();

	// Modal
	bool open = false;
	if (open = GetSaveFileName(&m_Ofn))
	{
		m_pFile = nullptr;
		_wfopen_s(&m_pFile, m_szName, L"wb");

		assert(m_pFile);
		pLayer->SaveToFile(m_pFile);

		fclose(m_pFile);
	}

}

DWORD_PTR CFileMgr::LoadGameObject(DWORD_PTR _param)
{
	//CGameObject* GameObj = (CGameObject*)_param;
	CGameObject* GameObj = nullptr;
	GameObj = new CGameObject;

	m_Ofn.lpstrFilter = L"all\0*.*\0GameObject\0*.gameObj\0";
	wstring strTileFolder = CPathMgr::GetInst()->GetContentPath();
	strTileFolder += L"GameObject";
	m_Ofn.lpstrInitialDir = strTileFolder.c_str();

	// Modal
	bool open = false;
	if (open = GetOpenFileName(&m_Ofn))
	{
		// 최종 타일데이터 파일 경로
		wstring strFullPath = m_szName;


		// LoadFromFile 함수는 ** 상대경로를 요구하기 때문에 ** ,
		// Content 폴더 까지의 경로부분을 제거해서 뒷부분을 전달한다.
		// 최종 경로에서 겹치는 Content 폴더까지의 경로 위치를 뒤에서 찾아서
		// 거기서 문자열 끝부분까지를 잘라낸다.
		size_t iFindIdx = strFullPath.rfind(CPathMgr::GetInst()->GetContentPath());

		if (-1 != iFindIdx)
		{
			iFindIdx = iFindIdx + wstring(CPathMgr::GetInst()->GetContentPath()).length();
			wstring strRelativePath = strFullPath.substr(iFindIdx, strFullPath.length());
			wstring strpath = CPathMgr::GetInst()->GetContentPath();
			strpath += strRelativePath;


			m_pFile = nullptr;
			_wfopen_s(&m_pFile, strpath.c_str(), L"rb");
			assert(m_pFile);

			GameObj->LoadFromFile(m_pFile);
			
		}
	}
	return (DWORD_PTR)GameObj;


}

DWORD_PTR CFileMgr::LoadScene(DWORD_PTR _param)
{
	CScene* pScene = (CScene*)_param;
	m_Ofn.lpstrFilter = L"all\0*.*\0Scene\0*.scene\0";
	wstring strTileFolder = CPathMgr::GetInst()->GetContentPath();
	strTileFolder += L"Scene";
	m_Ofn.lpstrInitialDir = strTileFolder.c_str();

	// Modal
	bool open = false;
	if (open = GetOpenFileName(&m_Ofn))
	{
		// 최종 타일데이터 파일 경로
		wstring strFullPath = m_szName;


		// LoadFromFile 함수는 ** 상대경로를 요구하기 때문에 ** ,
		// Content 폴더 까지의 경로부분을 제거해서 뒷부분을 전달한다.
		// 최종 경로에서 겹치는 Content 폴더까지의 경로 위치를 뒤에서 찾아서
		// 거기서 문자열 끝부분까지를 잘라낸다.
		size_t iFindIdx = strFullPath.rfind(CPathMgr::GetInst()->GetContentPath());

		if (-1 != iFindIdx)
		{
			iFindIdx = iFindIdx + wstring(CPathMgr::GetInst()->GetContentPath()).length();
			wstring strRelativePath = strFullPath.substr(iFindIdx, strFullPath.length());
			wstring strpath = CPathMgr::GetInst()->GetContentPath();
			strpath += strRelativePath;


			m_pFile = nullptr;
			_wfopen_s(&m_pFile, strpath.c_str(), L"rb");
			assert(m_pFile);

			pScene->LoadFromFile(m_pFile);

			return (DWORD_PTR)pScene;

		}
	}
}

DWORD_PTR CFileMgr::LoadLayer(DWORD_PTR _param)
{
	CLayer* pLayer = (CLayer*)_param;
	m_Ofn.lpstrFilter = L"all\0*.*\0Layer\0*.layer\0";
	wstring strTileFolder = CPathMgr::GetInst()->GetContentPath();
	strTileFolder += L"Layer";
	m_Ofn.lpstrInitialDir = strTileFolder.c_str();

	// Modal
	bool open = false;
	if (open = GetOpenFileName(&m_Ofn))
	{
		// 최종 타일데이터 파일 경로
		wstring strFullPath = m_szName;


		// LoadFromFile 함수는 ** 상대경로를 요구하기 때문에 ** ,
		// Content 폴더 까지의 경로부분을 제거해서 뒷부분을 전달한다.
		// 최종 경로에서 겹치는 Content 폴더까지의 경로 위치를 뒤에서 찾아서
		// 거기서 문자열 끝부분까지를 잘라낸다.
		size_t iFindIdx = strFullPath.rfind(CPathMgr::GetInst()->GetContentPath());

		if (-1 != iFindIdx)
		{
			iFindIdx = iFindIdx + wstring(CPathMgr::GetInst()->GetContentPath()).length();
			wstring strRelativePath = strFullPath.substr(iFindIdx, strFullPath.length());
			wstring strpath = CPathMgr::GetInst()->GetContentPath();
			strpath += strRelativePath;


			m_pFile = nullptr;
			_wfopen_s(&m_pFile, strpath.c_str(), L"rb");
			assert(m_pFile);

			pLayer->LoadFromFile(m_pFile);
			return (DWORD_PTR)pLayer;

		}
	}
}

