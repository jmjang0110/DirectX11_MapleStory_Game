#pragma once


#define SINGLE(TYPE) friend class CSingleton<TYPE>;\
					 private:\
						TYPE();\
						~TYPE();

#define DT CTimeMgr::GetInst()->GetDT()

#define KEY_TAP(key) KEY_STATE::TAP == CKeyMgr::GetInst()->GetKeyState(key)
#define KEY_PRESSED(key) KEY_STATE::PRESSED == CKeyMgr::GetInst()->GetKeyState(key)
#define KEY_AWAY(key) KEY_STATE::AWAY == CKeyMgr::GetInst()->GetKeyState(key)

#define DEVICE CDevice::GetInst()->GetDevice().Get()
#define CONTEXT CDevice::GetInst()->GetDeviceContext().Get()

#define MAX_LAYER 32

#define SAFE_DELETE(p) if(nullptr != p) delete p; p = nullptr;
#define CLONE(type) virtual type* Clone() { return new type(*this); }
#define CLONE_DISABLE(type) virtual type* Clone() { return nullptr; }


/*
	Maple Story 

*/
#define GRAVITY 9.80665f


typedef Vector2 Vec2;
typedef Vector3 Vec3;
typedef Vector4 Vec4;


enum class RES_TYPE
{
	PREFAB,
	MESHDATA,
	MATERIAL,
	MESH,
	TEXTURE,
	SOUND,
	GRAPHICS_SHADER,
	COMPUTE_SHADER,
	END,
};

enum class CB_TYPE
{
	TRANSFORM,		// b0
	SCALAR_PARAM,	// b1
	ANIM2D,			// b2
	
	//TILEMAP,		// b3 ( Test ) 
	GLOBAL,			// b3
	END,
};

// Rasterizer option
enum class RS_TYPE
{
	CULL_BACK,	// Default
	CULL_FRONT,
	CULL_NONE, 
	WIRE_FRAME, 
	END,
};

// DepthStencil Option
enum class DS_TYPE
{
	LESS,
	LESS_EQUAL,

	GREATER,
	GREATER_EQUAL,

	NO_TEST,	// 깊이테스트 하지 않음, 깊이는 기록
	NO_WRITE,	// 깊이테스트 진행, 깊이를 기록하지 않음

	NO_TEST_NO_WRITE, // 깊이테스트 하지 않음, 깊이 기록하지 않음
	END,
};


// BlendState Option
enum class BS_TYPE
{
	DEFAULT,		// SrcRGB * (1) + DestRGB * (0)
	ALPHA_BLEND,	// SrcRGB * (SrcAlpha) + DestRGB * (1 - SrcAlpha)
	END,
};


// 쉐이더의 렌더링 시점에 따른 분류
enum class SHADER_DOMAIN
{
	DOMAIN_FORWARD,	// 불투명
	DOMAIN_MASKED,	// 불투명, 투명
	DOMAIN_TRANSLUCENT,	// 반투명
	DOMAIN_POSTPROCESS, // 후 처리
};

enum class COMPONENT_TYPE
{
	TRANSFORM,	// 위치, 크기, 회전 (Location)

	CAMERA,		// 화면을 찍는 카메라 역할

	COLLIDER2D, // 2D 충돌체
	COLLIDER3D, // 3D 충돌체

	ANIMATOR2D, // 2D Sprite Animation
	ANIMATOR3D, // 3D Bone Skinning Animation

	BOUNDINGBOX, // Picking, FrustumCulling

	// renderer
	MESHRENDER,		// Mesh Renderer
	TILEMAP,		// 2D Tile Map Renderer
	PARTICLESYSTEM, // 입자 렌더링
	LANDSCAPE,		// 지형 렌더링
	DECAL,			// 데칼 오브젝트

	// Update
	SCRIPT,			// 로직

	END,
};


enum class DIR_TYPE
{
	RIGHT,
	UP,
	FRONT,
	END,

};

enum class SCALAR_PARAM
{
	INT_0,
	INT_1,
	INT_2,
	INT_3,

	FLOAT_0,
	FLOAT_1,
	FLOAT_2,
	FLOAT_3,

	VEC2_0,
	VEC2_1, 
	VEC2_2,
	VEC2_3,

	VEC4_0,
	VEC4_1,
	VEC4_2,
	VEC4_3,

	MAT_0,
	MAT_1,
	MAT_2,
	MAT_3,

	END,
};

enum class TEX_PARAM
{
	TEX_0,
	TEX_1,
	TEX_2,
	TEX_3,
	TEX_4,
	TEX_5,


	TEX_CUBE_0,
	TEX_CUBE_1,
		
	TEX_ARR_0,
	TEX_ARR_1,

	END,
};


enum  PIPELINE_STAGE
{
	VS = 0x01,
	HS = 0x02,
	DS = 0x04,
	GS = 0x08,
	PS = 0x10,	

	NO_PS = VS | HS | DS | GS,
	ALL = VS | HS | DS | GS | PS,
};


enum class EVENT_TYPE
{
	CREATE_OBJ,			// lParam : Object Adress, wParam : Layer Index
	DELETE_OBJ,			// lParam : Object Adress
	ADD_CHILD,			// lParam : Parent Object, wParam : Child Object
	SET_CAMERA_INDEX,	// lParam  : Camera Component Address , wParam : Camera Change Index  

	STAGE_CHANGE,		// lParam : Next Stage Enum
	CHANGE_AI_STATE,	// lParam : FSM Adress, wParam : Next State Type

	END,
};


// ======== ToDo =======
enum class PARTICLE_TYPE
{
	BOMB,		 // 폭발 
	FIRECRACKER, // 폭죽
	MAGIC_CIRCLE, // 마법진
	BUFF_EFFECT,
	ICE_SPIKE,
	ICE_SPRING,
	HYPO_STAR,
	END,
	

};