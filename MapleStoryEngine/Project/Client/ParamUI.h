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
	static void Param_Int(const string& _strName, int* _pInOut);
	static void Param_Float(const string& _strName, float* _pInOut);
	static void Param_Vec2(const string& _strName, Vec2* _pInOut);
	static void Param_Vec4(const string& _strName, Vec4* _pInOut);

	// 반환값 : 리스트UI 활성화 했을 때 true 아니면 false
	static bool Param_Tex(const string& _strName, CTexture* _pCurTex, UI* _pInst = nullptr, DBCLKED _pFunc = nullptr);
};

