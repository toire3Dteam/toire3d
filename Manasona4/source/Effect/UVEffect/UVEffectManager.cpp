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
		default:
			assert(0);	// "����ȃG�t�F�N�g�͂Ȃ�"
			break;
		}
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

		m_baseUVEffect[(int)i]->Render();
	}

}

/************************************************/
//	�G�t�F�N�g�ǉ�
/************************************************/
void UVEffectManager::AddEffect(Vector3 pos, UV_EFFECT_TYPE type)
{
	m_baseUVEffect[(int)type]->Action(pos);
}

void UVEffectManager::AddEffect(Vector3 pos, UV_EFFECT_TYPE type, float startScale, float endScale)
{
	m_baseUVEffect[(int)type]->Action(pos, startScale, endScale);

}
