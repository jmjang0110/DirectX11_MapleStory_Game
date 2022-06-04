#pragma once
#include "singleton.h"
#include "CGameObject.h"
#include "CLayer.h"
#include "CScene.h"


class CFileMgr :
    public CSingleton<CFileMgr>
{
    SINGLE(CFileMgr);

private:
    OPENFILENAME    m_Ofn;
    FILE* m_pFile;

    wchar_t         m_szName[256];



public:
    void Init();


public:
    template<typename type>
    void SaveToFile(DWORD_PTR _param);

    template<typename type>
    DWORD_PTR LoadFromFile(DWORD_PTR _param);

private:
    void SaveGameObject(DWORD_PTR _param);
    DWORD_PTR LoadGameObject(DWORD_PTR _param);

    void SaveScene(DWORD_PTR _param);
    DWORD_PTR LoadScene(DWORD_PTR _param);

    void SaveLayer(DWORD_PTR _param);
    DWORD_PTR LoadLayer(DWORD_PTR _param);


};


template <typename type>
void CFileMgr::SaveToFile(DWORD_PTR _param)
{
    const type_info& info = typeid(type);

    if (info.hash_code() == typeid(CGameObject).hash_code())
        SaveGameObject(_param);
    else if (info.hash_code() == typeid(CScene).hash_code())
        SaveScene(_param);
    else if (info.hash_code() == typeid(CLayer).hash_code())
        SaveLayer(_param);

}

template <typename type>
DWORD_PTR CFileMgr::LoadFromFile(DWORD_PTR _param)
{

    const type_info& info = typeid(type);
    DWORD_PTR pObj;

    if (info.hash_code() == typeid(CGameObject).hash_code())
        pObj = LoadGameObject(_param);
    else if (info.hash_code() == typeid(CScene).hash_code())
        pObj = LoadScene(_param);
    else if (info.hash_code() == typeid(CLayer).hash_code())
        pObj = LoadLayer(_param);

    return pObj;

}
