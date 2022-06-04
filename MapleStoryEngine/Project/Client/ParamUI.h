#pragma once

class CTexture;

class ParamUI
{
public:
	static int KeyCount;

public:
	static void Param_Int(const string& _strName, int* _pInOut);
	static void Param_Float(const string& _strName, float* _pInOut);
	static void Param_Vec2(const string& _strName, Vec2* _pInOut);
	static void Param_Vec4(const string& _strName, Vec4* _pInOut);

	static CTexture* Param_Tex(const string& _strName, CTexture* _pCurTex);
};

