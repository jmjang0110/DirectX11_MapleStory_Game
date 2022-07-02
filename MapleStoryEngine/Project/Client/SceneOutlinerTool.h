#pragma once
#include "UI.h"

#include "TreeUI.h"

/*

    본인이 툴에서 사용하기 위해서 
    임의로 제작하는 중 

*/


class CGameObject;
class CScene;
class CLayer;

class SceneOutlinerTool :
    public UI
{
private:
    TreeUI* m_TreeUI;

    // 선택된 SCene Layer GameObject 저장 
private:
    CScene*         m_pSelectedScene;
    CLayer*         m_pSelectedLayer;
    CGameObject*    m_pSelectedGameObject;


public:
    virtual void update() override;
    virtual void render_update() override;

public:
    void Reset();
    
    void PushGameObjectToTree(CGameObject* _pObject, TreeNode* _pDestNode);
    TreeNode* PushSceneToTree(CScene* _pScene, TreeNode* _pDestNode);
    TreeNode* PushLayerToTree(CLayer* _pLayer, TreeNode* _pDestNode);


    // Delegeate Funcs 
    void PressDelete(DWORD_PTR _dw);
    void ObjectClicked(DWORD_PTR _dw);
    void DragAndDropDelegate(DWORD_PTR _dwDrag, DWORD_PTR _dwDrop);

    void PrefabSelect(DWORD_PTR _param);
    // ======== Todo ==============
private:
    ENGINE_TYPE m_RecentClickedType;

private:
    void NewObjectButton();
    void NewSceneButton();
    void NewLayerButton();

    void SettingCollisionCheckButton();

    // ======== Todo ==============

public:
    SceneOutlinerTool();
    ~SceneOutlinerTool();
};

