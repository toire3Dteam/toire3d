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
	m_AttackDatas[(int)BASE_ATTACK_STATE::RUSH1].damage = 120;
	m_AttackDatas[(int)BASE_ATTACK_STATE::RUSH1].pierceLV = 0;
	m_AttackDatas[(int)BASE_ATTACK_STATE::RUSH1].SE_ID = "通常1段";

	// 通常2段
	m_AttackDatas[(int)BASE_ATTACK_STATE::RUSH2].damage = 240;
	m_AttackDatas[(int)BASE_ATTACK_STATE::RUSH2].pierceLV = 0;
	m_AttackDatas[(int)BASE_ATTACK_STATE::RUSH2].SE_ID = "通常2段";

	// 通常3段
	m_AttackDatas[(int)BASE_ATTACK_STATE::RUSH3].damage = 360;
	m_AttackDatas[(int)BASE_ATTACK_STATE::RUSH3].pierceLV = 0;
	m_AttackDatas[(int)BASE_ATTACK_STATE::RUSH3].SE_ID = "通常3段";
}