#pragma once


struct tEventInfo
{
	EVENT_TYPE	eType;
	DWORD_PTR	lParam;
	DWORD_PTR	wParam;
};


struct Vertex
{
	Vec3 vPos;		// 좌표	
	Vec4 vColor;	// 색상

	Vec2 vUV;

};

typedef Vertex Vtx;

//==========
// Animation2D
// ============
struct tAnim2DFrame
{
	Vec2	vLT;
	Vec2	vSlice;		
	float	fDuration;	// 노출 유지 시간

};


struct tAnim2D
{
	int     useAnim2D; // Animation 정보 사용 유무
	float   Atlas_Width;
	float   Atlas_Height;   // 해상도 정보 

	Vec2	vLT;
	Vec2	vSlice;
	Vec2	vBaseSize;

	Vec3    Anim2D_Padding;

};

// ===================
// 상수버퍼 대응 구조체
// ===================
struct tTransform
{
	Matrix matWorld;
	Matrix matView;
	Matrix matProj;

	Matrix matWV;
	Matrix matWVP;
};

struct tScalarParam
{
	int iArr[4];
	float fArr[4];
	Vec2 v2Arr[4];
	Vec4 v4Arr[4];
	Matrix matArr[4];
};