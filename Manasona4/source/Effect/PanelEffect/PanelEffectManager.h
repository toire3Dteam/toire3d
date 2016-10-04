#pragma once
#include "TDNLIB.h"
#include "PanelEffect.h"


/********************/
//	エフェクトマネージャー
/********************/
enum class PANEL_EFFECT_TYPE
{
	PLUS, HIT, PUT, DAMAGE, INEFFECT, INEFFECT_MINI, DOG_EFFECT,
	BURN, ClEAR, DON, PUSH, DELICIOUS, EAT, PERFECT, GREAT, BAD, HAPPY, DARK_NOTICE, SMOKE,
	BURST,ONEMORE_BURST,GUARD,BURST_PREV,GLASS,
	ARRAY_END
};

//class EffectManager
//{
//private:
//
//
//public:
//
//	// 実体
//	static EffectManager& GetInstance();
//	static void Release();
//	//Number_Effect();
//	virtual ~EffectManager();
//
//	// 更新・描画
//	void Update();
//	void Render();
//	void Render3D();// ビルボードで描画
//
//	// 追加
//	void AddEffect(int x, int y, PANEL_PANEL_EFFECT_TYPE type);
//	void AddEffect(Vector3 pos, PANEL_EFFECT_TYPE type);
//
//	//// NumberData
//	//NumberData  
//
//private:
//	// 実体
//	static EffectManager* m_pInstance;
//
//	// 数字のデータ
//	std::list<BaseEffect*> m_EffectData;
//	std::list<BaseEffect*>::iterator m_it;
//
//	// 封印
//	EffectManager();
//	EffectManager(const EffectManager&);
//	EffectManager& operator=(const EffectManager&);
//
//};
//
//// インスタンス化
//#define EffectMgr	(EffectManager::GetInstance())

class PanelEffectManager
{

public:
	PanelEffectManager();
	~PanelEffectManager();

	// 更新・描画
	void Update();
	void Render();
	void Render3D();// ビルボードで描画

	// 追加
	void AddEffect(int x, int y, PANEL_EFFECT_TYPE type);
	void AddEffect(Vector3 pos, PANEL_EFFECT_TYPE type);

private:

	BasePanelEffect* m_basePanelEffect[(int)PANEL_EFFECT_TYPE::ARRAY_END];

};