#include "UVEffectManager.h"

/*****************************/
//		UVEffectマネージャー
/****************************/
	
/***********************************************/
// 初期化・解放
/***********************************************/
UVEffectManager::UVEffectManager()
{
	for (int i = 0; i < (int)UV_EFFECT_TYPE::ARRAY_END; i++)
	{
		switch ((UV_EFFECT_TYPE)i)
		{
		case UV_EFFECT_TYPE::WAVE:
			m_baseUVEffect[i] = new WaveEffect();
			break;
		case UV_EFFECT_TYPE::HIT_IMPACT:
			m_baseUVEffect[i] = new HitImpactEffect();
			break;
		default:
			assert(0);	// "そんなエフェクトはない"
			break;
		}
	}
}

UVEffectManager::~UVEffectManager()
{
	// 全部消して
	for (int i = 0; i < (int)UV_EFFECT_TYPE::ARRAY_END; i++)
	{
		SAFE_DELETE(m_baseUVEffect[i]);
	}

}


/***********************************************/
// 更新
/***********************************************/
void UVEffectManager::Update()
{
	//
	for (int i = 0; i < (int)UV_EFFECT_TYPE::ARRAY_END; i++)
	{
		if (m_baseUVEffect[(int)i]->GetUV()->IsAction() == false)continue;// ハジく

		m_baseUVEffect[(int)i]->Update();
	}

}


/***********************************************/
// 描画
/***********************************************/
void UVEffectManager::Render()
{
	for (int i = 0; i < (int)UV_EFFECT_TYPE::ARRAY_END; i++)
	{
		if (m_baseUVEffect[(int)i]->GetUV()->IsAction() == false)continue;// ハジく

		m_baseUVEffect[(int)i]->Render();
	}

}

/************************************************/
//	エフェクト追加
/************************************************/
void UVEffectManager::AddEffect(Vector3 pos, UV_EFFECT_TYPE type)
{
	m_baseUVEffect[(int)type]->Action(pos);
}

void UVEffectManager::AddEffect(Vector3 pos, UV_EFFECT_TYPE type, float startScale, float endScale)
{
	m_baseUVEffect[(int)type]->Action(pos, startScale, endScale);
}

// アングル込みのエフェクト
void UVEffectManager::AddEffect(Vector3 pos, UV_EFFECT_TYPE type,
    float startScale, float endScale, Vector3 startAngle, Vector3 endAngle)
{
    m_baseUVEffect[(int)type]->Action(pos, startScale, endScale, startAngle, endAngle);
}