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
	Vec2	vLTUV;
	int		iPadding;

	tTileData()
		: iImgIdx(-1)
		, iPadding(0)
	{}
};




// ==========
// Animtion2D
// ==========
/*

	Vec2	vLT;
	Vec2	vSlice;
	Vec2	vOffset;
	float	fDuration;

*/
struct tAnim2DFrame
{
	Vec2	vLT;
	Vec2	vSlice;
	Vec2	vOffset;
	float	fDuration;
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
	float   m_fCurTime; // 생존 시간
	float	m_fMaxTime; // 최대 생명 시간
};

struct tParticleData
{
	int		iAliveCount;
	Vec3	vPadding;
};

// ========
//	Light 
// ========
struct tLightColor
{
	Vec4 vDiff;	// Diffuse Light	난반사광
	Vec4 vSpec; // Specular Light	반사광
	Vec4 vAmb;  // Ambient Light	환경광
};

struct tLightInfo
{
	tLightColor color;
	Vec3		vLightDir;
	int			iLightType;
	Vec3		vWorldPos;
	float		fAngle;
	float		fRange;
	Vec3		vPadding;
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

	int bTex[10];
	int iPadding[2];
};

struct tAnim2D
{
	Vec2	vLT;
	Vec2	vSlice;
	Vec2	vBackgroundSize;
	Vec2    vOffset;

	int     useAnim2D;
	float   Atlas_Width;
	float   Atlas_Height;
	float	Anim2D_Padding;
};

struct tGlobal
{
	Vec2	vResolution; // RenderTarget Resolution
	Vec2    vNoiseResolution; // NoiseTexture Resolution
	float	fDT;
	float	fAccTime;
	int		Light2DCount;
	int     Light3DCount;
};