#include "pch.h"

// COMPONENT_TYPE 
const wchar_t* ToWString(COMPONENT_TYPE _type)
{
    static const wchar_t* szWString[(UINT)COMPONENT_TYPE::END] = 
    {
        L"TRANSFORM",
        L"CAMERA",
        L"COLLIDER2D",
        L"COLLIDER3D",
        L"ANIMATOR2D",
        L"ANIMATOR3D",
        L"BOUNDINGBOX",
        L"MESHRENDER",
        L"TILEMAP",
        L"PARTICLESYSTEM",
        L"LANDSCAPE",
        L"DECAL",
    };

    return szWString[(UINT)_type];
}

const char* ToString(COMPONENT_TYPE _type)
{
    static const char* szString[(UINT)COMPONENT_TYPE::END] =
    {
        "TRANSFORM",
        "CAMERA",
        "COLLIDER2D",
        "COLLIDER3D",
        "ANIMATOR2D",
        "ANIMATOR3D",
        "BOUNDINGBOX",
        "MESHRENDER",
        "TILEMAP",
        "PARTICLESYSTEM",
        "LANDSCAPE",
        "DECAL",
    };

    return szString[(UINT)_type];
}

// RES_TYPE 
const wchar_t* ToWString(RES_TYPE _type)
{
    static const wchar_t* szWString[(UINT)RES_TYPE::END] =
    {
        L"PREFAB",
        L"MESHDATA",
        L"MATERIAL",
        L"GRAPHICS_SHADER",
        L"COMPUTE_SHADER",
        L"MESH",
        L"TEXTURE",
        L"SOUND",
    };

    return szWString[(UINT)_type];
}

const char* ToString(RES_TYPE _type)
{
    static const char* szString[(UINT)RES_TYPE::END] =
    {
        "PREFAB",
        "MESHDATA",
        "MATERIAL",
        "GRAPHICS_SHADER",
        "COMPUTE_SHADER",
        "MESH",
        "TEXTURE",
        "SOUND",
    };

    return szString[(UINT)_type];
}

// ============== Todo ===========================================

// SHADER_DOMAIN
const wchar_t* ToWString(SHADER_DOMAIN _type)
{
    static const wchar_t* szWString[(UINT)SHADER_DOMAIN::END] =
    {
        L"DOMAIN_FORWARD",
        L"DOMAIN_MASKED",
        L"DOMAIN_TRANSLUCENT",
        L"DOMAIN_POSTPROCESS",
    };

    return szWString[(UINT)_type];
}

const char* ToString(SHADER_DOMAIN _type)
{
    static const char* szString[(UINT)SHADER_DOMAIN::END] =
    {
        "DOMAIN_FORWARD",
        "DOMAIN_MASKED",
        "DOMAIN_TRANSLUCENT",
        "DOMAIN_POSTPROCESS",
    };

    return szString[(UINT)_type];
}

// RS_TYPE - RasterizerState
const wchar_t* ToWString(RS_TYPE _type)
{
    static const wchar_t* szWString[(UINT)RS_TYPE::END] =
    {
        L"CULL_BACK",
        L"CULL_FRONT",
        L"CULL_NONE",
        L"WIRE_FRAME",
    };

    return szWString[(UINT)_type];
}

// RasterizerState
const char* ToString(RS_TYPE _type)
{
    static const char* szString[(UINT)RS_TYPE::END] =
    {
        "CULL_BACK",
        "CULL_FRONT",
        "CULL_NONE",
        "WIRE_FRAME",
    };

    return szString[(UINT)_type];
}

// DS_TYPE - DepthSTencil State
const wchar_t* ToWString(DS_TYPE _type)
{
    static const wchar_t* szWString[(UINT)DS_TYPE::END] =
    {
        L"LESS",
        L"LESS_EQUAL",
        L"GREATER",
        L"GREATER_EQUAL",
        L"NO_TEST",
        L"NO_WRITE",
        L"NO_TEST_NO_WRITE",
    };

    return szWString[(UINT)_type];
}

// DS_TYPE
const char* ToString(DS_TYPE _type)
{
    static const char* szString[(UINT)DS_TYPE::END] =
    {
       "LESS",
       "LESS_EQUAL",
       "GREATER",
       "GREATER_EQUAL",
       "NO_TEST",
       "NO_WRITE",
       "NO_TEST_NO_WRITE",
    };

    return szString[(UINT)_type];
}

// BS_TYPE - Blend State State
const wchar_t* ToWString(BS_TYPE _type)
{
    static const wchar_t* szWString[(UINT)BS_TYPE::END] =
    {
        L"DEFAULT",
        L"ALPHA_BLEND",
    };

    return szWString[(UINT)_type];
}

// BS_TYPE
const char* ToString(BS_TYPE _type)
{
    static const char* szString[(UINT)BS_TYPE::END] =
    {
       "DEFAULT",
       "ALPHA_BLEND",

    };

    return szString[(UINT)_type];
}

// CB_TYPE - const Buffer Type 
const wchar_t* ToWString(CB_TYPE _type)
{
    static const wchar_t* szWString[(UINT)CB_TYPE::END] =
    {
        L"TRANSFORM",
        L"SCALAR_PARAM",
        L"ANIM2D",
        L"GLOBAL",
    };

    return szWString[(UINT)_type];
}
// CB_TYPE - const Buffer Type 
const char* ToString(CB_TYPE _type)
{
    static const char* szString[(UINT)CB_TYPE::END] =
    {
        "TRANSFORM",
        "SCALAR_PARAM",
        "ANIM2D",
        "GLOBAL",
    };

    return szString[(UINT)_type];
}

// Set/Get wstring To/From File 
const void SaveWString(const wstring& _str, FILE* _pFile)
{
    const wchar_t* pStr = _str.c_str();

    size_t iLen = _str.length();

    // 문자의 길이를 정수로 저장
    fwrite(&iLen, sizeof(size_t), 1, _pFile);

    // 문자 저장
    fwrite(pStr, sizeof(wchar_t), iLen, _pFile);
}

const void LoadWString(wstring& _str, FILE* _pFile)
{
    size_t iLen = 0;
    fread(&iLen, sizeof(size_t), 1, _pFile);

    wchar_t szData[256] = {};
    fread(szData, sizeof(wchar_t), iLen, _pFile);

    _str = szData;
}
