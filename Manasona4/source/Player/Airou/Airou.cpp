#include "Airou.h"

Airou::Airou(int deviceID) :BasePlayer(deviceID)
{
	// キャラ固有の情報の設定
	m_maxSpeed = 1.25f;
	BasePlayer::LoadAttackFrameList("DATA/CHR/Airou/FrameList.txt");

	// アイルーメッシュ
	m_pObj = new iex3DObj("DATA/CHR/Airou/airou_motion.IEM");
}

void Airou::InitAttackDatas()
{
	// 通常1段
	m_AttackDatas[(int)BASE_ATTACK_STATE::RUSH1].bBeInvincible = false;
	m_AttackDatas[(int)BASE_ATTACK_STATE::RUSH1].damage = 120;
	m_AttackDatas[(int)BASE_ATTACK_STATE::RUSH1].FlyVector.Set(.25f, .5f);
	m_AttackDatas[(int)BASE_ATTACK_STATE::RUSH1].pierceLV = 0;
	m_AttackDatas[(int)BASE_ATTACK_STATE::RUSH1].SE_ID = "通常1段";

	// 通常2段
	m_AttackDatas[(int)BASE_ATTACK_STATE::RUSH2].bBeInvincible = false;
	m_AttackDatas[(int)BASE_ATTACK_STATE::RUSH2].damage = 240;
	m_AttackDatas[(int)BASE_ATTACK_STATE::RUSH2].FlyVector.Set(.35f, .75f);
	m_AttackDatas[(int)BASE_ATTACK_STATE::RUSH2].pierceLV = 0;
	m_AttackDatas[(int)BASE_ATTACK_STATE::RUSH2].SE_ID = "通常2段";

	// 通常3段
	m_AttackDatas[(int)BASE_ATTACK_STATE::RUSH3].bBeInvincible = true;
	m_AttackDatas[(int)BASE_ATTACK_STATE::RUSH3].damage = 360;
	m_AttackDatas[(int)BASE_ATTACK_STATE::RUSH3].FlyVector.Set(3.5f, 2.0f);
	m_AttackDatas[(int)BASE_ATTACK_STATE::RUSH3].pierceLV = 0;
	m_AttackDatas[(int)BASE_ATTACK_STATE::RUSH3].SE_ID = "通常3段";

	// 全共通
	FOR((int)BASE_ATTACK_STATE::END)
	{
		MyAssert((m_AttackDatas[i].FlyVector.x >= 0), "吹っ飛びベクトル-にするなー");
		m_AttackDatas[i].bHit = false;
	}
}