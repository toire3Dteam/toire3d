#include "EffectManager.h"

/*****************************************************************/
//
//				エフェクトマネーhジャー
//
/*****************************************************************/

/***********************************************/
// インスタンス化
/***********************************************/
EffectManager* EffectManager::m_pInstance = nullptr;

EffectManager& EffectManager::GetInstance()
{
	if (!m_pInstance)
	{
		m_pInstance = new EffectManager();
	}

	// TODO: return ステートメントをここに挿入します
	return *m_pInstance;
}

void EffectManager::Release()
{
	SAFE_DELETE(m_pInstance);
}


/***********************************************/
// 初期化・解放
/***********************************************/
EffectManager::EffectManager()
{

}

EffectManager::~EffectManager()
{
	// 全部消して
	for (auto it : m_EffectData)
	{
		SAFE_DELETE(it);
	}

	//　データを空に
	m_EffectData.clear();
}


/***********************************************/
// 更新
/***********************************************/
void EffectManager::Update()
{
	// List
	for (auto it = m_EffectData.begin(); it != m_EffectData.end();)
	{

		(*it)->Update();

		//if ((*it)->isEnd == true)
		//{
		//	// 先に消す
		//	SAFE_DELETE((*it));
		//	// 勝手に更新される
		//	it = m_EffectData.erase(it);
		//}
		//else
		{
			// 自分で更新
			it++;
		}
	}
}


/***********************************************/
// 描画
/***********************************************/
void EffectManager::Render()
{
	// List
	for (auto it : m_EffectData)
	{
		//it->number->Render(it->x, it->y, it->score);
		it->Render();
	}

}

/************************************************/
//	数字追加
/************************************************/
void EffectManager::AddEffect(int x, int y, EFFECT_TYPE type)
{
	BaseEffect* data;

	switch (type)
	{
	case EFFECT_TYPE::PLUS:
		data = new PlusEffect();
		break;
	case EFFECT_TYPE::HIT:
		data = new HitEffect();
		break;
	case EFFECT_TYPE::PUT:
		data = new PutEffect();
		break;
	case EFFECT_TYPE::NOTICE:
		data = new NoticeEffect();
		break;
	case EFFECT_TYPE::INEFFECT:
		data = new InEffect();
		break;
	case EFFECT_TYPE::INEFFECT_MINI:
		data = new InEffectMini();
		break;
	case EFFECT_TYPE::DOG_EFFECT:
		data = new DogEffect();
		break;
	case EFFECT_TYPE::BURN:
		data = new BurnEffect();
		break;
	case EFFECT_TYPE::ClEAR:
		data = new ClearEffect();
		break;
	case EFFECT_TYPE::DON:
		data = new DonEffect();
		break;
	case EFFECT_TYPE::PUSH:
		data = new PushEffect();
		break;
	case EFFECT_TYPE::DELICIOUS:
		data = new DeliciousEffect();
		break;
	case EFFECT_TYPE::EAT:
		data = new EatEffect();
		break;
	case EFFECT_TYPE::PERFECT:
		data = new PerfectEffect();
		break;
	case EFFECT_TYPE::GREAT:
		data = new GreatEffect();
		break;
	case EFFECT_TYPE::BAD:
		data = new BadEffect();
		break;
	case EFFECT_TYPE::HAPPY:
		data = new HappyEffect();
		break;
	case EFFECT_TYPE::DARK_NOTICE:
		data = new DarkNoticeEffect();
		break;	
	case EFFECT_TYPE::SMOKE:
		data = new SmokeEffect();
		break;

	default:
		break;
	}

	data->Action(x, y);

	// 要素追加
	m_EffectData.push_back(data);


}

