#include "BaseUVEffect.h"

/***********************/
//	ベースUVエフェクト
/***********************/

BaseUVEffect::BaseUVEffect()
{
	m_pic = nullptr;
	m_pos.x = m_pos.y = m_pos.z = 0.0f;
	//m_startScale = 1.0f;
	//m_endScale = 1.0f;
}

BaseUVEffect::~BaseUVEffect()
{
	SAFE_DELETE(m_pic);
}

void BaseUVEffect::Update()
{
	// エフェクト
	m_pic->Update(m_pos);
}

void BaseUVEffect::Render()
{
	// エフェクト
	m_pic->Render();
}

void BaseUVEffect::RenderADD()
{
	// 乗算描画
	m_pic->Render_ADD();
}

void BaseUVEffect::Action(Vector3 pos, float startScale, float endScale)
{
	// スケールのアニメ
	m_pic->ScaleAnimation(startScale, endScale);
	// エフェクト発動
	m_pic->Action();
	m_pos = pos;

	//m_startScale = startScale;
	//m_endScale = endScale;
}

void BaseUVEffect::Stop()
{
	// エフェクト止める
	m_pic->Stop();
}
