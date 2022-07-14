#pragma once
#include <Engine/CScript.h>

class CTileMap;

enum class DAMAGE_TYPE
{
    BASIC,          // size : (37, 39)px | Start_px(0, 100)
    MISS,           // size : (37, 39)px | Start_px(0, 187)  

    CRITICAL_1,     // size : (37, 40)px | Start_px(0, 0)
    CRITICAL_2,     // size : (39, 40)px | Start_px(0, 143)
    CRITICAL_3,     // size : (37, 39)px | Start_px(0, 187)

    END,
};

class CDamageScript :
    public CScript
{
private:
    DAMAGE_TYPE             m_eType;
    vector<tAnim2DFrame>    m_vecTexInfo;
    
    vector<UINT>            m_vecDamageNum;
    CGameObject*            m_DamagePrefab;

    int                     m_iDamage;

    float                   m_fMaxTime; // 최대 유지 시간 
    float                   m_fTimer;
    float                   m_fDeltatime;


public:
    void Init(DAMAGE_TYPE _eType, int _Damage, float _fMaxTime);


public:
    virtual void start();
    virtual void update();
    virtual void lateupdate();

    virtual void OnCollisionEnter(CGameObject* _OtherObject) override;
    virtual void OnCollision(CGameObject* _OtherObject) override;
    virtual void OnCollisionExit(CGameObject* _OtherObject) override;



public:
    virtual void SaveToScene(FILE* _pFile) override;
    virtual void LoadFromScene(FILE* _pFile) override;


    CLONE(CDamageScript)

public:
    CDamageScript();
    CDamageScript(const CDamageScript& _origin);
    ~CDamageScript();


};

