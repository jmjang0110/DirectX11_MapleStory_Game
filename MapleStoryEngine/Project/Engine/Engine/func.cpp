#include "pch.h"

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
