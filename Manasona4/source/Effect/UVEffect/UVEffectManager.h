#pragma once
#include "BaseUVEffect.h"

/**************************************/
//	マネージャー ※キャラクターに一つ・可変長配列使わない（newは最初だけ）
/**************************************/

class BaseUVEffect;

enum class UV_EFFECT_TYPE
{
	WAVE, HIT_IMPACT, PERSONA, IMPACT, SHOCK_WAVE, UPPER,GUARD,
	RUN, CONV, CONV2, CONV3, CONV4, BURST_BALL,
	SPLASH,
	ARRAY_END, // ※配列の終わり
};

// (TODO) ディレイつくれません？
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
    void AddEffect(Vector3 pos, UV_EFFECT_TYPE type, float startScale, float endScale,
        Vector3 startAngle, Vector3 endAngle);
	void AddEffect(Vector3 pos, UV_EFFECT_TYPE type, float startScale, float endScale,
		Vector3 startAngle, Vector3 endAngle, int deayTimer);
	void StopEffect(UV_EFFECT_TYPE type);

	// ループとしてエフェクト発動
	void AddEffectRoop(Vector3 pos, UV_EFFECT_TYPE type);
	void AddEffectRoop(Vector3 pos, UV_EFFECT_TYPE type, float startScale, float endScale);

	void StopEffectRoop(UV_EFFECT_TYPE type);

	// アクセサ
	BaseUVEffect* GetBaseUVEffect(UV_EFFECT_TYPE type){ return m_baseUVEffect[(int)type]; }

private:

	BaseUVEffect* m_baseUVEffect[(int)UV_EFFECT_TYPE::ARRAY_END];

	int m_delayTimer[(int)UV_EFFECT_TYPE::ARRAY_END];
	
};
