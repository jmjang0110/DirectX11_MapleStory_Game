#pragma once

class CTexture;

#include "ListUI.h"

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

