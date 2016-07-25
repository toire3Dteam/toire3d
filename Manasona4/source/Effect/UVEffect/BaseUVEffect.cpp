#include "BaseUVEffect.h"

/***********************/
//	�x�[�XUV�G�t�F�N�g
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
	// �G�t�F�N�g
	m_pic->Update(m_pos);
}

void BaseUVEffect::Render()
{
	// �G�t�F�N�g
	m_pic->Render();
}

void BaseUVEffect::RenderADD()
{
	// ��Z�`��
	m_pic->Render_ADD();
}

void BaseUVEffect::Action(Vector3 pos, float startScale, float endScale)
{
	// �X�P�[���̃A�j��
	m_pic->ScaleAnimation(startScale, endScale);
	// �G�t�F�N�g����
	m_pic->Action();
	m_pos = pos;

	//m_startScale = startScale;
	//m_endScale = endScale;
}

void BaseUVEffect::Stop()
{
	// �G�t�F�N�g�~�߂�
	m_pic->Stop();
}
