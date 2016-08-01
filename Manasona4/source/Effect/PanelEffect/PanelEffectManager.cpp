#include "PanelEffectManager.h"

/*****************************************************************/
//
///				エフェクトマネーhジャー
//
/*****************************************************************/
//

///***********************************************/
//// インスタンス化
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
//	// TODO: return ステートメントをここに挿入します
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
//// 初期化・解放
///***********************************************/
//EffectManager::EffectManager()
//{
//
//}
//
//EffectManager::~EffectManager()
//{
//	// 全部消して
//	for (auto it : m_EffectData)
//	{
//		SAFE_DELETE(it);
//	}
//
//	//　データを空に
//	m_EffectData.clear();
//}
//
//
///***********************************************/
//// 更新
///***********************************************/
//void EffectManager::Update()
//{
//	// List
//	for (auto it = m_EffectData.begin(); it != m_EffectData.end();)
//	{
//
//		(*it)->Update();
//
//		if ((*it)->GetPanel()->isEndFlag() == true)// エフェクト終了
//		{
//			// 先に消す
//			SAFE_DELETE((*it));
//			// 勝手に更新される
//			it = m_EffectData.erase(it);
//		}
//		else
//		{
//			// 自分で更新
//			it++;
//		}
//	}
//}
//
//
///***********************************************/
//// 描画
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
//// 3D描画
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
////	数字追加
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
//	// 要素追加
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
//	// 要素追加
//	m_EffectData.push_back(data);
//
//
//}


/***********************************************/
// 初期化・解放
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
			assert(0);		// "そんなエフェクトはない"
			break;
		}
	}
}

PanelEffectManager::~PanelEffectManager()
{
	// 全部消して
	for (int i = 0; i < (int)PANEL_EFFECT_TYPE::ARRAY_END; i++)
	{
		SAFE_DELETE(m_basePanelEffect[i]);
	}

}


/***********************************************/
// 更新
/***********************************************/
void PanelEffectManager::Update()
{
	//
	for (int i = 0; i < (int)PANEL_EFFECT_TYPE::ARRAY_END; i++)
	{
		if (m_basePanelEffect[(int)i]->GetPanel()->GetisAction() == false)continue;// ハジく
	
		m_basePanelEffect[(int)i]->Update();
	}

}


/***********************************************/
// 描画
/***********************************************/
void PanelEffectManager::Render()
{
	for (int i = 0; i < (int)PANEL_EFFECT_TYPE::ARRAY_END; i++)
	{
		if (m_basePanelEffect[(int)i]->GetPanel()->GetisAction() == false)continue;// ハジく

		m_basePanelEffect[(int)i]->Render();
	}

}

// 3D描画
void PanelEffectManager::Render3D()
{
	for (int i = 0; i < (int)PANEL_EFFECT_TYPE::ARRAY_END; i++)
	{
		if (m_basePanelEffect[(int)i]->GetPanel()->GetisAction() == false)continue;// ハジく

		m_basePanelEffect[(int)i]->Render3D();
	}

}

/************************************************/
//	エフェクト追加
/************************************************/
void PanelEffectManager::AddEffect(int x, int y, PANEL_EFFECT_TYPE type)
{
	m_basePanelEffect[(int)type]->Action(x, y);
}

void PanelEffectManager::AddEffect(Vector3 pos, PANEL_EFFECT_TYPE type)
{
	m_basePanelEffect[(int)type]->Action(pos);
}