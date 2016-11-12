#pragma once
#include "BaseUVEffect.h"

/**************************************/
//	マネージャー ※キャラクターに一つ・可変長配列使わない（newは最初だけ）
/**************************************/

class BaseUVEffect;

enum class UV_EFFECT_TYPE
{
	WAVE, HIT_IMPACT, PERSONA, IMPACT, SHOCK_WAVE, UPPER, GUARD,
	RUN, CONV, CONV2, CONV3, CONV4, BURST_BALL,
	SPLASH,
	HIT_RING, AIROU_DRILL, BREAK_IMPACT, FIRE_RING,
	AIROU_CIRCLE,
	OVER_DRIVE_RING,
	JUMP_WAVE, JUMP_SPEED_LINE,
	PRESSURE,ORANGE_BURST,
	ARRAY_END, // ※配列の終わり
};

enum class UV_EFFECT_MULTIPLE_TYPE
{
	GUARD_WAVE,
	GUARD_GRID,
	HIT_RIP,
	HIT_SLASH,
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
	void SetPos(Vector3 pos, UV_EFFECT_TYPE type) { m_baseUVEffect[(int)type]->SetPos(pos); };// ポジションの設定のやり直し

private:

	BaseUVEffect* m_baseUVEffect[(int)UV_EFFECT_TYPE::ARRAY_END];

	int m_delayTimer[(int)UV_EFFECT_TYPE::ARRAY_END];
		
public:

	// 多段ヒット用
	static const int MULTIPLE_HIT_MAX = 4;
	
	struct MULTIPLE_EFFECT
	{
		// 多段用分用意
		BaseUVEffect* pBaseUVEffectMultiple[MULTIPLE_HIT_MAX];
		int iDelayTimer[MULTIPLE_HIT_MAX];
	};

	//  
	MULTIPLE_EFFECT m_pMultipleEffect[(int)UV_EFFECT_MULTIPLE_TYPE::ARRAY_END];

	void AddMultipleEffect(Vector3 pos, UV_EFFECT_TYPE type);
	
	void AddMultipleEffect(Vector3 pos, UV_EFFECT_MULTIPLE_TYPE type, float startScale, float endScale,
		Vector3 startAngle, Vector3 endAngle, int deayTimer = 0);

};
