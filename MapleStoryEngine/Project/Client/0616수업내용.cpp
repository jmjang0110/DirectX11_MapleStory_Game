/*
	0616 -  아이패드 안가져와서 여기다가 적겠습니다...


	
	1. CGameObject.cpp 수정 - COmpoennt Load 추가하는중 
		- CGameObject - 생성자 수정 
		- 7분14초에 끝 

	2. Component - Save/ Load Scene 구현 
		- Transform 구현 ( 10:37 ) 
			* arrRElativeDir, arrworldDir, m_matWorld -> 어차피 finmalUpdate에서 계산되므로 저장할 필요 없음 

		- CCamera 구현 ( 12:47 )
			* iLayerMask 몇번째 레이어 그리는지 알려준다. - 그냥 이거 뭔지 까먹었어서 저긍aa
			* ( 14분10초) 다시보기 
		- CCollider2D 구현 (15:57)
			* Mesh , Mtrl 어떻게 할 것인가 - Collider 타입에 따라 로드하기 때문에 상관없음
			 
		- CAnimator2D 구현 - 22:58 Animation2D - (24:50)-이름을 저장하는 이유
			*   애니메이션 원본 파일을 하나 만들어서 객체가 그 애니메이션을 받아올 수있게 해도 된다고 하네? 근데
			*	이렇게만 해도 크게 상관없음 
			*	Animation2D - Component 아님 따로따로 객체
			*	CAnimation2D Save/Load 구현 
			*	CAnimation2D 생성자 수정 
			*	CAnimation2D - LoadFromSCene - resize 
			*	어떤 리소스를 참조 하고 있었다. - 없으면 다시 로딩하고 참조해야한다 - SvaeResroucePtr() 함수 만들것임 - (37:22)
			
			*	Engine - func.h - SaveREsPtr(Ptr<ReS> _ptr, FILE* _pFile), LoadResPtr() 구현 
			*	pAtlasTex 이런게 nullptr 이었다면 따라서 bNullPtr 유무를 저장할 것이다. 
			*	LoadResPtr 은 원본을 받아올것이라서 & _Ptr 을 받는다. 
			*	-> 사실상 Save / Load REsPtr  되는건 Texture 밖에 없다. 다른거 구현을 안함 
			*	global.h 수정 
			*	헤더 순서 박살남 - singleton.h -> fun.h 순서로 바꿔야함  
			

// 2교시 
		 ------ RenderComponent ------

		- CMeshRender 구현 
			* Save / Load scene 구현 
			* 동적 Mayerail 은 게임 RUntime 시에 만들어진다. - 미리 만들어질 필요가 없기 대문에 
			* 신경쓸 필요가 없다. ( File 구현시) 
		- CTileMap
			*	Save/Load Scene 구현 
			*  structuredBuffer  - 재확장하는 구조라서 할 필요 없다? ( 12:00 몇초 앞 ) 
		- CParticleSystem 구현 
			*  Save / Load Scene 구현 
			* ParticleUpdateSHader - > 그냥 SaveResPtr 로 할 때 명확하게 해야함 - CCOmputeShader 로 받을거임 
			* MaxCount 바뀌면 StructuredBuffer 를 다시 Create 하게 한다. - 20분부터 다시 듣기  
			* 
			* UI 에서 MaxCount 바꾸고 한번 Create 를 다시 호출하게 해보자 
			* CPArticleUpdateShader 를 CParticleUpdateShader 말고 ComputeShader 로 만들어야겠다? 25 분 다시듣기 
			* -> ComputeShader 여러개 썼을 떄 문제될 수 있음 
			* MaxCount 바뀌면 크기가 (생성자때 만든)기존보다 커지면 Create 를 다시 해준다. - StructuredBuffer 크기를 늘린다. ( 27분) 
			* CParticleSystem.h -> setMaxParticleCount(UINT _iMax) 추가 
			* 
			* ComputeShader 생성자 없어서 설며우..???? CResMGr::Load 에서 설명중 
			* CComputeShader 에서 기본생성자 추가 
		- RenderComponent
		- 











*/