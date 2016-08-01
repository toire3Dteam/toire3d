#include "PanelEffectManager.h"

/*****************************************************************/
//
///				�G�t�F�N�g�}�l�[h�W���[
//
/*****************************************************************/
//

///***********************************************/
//// �C���X�^���X��
///***********************************************/
//EffectManager* EffectManager::m_pInstance = nullptr;
//
//EffectManager& EffectManager::GetInstance()
//{
//	if (!m_pInstance)
//	{
//		m_pInstance = new EffectManager();
//	}
//
//	// TODO: return �X�e�[�g�����g�������ɑ}�����܂�
//	return *m_pInstance;
//}
//
//void EffectManager::Release()
//{
//	SAFE_DELETE(m_pInstance);
//}
//
//
///***********************************************/
//// �������E���
///***********************************************/
//EffectManager::EffectManager()
//{
//
//}
//
//EffectManager::~EffectManager()
//{
//	// �S��������
//	for (auto it : m_EffectData)
//	{
//		SAFE_DELETE(it);
//	}
//
//	//�@�f�[�^�����
//	m_EffectData.clear();
//}
//
//
///***********************************************/
//// �X�V
///***********************************************/
//void EffectManager::Update()
//{
//	// List
//	for (auto it = m_EffectData.begin(); it != m_EffectData.end();)
//	{
//
//		(*it)->Update();
//
//		if ((*it)->GetPanel()->isEndFlag() == true)// �G�t�F�N�g�I��
//		{
//			// ��ɏ���
//			SAFE_DELETE((*it));
//			// ����ɍX�V�����
//			it = m_EffectData.erase(it);
//		}
//		else
//		{
//			// �����ōX�V
//			it++;
//		}
//	}
//}
//
//
///***********************************************/
//// �`��
///***********************************************/
//void EffectManager::Render()
//{
//	// List
//	for (auto it : m_EffectData)
//	{
//		//it->number->Render(it->x, it->y, it->score);
//		it->Render();
//	}
//
//}
//
//// 3D�`��
//void EffectManager::Render3D()
//{
//	// List
//	for (auto it : m_EffectData)
//	{
//		it->Render3D();
//	}
//
//}
//
///************************************************/
////	�����ǉ�
///************************************************/
//void EffectManager::AddEffect(int x, int y, EFFECT_TYPE type)
//{
//	BaseEffect* data;
//
//	switch (type)
//	{
//	case EFFECT_TYPE::PLUS:
//		data = new PlusEffect();
//		break;
//	case EFFECT_TYPE::HIT:
//		data = new HitEffect();
//		break;
//	case EFFECT_TYPE::PUT:
//		data = new PutEffect();
//		break;
//	case EFFECT_TYPE::NOTICE:
//		data = new NoticeEffect();
//		break;
//	case EFFECT_TYPE::INEFFECT:
//		data = new InEffect();
//		break;
//	case EFFECT_TYPE::INEFFECT_MINI:
//		data = new InEffectMini();
//		break;
//	case EFFECT_TYPE::DOG_EFFECT:
//		data = new DogEffect();
//		break;
//	case EFFECT_TYPE::BURN:
//		data = new BurnEffect();
//		break;
//	case EFFECT_TYPE::ClEAR:
//		data = new ClearEffect();
//		break;
//	case EFFECT_TYPE::DON:
//		data = new DonEffect();
//		break;
//	case EFFECT_TYPE::PUSH:
//		data = new PushEffect();
//		break;
//	case EFFECT_TYPE::DELICIOUS:
//		data = new DeliciousEffect();
//		break;
//	case EFFECT_TYPE::EAT:
//		data = new EatEffect();
//		break;
//	case EFFECT_TYPE::PERFECT:
//		data = new PerfectEffect();
//		break;
//	case EFFECT_TYPE::GREAT:
//		data = new GreatEffect();
//		break;
//	case EFFECT_TYPE::BAD:
//		data = new BadEffect();
//		break;
//	case EFFECT_TYPE::HAPPY:
//		data = new HappyEffect();
//		break;
//	case EFFECT_TYPE::DARK_NOTICE:
//		data = new DarkNoticeEffect();
//		break;	
//	case EFFECT_TYPE::SMOKE:
//		data = new SmokeEffect();
//		break;
//
//	default:
//		break;
//	}
//
//	data->Action(x, y);
//
//	// �v�f�ǉ�
//	m_EffectData.push_back(data);
//
//
//}
//
//void EffectManager::AddEffect(Vector3 pos, EFFECT_TYPE type)
//{
//	BaseEffect* data;
//
//	switch (type)
//	{
//	case EFFECT_TYPE::PLUS:
//		data = new PlusEffect();
//		break;
//	case EFFECT_TYPE::HIT:
//		data = new HitEffect();
//		break;
//	case EFFECT_TYPE::PUT:
//		data = new PutEffect();
//		break;
//	case EFFECT_TYPE::NOTICE:
//		data = new NoticeEffect();
//		break;
//	case EFFECT_TYPE::INEFFECT:
//		data = new InEffect();
//		break;
//	case EFFECT_TYPE::INEFFECT_MINI:
//		data = new InEffectMini();
//		break;
//	case EFFECT_TYPE::DOG_EFFECT:
//		data = new DogEffect();
//		break;
//	case EFFECT_TYPE::BURN:
//		data = new BurnEffect();
//		break;
//	case EFFECT_TYPE::ClEAR:
//		data = new ClearEffect();
//		break;
//	case EFFECT_TYPE::DON:
//		data = new DonEffect();
//		break;
//	case EFFECT_TYPE::PUSH:
//		data = new PushEffect();
//		break;
//	case EFFECT_TYPE::DELICIOUS:
//		data = new DeliciousEffect();
//		break;
//	case EFFECT_TYPE::EAT:
//		data = new EatEffect();
//		break;
//	case EFFECT_TYPE::PERFECT:
//		data = new PerfectEffect();
//		break;
//	case EFFECT_TYPE::GREAT:
//		data = new GreatEffect();
//		break;
//	case EFFECT_TYPE::BAD:
//		data = new BadEffect();
//		break;
//	case EFFECT_TYPE::HAPPY:
//		data = new HappyEffect();
//		break;
//	case EFFECT_TYPE::DARK_NOTICE:
//		data = new DarkNoticeEffect();
//		break;
//	case EFFECT_TYPE::SMOKE:
//		data = new SmokeEffect();
//		break;
//
//	default:
//		break;
//	}
//
//	data->Action(pos);
//
//	// �v�f�ǉ�
//	m_EffectData.push_back(data);
//
//
//}


/***********************************************/
// �������E���
/***********************************************/
PanelEffectManager::PanelEffectManager()
{
	for (int i = 0; i < (int)PANEL_EFFECT_TYPE::ARRAY_END; i++)
	{
		switch ((PANEL_EFFECT_TYPE)i)
		{
		case PANEL_EFFECT_TYPE::PLUS:
			m_basePanelEffect[i] = new PlusEffect();
			break;
		case PANEL_EFFECT_TYPE::HIT:
			m_basePanelEffect[i] = new HitEffect();
			break;
		case PANEL_EFFECT_TYPE::PUT:
			m_basePanelEffect[i] = new PutEffect();
			break;
		case PANEL_EFFECT_TYPE::DAMAGE:
			m_basePanelEffect[i] = new DamageEffect();
			break;
		case PANEL_EFFECT_TYPE::INEFFECT:
			m_basePanelEffect[i] = new InEffect();
			break;
		case PANEL_EFFECT_TYPE::INEFFECT_MINI:
			m_basePanelEffect[i] = new InEffectMini();
			break;
		case PANEL_EFFECT_TYPE::DOG_EFFECT:
			m_basePanelEffect[i] = new DogEffect();
			break;
		case PANEL_EFFECT_TYPE::BURN:
			m_basePanelEffect[i] = new BurnEffect();
			break;
		case PANEL_EFFECT_TYPE::ClEAR:
			m_basePanelEffect[i] = new ClearEffect();
			break;
		case PANEL_EFFECT_TYPE::DON:
			m_basePanelEffect[i] = new DonEffect();
			break;
		case PANEL_EFFECT_TYPE::PUSH:
			m_basePanelEffect[i] = new PushEffect();
			break;
		case PANEL_EFFECT_TYPE::DELICIOUS:
			m_basePanelEffect[i] = new DeliciousEffect();
			break;
		case PANEL_EFFECT_TYPE::EAT:
			m_basePanelEffect[i] = new EatEffect();
			break;
		case PANEL_EFFECT_TYPE::PERFECT:
			m_basePanelEffect[i] = new PerfectEffect();
			break;
		case PANEL_EFFECT_TYPE::GREAT:
			m_basePanelEffect[i] = new GreatEffect();
			break;
		case PANEL_EFFECT_TYPE::BAD:
			m_basePanelEffect[i] = new BadEffect();
			break;
		case PANEL_EFFECT_TYPE::HAPPY:
			m_basePanelEffect[i] = new HappyEffect();
			break;
		case PANEL_EFFECT_TYPE::DARK_NOTICE:
			m_basePanelEffect[i] = new DarkNoticeEffect();
			break;
		case PANEL_EFFECT_TYPE::SMOKE:
			m_basePanelEffect[i] = new SmokeEffect();
			break;

		default:
			assert(0);		// "����ȃG�t�F�N�g�͂Ȃ�"
			break;
		}
	}
}

PanelEffectManager::~PanelEffectManager()
{
	// �S��������
	for (int i = 0; i < (int)PANEL_EFFECT_TYPE::ARRAY_END; i++)
	{
		SAFE_DELETE(m_basePanelEffect[i]);
	}

}


/***********************************************/
// �X�V
/***********************************************/
void PanelEffectManager::Update()
{
	//
	for (int i = 0; i < (int)PANEL_EFFECT_TYPE::ARRAY_END; i++)
	{
		if (m_basePanelEffect[(int)i]->GetPanel()->GetisAction() == false)continue;// �n�W��
	
		m_basePanelEffect[(int)i]->Update();
	}

}


/***********************************************/
// �`��
/***********************************************/
void PanelEffectManager::Render()
{
	for (int i = 0; i < (int)PANEL_EFFECT_TYPE::ARRAY_END; i++)
	{
		if (m_basePanelEffect[(int)i]->GetPanel()->GetisAction() == false)continue;// �n�W��

		m_basePanelEffect[(int)i]->Render();
	}

}

// 3D�`��
void PanelEffectManager::Render3D()
{
	for (int i = 0; i < (int)PANEL_EFFECT_TYPE::ARRAY_END; i++)
	{
		if (m_basePanelEffect[(int)i]->GetPanel()->GetisAction() == false)continue;// �n�W��

		m_basePanelEffect[(int)i]->Render3D();
	}

}

/************************************************/
//	�G�t�F�N�g�ǉ�
/************************************************/
void PanelEffectManager::AddEffect(int x, int y, PANEL_EFFECT_TYPE type)
{
	m_basePanelEffect[(int)type]->Action(x, y);
}

void PanelEffectManager::AddEffect(Vector3 pos, PANEL_EFFECT_TYPE type)
{
	m_basePanelEffect[(int)type]->Action(pos);
}