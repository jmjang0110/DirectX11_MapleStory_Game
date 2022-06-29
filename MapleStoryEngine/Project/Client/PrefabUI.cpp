#include "pch.h"
#include "PrefabUI.h"

// Engine Obj headers
#include <Engine/CScene.h>
#include <Engine/CLayer.h>
#include <Engine/CGameObject.h>
#include <Engine/CPrefab.h>


// Mgr headers 
#include <Engine/CSceneMgr.h>
#include <Engine/CResMgr.h>



PrefabUI::PrefabUI()
	: ResInfoUI("Prefab", RES_TYPE::PREFAB)

{
}

PrefabUI::~PrefabUI()
{
}


void PrefabUI::update()
{
	ResInfoUI::update();


}

void PrefabUI::render_update()
{
	ResInfoUI::render_update();


}

