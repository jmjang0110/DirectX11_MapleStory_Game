#include "pch.h"
#include "Animator2DUI.h"

#include "ListUI.h"


#include <Engine/CAnimation2D.h>
#include <Engine/CAnimator2D.h>
#include <Engine/CResMgr.h>
#include <Engine/CDevice.h>
#include <Engine/CTexture.h>
#include <Engine/CTimeMgr.h>


#include "CImGuiMgr.h"
#include "ListUI.h"


Animator2DUI::Animator2DUI()
	: ComponentUI("Animator2D", COMPONENT_TYPE::ANIMATOR2D)
	, m_FrmCnt(0)
	, m_iDemoFrameIdx(0)
	, m_ModifyFrameIdx(-1)
	, m_CurMode(ANIMATOR_MODE::CREATE)
	, m_bSwapBtn(false)
	, m_SwapIdx(Vec2(-1, -1))
	, m_pNewAnim2D(nullptr)
{
	SetSize(Vec2(0.f, 200.f));

	m_NewFrameInfo.fDuration = 0.1f;
	m_NewFrameInfo.vLT = Vec2(0.f, 0.f);
	m_NewFrameInfo.vSlice = Vec2(0.f, 0.f);
	m_NewFrameInfo.vOffset = Vec2(0.f, 0.f);


}

Animator2DUI::~Animator2DUI()
{

}

void Animator2DUI::update()
{

}

void Animator2DUI::render_update()
{

	
	ComponentUI::render_update();

	if (IsModal())
		render_update_CreateAnim2DTool();



	ImGui::PushItemWidth(200);

	// 현재 애니메이션을 얻는다 .
	CAnimator2D* pAnimator2D = GetTargetObject()->Animator2D();
	CAnimation2D* pCurAnim = nullptr;

	if(nullptr != pAnimator2D )
		pCurAnim = pAnimator2D->GetCurAnim();

	string strCurAnim;// = string(pCurAnim->GetName().begin(), pCurAnim->GetName().end());
	string strCurAnimTex;// = string(pCurAnim->GetTex()->GetKey().begin(), pCurAnim->GetTex()->GetKey().end());

	const map<wstring, CRes*>& mapRes = CResMgr::GetInst()->GetResList(RES_TYPE::TEXTURE);
	const map<wstring, CAnimation2D*> mapAnim = pAnimator2D->GetAnimList();

	// 현재 애니메이션의 이름과 해당 텍스처(Key 값)의 이름을 받는다. 
	if (nullptr != pCurAnim)
	{
		 strCurAnim = string(pCurAnim->GetName().begin(), pCurAnim->GetName().end());
		 strCurAnimTex = string(pCurAnim->GetTex()->GetKey().begin(), pCurAnim->GetTex()->GetKey().end());
	}
	

	m_vecTexList.clear();
	m_vecAnimList.clear();

	for (const auto& pair : mapRes)
	{
		m_vecTexList.push_back(string(pair.first.begin(), pair.first.end()));
	}

	for (const auto& pair : mapAnim)
	{
		m_vecAnimList.push_back(string(pair.first.begin(), pair.first.end()));

	}

	static int item_current_idx = 0; // Here we store our selection data as an index.
	ImGuiComboFlags flags = 0;

	// 현재 애니메이션 정보 출력 
	if (ImGui::BeginCombo("Animation2D", strCurAnim.c_str(), ImGuiComboFlags_NoArrowButton))
	{

		for (int n = 0; n < m_vecAnimList.size(); ++n)
		{
			const bool is_selected = (item_current_idx == n);
			if (ImGui::Selectable(m_vecAnimList[n].c_str(), is_selected))
			{
				item_current_idx = n;

				// 선택된 애니메이션을 PLay 한다. 
				wstring AnimName = wstring(m_vecAnimList[n].begin(), m_vecAnimList[n].end());
				pAnimator2D->Play(AnimName, true);


			}

			// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
			if (is_selected)
				ImGui::SetItemDefaultFocus();
		}
		ImGui::EndCombo();
	}

	
	// Animation 을 삭제하는 기능 경고 팝업창 
	static bool bDelButton = false;
	ImGui::SameLine();
	if (ImGui::Button("Delete This Anim"))
	{
		bDelButton = true;
	}

	if (bDelButton)
	{
		ImGui::OpenPopup("ReallyDelete?");
		bool unused_open = true;
		if (ImGui::BeginPopupModal("ReallyDelete?", &unused_open))
		{

			ImGui::Text("\t\tWARNING!!\n\nAre You Really DELETE this Animation ?? \n\n");
			if (ImGui::Button("Yes"))
			{
				bDelButton = false;

				int delIdx = item_current_idx;
				if (m_vecAnimList.size() >= 0 && delIdx >= 0)
				{
					item_current_idx = -1;
					wstring AnimName = wstring(m_vecAnimList[delIdx].begin(), m_vecAnimList[delIdx].end());
					pAnimator2D->DeleteAnim(AnimName);
				}
				ImGui::CloseCurrentPopup();
			}
	
			ImGui::SameLine();
			if (ImGui::Button("No"))
			{
				bDelButton = false;
				ImGui::CloseCurrentPopup();
			}
			ImGui::EndPopup();
		}
	}
	


	// 현재 쓰이고 있는 texture Key 출력 
	if (ImGui::BeginCombo("Texture", strCurAnimTex.c_str(), ImGuiComboFlags_NoArrowButton))
	{

		for(int n = 0 ; n < m_vecTexList.size(); ++n)
		{
			const bool is_selected = (item_current_idx == n);
			if (ImGui::Selectable(m_vecTexList[n].c_str(), is_selected))
				item_current_idx = n;

			// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
			if (is_selected)
				ImGui::SetItemDefaultFocus();
		}
		ImGui::EndCombo();
	}


	// Create New Anim btn 
	ImGui::Text("Create New Anim");
	ImGui::SameLine();
	if (ImGui::Button("##CreateAnim", Vec2(15, 15)))
	{

		SetModalUI(true);
	}

	

}



void Animator2DUI::TextureSelect(DWORD_PTR _param)
{
	string strSelectedName = (char*)_param;
	wstring strTexKey = wstring(strSelectedName.begin(), strSelectedName.end());

	Ptr<CTexture> pTexture = CResMgr::GetInst()->FindRes<CTexture>(strTexKey);
	assert(pTexture.Get());

	CAnimator2D* pAnimator2D = GetTargetObject()->Animator2D();
	//pAnimator2D->SetTexture(pTexture);

	
}

void Animator2DUI::render_update_CreateAnim2DTool()
{
		// 모달 사이즈 조정 
		Vec2 Size = CDevice::GetInst()->GetRenderResolution();
		Size.x -= 50.f * 4;
		Size.y -= 50.f * 4;
		ImGui::SetNextWindowSize(Size);
		ImVec2 vWinSize = ImGui::GetWindowContentRegionMax();

		
		// 모달은 가운데 고정 ( 위치 조정 )
		ImVec2 center = ImGui::GetMainViewport()->GetCenter();
		ImGui::SetNextWindowPos(center, ImGuiCond_Always, ImVec2(0.5f, 0.5f));
		//bool bModal = IsModal();
		ImGui::OpenPopup("Create Animation 2D!");
		if (ImGui::BeginPopupModal("Create Animation 2D!", NULL, ImGuiWindowFlags_AlwaysAutoResize))
		{
			ImGui::Text("Create Animation by Texture 2D \n\n");
			ImGui::SameLine();
			
			if (ImGui::Button("Save", ImVec2(120, 0)))
				ImGui::OpenPopup("Save New Animation!");

			bool unused_open = true;
			if (ImGui::BeginPopupModal("Save New Animation!", &unused_open))
			{
				ImGui::Text("Set New Animation Name!\n\n");
				
				static char buf[512];
				ImGui::InputText("SetName", buf,IM_ARRAYSIZE(buf), ImGuiInputTextFlags_None);
			
				if (ImGui::Button("Complete"))
				{
					string name = buf;
					m_NewAnim_strName = wstring(name.begin(), name.end());

					// Make !! New Anim 
					m_pNewAnim2D = new CAnimation2D;
					m_pNewAnim2D->SetName(m_NewAnim_strName);
					m_pNewAnim2D->Create(m_pChoosenAtlas, m_NewAnim_vecFrm, m_NewAnim_vBackSize);

					m_pNewAnim2D->SetHorizontalFlip(m_iHorizontal_Flip);
					m_pNewAnim2D->SetVerticalFlip(m_iVertical_Flip);


					GetTargetObject()->Animator2D()->CreateAnim(m_NewAnim_strName, m_pNewAnim2D);

					buf[0] = '\0';
					ImGui::CloseCurrentPopup();
					SetModalUI(false);
				}
				ImGui::EndPopup();
			}

			
			ImGui::SameLine();
			if (ImGui::Button("Cancel", ImVec2(120, 0))) 
			{
				ImGui::CloseCurrentPopup();
				SetModalUI(false);

			}

			 // Todo 
			
			UpdateTool();

			ImGui::EndPopup();
		}

}

void Animator2DUI::UpdateTool()
{

	SelectAtlasTexture();

	
	ShowAtlasTexTool();
	ImGui::SameLine();
	ShowFrameTool();

	
}

void Animator2DUI::SelectAtlasTexture()
{
	// 현재 애니메이션을 얻는다 .
	CAnimator2D* pAnimator2D = GetTargetObject()->Animator2D();
	CAnimation2D* pCurAnim = nullptr;

	if (nullptr != pAnimator2D)
		pCurAnim = pAnimator2D->GetCurAnim();

	string strCurAnim;
	// 현재 애니메이션의 이름과 해당 텍스처(Key 값)의 이름을 받는다. 
	if(nullptr != pCurAnim)
		strCurAnim = string(pCurAnim->GetName().begin(), pCurAnim->GetName().end());

	const map<wstring, CRes*>& mapRes = CResMgr::GetInst()->GetResList(RES_TYPE::TEXTURE);

	m_vecTexList.clear();

	for (const auto& pair : mapRes)
	{
		m_vecTexList.push_back(string(pair.first.begin(), pair.first.end()));
	}

	static int item_current_idx2 = 0;

	string m_pChoosenAtlas_TexKey = "";

	if (nullptr != m_pChoosenAtlas)
		m_pChoosenAtlas_TexKey = string(m_pChoosenAtlas->GetKey().begin(), m_pChoosenAtlas->GetKey().end());

	if (ImGui::BeginCombo("Atlas Texture", m_pChoosenAtlas_TexKey.c_str(), ImGuiComboFlags_NoArrowButton))
	{

		for (int n = 0; n < m_vecTexList.size(); ++n)
		{
			const bool is_selected2 = (item_current_idx2 == n);
			if (ImGui::Selectable(m_vecTexList[n].c_str(), is_selected2))
			{
				item_current_idx2 = n;
				m_vecTexList[n].c_str();

				// Tex 선택 
				wstring TexKey = wstring(m_vecTexList[n].begin(), m_vecTexList[n].end());
				m_pChoosenAtlas = CResMgr::GetInst()->FindRes<CTexture>(TexKey);

			}

			// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
			if (is_selected2)
				ImGui::SetItemDefaultFocus();
		}
		ImGui::EndCombo();
	}




}

void Animator2DUI::ShowAtlasTexTool()
{
 
	ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 3.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(2.0f, 1.0f));
	ImVec2 scrolling_child_size = ImVec2(0, ImGui::GetFrameHeightWithSpacing() * 7 + 30);
	static ImDrawList* draw_list = ImGui::GetWindowDrawList();

	
	if (ImGui::BeginChild("child", ImVec2(700.f, 600.f), true, ImGuiWindowFlags_HorizontalScrollbar))
	{
		if (nullptr != m_pChoosenAtlas.Get())
		{
			Vec2 Size = CDevice::GetInst()->GetRenderResolution();


			ImGuiIO& io = ImGui::GetIO();

			ImTextureID my_tex_id = m_pChoosenAtlas->GetSRV().Get();
			m_TexSize = Vec2{ m_pChoosenAtlas->Width(), m_pChoosenAtlas->Height() };
			ImGui::Text("%.0fx%.0f", m_TexSize.x, m_TexSize.y);
			ImVec2 uv_min = ImVec2(0.0f, 0.0f);                 // Top-left
			ImVec2 uv_max = ImVec2(1.0f, 1.0f);                 // Lower-right
			ImVec4 tint_col = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);   // No tint - 원본 색 그대로 가져온다.
			ImVec4 border_col = ImVec4(0.f, 1.0f, 1.0f, 0.5f); // 50% opaque white


			//ImGui::Text("MousePos: (%.2f, %.2f)", io.MousePos.x, io.MousePos.y);
			ImVec2 pos = ImGui::GetCursorScreenPos(); // Image 의 시작점 위치 (SCreen) LT
			m_TextureStartPos.x = pos.x;
			m_TextureStartPos.y = pos.y;

			// Draw Atlas Tex 
			ImGui::Image(my_tex_id, ImVec2(m_TexSize.x, m_TexSize.y), uv_min, uv_max, tint_col, border_col);
			if (ImGui::IsItemHovered())
			{

				ImGui::Image(my_tex_id, ImVec2(50.f, 50.f));
				//ImGui::BeginTooltip();

				float PIxelPos_x = io.MousePos.x - m_TextureStartPos.x;
				float PixelPos_y = io.MousePos.y - m_TextureStartPos.y;
				ImGui::Text("PIxel Pos: (%.2f, %.2f)", PIxelPos_x, PixelPos_y);
				ImGui::Text("pos Pos: (%.2f, %.2f)", m_TextureStartPos.x, m_TextureStartPos.y);

				float region_sz = 50.f;
				ImVec2 uv0 = ImVec2((PIxelPos_x) / m_TexSize.x, (PixelPos_y) / m_TexSize.y);
				ImVec2 uv1 = ImVec2((PIxelPos_x + region_sz) / m_TexSize.x, (PixelPos_y + region_sz) / m_TexSize.y);



				static ImVec2 offset(0.f, 0.f);

				// Mouse Clicked - Start 
				if (ImGui::IsMouseClicked(ImGuiMouseButton_Left))
				{
					offset = ImVec2(0.f, 0.f);
					tAnim2DFrame ClearFrame;
					m_NewFrameInfo = ClearFrame;
					m_NewFrameInfo.fDuration = 0.1f;

					// NewFrame 의 시작위치 설정 ( Pixel LT )

					float PIxelPos_x = io.MousePos.x - m_TextureStartPos.x;
					float PixelPos_y = io.MousePos.y - m_TextureStartPos.y;
					m_NewFrameInfo.vLT = Vec2(PIxelPos_x, PixelPos_y);
				}

				// Mouse Drag - Frame Resize 
				if (ImGui::IsMouseDragging(ImGuiMouseButton_Left))
				{
					ImGui::Text("PIxel LT Pos: (%.2f, %.2f)", m_NewFrameInfo.vLT.x, m_NewFrameInfo.vLT.y);

					// Frame 설정 
					// NewFrame 의 간격 설정 ( Pixel Slice )
					offset.x += ImGui::GetIO().MouseDelta.x;
					offset.y += ImGui::GetIO().MouseDelta.y;
					ImGui::Text("offset: (%.2f, %.2f)", offset.x, offset.y);
					m_NewFrameInfo.vSlice.x = offset.x;
					m_NewFrameInfo.vSlice.y = offset.y;

					Vec2 LT = m_NewFrameInfo.vLT;
					Vec2 Slice = m_NewFrameInfo.vSlice;

					ImVec2 p0 = ImVec2(m_TextureStartPos.x + LT.x, m_TextureStartPos.y + LT.y); // Pixel LT
					ImVec2 p1 = ImVec2(m_TextureStartPos.x + Slice.x + LT.x, m_TextureStartPos.y + Slice.y + LT.y); // Pixel RB

					draw_list->AddRect(p0, p1, IM_COL32(255, 255, 0.f, 255)); // RGBA

					// 백그라운드 설정 
					float CenterX, CenterY;

					CenterX = m_NewFrameInfo.vLT.x + (m_NewFrameInfo.vSlice.x / 2);
					CenterY = m_NewFrameInfo.vLT.y + (m_NewFrameInfo.vSlice.y / 2);

					m_NewAnim_vBackLT.x = CenterX - (m_NewAnim_vBackSize.x / 2);
					m_NewAnim_vBackLT.y = CenterY - (m_NewAnim_vBackSize.y / 2);

					p0 = ImVec2(m_TextureStartPos.x + m_NewAnim_vBackLT.x, m_TextureStartPos.y + m_NewAnim_vBackLT.y);
					p1 = ImVec2(m_TextureStartPos.x + m_NewAnim_vBackLT.x + m_NewAnim_vBackSize.x
						, m_TextureStartPos.y + m_NewAnim_vBackLT.y + m_NewAnim_vBackSize.y);

					draw_list->AddRect(p0, p1, IM_COL32(255, 0.f, 0.f, 255)); // RGBA

				}
				// Mouse Released - End 
				if (ImGui::IsMouseReleased(ImGuiMouseButton_Left))
				{

				}

				// Draw Frame/Background ( Yellow, Red Rect )
				DrawFrameRect();
			}
			// Draw Stored vecFrm - In Atlas Texture - Green Rect 
			DrawFrmaes();
		}


		ImGui::EndChild();
		ImGui::PopStyleVar(2);
	}
	
	

}

void Animator2DUI::ShowFrameTool()
{

	
	Vec2 LT = m_NewFrameInfo.vLT;
	Vec2 Slice = m_NewFrameInfo.vSlice;

	ImVec2 p0 = ImVec2(m_TextureStartPos.x + LT.x, m_TextureStartPos.y + LT.y); // Pixel LT
	ImVec2 p1 = ImVec2(m_TextureStartPos.x + Slice.x + LT.x, m_TextureStartPos.y + Slice.y + LT.y); // Pixel RB
	ImDrawList* draw_list = ImGui::GetWindowDrawList();
	draw_list->AddRect(p0, p1, IM_COL32(255, 255, 0.f, 255)); // RGBA
	
	
	// Cur Frame _ 제작중인 Frame 
	ImTextureID my_tex_id = nullptr;
	if (nullptr != m_pChoosenAtlas)
		my_tex_id = m_pChoosenAtlas->GetSRV().Get();

	// if 문 쓰면 assert 걸리니까 쓰지말자 
	ImGui::BeginChild("Frame Tool", ImVec2(675.f, 600.f), true, ImGuiWindowFlags_HorizontalScrollbar);
	{
		// Button Part
		ImGui::BeginChild("Setting Button ", ImVec2(400.f, 180.f), true, ImGuiWindowFlags_HorizontalScrollbar);
		{
			DrawSettingBtn(); // 새로 만들 프레임 정보 조정버튼 
			ImGui::EndChild();

		}

		ImGui::SameLine();
		ImGui::BeginChild("Decision Button ", ImVec2(250.f, 180.f), true, ImGuiWindowFlags_HorizontalScrollbar);
		{

			DrawDecisionFrameBtn();			// Add / Delete Frame 버튼 
			ImGui::EndChild();
		}

		// Frame / video Part
		DrawShowNewFrame(my_tex_id);	// 만들고 있는 프레임 실시간 출력 
		DrawPlayAnimation(my_tex_id);	// 저장된 프레임으로 애니메이션 비디오 출력 

		// Seperate Frame Part 
		ImGui::Separator();
		ImGui::BeginChild("Seperate Frames", ImVec2(658.f, 200.f), true, ImGuiWindowFlags_HorizontalScrollbar);
		{
			DrawAllNewFrames(my_tex_id);	// 저장된 프레임 각각을 출력 
			ImGui::EndChild();
		}

		
		
		ImGui::EndChild();
	}
	
	
	
}




void Animator2DUI::DrawFrmaes()
{
	
	float fWidth = m_pChoosenAtlas->Width();
	float fHeight = m_pChoosenAtlas->Height();
	ImDrawList* draw_list = ImGui::GetWindowDrawList();

	for (int i = 0; i < m_FrmCnt; ++i)
	{
		Vec2 LT = m_NewAnim_vecFrm[i].vLT * Vec2(fWidth, fHeight);
		Vec2 Slice = m_NewAnim_vecFrm[i].vSlice * Vec2(fWidth, fHeight);


		ImVec2 p0 = ImVec2(m_TextureStartPos.x + LT.x, m_TextureStartPos.y + LT.y); // Pixel LT
		ImVec2 p1 = ImVec2(m_TextureStartPos.x + Slice.x + LT.x, m_TextureStartPos.y + Slice.y + LT.y); // Pixel RB
		
		if(m_ModifyFrameIdx == i)
			draw_list->AddRect(p0, p1, IM_COL32(255, 153, 0, 255), 0.f, ImDrawFlags_None, 1.f);
		else 
			draw_list->AddRect(p0, p1, IM_COL32(0.f, 255, 50.f, 255)); // RGBA

	}

}



void Animator2DUI::DrawFrameRect()
{
	// Cur New  Frame Rect in texture - Yellow 
	Vec2 LT = m_NewFrameInfo.vLT;
	Vec2 Slice = m_NewFrameInfo.vSlice;

	ImVec2 p0 = ImVec2(m_TextureStartPos.x + LT.x, m_TextureStartPos.y + LT.y); // Pixel LT
	ImVec2 p1 = ImVec2(m_TextureStartPos.x + Slice.x + LT.x, m_TextureStartPos.y + Slice.y + LT.y); // Pixel RB
	ImDrawList* draw_list = ImGui::GetWindowDrawList();

	
	draw_list->AddRect(p0, p1, IM_COL32(255, 255, 0.f, 255)); // RGBA
	



	// BackGround Rect in texture - Red 
	float CenterX, CenterY;

	CenterX = m_NewFrameInfo.vLT.x + (m_NewFrameInfo.vSlice.x / 2);
	CenterY = m_NewFrameInfo.vLT.y + (m_NewFrameInfo.vSlice.y / 2);

	m_NewAnim_vBackLT.x = CenterX - (m_NewAnim_vBackSize.x / 2);
	m_NewAnim_vBackLT.y = CenterY - (m_NewAnim_vBackSize.y / 2);

	p0 = ImVec2(m_TextureStartPos.x + m_NewAnim_vBackLT.x, m_TextureStartPos.y + m_NewAnim_vBackLT.y);
	p1 = ImVec2(m_TextureStartPos.x + m_NewAnim_vBackLT.x + m_NewAnim_vBackSize.x
		, m_TextureStartPos.y + m_NewAnim_vBackLT.y + m_NewAnim_vBackSize.y);

	draw_list->AddRect(p0, p1, IM_COL32(255, 0.f, 0.f, 255)); // RGBA
}

void Animator2DUI::DrawSettingBtn()
{
	tAnim2DFrame frameInfo{};
	 
	if (m_CurMode == ANIMATOR_MODE::CREATE)
	{
		frameInfo = m_NewFrameInfo;
		// 인덱스 값은 0 부터 보여지는 건 1번으로 
		ImGui::TextColored(ImVec4(0, 255, 50, 255), "CREATE NEW FRAME : %d frame", m_FrmCnt + 1);
		ImGui::Separator();
	}
	else if (m_CurMode == ANIMATOR_MODE::MODIFY)
	{
		// 인덱스 값은 0 부터 보여지는 건 1번으로 
		ImGui::TextColored(ImVec4(255, 50, 50, 255), "MODIFY MODE : %d frame", m_ModifyFrameIdx + 1);
		ImGui::SameLine();
		if (ImGui::Button("Modify Complete", ImVec2(125, 20)))
		{
			m_CurMode = ANIMATOR_MODE::CREATE;
			m_bModifyBtn = false;
			m_ModifyFrameIdx = -1;
			return;
		}
		
		if (0 <= m_ModifyFrameIdx && m_ModifyFrameIdx < m_FrmCnt)
		{
			frameInfo = m_NewAnim_vecFrm[m_ModifyFrameIdx];
			frameInfo.vLT *= m_TexSize;
			frameInfo.vSlice *= m_TexSize;
		}
			

	}
	/*
	ImGui::DragFloat("LeftTop X ", &frameInfo.vLT.x, 0.5f, 0.f, m_TexSize.x - frameInfo.vSlice.x, "%.0f");
	ImGui::SameLine();		ImGui::PushItemWidth(40);
	ImGui::InputFloat("##LeftTop X", &frameInfo.vLT.x, 0.f, 0.f, "%.0f", 0);
	ImGui::PopItemWidth();

	ImGui::DragFloat("LeftTop Y ", &frameInfo.vLT.y, 0.5f, 0.f, m_TexSize.y - frameInfo.vSlice.y, "%.0f");
	ImGui::SameLine();		ImGui::PushItemWidth(40);
	ImGui::InputFloat("##LeftTop Y", &frameInfo.vLT.y, 0.f, 0.f, "%.0f", 0);
	ImGui::PopItemWidth();
	*/

	//ImGui::DragFloat2("size", (float*)&m_NewFrameInfo.vLT, 0.5f, 0.f, , "%.0f");

	// ******** Left Top ***********
	ImGui::Text("LeftTop X"); ImGui::SameLine();
	ImGui::PushItemWidth(60);
	ImGui::DragFloat("##LeftTop X_drag", &frameInfo.vLT.x, 0.5f, 0.f, m_TexSize.x - frameInfo.vSlice.x, "%.0f");
	ImGui::SameLine();		
	ImGui::PopItemWidth(); 
	
	ImGui::PushItemWidth(40);
	ImGui::InputFloat("##LeftTop X_input", &frameInfo.vLT.x, 0.f, 0.f, "%.0f", 0); 
	ImGui::SameLine();		
	ImGui::PopItemWidth(); 
	
	ImGui::SameLine();

	ImGui::Text("LeftTop Y"); ImGui::SameLine();
	ImGui::PushItemWidth(60);
	ImGui::DragFloat("##LeftTop Y_drag", &frameInfo.vLT.y, 0.5f, 0.f, m_TexSize.y - frameInfo.vSlice.y, "%.0f");
	ImGui::SameLine();	
	ImGui::PopItemWidth(); 

	ImGui::PushItemWidth(40);
	ImGui::InputFloat("##LeftTop Y_input", &frameInfo.vLT.y, 0.f, 0.f, "%.0f", 0);	
	ImGui::PopItemWidth();

	// ******** Slice  ***********
	ImGui::Text("Slice W  "); ImGui::SameLine();
	ImGui::PushItemWidth(60);
	ImGui::DragFloat("##Slice W_drag", &frameInfo.vSlice.x, 0.5f, 0.f, m_TexSize.x, "%.0f");
	ImGui::SameLine();		
	ImGui::PopItemWidth();

	ImGui::PushItemWidth(40);
	ImGui::InputFloat("##Slice W_input", &frameInfo.vSlice.x, 0.f, 0.f, "%.0f", 0);
	ImGui::SameLine();
	ImGui::PopItemWidth();

	ImGui::SameLine();

	ImGui::Text("Slice H  "); ImGui::SameLine();
	ImGui::PushItemWidth(60);
	ImGui::DragFloat("##Slice H_drag", &frameInfo.vSlice.y, 0.5f, 0.f, m_TexSize.y, "%.0f");
	ImGui::SameLine();
	ImGui::PopItemWidth();

	ImGui::PushItemWidth(40);
	ImGui::InputFloat("##Slice H_input", &frameInfo.vSlice.y, 0.f, 0.f, "%.0f", 0);
	ImGui::PopItemWidth();


	// ******** OffSet  ***********
	ImGui::Text("OffSet W "); ImGui::SameLine();
	ImGui::PushItemWidth(60);
	ImGui::DragFloat("##OffSet W_drag", &frameInfo.vOffset.x, 0.5f, 0.f, m_TexSize.x, "%.0f");
	ImGui::SameLine();
	ImGui::PopItemWidth();

	ImGui::PushItemWidth(40);
	ImGui::InputFloat("##OffSet W_input", &frameInfo.vOffset.x, 0.f, 0.f, "%.0f", 0);
	ImGui::SameLine();
	ImGui::PopItemWidth();

	ImGui::SameLine();

	ImGui::Text("OffSet H "); ImGui::SameLine();
	ImGui::PushItemWidth(60);
	ImGui::DragFloat("##OffSet H_drag", &frameInfo.vOffset.y, 0.5f, 0.f, m_TexSize.y, "%.0f");
	ImGui::SameLine();
	ImGui::PopItemWidth();

	ImGui::PushItemWidth(40);
	ImGui::InputFloat("##OffSet H_input", &frameInfo.vOffset.y, 0.f, 0.f, "%.0f", 0);
	ImGui::PopItemWidth();

	// 아직 구현 못함

	// *************** duration *****************
	ImGui::Text("Duration "); ImGui::SameLine();
	ImGui::PushItemWidth(60);
	ImGui::DragFloat("##Duration_drag", &frameInfo.fDuration, 0.5f, 0.f, 100.f, "%.3f");
	ImGui::SameLine();
	ImGui::PopItemWidth();

	ImGui::PushItemWidth(40);
	ImGui::InputFloat("##Duration_input", &frameInfo.fDuration, 0.f, 0.f, "%.3f", 0);
	ImGui::PopItemWidth();

	ImGui::Separator();

	ImGui::SliderFloat("BackSize W", &m_NewAnim_vBackSize.x, 0.0f, m_TexSize.x, "%.0f", ImGuiSliderFlags_None);
	ImGui::SameLine();		ImGui::PushItemWidth(40);
	ImGui::InputFloat("##BackSize W", &m_NewAnim_vBackSize.x, 0.f, 0.f, "%.0f", 0);
	ImGui::PopItemWidth();
	ImGui::SliderFloat("BackSize H", &m_NewAnim_vBackSize.y, 0.0f, m_TexSize.y, "%.0f", ImGuiSliderFlags_None);
	ImGui::SameLine();		ImGui::PushItemWidth(40);
	ImGui::InputFloat("##BackSize H", &m_NewAnim_vBackSize.y, 0.f, 0.f, "%.0f", 0);
	ImGui::PopItemWidth();

	if (m_CurMode == ANIMATOR_MODE::CREATE)
		m_NewFrameInfo = frameInfo;
	else if (m_CurMode == ANIMATOR_MODE::MODIFY)
	{
		

		frameInfo.vLT = frameInfo.vLT / m_TexSize;
		frameInfo.vSlice = frameInfo.vSlice /m_TexSize;

		m_NewAnim_vecFrm[m_ModifyFrameIdx] = frameInfo;
	}

}

void Animator2DUI::DrawShowNewFrame(ImTextureID _texid)
{
	ImGui::Separator();
	ImGui::Text("Show Frame");



	ImGui::PushID(0);

	ImVec2 size(150.f, 150.f);
	ImGui::InvisibleButton("##empty", size);

	const ImVec2 p0 = ImGui::GetItemRectMin();
	const ImVec2 p1 = ImGui::GetItemRectMax();  // Pixel RB
	ImDrawList* draw_list = ImGui::GetWindowDrawList();
	ImGui::PushClipRect(ImVec2(100.f, 100.f), p1, true);

	static Vec3 color = { 120.f, 120.f, 120.f };
	static int direction = 1;
	color.x += DT * direction * 200.f;
	if (color.x >= 255.f)
	{
		color.x = 255.f;
		direction *= -1;
	}

	else if (color.x < 0.f)
	{
		color.x = 0.f;
		direction *= -1;
	}

	draw_list->AddRect(p0, p1, IM_COL32(50.f, color.x, 50.f, 255)); // RGBA
	ImVec2 uv0 = ImVec2((m_NewFrameInfo.vLT.x) / m_TexSize.x, (m_NewFrameInfo.vLT.y) / m_TexSize.y);
	ImVec2 uv1 = ImVec2((m_NewFrameInfo.vLT.x + m_NewFrameInfo.vSlice.x) / m_TexSize.x, (m_NewFrameInfo.vLT.y + m_NewFrameInfo.vSlice.y) / m_TexSize.y);
	//draw_list->AddText(text_pos, IM_COL32_WHITE, text_str);
	draw_list->AddImage(_texid, p0, p1, uv0, uv1);


	ImGui::PopClipRect();
	ImGui::PopID();

}

void Animator2DUI::DrawPlayAnimation(ImTextureID _texid)
{
	ImGui::SameLine();
	ImGui::Text("PLAY\nAnimation\n\nFrmCnt : %d",m_FrmCnt);
	ImGui::SameLine();
	ImGui::PushID(m_FrmCnt + 1);

	ImVec2 size(150.f, 150.f);
	ImGui::InvisibleButton("##empty", size);
	const ImVec2 p0 = ImGui::GetItemRectMin(); // ImVec2(ImGui::GetItemRectMin().x, ImGui::GetItemRectMin().y + 50.f); // Pixel LT
	const ImVec2 p1 = ImGui::GetItemRectMax(); //ImVec2(ImGui::GetItemRectMax().x, ImGui::GetItemRectMax().y + 50.f); // Pixel RB
	ImDrawList* draw_list = ImGui::GetWindowDrawList();

	static float fAccTime = 0.f;

	fAccTime += DT;

	if (0 <= m_iDemoFrameIdx && m_iDemoFrameIdx < m_FrmCnt)
	{
		if (fAccTime >= m_NewAnim_vecFrm[m_iDemoFrameIdx].fDuration)
		{
			fAccTime = 0.f;
			m_iDemoFrameIdx += 1;
			if (m_iDemoFrameIdx >= m_FrmCnt)
				m_iDemoFrameIdx = 0;
		}
		ImGui::Text("\t\t\t\t\t\t\t\tframe : %d duration : %.3f", m_iDemoFrameIdx, fAccTime);

	}


	ImGui::PushClipRect(ImVec2(150.f, 150.f), p1, true);
	draw_list->AddRect(p0, p1, IM_COL32(120, 120, 120, 255)); // RGBA

	// Frame 이 있다면 
	if (nullptr != m_pChoosenAtlas && m_FrmCnt != 0)
	{
		if (0 <= m_iDemoFrameIdx && m_iDemoFrameIdx < m_FrmCnt)
		{

			Vec2 vLT = m_NewAnim_vecFrm[m_iDemoFrameIdx].vLT * Vec2(m_TexSize.x, m_TexSize.y);
			Vec2 vSlice = m_NewAnim_vecFrm[m_iDemoFrameIdx].vSlice * Vec2(m_TexSize.x, m_TexSize.y);

			ImVec2 uv0 = ImVec2((vLT.x) / m_TexSize.x, (vLT.y) / m_TexSize.y);
			ImVec2 uv1 = ImVec2((vLT.x + vSlice.x) / m_TexSize.x
				, (vLT.y + vSlice.y) / m_TexSize.y);
			//draw_list->AddText(text_pos, IM_COL32_WHITE, text_str);
			
			
			

			draw_list->AddImage(_texid, p0, p1, uv0, uv1);

		}

	}
	
	
	ImGui::PopClipRect();
	
	ImGui::PopID();

}

void Animator2DUI::DrawAllNewFrames(ImTextureID _texid)
{
	

	// 저장된 프레임 0 [ vector ] ~ 
	for (int n = 1; n <= m_FrmCnt; n++)
	{
		if (n != 1 && n % 6 != 1)
			ImGui::SameLine();


		ImGui::PushID(n);
		ImVec2 size(100.0f, 100.0f);
		ImGui::InvisibleButton("##empty", size);
		if (ImGui::IsItemActive() && ImGui::IsMouseClicked(ImGuiMouseButton_Left))
		{
			m_bModifyBtn = true;
				m_ModifyFrameIdx = n - 1;

		}

		const ImVec2 p0 = ImGui::GetItemRectMin(); // ImVec2(ImGui::GetItemRectMin().x, ImGui::GetItemRectMin().y + 50.f); // Pixel LT
		const ImVec2 p1 = ImGui::GetItemRectMax(); //ImVec2(ImGui::GetItemRectMax().x, ImGui::GetItemRectMax().y + 50.f); // Pixel RB


		ImVec2 uv0;
		ImVec2 uv1;
		ImDrawList* draw_list = ImGui::GetWindowDrawList();


		ImGui::PushClipRect(ImVec2(150.f, 150.f), p1, true);

		if(m_bModifyBtn && n - 1 == m_ModifyFrameIdx)
			draw_list->AddRect(p0, p1, IM_COL32(255, 50, 50, 255)); // RGBA Red
		else if(m_bSwapBtn && n - 1 == m_SwapIdx.x)
			draw_list->AddRect(p0, p1, IM_COL32(50, 50, 255, 255)); // RGBA 
		else
			draw_list->AddRect(p0, p1, IM_COL32(90, 120, 120, 255)); // RGBA

	

		if (0 <= n - 1 && n - 1 < m_FrmCnt)
		{
			Vec2 vLT = m_NewAnim_vecFrm[n - 1].vLT * Vec2(m_TexSize.x, m_TexSize.y);
			Vec2 vSlice = m_NewAnim_vecFrm[n - 1].vSlice * Vec2(m_TexSize.x, m_TexSize.y);

			uv0 = ImVec2((vLT.x) / m_TexSize.x, (vLT.y) / m_TexSize.y);
			uv1 = ImVec2((vLT.x + vSlice.x) / m_TexSize.x
				, (vLT.y + vSlice.y) / m_TexSize.y);

			//draw_list->AddText(text_pos, IM_COL32_WHITE, text_str);
			draw_list->AddImage(_texid, p0, p1, uv0, uv1);
		}
	
		ImGui::PopClipRect();

		// ======================= Option ==========================
		if (m_bModifyBtn && n -1 == m_ModifyFrameIdx)
		{
			ImGui::SameLine();

			//ImGui::Text("test");
			const char* names[] = { "Modify", "Swap", "Delete","Exit"};

			int selected_fish = -1;
			if (ImGui::Button("Option"))
				ImGui::OpenPopup("my_select_popup");

			if (ImGui::BeginPopup("my_select_popup"))
			{
				ImGui::TextColored(ImVec4(0.6f, 1.0f, 1.0f, 1.0f), "OPTIONS");
				ImGui::Separator();
				for (int i = 0; i < IM_ARRAYSIZE(names); i++)
					if (ImGui::Selectable(names[i]))
						selected_fish = i;  
				ImGui::EndPopup();
			}

			switch (selected_fish)
			{
			case 0: //  "Modify Frame"
				{
					m_CurMode = ANIMATOR_MODE::MODIFY;
					
				}
				break;
			case 1: // "Swap Other Frame"
				{
					if (false == m_bSwapBtn) // First Swap Frame
					{
						m_bSwapBtn = true;
						m_SwapIdx.x = m_ModifyFrameIdx; // 첫번째 Swap Frame 
						m_CurMode = ANIMATOR_MODE::CREATE;
						m_bModifyBtn = false;
						m_ModifyFrameIdx = -1;
					}
					else
					{
						// Second Swap Frame 
						if (-1 != m_SwapIdx.x)
						{
						
							m_SwapIdx.y = m_ModifyFrameIdx;
							m_bSwapBtn = false;
							m_CurMode = ANIMATOR_MODE::CREATE;
							m_bModifyBtn = false;
							m_ModifyFrameIdx = -1;

							// Swap !! 
							tAnim2DFrame SwapFrame = m_NewAnim_vecFrm[m_SwapIdx.x];
							m_NewAnim_vecFrm[m_SwapIdx.x] = m_NewAnim_vecFrm[m_SwapIdx.y];
							m_NewAnim_vecFrm[m_SwapIdx.y] = SwapFrame;

						}
					}

				}
				break;
			case 2: // "Delete this Frame"
				{
					m_NewAnim_vecFrm.erase(m_NewAnim_vecFrm.begin() + m_ModifyFrameIdx);
					m_bModifyBtn = false;
					m_ModifyFrameIdx = -1;
					m_FrmCnt--;

					if (m_FrmCnt <= 0)
						m_FrmCnt = 0;

					m_iDemoFrameIdx = 0;
					m_CurMode = ANIMATOR_MODE::CREATE;

				}
				break;
			case 3: // "Exit"
				{
					m_CurMode = ANIMATOR_MODE::CREATE;
					m_bModifyBtn = false;
					m_ModifyFrameIdx = -1;
				}

				break;
			default:
				break;
			}

		}

		
		// ======================= Option End ==========================
		ImGui::PopID();

	

	}

	
}

void Animator2DUI::DrawDecisionFrameBtn()
{

	if (ImGui::Button("Add Frame", ImVec2(100.f, 50.f)) && nullptr != m_pChoosenAtlas.Get())
	{
		

		tAnim2DFrame frameinfo = m_NewFrameInfo;

		frameinfo.vLT = frameinfo.vLT / Vec2(m_TexSize.x, m_TexSize.y);
		frameinfo.vSlice = frameinfo.vSlice / Vec2(m_TexSize.x, m_TexSize.y);

		m_NewAnim_vecFrm.push_back(frameinfo);
		m_FrmCnt += 1;

		m_NewFrameInfo.fDuration = 0.1f;
		m_NewFrameInfo.vLT = Vec2(0.f, 0.f);
		m_NewFrameInfo.vSlice = Vec2(0.f, 0.f);
		m_NewFrameInfo.vOffset = Vec2(0.f, 0.f);
	}

	ImGui::SameLine();
	if (ImGui::Button("Auto Set \n\nNext Frame", ImVec2(100.f, 50.f)))
	{
		if (m_FrmCnt != 0)
		{
			m_NewFrameInfo = m_NewAnim_vecFrm[m_FrmCnt - 1];		// Last Frame 
			// UV -> Pixel
			m_NewFrameInfo.vLT *= Vec2(m_TexSize.x, m_TexSize.y);
			m_NewFrameInfo.vSlice *= Vec2(m_TexSize.x, m_TexSize.y);
  
			// Texture Size 를 넘어간다면 왼쪽 아래칸으로 
			if (m_NewFrameInfo.vLT.x + (m_NewFrameInfo.vSlice.x * 2) > m_TexSize.x)
			{
				m_NewFrameInfo.vLT.y += m_NewFrameInfo.vSlice.y ;
				m_NewFrameInfo.vLT.x = 0.f;

			}
			else
			{
				m_NewFrameInfo.vLT.x = m_NewFrameInfo.vLT.x + m_NewFrameInfo.vSlice.x;
			}

		}
	}


	if (ImGui::Button("Horizontal \n\nFlip", ImVec2(100.f, 50.f)) && !m_NewAnim_vecFrm.empty())
	{
		/*CAnimator2D* pAnimator2D = GetTargetObject()->Animator2D();
		if (1 == pAnimator2D->GetHorizontalFlip())
			pAnimator2D->SetHorizontalFlip(false);
		else
			pAnimator2D->SetHorizontalFlip(true);*/

		if (1 == m_iHorizontal_Flip)
			m_iHorizontal_Flip = 0;
		else if (0 == m_iHorizontal_Flip)
			m_iHorizontal_Flip = 1;

		for (int i = 0; i < m_FrmCnt; ++i)
		{
			m_NewAnim_vecFrm[i].vLT.x += m_NewAnim_vecFrm[i].vSlice.x;
			m_NewAnim_vecFrm[i].vSlice.x *= -1;

		}

	}

	ImGui::SameLine();
	if (ImGui::Button("Vertical \n\nFlip", ImVec2(100.f, 50.f)) && !m_NewAnim_vecFrm.empty())
	{
	/*	CAnimator2D* pAnimator2D = GetTargetObject()->Animator2D();
		if (1 == pAnimator2D->GetVerticalFlip())
			pAnimator2D->SetVerticalFlip(false);
		else
			pAnimator2D->SetVerticalFlip(true);*/
		if (1 == m_iVertical_Flip)
			m_iVertical_Flip = 0;
		else if (0 == m_iVertical_Flip)
			m_iVertical_Flip = 1;


		for (int i = 0; i < m_FrmCnt; ++i)
		{
			m_NewAnim_vecFrm[i].vLT.y += m_NewAnim_vecFrm[i].vSlice.y;
			m_NewAnim_vecFrm[i].vSlice.y *= -1;

		}

	}


	ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(0.f, 0.8f, 0.8f));
	if (ImGui::Button("delete \n\nAll Frame", ImVec2(100.f, 50.f)) && !m_NewAnim_vecFrm.empty())
	{

		m_NewAnim_vecFrm.clear();
		m_FrmCnt = 0;
		m_iDemoFrameIdx = 0;


		m_CurMode = ANIMATOR_MODE::CREATE;
		m_ModifyFrameIdx = -1;
		m_bModifyBtn = false;


		m_NewFrameInfo.fDuration = 0.1f;
		m_NewFrameInfo.vLT = Vec2(0.f, 0.f);
		m_NewFrameInfo.vSlice = Vec2(0.f, 0.f);
		m_NewFrameInfo.vOffset = Vec2(0.f, 0.f);

	}
	ImGui::PopStyleColor(1);

	ImGui::SameLine();
	ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(0.f, 0.5f, 0.5f));
	if (ImGui::Button("delete \n\nPrev Frame", ImVec2(100.f, 50.f))&& !m_NewAnim_vecFrm.empty())
	{
		if (m_FrmCnt != 0)
		{

			if (m_ModifyFrameIdx == m_FrmCnt - 1)
			{
				m_CurMode = ANIMATOR_MODE::CREATE;
				m_ModifyFrameIdx = -1;
				m_bModifyBtn = false;

			}
			m_FrmCnt -= 1;
			m_iDemoFrameIdx = 0;
		}

		m_NewAnim_vecFrm.pop_back();
		m_NewFrameInfo.fDuration = 0.1f;
		m_NewFrameInfo.vLT = Vec2(0.f, 0.f);
		m_NewFrameInfo.vSlice = Vec2(0.f, 0.f);
		m_NewFrameInfo.vOffset = Vec2(0.f, 0.f);

	}
	ImGui::PopStyleColor(1);

	

}





