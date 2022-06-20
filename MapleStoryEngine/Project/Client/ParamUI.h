#pragma once

class CTexture;

#include "ListUI.h"
/*

	생성목적
	-   해당 객체의 파라미터로 지정된 값들을 출력하고
		수정할 수 있게 하기 위해서 생성되었다.
*/
class ParamUI
{
public:
	static int KeyCount;

public:
	static int Param_Int(const string& _strName, const int* _pInOut);
	static float Param_Float(const string& _strName, const float* _pInOut);
	static Vec2 Param_Vec2(const string& _strName, const Vec2* _pInOut);
	static Vec4 Param_Vec4(const string& _strName, const Vec4* _pInOut);

	// 반환값 : 리스트UI 활성화 했을 때 true 아니면 false
	static bool Param_Tex(const string& _strName, CTexture* _pCurTex, UI* _pInst = nullptr, DBCLKED _pFunc = nullptr);
};

