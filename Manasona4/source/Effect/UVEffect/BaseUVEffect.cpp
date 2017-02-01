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

void BaseUVEffect::RenderADDNOZ()
{
	// ��Z�`��(Z�l�l�����Ȃ�)
	m_pic->Render_ADD_NOZ();
}

void BaseUVEffect::RenderGuard()
{
	// �K�[�h�p�`��
	m_pic->Render_Guard();
}

void BaseUVEffect::RenderSoul()
{
	// ���p�`��
	m_pic->Render_Soul();
}


void BaseUVEffect::RenderAreaWall()
{
	// �G���A�̕Ǘp�`��
	m_pic->Render_AreaWall();

}

void BaseUVEffect::Action(Vector3 pos, 
    float startScale, float endScale,
    Vector3 startAngle , Vector3 endAngle)
{
	// �X�P�[���̃A�j��
	m_pic->ScaleAnimation(startScale, endScale);
    // �A���O���̃A�j��
    m_pic->AngleAnimation(startAngle, endAngle);

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
	
	//m_pic->StopRoop();
}


void BaseUVEffect::ActionRoop(Vector3 pos,
	float startScale, float endScale,
	Vector3 startAngle, Vector3 endAngle)
{
	// �X�P�[���̃A�j��
	m_pic->ScaleAnimation(startScale, endScale);
	// �A���O���̃A�j��
	m_pic->AngleAnimation(startAngle, endAngle);

	// �G�t�F�N�g����
	m_pic->ActionRoop();
	m_pos = pos;

	//m_startScale = startScale;
	//m_endScale = endScale;
}

void BaseUVEffect::StopRoop()
{
	// �G�t�F�N�g�~�߂�
	m_pic->StopRoop();
}

// �r���h���ז��������̂ł�����ֈړ�
ConvEffect::ConvEffect()
{
	m_pic = new AnimationUV("Data/UVeffect/convEffect/convEffect2.IMO", 0.00f, 0.03f, 10, false, 2, 6);
};
ConvEffect2::ConvEffect2()
{
	m_pic = new AnimationUV("Data/UVeffect/convEffect/convEffect3.IMO", 0.00f, 0.03f, 10, false, 2, 6);
};
ConvEffect3::ConvEffect3()
{
	m_pic = new AnimationUV("Data/UVeffect/convEffect/convEffect4.IMO", 0.00f, 0.04f, 20, false, 2, 16);
}
ConvEffect4::ConvEffect4()
{

	m_pic = new AnimationUV("Data/UVeffect/convEffect/convEffect5.IMO", 0.00f, 0.04f, 20, false, 0, 16);

}

BurstBallEffect::BurstBallEffect()
{
	m_pic = new AnimationUV("Data/UVeffect/Burst/burstBallEffect.IMO", 0.05f, 0.01f, 40, false, 1, 24);
};


SummonEffect::SummonEffect()
{
	m_pic = new AnimationUV("Data/UVeffect/Stand/Summon.IMO", 0.0f, 0.06f, 12, false, 3, 8);
}

SummonWaveEffect::SummonWaveEffect()
{
	m_pic = new AnimationUV("Data/UVeffect/Stand/SummonWave.IMO", 0.0f, 0.0f, 12, false, 1, 8);
}

ExitEffect::ExitEffect()
{
	m_pic = new AnimationUV("Data/UVeffect/Stand/Exit.IMO", 0.0f, -0.04f, 12, false, 3, 10);
}