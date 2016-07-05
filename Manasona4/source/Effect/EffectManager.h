#pragma once
#include "TDNLIB.h"
#include "BaseEffect\BaseEffect.h"


/********************/
//	エフェクトマネージャー
/********************/
enum class EFFECT_TYPE
{
	PLUS, HIT, PUT, NOTICE, INEFFECT, INEFFECT_MINI, DOG_EFFECT,
	BURN, ClEAR, DON, PUSH, DELICIOUS, EAT, PERFECT, GREAT, BAD, HAPPY, DARK_NOTICE,SMOKE
	};
class EffectManager
{
private:


public:


	// 実体
	static EffectManager& GetInstance();
	static void Release();
	//Number_Effect();
	virtual ~EffectManager();

	// 更新・描画
	void Update();
	void Render();

	// 追加
	void AddEffect(int x, int y, EFFECT_TYPE type);
	
	//// NumberData
	//NumberData  

private:
	// 実体
	static EffectManager* m_pInstance;

	// 数字のデータ
	std::list<BaseEffect*> m_EffectData;
	std::list<BaseEffect*>::iterator m_it;

	// 封印
	EffectManager();
	EffectManager(const EffectManager&);
	EffectManager& operator=(const EffectManager&);

};

// インスタンス化
#define EffectMgr	(EffectManager::GetInstance())
