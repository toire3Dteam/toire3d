#include "UVEffectManager.h"

/*****************************/
//		UVEffect�}�l�[�W���[
/****************************/
	
/***********************************************/
// �������E���
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
		case UV_EFFECT_TYPE::PERSONA:
			m_baseUVEffect[i] = new PersonaEffect();
			break;
		case UV_EFFECT_TYPE::IMPACT:
			m_baseUVEffect[i] = new ImpactEffect();
			break;
		case UV_EFFECT_TYPE::SHOCK_WAVE:
			m_baseUVEffect[i] = new ShockWaveEffect();
			break;
		case UV_EFFECT_TYPE::UPPER:
			m_baseUVEffect[i] = new UpperEffect();
			break;
		case UV_EFFECT_TYPE::GUARD:
			m_baseUVEffect[i] = new GuardEffect();
			break;
		case UV_EFFECT_TYPE::RUN:
			m_baseUVEffect[i] = new RunEffect();
			break;
		case UV_EFFECT_TYPE::CONV:
			m_baseUVEffect[i] = new ConvEffect();
			break;
		case UV_EFFECT_TYPE::CONV2:
			m_baseUVEffect[i] = new ConvEffect2();
			break;
		case UV_EFFECT_TYPE::CONV3:
			m_baseUVEffect[i] = new ConvEffect3();
			break;
		case UV_EFFECT_TYPE::CONV4:
			m_baseUVEffect[i] = new ConvEffect4();
			break;
		case UV_EFFECT_TYPE::BURST_BALL:
			m_baseUVEffect[i] = new BurstBallEffect();
			break;
		case UV_EFFECT_TYPE::SPLASH:
			m_baseUVEffect[i] = new SplashEffect();
			break;
		default:
			assert(0);	// "����ȃG�t�F�N�g�͂Ȃ�"
			break;
		}
	}

	// �f�B���C�^�C�}�[������
	for (int i = 0; i < (int)UV_EFFECT_TYPE::ARRAY_END; i++)
	{
		m_delayTimer[i] = 0;
	}


}

UVEffectManager::~UVEffectManager()
{
	// �S��������
	for (int i = 0; i < (int)UV_EFFECT_TYPE::ARRAY_END; i++)
	{
		SAFE_DELETE(m_baseUVEffect[i]);
	}

}


/***********************************************/
// �X�V
/***********************************************/
void UVEffectManager::Update()
{
	//
	for (int i = 0; i < (int)UV_EFFECT_TYPE::ARRAY_END; i++)
	{
		if (m_baseUVEffect[(int)i]->GetUV()->IsAction() == false)continue;// �n�W��
		if (--m_delayTimer[(int)i] >= 0){ continue; };// ����Ƀf�N�������g���ăf�B���C���Ȃ�͂���	
		m_baseUVEffect[(int)i]->Update();
	}

}


/***********************************************/
// �`��
/***********************************************/
void UVEffectManager::Render()
{
	for (int i = 0; i < (int)UV_EFFECT_TYPE::ARRAY_END; i++)
	{
		if (m_baseUVEffect[(int)i]->GetUV()->IsAction() == false)continue;// �n�W��
		if (m_delayTimer[(int)i] >= 0){  continue; };// �f�B���C���Ȃ�͂���	
		m_baseUVEffect[(int)i]->Render();
	}

}

/************************************************/
//	�G�t�F�N�g�ǉ�
/************************************************/
void UVEffectManager::AddEffect(Vector3 pos, UV_EFFECT_TYPE type)
{
	m_baseUVEffect[(int)type]->Action(pos);
	m_delayTimer[(int)type] = 0;
}

void UVEffectManager::AddEffect(Vector3 pos, UV_EFFECT_TYPE type, float startScale, float endScale)
{
	m_baseUVEffect[(int)type]->Action(pos, startScale, endScale);
	m_delayTimer[(int)type] = 0;
}

// �A���O�����݂̃G�t�F�N�g
void UVEffectManager::AddEffect(Vector3 pos, UV_EFFECT_TYPE type,
    float startScale, float endScale, Vector3 startAngle, Vector3 endAngle)
{
    m_baseUVEffect[(int)type]->Action(pos, startScale, endScale, startAngle, endAngle);
	m_delayTimer[(int)type] = 0;
}

// �A���O�����݂̃G�t�F�N�g
void UVEffectManager::AddEffect(Vector3 pos, UV_EFFECT_TYPE type,
	float startScale, float endScale, Vector3 startAngle, Vector3 endAngle,int deayTimer)
{
    m_baseUVEffect[(int)type]->Action(pos, startScale, endScale, startAngle, endAngle);
	m_delayTimer[(int)type] = deayTimer;
}


/************************************************/
//	�G�t�F�N�g���~�߂�
/************************************************/
void UVEffectManager::StopEffect(UV_EFFECT_TYPE type)
{
	m_baseUVEffect[(int)type]->Stop();
	m_delayTimer[(int)type] = 0;
}



/************************************************/
//	���[�v�G�t�F�N�g�֘A
/************************************************/
void UVEffectManager::AddEffectRoop(Vector3 pos, UV_EFFECT_TYPE type)
{
	m_baseUVEffect[(int)type]->ActionRoop(pos);
	m_delayTimer[(int)type] = 0;
}
void UVEffectManager::AddEffectRoop(Vector3 pos, UV_EFFECT_TYPE type, float startScale, float endScale)
{
	m_baseUVEffect[(int)type]->ActionRoop(pos, startScale, endScale);
	m_delayTimer[(int)type] = 0;
}

void UVEffectManager::StopEffectRoop(UV_EFFECT_TYPE type)
{
	m_baseUVEffect[(int)type]->StopRoop();
	m_delayTimer[(int)type] = 0;
}