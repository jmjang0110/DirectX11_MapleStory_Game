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

// =======
// TileMap
// =======
struct tTileData
{
	int		iImgIdx;
	Vec2	vLTUV;		// 원하는 타일의 UV 좌상단 좌표 
	int		iPadding;

	tTileData()
		: iImgIdx(-1)
		, iPadding(0)
	{}
};



// ============
// Animation2D
// ============
struct tAnim2DFrame
{
	Vec2	vLT;
	Vec2	vSlice;		
	Vec2	vOffset;
	float	fDuration;	// 노출 유지 시간



};




// ========
// Particle 
// ========
struct tParticle
{
	Vec3	vPos;
	Vec3	vScale;
	Vec3	vDir;
	Vec4	vColor;

	int		Alive;
	Vec2	Padding;
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
	// 각각 16 바이트 안맞추면 err 남 
	int iArr[4];
	float fArr[4];
	Vec2 v2Arr[4];
	Vec4 v4Arr[4];
	Matrix matArr[4];

	int bTex[10];
	int iPadding[2];

};


struct tAnim2D
{

	Vec2	vLT;
	Vec2	vSlice;
	Vec2	vBackgroundSize;		//
	Vec2	vOffset;
	// 16 byte 

	int     useAnim2D;		// Animation 정보 사용 유무
	float   Atlas_Width;
	float   Atlas_Height;   // 해상도 정보 
	float	Anim2D_Padding;


};

struct tGlobal
{
	Vec2	vResolution;
	float	fDT;
	float	fAccTime;
};
