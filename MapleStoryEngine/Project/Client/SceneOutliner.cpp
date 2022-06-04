#include "pch.h"
#include "SceneOutliner.h"

#include "TreeUI.h"

SceneOutliner::SceneOutliner()
	: UI("SceneOutliner")
{
	m_TreeUI = new TreeUI(true);
	m_TreeUI->SetTitle("SceneOutliner");
	AddChild(m_TreeUI);
}

SceneOutliner::~SceneOutliner()
{
}



void SceneOutliner::update()
{
}

void SceneOutliner::render_update()
{
}
