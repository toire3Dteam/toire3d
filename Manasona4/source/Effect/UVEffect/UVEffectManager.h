#pragma once
#include "BaseUVEffect.h"

/**************************************/
//	マネージャー ※キャラクターに一つ・可変長配列使わない（newは最初だけ）
/**************************************/

class BaseUVEffect;

enum class UV_EFFECT_TYPE
{
	WAVE, HIT_IMPACT, ARRAY_END, 
};

class UVEffectManager
{
public:
	UVEffectManager();
	~UVEffectManager();

	// 更新・描画
	void Update();
	void Render();

	// 発動
	void AddEffect(Vector3 pos, UV_EFFECT_TYPE type);
	void AddEffect(Vector3 pos, UV_EFFECT_TYPE type,float startScale, float endScale);
	//void StopEffect(UV_EFFECT_TYPE type);

private:

	BaseUVEffect* m_baseUVEffect[(int)UV_EFFECT_TYPE::ARRAY_END];

};
