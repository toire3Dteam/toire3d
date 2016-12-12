#include "Teki.h"
#include "../../Sound/SoundManager.h"
#include "UI/HeaveHoFinish/HeaveHoFinish.h"
#include "../../DeferredEx/DeferredEx.h"
#include "../../BaseEntity/Message/MessageDispatcher.h"


Teki::Teki(SIDE side, const SideData &data) :BasePlayer(side, data)
{
	// エフェクトカメラID
	m_tagCharacterParam.eHeaveHoOverFlowCameraID = EFFECT_CAMERA_ID::OVERFLOW_TEST;

	Reset();

	// キャラごとの設定を読み込む
	BasePlayer::LoadCharacterParam("DATA/CHR/Teki/param.txt");

	m_pName = "Teki";
	m_pCutinPic = new tdn2DAnim("DATA/UI/Game/OverDriveCutin/teki.png");
	m_pCutinPic->OrderMoveAppeared(8, -400, +200);

	// フレーム読み込み
	BasePlayer::LoadAttackFrameList("DATA/CHR/Teki/FrameList.txt");
	// ★フレーム独自改造
	{
		// 2段目は2HITじゃなくなったので、ボツニナリマシタ
		//m_ActionFrameList[(int)BASE_ACTION_STATE::RUSH2][13] = FRAME_STATE::RECOVERY_HIT;
		//m_ActionFrameList[(int)BASE_ACTION_STATE::RUSH2][14] = FRAME_STATE::ACTIVE;
		//m_ActionFrameList[(int)BASE_ACTION_STATE::RUSH2][15] = FRAME_STATE::ACTIVE;

		// キルラ
		m_ActionFrameList[(int)BASE_ACTION_STATE::SKILL][5 + 7] = FRAME_STATE::RECOVERY_HIT;
		m_ActionFrameList[(int)BASE_ACTION_STATE::SKILL][5 + 13] = FRAME_STATE::RECOVERY_HIT;
		m_ActionFrameList[(int)BASE_ACTION_STATE::SKILL][5 + 19] = FRAME_STATE::RECOVERY_HIT;
	}
	// てきメッシュ
	m_pObj = m_pDefaultObj;

	// 顔グラ
	m_pFacePic = new tdn2DAnim("Data/UI/Game/CharaIcon/TekiIcon.png");
	//m_pFacePic->OrderNone();
	m_pFacePic->OrderShrink(8, 1, 1.25f);

	// スピードライン
	m_pSpeedLine = new SpeedLineGreenEffect;

	if (m_side == SIDE::RIGHT)
	{
		LPSTR path = "DATA/CHR/Teki/teki2.png";
		m_pDefaultObj->SetTexture(tdnTexture::Load(path), 0);
		m_pHHDOFObj->SetTexture(tdnTexture::Load(path), 0);
	}
}

void Teki::Reset()
{
	// 必ず書く
	BasePlayer::Reset();
}

void Teki::InitActionDatas()
{
	m_ActionDatas[(int)BASE_ACTION_STATE::ESCAPE].pAttackData = nullptr;	// このデータは攻撃ではない
	m_ActionDatas[(int)BASE_ACTION_STATE::STAND].pAttackData = nullptr;	// このデータは攻撃ではない
	m_ActionDatas[(int)BASE_ACTION_STATE::OVERDRIVE_ONEMORE].pAttackData = nullptr;	// このデータは攻撃ではない
																					//m_ActionDatas[(int)BASE_ACTION_STATE::OVERDRIVE_BURST].pAttackData = nullptr;	// このデータは攻撃ではない

	/********************************************************************/
	// (※)アクションアタックデータ変えたら　スタンドの方も変えるんやで
	/********************************************************************/

	//==============================================================================================================
	//	バースト　（吹っ飛ばすために攻撃系アクションデータに）
	// 地上ヒットも空中ヒットも共通の情報
	m_ActionDatas[(int)BASE_ACTION_STATE::OVERDRIVE_BURST].InstanceAttackData();	// アタックデータ作成
	m_ActionDatas[(int)BASE_ACTION_STATE::OVERDRIVE_BURST].pAttackData->damage = 0;
	m_ActionDatas[(int)BASE_ACTION_STATE::OVERDRIVE_BURST].pAttackData->pierceLV = 0;
	m_ActionDatas[(int)BASE_ACTION_STATE::OVERDRIVE_BURST].pAttackData->HitSE = "ヒット6";
	m_ActionDatas[(int)BASE_ACTION_STATE::OVERDRIVE_BURST].pAttackData->WhiffSE = "空振り5";
	m_ActionDatas[(int)BASE_ACTION_STATE::OVERDRIVE_BURST].pAttackData->HitEffectType = EFFECT_TYPE::DAMAGE;
	m_ActionDatas[(int)BASE_ACTION_STATE::OVERDRIVE_BURST].pAttackData->WhiffEffectType = EFFECT_TYPE::WHIFF;
	m_ActionDatas[(int)BASE_ACTION_STATE::OVERDRIVE_BURST].pAttackData->bAntiAir = false;
	m_ActionDatas[(int)BASE_ACTION_STATE::OVERDRIVE_BURST].pAttackData->bFinish = false;
	m_ActionDatas[(int)BASE_ACTION_STATE::OVERDRIVE_BURST].pAttackData->AntiGuard = ANTIGUARD_ATTACK::NONE;
	m_ActionDatas[(int)BASE_ACTION_STATE::OVERDRIVE_BURST].pAttackData->ShakeCameraInfo.Set(0, 0);
	m_ActionDatas[(int)BASE_ACTION_STATE::OVERDRIVE_BURST].pAttackData->GuardRecoveryFrame = 20;
	m_ActionDatas[(int)BASE_ACTION_STATE::OVERDRIVE_BURST].pAttackData->attribute = ATTACK_ATTRIBUTE::BULLET;
	m_ActionDatas[(int)BASE_ACTION_STATE::OVERDRIVE_BURST].pAttackData->fComboRate = 1.0f;
	// 地上ヒットと空中ヒットで挙動が変わるもの
	m_ActionDatas[(int)BASE_ACTION_STATE::OVERDRIVE_BURST].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].bBeInvincible = true;
	m_ActionDatas[(int)BASE_ACTION_STATE::OVERDRIVE_BURST].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].bBeInvincible = true;
	m_ActionDatas[(int)BASE_ACTION_STATE::OVERDRIVE_BURST].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].FlyVector.Set(3.25f, 3.0f);
	m_ActionDatas[(int)BASE_ACTION_STATE::OVERDRIVE_BURST].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].FlyVector.Set(3.25f, 3.0f);
	m_ActionDatas[(int)BASE_ACTION_STATE::OVERDRIVE_BURST].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].hitStopFlame = 0;
	m_ActionDatas[(int)BASE_ACTION_STATE::OVERDRIVE_BURST].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].hitStopFlame = 0;
	m_ActionDatas[(int)BASE_ACTION_STATE::OVERDRIVE_BURST].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].HitRecoveryFrame = 60;
	m_ActionDatas[(int)BASE_ACTION_STATE::OVERDRIVE_BURST].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].HitRecoveryFrame = 60;
	// 判定形状
	m_ActionDatas[(int)BASE_ACTION_STATE::OVERDRIVE_BURST].pAttackData->pCollisionShape->width = 18;
	m_ActionDatas[(int)BASE_ACTION_STATE::OVERDRIVE_BURST].pAttackData->pCollisionShape->height = 17;
	m_ActionDatas[(int)BASE_ACTION_STATE::OVERDRIVE_BURST].pAttackData->pCollisionShape->pos.Set(0, 3.5f, 0);


	//==============================================================================================================
	//	通常1段
	// 地上ヒットも空中ヒットも共通の情報
	m_ActionDatas[(int)BASE_ACTION_STATE::RUSH1].InstanceAttackData();	// アタックデータ作成
	m_ActionDatas[(int)BASE_ACTION_STATE::RUSH1].pAttackData->damage = 240;
	m_ActionDatas[(int)BASE_ACTION_STATE::RUSH1].pAttackData->pierceLV = 0;
	m_ActionDatas[(int)BASE_ACTION_STATE::RUSH1].pAttackData->HitSE = "ヒット4";
	m_ActionDatas[(int)BASE_ACTION_STATE::RUSH1].pAttackData->WhiffSE = "空振り5";
	m_ActionDatas[(int)BASE_ACTION_STATE::RUSH1].pAttackData->HitEffectType = EFFECT_TYPE::DAMAGE;
	m_ActionDatas[(int)BASE_ACTION_STATE::RUSH1].pAttackData->WhiffEffectType = EFFECT_TYPE::WHIFF;
	m_ActionDatas[(int)BASE_ACTION_STATE::RUSH1].pAttackData->bAntiAir = false;
	m_ActionDatas[(int)BASE_ACTION_STATE::RUSH1].pAttackData->bFinish = true;
	m_ActionDatas[(int)BASE_ACTION_STATE::RUSH1].pAttackData->AntiGuard = ANTIGUARD_ATTACK::NONE;
	m_ActionDatas[(int)BASE_ACTION_STATE::RUSH1].pAttackData->ShakeCameraInfo.Set(0, 0);
	m_ActionDatas[(int)BASE_ACTION_STATE::RUSH1].pAttackData->GuardRecoveryFrame = 20;
	m_ActionDatas[(int)BASE_ACTION_STATE::RUSH1].pAttackData->fGuardKnockBackPower = .1f;
	m_ActionDatas[(int)BASE_ACTION_STATE::RUSH1].pAttackData->fComboRate = .85f;
	// 地上ヒットと空中ヒットで挙動が変わるもの
	m_ActionDatas[(int)BASE_ACTION_STATE::RUSH1].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].bBeInvincible = false;
	m_ActionDatas[(int)BASE_ACTION_STATE::RUSH1].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].bBeInvincible = false;
	m_ActionDatas[(int)BASE_ACTION_STATE::RUSH1].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].FlyVector.Set(.2f, .0f);
	m_ActionDatas[(int)BASE_ACTION_STATE::RUSH1].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].FlyVector.Set(.2f, .70f);
	m_ActionDatas[(int)BASE_ACTION_STATE::RUSH1].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].hitStopFlame = 12;
	m_ActionDatas[(int)BASE_ACTION_STATE::RUSH1].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].hitStopFlame = 13;
	m_ActionDatas[(int)BASE_ACTION_STATE::RUSH1].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].HitRecoveryFrame = 20;
	m_ActionDatas[(int)BASE_ACTION_STATE::RUSH1].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].HitRecoveryFrame = 20;
	m_ActionDatas[(int)BASE_ACTION_STATE::RUSH1].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].DamageMotion = DAMAGE_MOTION::KNOCK_BACK;
	m_ActionDatas[(int)BASE_ACTION_STATE::RUSH1].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].DamageMotion = DAMAGE_MOTION::KNOCK_DOWN;
	// 判定形状
	m_ActionDatas[(int)BASE_ACTION_STATE::RUSH1].pAttackData->pCollisionShape->width = 7;
	m_ActionDatas[(int)BASE_ACTION_STATE::RUSH1].pAttackData->pCollisionShape->height = 6;
	m_ActionDatas[(int)BASE_ACTION_STATE::RUSH1].pAttackData->pCollisionShape->pos.Set(6, 10, 0);

	//==============================================================================================================
	//	通常2段
	// 地上ヒットも空中ヒットも共通の情報
	m_ActionDatas[(int)BASE_ACTION_STATE::RUSH2].InstanceAttackData();	// アタックデータ作成
	m_ActionDatas[(int)BASE_ACTION_STATE::RUSH2].pAttackData->damage = 320;
	m_ActionDatas[(int)BASE_ACTION_STATE::RUSH2].pAttackData->pierceLV = 0;
	m_ActionDatas[(int)BASE_ACTION_STATE::RUSH2].pAttackData->HitSE = "ヒット5";
	m_ActionDatas[(int)BASE_ACTION_STATE::RUSH2].pAttackData->WhiffSE = "空振り5";
	m_ActionDatas[(int)BASE_ACTION_STATE::RUSH2].pAttackData->HitEffectType = EFFECT_TYPE::DAMAGE;
	m_ActionDatas[(int)BASE_ACTION_STATE::RUSH2].pAttackData->WhiffEffectType = EFFECT_TYPE::WHIFF;
	m_ActionDatas[(int)BASE_ACTION_STATE::RUSH2].pAttackData->bAntiAir = false;
	m_ActionDatas[(int)BASE_ACTION_STATE::RUSH2].pAttackData->bFinish = true;
	m_ActionDatas[(int)BASE_ACTION_STATE::RUSH2].pAttackData->AntiGuard = ANTIGUARD_ATTACK::NONE;
	m_ActionDatas[(int)BASE_ACTION_STATE::RUSH2].pAttackData->ShakeCameraInfo.Set(0, 0);
	m_ActionDatas[(int)BASE_ACTION_STATE::RUSH2].pAttackData->GuardRecoveryFrame = 25;
	m_ActionDatas[(int)BASE_ACTION_STATE::RUSH2].pAttackData->fGuardKnockBackPower = .4f;
	m_ActionDatas[(int)BASE_ACTION_STATE::RUSH2].pAttackData->fComboRate = 0.85f;
	// 地上ヒットと空中ヒットで挙動が変わるもの
	m_ActionDatas[(int)BASE_ACTION_STATE::RUSH2].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].bBeInvincible = false;
	m_ActionDatas[(int)BASE_ACTION_STATE::RUSH2].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].bBeInvincible = false;
	m_ActionDatas[(int)BASE_ACTION_STATE::RUSH2].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].FlyVector.Set(.3f, .0f);
	m_ActionDatas[(int)BASE_ACTION_STATE::RUSH2].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].FlyVector.Set(.3f, .75f);
	m_ActionDatas[(int)BASE_ACTION_STATE::RUSH2].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].hitStopFlame = 12;
	m_ActionDatas[(int)BASE_ACTION_STATE::RUSH2].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].hitStopFlame = 13;
	m_ActionDatas[(int)BASE_ACTION_STATE::RUSH2].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].HitRecoveryFrame = 24;
	m_ActionDatas[(int)BASE_ACTION_STATE::RUSH2].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].HitRecoveryFrame = 24;
	m_ActionDatas[(int)BASE_ACTION_STATE::RUSH2].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].DamageMotion = DAMAGE_MOTION::KNOCK_BACK;
	m_ActionDatas[(int)BASE_ACTION_STATE::RUSH2].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].DamageMotion = DAMAGE_MOTION::KNOCK_DOWN;
	// 判定形状
	m_ActionDatas[(int)BASE_ACTION_STATE::RUSH2].pAttackData->pCollisionShape->width = 8;
	m_ActionDatas[(int)BASE_ACTION_STATE::RUSH2].pAttackData->pCollisionShape->height = 8;
	m_ActionDatas[(int)BASE_ACTION_STATE::RUSH2].pAttackData->pCollisionShape->pos.Set(7, 5.5, 0);

	//==============================================================================================================
	//	対空攻撃
	// 地上ヒットも空中ヒットも共通の情報
	m_ActionDatas[(int)BASE_ACTION_STATE::ANTI_AIR].InstanceAttackData();	// アタックデータ作成
	m_ActionDatas[(int)BASE_ACTION_STATE::ANTI_AIR].pAttackData->damage = 400;
	m_ActionDatas[(int)BASE_ACTION_STATE::ANTI_AIR].pAttackData->pierceLV = 0;
	m_ActionDatas[(int)BASE_ACTION_STATE::ANTI_AIR].pAttackData->HitSE = "ヒット6";
	m_ActionDatas[(int)BASE_ACTION_STATE::ANTI_AIR].pAttackData->WhiffSE = "空振り5";
	m_ActionDatas[(int)BASE_ACTION_STATE::ANTI_AIR].pAttackData->HitEffectType = EFFECT_TYPE::DAMAGE;
	m_ActionDatas[(int)BASE_ACTION_STATE::ANTI_AIR].pAttackData->WhiffEffectType = EFFECT_TYPE::WHIFF;
	m_ActionDatas[(int)BASE_ACTION_STATE::ANTI_AIR].pAttackData->bAntiAir = true;
	m_ActionDatas[(int)BASE_ACTION_STATE::ANTI_AIR].pAttackData->bFinish = true;
	m_ActionDatas[(int)BASE_ACTION_STATE::ANTI_AIR].pAttackData->AntiGuard = ANTIGUARD_ATTACK::NONE;
	m_ActionDatas[(int)BASE_ACTION_STATE::ANTI_AIR].pAttackData->ShakeCameraInfo.Set(.1f, 2);
	m_ActionDatas[(int)BASE_ACTION_STATE::ANTI_AIR].pAttackData->GuardRecoveryFrame = 20;
	m_ActionDatas[(int)BASE_ACTION_STATE::ANTI_AIR].pAttackData->fGuardKnockBackPower = .4f;
	m_ActionDatas[(int)BASE_ACTION_STATE::ANTI_AIR].pAttackData->fComboRate = 0.85f;
	// 地上ヒットと空中ヒットで挙動が変わるもの
	m_ActionDatas[(int)BASE_ACTION_STATE::ANTI_AIR].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].bBeInvincible = false;
	m_ActionDatas[(int)BASE_ACTION_STATE::ANTI_AIR].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].bBeInvincible = false;
	m_ActionDatas[(int)BASE_ACTION_STATE::ANTI_AIR].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].FlyVector.Set(.5f, .0f);
	m_ActionDatas[(int)BASE_ACTION_STATE::ANTI_AIR].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].FlyVector.Set(.25f, 1.9f);
	m_ActionDatas[(int)BASE_ACTION_STATE::ANTI_AIR].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].hitStopFlame = 4;
	m_ActionDatas[(int)BASE_ACTION_STATE::ANTI_AIR].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].hitStopFlame = 4;
	m_ActionDatas[(int)BASE_ACTION_STATE::ANTI_AIR].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].HitRecoveryFrame = 24;
	m_ActionDatas[(int)BASE_ACTION_STATE::ANTI_AIR].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].HitRecoveryFrame = 45;
	m_ActionDatas[(int)BASE_ACTION_STATE::ANTI_AIR].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].DamageMotion = DAMAGE_MOTION::KNOCK_BACK;
	m_ActionDatas[(int)BASE_ACTION_STATE::ANTI_AIR].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].DamageMotion = DAMAGE_MOTION::KNOCK_DOWN;
	// 判定形状
	m_ActionDatas[(int)BASE_ACTION_STATE::ANTI_AIR].pAttackData->pCollisionShape->width = 8;
	m_ActionDatas[(int)BASE_ACTION_STATE::ANTI_AIR].pAttackData->pCollisionShape->height = 8;
	m_ActionDatas[(int)BASE_ACTION_STATE::ANTI_AIR].pAttackData->pCollisionShape->pos.Set(2.5f, 12, 0);

	//==============================================================================================================
	//	中断攻撃(どっこいアタック)
	// 地上ヒットも空中ヒットも共通の情報
	m_ActionDatas[(int)BASE_ACTION_STATE::DOKKOI_ATTACK].InstanceAttackData();	// アタックデータ作成
	m_ActionDatas[(int)BASE_ACTION_STATE::DOKKOI_ATTACK].pAttackData->damage = 500;
	m_ActionDatas[(int)BASE_ACTION_STATE::DOKKOI_ATTACK].pAttackData->pierceLV = 0;
	m_ActionDatas[(int)BASE_ACTION_STATE::DOKKOI_ATTACK].pAttackData->HitSE = "ヒット4";
	m_ActionDatas[(int)BASE_ACTION_STATE::DOKKOI_ATTACK].pAttackData->WhiffSE = "空振り5";
	m_ActionDatas[(int)BASE_ACTION_STATE::DOKKOI_ATTACK].pAttackData->HitEffectType = EFFECT_TYPE::DAMAGE;
	m_ActionDatas[(int)BASE_ACTION_STATE::DOKKOI_ATTACK].pAttackData->WhiffEffectType = EFFECT_TYPE::WHIFF;
	m_ActionDatas[(int)BASE_ACTION_STATE::DOKKOI_ATTACK].pAttackData->bAntiAir = false;
	m_ActionDatas[(int)BASE_ACTION_STATE::DOKKOI_ATTACK].pAttackData->bFinish = true;
	m_ActionDatas[(int)BASE_ACTION_STATE::DOKKOI_ATTACK].pAttackData->AntiGuard = ANTIGUARD_ATTACK::UP_ATTACK;	// ★しゃがみガードをつぶす
	m_ActionDatas[(int)BASE_ACTION_STATE::DOKKOI_ATTACK].pAttackData->ShakeCameraInfo.Set(.1f, 2);
	m_ActionDatas[(int)BASE_ACTION_STATE::DOKKOI_ATTACK].pAttackData->GuardRecoveryFrame = 20;
	m_ActionDatas[(int)BASE_ACTION_STATE::DOKKOI_ATTACK].pAttackData->fGuardKnockBackPower = .45f;
	m_ActionDatas[(int)BASE_ACTION_STATE::DOKKOI_ATTACK].pAttackData->fComboRate = 0.85f;
	// 地上ヒットと空中ヒットで挙動が変わるもの
	m_ActionDatas[(int)BASE_ACTION_STATE::DOKKOI_ATTACK].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].bBeInvincible = false;
	m_ActionDatas[(int)BASE_ACTION_STATE::DOKKOI_ATTACK].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].bBeInvincible = false;
	m_ActionDatas[(int)BASE_ACTION_STATE::DOKKOI_ATTACK].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].FlyVector.Set(.5f, .25f);
	m_ActionDatas[(int)BASE_ACTION_STATE::DOKKOI_ATTACK].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].FlyVector.Set(.5f, 1.0f);
	m_ActionDatas[(int)BASE_ACTION_STATE::DOKKOI_ATTACK].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].hitStopFlame = 16;
	m_ActionDatas[(int)BASE_ACTION_STATE::DOKKOI_ATTACK].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].hitStopFlame = 16;
	m_ActionDatas[(int)BASE_ACTION_STATE::DOKKOI_ATTACK].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].HitRecoveryFrame = 55;
	m_ActionDatas[(int)BASE_ACTION_STATE::DOKKOI_ATTACK].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].HitRecoveryFrame = 55;
	m_ActionDatas[(int)BASE_ACTION_STATE::DOKKOI_ATTACK].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].DamageMotion = DAMAGE_MOTION::KNOCK_BACK;
	m_ActionDatas[(int)BASE_ACTION_STATE::DOKKOI_ATTACK].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].DamageMotion = DAMAGE_MOTION::KNOCK_DOWN;
	// 判定形状
	m_ActionDatas[(int)BASE_ACTION_STATE::DOKKOI_ATTACK].pAttackData->pCollisionShape->width = 7;
	m_ActionDatas[(int)BASE_ACTION_STATE::DOKKOI_ATTACK].pAttackData->pCollisionShape->height = 8;
	m_ActionDatas[(int)BASE_ACTION_STATE::DOKKOI_ATTACK].pAttackData->pCollisionShape->pos.Set(7, 5.5, 0);


	//==============================================================================================================
	//	しゃがみ下段攻撃
	// 地上ヒットも空中ヒットも共通の情報
	m_ActionDatas[(int)BASE_ACTION_STATE::SQUAT_ATTACK].InstanceAttackData();	// アタックデータ作成
	m_ActionDatas[(int)BASE_ACTION_STATE::SQUAT_ATTACK].pAttackData->damage = 300;
	m_ActionDatas[(int)BASE_ACTION_STATE::SQUAT_ATTACK].pAttackData->pierceLV = 0;
	m_ActionDatas[(int)BASE_ACTION_STATE::SQUAT_ATTACK].pAttackData->HitSE = "ヒット4";
	m_ActionDatas[(int)BASE_ACTION_STATE::SQUAT_ATTACK].pAttackData->WhiffSE = "空振り5";
	m_ActionDatas[(int)BASE_ACTION_STATE::SQUAT_ATTACK].pAttackData->HitEffectType = EFFECT_TYPE::DAMAGE;
	m_ActionDatas[(int)BASE_ACTION_STATE::SQUAT_ATTACK].pAttackData->WhiffEffectType = EFFECT_TYPE::WHIFF;
	m_ActionDatas[(int)BASE_ACTION_STATE::SQUAT_ATTACK].pAttackData->bAntiAir = false;
	m_ActionDatas[(int)BASE_ACTION_STATE::SQUAT_ATTACK].pAttackData->bFinish = true;
	m_ActionDatas[(int)BASE_ACTION_STATE::SQUAT_ATTACK].pAttackData->AntiGuard = ANTIGUARD_ATTACK::DOWN_ATTACK;	// ★立ちガードをつぶす
	m_ActionDatas[(int)BASE_ACTION_STATE::SQUAT_ATTACK].pAttackData->ShakeCameraInfo.Set(.1f, 2);
	m_ActionDatas[(int)BASE_ACTION_STATE::SQUAT_ATTACK].pAttackData->GuardRecoveryFrame = 20;
	m_ActionDatas[(int)BASE_ACTION_STATE::SQUAT_ATTACK].pAttackData->fGuardKnockBackPower = .3f;
	m_ActionDatas[(int)BASE_ACTION_STATE::SQUAT_ATTACK].pAttackData->fComboRate = 0.85f;
	// 地上ヒットと空中ヒットで挙動が変わるもの
	m_ActionDatas[(int)BASE_ACTION_STATE::SQUAT_ATTACK].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].bBeInvincible = false;
	m_ActionDatas[(int)BASE_ACTION_STATE::SQUAT_ATTACK].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].bBeInvincible = false;
	m_ActionDatas[(int)BASE_ACTION_STATE::SQUAT_ATTACK].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].FlyVector.Set(.25f, .25f);
	m_ActionDatas[(int)BASE_ACTION_STATE::SQUAT_ATTACK].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].FlyVector.Set(.1f, .25f);
	m_ActionDatas[(int)BASE_ACTION_STATE::SQUAT_ATTACK].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].hitStopFlame = 7;
	m_ActionDatas[(int)BASE_ACTION_STATE::SQUAT_ATTACK].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].hitStopFlame = 7;
	m_ActionDatas[(int)BASE_ACTION_STATE::SQUAT_ATTACK].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].HitRecoveryFrame = 26;
	m_ActionDatas[(int)BASE_ACTION_STATE::SQUAT_ATTACK].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].HitRecoveryFrame = 48;
	m_ActionDatas[(int)BASE_ACTION_STATE::SQUAT_ATTACK].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].DamageMotion = DAMAGE_MOTION::KNOCK_BACK;
	m_ActionDatas[(int)BASE_ACTION_STATE::SQUAT_ATTACK].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].DamageMotion = DAMAGE_MOTION::KNOCK_DOWN;
	// 判定形状
	m_ActionDatas[(int)BASE_ACTION_STATE::SQUAT_ATTACK].pAttackData->pCollisionShape->width = 6;
	m_ActionDatas[(int)BASE_ACTION_STATE::SQUAT_ATTACK].pAttackData->pCollisionShape->height = 3;
	m_ActionDatas[(int)BASE_ACTION_STATE::SQUAT_ATTACK].pAttackData->pCollisionShape->pos.Set(6, 2, 0);



	//==============================================================================================================
	//	足払い攻撃
	// 地上ヒットも空中ヒットも共通の情報
	m_ActionDatas[(int)BASE_ACTION_STATE::DOWN_ATTACK].InstanceAttackData();	// アタックデータ作成
	m_ActionDatas[(int)BASE_ACTION_STATE::DOWN_ATTACK].pAttackData->damage = 280;
	m_ActionDatas[(int)BASE_ACTION_STATE::DOWN_ATTACK].pAttackData->pierceLV = 0;
	m_ActionDatas[(int)BASE_ACTION_STATE::DOWN_ATTACK].pAttackData->HitSE = "ヒット4";
	m_ActionDatas[(int)BASE_ACTION_STATE::DOWN_ATTACK].pAttackData->WhiffSE = "空振り5";
	m_ActionDatas[(int)BASE_ACTION_STATE::DOWN_ATTACK].pAttackData->HitEffectType = EFFECT_TYPE::DAMAGE;
	m_ActionDatas[(int)BASE_ACTION_STATE::DOWN_ATTACK].pAttackData->WhiffEffectType = EFFECT_TYPE::WHIFF;
	m_ActionDatas[(int)BASE_ACTION_STATE::DOWN_ATTACK].pAttackData->bAntiAir = false;
	m_ActionDatas[(int)BASE_ACTION_STATE::DOWN_ATTACK].pAttackData->bFinish = true;
	m_ActionDatas[(int)BASE_ACTION_STATE::DOWN_ATTACK].pAttackData->AntiGuard = ANTIGUARD_ATTACK::DOWN_ATTACK;	// ★立ちガードをつぶす
	m_ActionDatas[(int)BASE_ACTION_STATE::DOWN_ATTACK].pAttackData->ShakeCameraInfo.Set(.1f, 2);
	m_ActionDatas[(int)BASE_ACTION_STATE::DOWN_ATTACK].pAttackData->GuardRecoveryFrame = 20;
	m_ActionDatas[(int)BASE_ACTION_STATE::DOWN_ATTACK].pAttackData->fGuardKnockBackPower = .3f;
	m_ActionDatas[(int)BASE_ACTION_STATE::DOWN_ATTACK].pAttackData->fComboRate = 0.85f;
	// 地上ヒットと空中ヒットで挙動が変わるもの
	m_ActionDatas[(int)BASE_ACTION_STATE::DOWN_ATTACK].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].bBeInvincible = false;
	m_ActionDatas[(int)BASE_ACTION_STATE::DOWN_ATTACK].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].bBeInvincible = false;
	m_ActionDatas[(int)BASE_ACTION_STATE::DOWN_ATTACK].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].FlyVector.Set(.35f, .65f);
	m_ActionDatas[(int)BASE_ACTION_STATE::DOWN_ATTACK].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].FlyVector.Set(.25f, .75f);
	m_ActionDatas[(int)BASE_ACTION_STATE::DOWN_ATTACK].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].hitStopFlame = 11;
	m_ActionDatas[(int)BASE_ACTION_STATE::DOWN_ATTACK].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].hitStopFlame = 11;
	m_ActionDatas[(int)BASE_ACTION_STATE::DOWN_ATTACK].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].HitRecoveryFrame = 32;
	m_ActionDatas[(int)BASE_ACTION_STATE::DOWN_ATTACK].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].HitRecoveryFrame = 55;
	m_ActionDatas[(int)BASE_ACTION_STATE::DOWN_ATTACK].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].DamageMotion = DAMAGE_MOTION::DOWN_FALL;
	m_ActionDatas[(int)BASE_ACTION_STATE::DOWN_ATTACK].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].DamageMotion = DAMAGE_MOTION::DOWN_FALL;
	// 判定形状
	m_ActionDatas[(int)BASE_ACTION_STATE::DOWN_ATTACK].pAttackData->pCollisionShape->width = 8;
	m_ActionDatas[(int)BASE_ACTION_STATE::DOWN_ATTACK].pAttackData->pCollisionShape->height = 3;
	m_ActionDatas[(int)BASE_ACTION_STATE::DOWN_ATTACK].pAttackData->pCollisionShape->pos.Set(8, 2, 0);


	//==============================================================================================================
	//	空中攻撃
	// 地上ヒットも空中ヒットも共通の情報
	m_ActionDatas[(int)BASE_ACTION_STATE::AERIAL].InstanceAttackData();	// アタックデータ作成
	m_ActionDatas[(int)BASE_ACTION_STATE::AERIAL].pAttackData->damage = 300;
	m_ActionDatas[(int)BASE_ACTION_STATE::AERIAL].pAttackData->pierceLV = 0;
	m_ActionDatas[(int)BASE_ACTION_STATE::AERIAL].pAttackData->HitSE = "ヒット5";
	m_ActionDatas[(int)BASE_ACTION_STATE::AERIAL].pAttackData->WhiffSE = "空振り5";
	m_ActionDatas[(int)BASE_ACTION_STATE::AERIAL].pAttackData->HitEffectType = EFFECT_TYPE::DAMAGE;
	m_ActionDatas[(int)BASE_ACTION_STATE::AERIAL].pAttackData->WhiffEffectType = EFFECT_TYPE::WHIFF;
	m_ActionDatas[(int)BASE_ACTION_STATE::AERIAL].pAttackData->bAntiAir = false;
	m_ActionDatas[(int)BASE_ACTION_STATE::AERIAL].pAttackData->bFinish = true;
	m_ActionDatas[(int)BASE_ACTION_STATE::AERIAL].pAttackData->AntiGuard = ANTIGUARD_ATTACK::UP_ATTACK;		// ★しゃがみガードをつぶす
	m_ActionDatas[(int)BASE_ACTION_STATE::AERIAL].pAttackData->ShakeCameraInfo.Set(.1f, 3);
	m_ActionDatas[(int)BASE_ACTION_STATE::AERIAL].pAttackData->GuardRecoveryFrame = 20;
	m_ActionDatas[(int)BASE_ACTION_STATE::AERIAL].pAttackData->fGuardKnockBackPower = .2f;
	m_ActionDatas[(int)BASE_ACTION_STATE::AERIAL].pAttackData->fComboRate = 0.85f;
	// 地上ヒットと空中ヒットで挙動が変わるもの
	m_ActionDatas[(int)BASE_ACTION_STATE::AERIAL].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].bBeInvincible = false;
	m_ActionDatas[(int)BASE_ACTION_STATE::AERIAL].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].bBeInvincible = false;
	m_ActionDatas[(int)BASE_ACTION_STATE::AERIAL].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].FlyVector.Set(.5f, .0f);
	m_ActionDatas[(int)BASE_ACTION_STATE::AERIAL].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].FlyVector.Set(.35f, 1.75f);
	m_ActionDatas[(int)BASE_ACTION_STATE::AERIAL].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].hitStopFlame = 7;
	m_ActionDatas[(int)BASE_ACTION_STATE::AERIAL].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].hitStopFlame = 7;
	m_ActionDatas[(int)BASE_ACTION_STATE::AERIAL].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].HitRecoveryFrame = 35;
	m_ActionDatas[(int)BASE_ACTION_STATE::AERIAL].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].HitRecoveryFrame = 50;
	m_ActionDatas[(int)BASE_ACTION_STATE::AERIAL].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].DamageMotion = DAMAGE_MOTION::KNOCK_BACK;
	m_ActionDatas[(int)BASE_ACTION_STATE::AERIAL].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].DamageMotion = DAMAGE_MOTION::KNOCK_DOWN;
	// 判定形状
	m_ActionDatas[(int)BASE_ACTION_STATE::AERIAL].pAttackData->pCollisionShape->width = 11;
	m_ActionDatas[(int)BASE_ACTION_STATE::AERIAL].pAttackData->pCollisionShape->height = 7;
	m_ActionDatas[(int)BASE_ACTION_STATE::AERIAL].pAttackData->pCollisionShape->pos.Set(6.5f, 6, 0);

	//==============================================================================================================
	//	逆切れ攻撃
	// 地上ヒットも空中ヒットも共通の情報
	m_ActionDatas[(int)BASE_ACTION_STATE::INVINCIBLE_ATTACK].InstanceAttackData();	// アタックデータ作成
	m_ActionDatas[(int)BASE_ACTION_STATE::INVINCIBLE_ATTACK].pAttackData->damage = 514;
	m_ActionDatas[(int)BASE_ACTION_STATE::INVINCIBLE_ATTACK].pAttackData->pierceLV = 0;
	m_ActionDatas[(int)BASE_ACTION_STATE::INVINCIBLE_ATTACK].pAttackData->HitSE = "フィニッシュ小";
	m_ActionDatas[(int)BASE_ACTION_STATE::INVINCIBLE_ATTACK].pAttackData->WhiffSE = "空振り5";
	m_ActionDatas[(int)BASE_ACTION_STATE::INVINCIBLE_ATTACK].pAttackData->HitEffectType = EFFECT_TYPE::DAMAGE;
	m_ActionDatas[(int)BASE_ACTION_STATE::INVINCIBLE_ATTACK].pAttackData->WhiffEffectType = EFFECT_TYPE::WHIFF;
	m_ActionDatas[(int)BASE_ACTION_STATE::INVINCIBLE_ATTACK].pAttackData->bAntiAir = false;
	m_ActionDatas[(int)BASE_ACTION_STATE::INVINCIBLE_ATTACK].pAttackData->bFinish = true;
	m_ActionDatas[(int)BASE_ACTION_STATE::INVINCIBLE_ATTACK].pAttackData->AntiGuard = ANTIGUARD_ATTACK::NONE;
	m_ActionDatas[(int)BASE_ACTION_STATE::INVINCIBLE_ATTACK].pAttackData->ShakeCameraInfo.Set(1.0f, 20);
	m_ActionDatas[(int)BASE_ACTION_STATE::INVINCIBLE_ATTACK].pAttackData->GuardRecoveryFrame = 25;
	m_ActionDatas[(int)BASE_ACTION_STATE::INVINCIBLE_ATTACK].pAttackData->fComboRate = 0.85f;
	// 地上ヒットと空中ヒットで挙動が変わるもの
	m_ActionDatas[(int)BASE_ACTION_STATE::INVINCIBLE_ATTACK].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].bBeInvincible = true;
	m_ActionDatas[(int)BASE_ACTION_STATE::INVINCIBLE_ATTACK].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].bBeInvincible = true;
	m_ActionDatas[(int)BASE_ACTION_STATE::INVINCIBLE_ATTACK].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].FlyVector.Set(3.0f, 2.5f);
	m_ActionDatas[(int)BASE_ACTION_STATE::INVINCIBLE_ATTACK].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].FlyVector.Set(3.5f, 2.5f);
	m_ActionDatas[(int)BASE_ACTION_STATE::INVINCIBLE_ATTACK].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].hitStopFlame = 25;
	m_ActionDatas[(int)BASE_ACTION_STATE::INVINCIBLE_ATTACK].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].hitStopFlame = 25;
	m_ActionDatas[(int)BASE_ACTION_STATE::INVINCIBLE_ATTACK].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].HitRecoveryFrame = 60;
	m_ActionDatas[(int)BASE_ACTION_STATE::INVINCIBLE_ATTACK].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].HitRecoveryFrame = 60;
	m_ActionDatas[(int)BASE_ACTION_STATE::INVINCIBLE_ATTACK].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].DamageMotion = DAMAGE_MOTION::KNOCK_DOWN;
	m_ActionDatas[(int)BASE_ACTION_STATE::INVINCIBLE_ATTACK].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].DamageMotion = DAMAGE_MOTION::KNOCK_DOWN;
	// 判定形状
	m_ActionDatas[(int)BASE_ACTION_STATE::INVINCIBLE_ATTACK].pAttackData->pCollisionShape->width = 22;
	m_ActionDatas[(int)BASE_ACTION_STATE::INVINCIBLE_ATTACK].pAttackData->pCollisionShape->height = 6.5f;
	m_ActionDatas[(int)BASE_ACTION_STATE::INVINCIBLE_ATTACK].pAttackData->pCollisionShape->pos.Set(0, 4.5f, 0);


	//==============================================================================================================
	//	スキル攻撃(キルラッシュ)
	// 地上ヒットも空中ヒットも共通の情報
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL].InstanceAttackData();	// アタックデータ作成
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL].pAttackData->damage = 180;
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL].pAttackData->pierceLV = 0;
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL].pAttackData->HitSE = "ヒット6";
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL].pAttackData->WhiffSE = "空振り5";
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL].pAttackData->HitEffectType = EFFECT_TYPE::DAMAGE;
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL].pAttackData->WhiffEffectType = EFFECT_TYPE::WHIFF;
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL].pAttackData->bAntiAir = false;
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL].pAttackData->bFinish = true;
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL].pAttackData->AntiGuard = ANTIGUARD_ATTACK::NONE;
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL].pAttackData->ShakeCameraInfo.Set(.2f, 3);
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL].pAttackData->GuardRecoveryFrame = 18;
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL].pAttackData->fGuardKnockBackPower = .1f;
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL].pAttackData->fComboRate = 0.85f;
	// 地上ヒットと空中ヒットで挙動が変わるもの
	// [メモ] アイル―は最後吹き飛ばす　棒人間は三段目ものけぞりで隙が短い
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].bBeInvincible = false;
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].bBeInvincible = false;
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].FlyVector.Set(.1f, .0f);
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].FlyVector.Set(.1f, .0f);
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].hitStopFlame = 1;
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].hitStopFlame = 2;
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].HitRecoveryFrame = 50;
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].HitRecoveryFrame = 50;
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].DamageMotion = DAMAGE_MOTION::KNOCK_BACK;
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].DamageMotion = DAMAGE_MOTION::KNOCK_DOWN;
	// 判定形状
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL].pAttackData->pCollisionShape->width = 6;
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL].pAttackData->pCollisionShape->height = 5;
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL].pAttackData->pCollisionShape->pos.Set(5, 5, 0);


	//==============================================================================================================
	//	第2キャラクター固有スキル(コークスクリュー)
	// 地上ヒットも空中ヒットも共通の情報
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL2].InstanceAttackData();	// アタックデータ作成
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL2].pAttackData->damage = 400;
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL2].pAttackData->pierceLV = 0;
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL2].pAttackData->HitSE = "ヒット5";
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL2].pAttackData->WhiffSE = "空振り5";
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL2].pAttackData->HitEffectType = EFFECT_TYPE::DAMAGE;
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL2].pAttackData->WhiffEffectType = EFFECT_TYPE::WHIFF;
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL2].pAttackData->bAntiAir = false;
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL2].pAttackData->bFinish = true;
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL2].pAttackData->AntiGuard = ANTIGUARD_ATTACK::NONE;
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL2].pAttackData->ShakeCameraInfo.Set(.1f, 2);
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL2].pAttackData->GuardRecoveryFrame = 15;
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL2].pAttackData->fComboRate = 0.85f;
	// 地上ヒットと空中ヒットで挙動が変わるもの
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL2].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].bBeInvincible = false;
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL2].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].bBeInvincible = false;
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL2].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].FlyVector.Set(4.1f, .25f);
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL2].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].FlyVector.Set(4.1f, .5f);
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL2].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].hitStopFlame = 8;
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL2].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].hitStopFlame = 5;
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL2].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].HitRecoveryFrame = 40;
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL2].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].HitRecoveryFrame = 40;
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL2].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].DamageMotion = DAMAGE_MOTION::KNOCK_DOWN;
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL2].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].DamageMotion = DAMAGE_MOTION::KNOCK_DOWN;

	// 判定形状
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL2].pAttackData->pCollisionShape->width = 10;
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL2].pAttackData->pCollisionShape->height = 6;
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL2].pAttackData->pCollisionShape->pos.Set(10, 6, 0);


	//==============================================================================================================
	//	掴み
	// 地上ヒットも空中ヒットも共通の情報
	m_ActionDatas[(int)BASE_ACTION_STATE::THROW].InstanceAttackData();	// アタックデータ作成
	m_ActionDatas[(int)BASE_ACTION_STATE::THROW].pAttackData->damage = 0;
	m_ActionDatas[(int)BASE_ACTION_STATE::THROW].pAttackData->pierceLV = 0;
	m_ActionDatas[(int)BASE_ACTION_STATE::THROW].pAttackData->HitSE = "掴み成功";
	m_ActionDatas[(int)BASE_ACTION_STATE::THROW].pAttackData->WhiffSE = nullptr;
	m_ActionDatas[(int)BASE_ACTION_STATE::THROW].pAttackData->HitEffectType = EFFECT_TYPE::DAMAGE;
	m_ActionDatas[(int)BASE_ACTION_STATE::THROW].pAttackData->WhiffEffectType = EFFECT_TYPE::WHIFF;
	m_ActionDatas[(int)BASE_ACTION_STATE::THROW].pAttackData->bAntiAir = false;
	m_ActionDatas[(int)BASE_ACTION_STATE::THROW].pAttackData->bFinish = false;
	m_ActionDatas[(int)BASE_ACTION_STATE::THROW].pAttackData->AntiGuard = ANTIGUARD_ATTACK::ALL_BREAK;
	m_ActionDatas[(int)BASE_ACTION_STATE::THROW].pAttackData->ShakeCameraInfo.Set(0, 0);
	m_ActionDatas[(int)BASE_ACTION_STATE::THROW].pAttackData->GuardRecoveryFrame = 0;
	m_ActionDatas[(int)BASE_ACTION_STATE::THROW].pAttackData->attribute = ATTACK_ATTRIBUTE::THROW;
	// 地上ヒットと空中ヒットで挙動が変わるもの
	m_ActionDatas[(int)BASE_ACTION_STATE::THROW].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].bBeInvincible = false;
	m_ActionDatas[(int)BASE_ACTION_STATE::THROW].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].FlyVector.Set(0, 0);
	m_ActionDatas[(int)BASE_ACTION_STATE::THROW].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].hitStopFlame = c_THROW_ESCAPE_FRAME;
	m_ActionDatas[(int)BASE_ACTION_STATE::THROW].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].HitRecoveryFrame = 60;

	// 判定形状
	m_ActionDatas[(int)BASE_ACTION_STATE::THROW].pAttackData->pCollisionShape->width = 4;
	m_ActionDatas[(int)BASE_ACTION_STATE::THROW].pAttackData->pCollisionShape->height = 4;
	m_ActionDatas[(int)BASE_ACTION_STATE::THROW].pAttackData->pCollisionShape->pos.Set(2, 8, 0);

	//==============================================================================================================
	//	投げ成功
	// 地上ヒットも空中ヒットも共通の情報
	m_ActionDatas[(int)BASE_ACTION_STATE::THROW_SUCCESS].InstanceAttackData();	// アタックデータ作成
	m_ActionDatas[(int)BASE_ACTION_STATE::THROW_SUCCESS].pAttackData->damage = 600;
	m_ActionDatas[(int)BASE_ACTION_STATE::THROW_SUCCESS].pAttackData->pierceLV = 0;
	m_ActionDatas[(int)BASE_ACTION_STATE::THROW_SUCCESS].pAttackData->HitSE = "ヒット3";
	m_ActionDatas[(int)BASE_ACTION_STATE::THROW_SUCCESS].pAttackData->WhiffSE = nullptr;
	m_ActionDatas[(int)BASE_ACTION_STATE::THROW_SUCCESS].pAttackData->HitEffectType = EFFECT_TYPE::DAMAGE;
	m_ActionDatas[(int)BASE_ACTION_STATE::THROW_SUCCESS].pAttackData->WhiffEffectType = EFFECT_TYPE::WHIFF;
	m_ActionDatas[(int)BASE_ACTION_STATE::THROW_SUCCESS].pAttackData->bAntiAir = false;
	m_ActionDatas[(int)BASE_ACTION_STATE::THROW_SUCCESS].pAttackData->bFinish = true;
	m_ActionDatas[(int)BASE_ACTION_STATE::THROW_SUCCESS].pAttackData->AntiGuard = ANTIGUARD_ATTACK::NONE;
	m_ActionDatas[(int)BASE_ACTION_STATE::THROW_SUCCESS].pAttackData->ShakeCameraInfo.Set(.1f, 2);
	m_ActionDatas[(int)BASE_ACTION_STATE::THROW_SUCCESS].pAttackData->GuardRecoveryFrame = 0;
	m_ActionDatas[(int)BASE_ACTION_STATE::THROW_SUCCESS].pAttackData->fComboRate = 0.85f;
	// 地上ヒットと空中ヒットで挙動が変わるもの
	m_ActionDatas[(int)BASE_ACTION_STATE::THROW_SUCCESS].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].bBeInvincible = false;
	m_ActionDatas[(int)BASE_ACTION_STATE::THROW_SUCCESS].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].bBeInvincible = false;
	m_ActionDatas[(int)BASE_ACTION_STATE::THROW_SUCCESS].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].FlyVector.Set(1.2f, 2.4f);
	m_ActionDatas[(int)BASE_ACTION_STATE::THROW_SUCCESS].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].FlyVector.Set(1.3f, 2.5f);
	m_ActionDatas[(int)BASE_ACTION_STATE::THROW_SUCCESS].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].hitStopFlame = 1;
	m_ActionDatas[(int)BASE_ACTION_STATE::THROW_SUCCESS].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].hitStopFlame = 1;
	m_ActionDatas[(int)BASE_ACTION_STATE::THROW_SUCCESS].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].HitRecoveryFrame = 60;
	m_ActionDatas[(int)BASE_ACTION_STATE::THROW_SUCCESS].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].HitRecoveryFrame = 60;
	m_ActionDatas[(int)BASE_ACTION_STATE::THROW_SUCCESS].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].DamageMotion = DAMAGE_MOTION::KNOCK_DOWN;
	m_ActionDatas[(int)BASE_ACTION_STATE::THROW_SUCCESS].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].DamageMotion = DAMAGE_MOTION::KNOCK_DOWN;

	// 判定形状
	m_ActionDatas[(int)BASE_ACTION_STATE::THROW_SUCCESS].pAttackData->pCollisionShape->width = 16;
	m_ActionDatas[(int)BASE_ACTION_STATE::THROW_SUCCESS].pAttackData->pCollisionShape->height = 11;
	m_ActionDatas[(int)BASE_ACTION_STATE::THROW_SUCCESS].pAttackData->pCollisionShape->pos.Set(0, 4, 0);


	//==============================================================================================================
	//	ヒーホードライブ
	// 地上ヒットも空中ヒットも共通の情報
	m_ActionDatas[(int)BASE_ACTION_STATE::HEAVEHO_DRIVE].InstanceAttackData();	// アタックデータ作成
	m_ActionDatas[(int)BASE_ACTION_STATE::HEAVEHO_DRIVE].pAttackData->damage = 1700;
	m_ActionDatas[(int)BASE_ACTION_STATE::HEAVEHO_DRIVE].pAttackData->pierceLV = 0;
	m_ActionDatas[(int)BASE_ACTION_STATE::HEAVEHO_DRIVE].pAttackData->HitSE = "フィニッシュ大";
	m_ActionDatas[(int)BASE_ACTION_STATE::HEAVEHO_DRIVE].pAttackData->WhiffSE = "空振り5";
	m_ActionDatas[(int)BASE_ACTION_STATE::HEAVEHO_DRIVE].pAttackData->HitEffectType = EFFECT_TYPE::FINISH_HIT;
	m_ActionDatas[(int)BASE_ACTION_STATE::HEAVEHO_DRIVE].pAttackData->WhiffEffectType = EFFECT_TYPE::WHIFF;
	m_ActionDatas[(int)BASE_ACTION_STATE::HEAVEHO_DRIVE].pAttackData->bAntiAir = false;
	m_ActionDatas[(int)BASE_ACTION_STATE::HEAVEHO_DRIVE].pAttackData->bFinish = true;
	m_ActionDatas[(int)BASE_ACTION_STATE::HEAVEHO_DRIVE].pAttackData->AntiGuard = ANTIGUARD_ATTACK::NONE;
	m_ActionDatas[(int)BASE_ACTION_STATE::HEAVEHO_DRIVE].pAttackData->ShakeCameraInfo.Set(1.0f, 20);
	m_ActionDatas[(int)BASE_ACTION_STATE::HEAVEHO_DRIVE].pAttackData->GuardRecoveryFrame = 18;
	m_ActionDatas[(int)BASE_ACTION_STATE::HEAVEHO_DRIVE].pAttackData->attribute = ATTACK_ATTRIBUTE::BULLET;	// 弾属性
	m_ActionDatas[(int)BASE_ACTION_STATE::HEAVEHO_DRIVE].pAttackData->fComboRate = 0.85f;
	// 地上ヒットと空中ヒットで挙動が変わるもの
	m_ActionDatas[(int)BASE_ACTION_STATE::HEAVEHO_DRIVE].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].bBeInvincible = true;
	m_ActionDatas[(int)BASE_ACTION_STATE::HEAVEHO_DRIVE].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].bBeInvincible = true;
	m_ActionDatas[(int)BASE_ACTION_STATE::HEAVEHO_DRIVE].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].FlyVector.Set(1.0f, 4.5f);
	m_ActionDatas[(int)BASE_ACTION_STATE::HEAVEHO_DRIVE].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].FlyVector.Set(1.0f, 4.5f);
	m_ActionDatas[(int)BASE_ACTION_STATE::HEAVEHO_DRIVE].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].hitStopFlame = 12;
	m_ActionDatas[(int)BASE_ACTION_STATE::HEAVEHO_DRIVE].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].hitStopFlame = 12;
	m_ActionDatas[(int)BASE_ACTION_STATE::HEAVEHO_DRIVE].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].HitRecoveryFrame = 100;
	m_ActionDatas[(int)BASE_ACTION_STATE::HEAVEHO_DRIVE].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].HitRecoveryFrame = 100;
	m_ActionDatas[(int)BASE_ACTION_STATE::HEAVEHO_DRIVE].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].DamageMotion = DAMAGE_MOTION::KNOCK_DOWN;
	m_ActionDatas[(int)BASE_ACTION_STATE::HEAVEHO_DRIVE].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].DamageMotion = DAMAGE_MOTION::KNOCK_DOWN;
	// 判定形状
	m_ActionDatas[(int)BASE_ACTION_STATE::HEAVEHO_DRIVE].pAttackData->pCollisionShape->width = 12;
	m_ActionDatas[(int)BASE_ACTION_STATE::HEAVEHO_DRIVE].pAttackData->pCollisionShape->height = 16;
	m_ActionDatas[(int)BASE_ACTION_STATE::HEAVEHO_DRIVE].pAttackData->pCollisionShape->pos.Set(12, 16, 0);

	//==============================================================================================================
	//	ヒーホードライブ_オーバーフロー
	// 地上ヒットも空中ヒットも共通の情報
	m_ActionDatas[(int)BASE_ACTION_STATE::HEAVEHO_DRIVE_OVERFLOW].InstanceAttackData();	// アタックデータ作成
	m_ActionDatas[(int)BASE_ACTION_STATE::HEAVEHO_DRIVE_OVERFLOW].pAttackData->damage = 514;
	m_ActionDatas[(int)BASE_ACTION_STATE::HEAVEHO_DRIVE_OVERFLOW].pAttackData->pierceLV = 0;
	m_ActionDatas[(int)BASE_ACTION_STATE::HEAVEHO_DRIVE_OVERFLOW].pAttackData->HitSE = "フィニッシュ大";
	m_ActionDatas[(int)BASE_ACTION_STATE::HEAVEHO_DRIVE_OVERFLOW].pAttackData->WhiffSE = "空振り5";
	m_ActionDatas[(int)BASE_ACTION_STATE::HEAVEHO_DRIVE_OVERFLOW].pAttackData->HitEffectType = EFFECT_TYPE::FINISH_HIT;
	m_ActionDatas[(int)BASE_ACTION_STATE::HEAVEHO_DRIVE_OVERFLOW].pAttackData->WhiffEffectType = EFFECT_TYPE::WHIFF;
	m_ActionDatas[(int)BASE_ACTION_STATE::HEAVEHO_DRIVE_OVERFLOW].pAttackData->bAntiAir = false;
	m_ActionDatas[(int)BASE_ACTION_STATE::HEAVEHO_DRIVE_OVERFLOW].pAttackData->bFinish = false;
	m_ActionDatas[(int)BASE_ACTION_STATE::HEAVEHO_DRIVE_OVERFLOW].pAttackData->AntiGuard = ANTIGUARD_ATTACK::UP_ATTACK;	// 中断攻撃
	m_ActionDatas[(int)BASE_ACTION_STATE::HEAVEHO_DRIVE_OVERFLOW].pAttackData->ShakeCameraInfo.Set(0, 0);
	m_ActionDatas[(int)BASE_ACTION_STATE::HEAVEHO_DRIVE_OVERFLOW].pAttackData->GuardRecoveryFrame = 18;
	m_ActionDatas[(int)BASE_ACTION_STATE::HEAVEHO_DRIVE_OVERFLOW].pAttackData->fComboRate = 0.85f;
	// 地上ヒットと空中ヒットで挙動が変わるもの
	m_ActionDatas[(int)BASE_ACTION_STATE::HEAVEHO_DRIVE_OVERFLOW].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].bBeInvincible = true;
	m_ActionDatas[(int)BASE_ACTION_STATE::HEAVEHO_DRIVE_OVERFLOW].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].bBeInvincible = true;
	m_ActionDatas[(int)BASE_ACTION_STATE::HEAVEHO_DRIVE_OVERFLOW].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].FlyVector.Set(3.0f, 2.5f);
	m_ActionDatas[(int)BASE_ACTION_STATE::HEAVEHO_DRIVE_OVERFLOW].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].FlyVector.Set(3.5f, 2.5f);
	m_ActionDatas[(int)BASE_ACTION_STATE::HEAVEHO_DRIVE_OVERFLOW].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].hitStopFlame = 25;
	m_ActionDatas[(int)BASE_ACTION_STATE::HEAVEHO_DRIVE_OVERFLOW].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].hitStopFlame = 25;
	m_ActionDatas[(int)BASE_ACTION_STATE::HEAVEHO_DRIVE_OVERFLOW].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].HitRecoveryFrame = 1000;
	m_ActionDatas[(int)BASE_ACTION_STATE::HEAVEHO_DRIVE_OVERFLOW].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].HitRecoveryFrame = 1000;
	m_ActionDatas[(int)BASE_ACTION_STATE::HEAVEHO_DRIVE_OVERFLOW].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].DamageMotion = DAMAGE_MOTION::KNOCK_BACK;
	m_ActionDatas[(int)BASE_ACTION_STATE::HEAVEHO_DRIVE_OVERFLOW].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].DamageMotion = DAMAGE_MOTION::KNOCK_BACK;
	// 判定形状
	m_ActionDatas[(int)BASE_ACTION_STATE::HEAVEHO_DRIVE_OVERFLOW].pAttackData->pCollisionShape->width = 12;
	m_ActionDatas[(int)BASE_ACTION_STATE::HEAVEHO_DRIVE_OVERFLOW].pAttackData->pCollisionShape->height = 12;
	m_ActionDatas[(int)BASE_ACTION_STATE::HEAVEHO_DRIVE_OVERFLOW].pAttackData->pCollisionShape->pos.Set(0, 0, 0);

	//==============================================================================================================
	//	全共通
	FOR((int)BASE_ACTION_STATE::END)
	{
		if (m_ActionDatas[i].isAttackData())
		{
			// ★引き寄せ効果のある技がほしいのでコメントアウト
			//for (int place = 0; place < (int)AttackData::HIT_PLACE::MAX;place++) MyAssert((m_ActionDatas[i].pAttackData->places[place].FlyVector.x >= 0), "Xの値が-になることは絶対ない");

			MyAssert((m_ActionDatas[i].pAttackData->pCollisionShape->pos.x >= 0), "Xの値が-になることは絶対ない");
			m_ActionDatas[i].pAttackData->bHit = false;
			// 判定発動した瞬間に空振りSEを再生してみる
			for (int frame = 0;; frame++)if (m_ActionFrameList[i][frame] == FRAME_STATE::ACTIVE) { m_ActionDatas[i].pAttackData->WhiffDelayFrame = frame; break; }
		}
	}


	// スキルアクションの初期化(★★★ここに書くのは、この中でAttackDataを参照するから)
	m_pSkillActions[(int)SKILL_ACTION_TYPE::LAND] = new SkillAction::Land(this);
	m_pSkillActions[(int)SKILL_ACTION_TYPE::LAND2] = new SkillAction::Land2(this);
	m_pSkillActions[(int)SKILL_ACTION_TYPE::SQUAT] = new SkillAction::Squat(this);
	m_pSkillActions[(int)SKILL_ACTION_TYPE::AERIAL] = new SkillAction::Land(this);
	m_pSkillActions[(int)SKILL_ACTION_TYPE::AERIALDROP] = new SkillAction::Land(this);
}

Teki::~Teki()
{
	FOR((int)SKILL_ACTION_TYPE::MAX) SAFE_DELETE(m_pSkillActions[i])
}

void Teki::Update(PLAYER_UPDATE flag)
{
	// 基底クラスの更新
	BasePlayer::Update(flag);
}

void Teki::Render()
{
	// 基底クラスの更新
	BasePlayer::Render();

}

void Teki::RenderDrive()
{
	// 基底クラスの更新
	BasePlayer::RenderDrive();
}

void Teki::InitMotionDatas()
{
	m_iMotionNumbers[(int)MOTION_TYPE::WAIT] = 0;
	m_iMotionNumbers[(int)MOTION_TYPE::WALK] = 1;
	m_iMotionNumbers[(int)MOTION_TYPE::RUN] = 2;
	m_iMotionNumbers[(int)MOTION_TYPE::BACK_WALK] = 3;
	m_iMotionNumbers[(int)MOTION_TYPE::BACK_STEP] = 4;
	m_iMotionNumbers[(int)MOTION_TYPE::RUSH_ATTACK1] = 5;
	m_iMotionNumbers[(int)MOTION_TYPE::RUSH_ATTACK2] = 6;
	m_iMotionNumbers[(int)MOTION_TYPE::RUSH_ATTACK3] = 7;
	m_iMotionNumbers[(int)MOTION_TYPE::SKILL2] = 8;
	m_iMotionNumbers[(int)MOTION_TYPE::FINISH_ATTACK] = 9;
	m_iMotionNumbers[(int)MOTION_TYPE::SQUAT] = 10;
	m_iMotionNumbers[(int)MOTION_TYPE::ANTI_AIR_ATTACK] = 11;
	m_iMotionNumbers[(int)MOTION_TYPE::DOKKOI_ATTACK] = 12;
	m_iMotionNumbers[(int)MOTION_TYPE::SQUAT_ATTACK] = 13;
	m_iMotionNumbers[(int)MOTION_TYPE::DOWN_ATTACK] = 14;
	m_iMotionNumbers[(int)MOTION_TYPE::JUMP] = 15;
	m_iMotionNumbers[(int)MOTION_TYPE::FALL] = 16;
	m_iMotionNumbers[(int)MOTION_TYPE::RAND] = 17;
	m_iMotionNumbers[(int)MOTION_TYPE::AERIAL_ATTACK] = 18;
	m_iMotionNumbers[(int)MOTION_TYPE::AERIAL_DASH] = 29;
	m_iMotionNumbers[(int)MOTION_TYPE::PERSONA] = 19;
	m_iMotionNumbers[(int)MOTION_TYPE::SKILL_LAND] = 21;
	m_iMotionNumbers[(int)MOTION_TYPE::SKILL_SQUAT] = 20;
	m_iMotionNumbers[(int)MOTION_TYPE::HEAVEHO_DRIVE] = 22;
	m_iMotionNumbers[(int)MOTION_TYPE::BURST] = 36;
	m_iMotionNumbers[(int)MOTION_TYPE::HEAVEHO_DRIVE_OVERFLOW] = 23;
	m_iMotionNumbers[(int)MOTION_TYPE::THROW] = 24;
	m_iMotionNumbers[(int)MOTION_TYPE::THROW_SUCCESS] = 25;
	m_iMotionNumbers[(int)MOTION_TYPE::THROW_DRAW] = 26;
	m_iMotionNumbers[(int)MOTION_TYPE::UP_GUARD] = 27;
	m_iMotionNumbers[(int)MOTION_TYPE::DOWN_GUARD] = 28;
	m_iMotionNumbers[(int)MOTION_TYPE::ESCAPE] = 29;
	m_iMotionNumbers[(int)MOTION_TYPE::KNOCKBACK] = 30;
	m_iMotionNumbers[(int)MOTION_TYPE::KNOCKDOWN] = 31;
	m_iMotionNumbers[(int)MOTION_TYPE::KNOCKDOWN_DOWN] = 32;
	m_iMotionNumbers[(int)MOTION_TYPE::REVERSAL] = 33;
	m_iMotionNumbers[(int)MOTION_TYPE::APPEAR] = 34;
	m_iMotionNumbers[(int)MOTION_TYPE::WIN] = 35;
}


void Teki::KakeaiInit()
{

}

void Teki::KakeaiUpdate()
{

}

void Teki::KakeaiExit()
{

}

void Teki::ThirdRushInit()
{
	// モーションに変える
	SetMotion(MOTION_TYPE::RUSH_ATTACK3);

	// アクションステート変更
	SetActionState(BASE_ACTION_STATE::SKILL);

	// アングルセット
	SetDirAngle();
}

void Teki::ThirdRushUpdate()
{
	if (!isAttackState()) return;

	// ヒットしてる状態
	if (GetAttackData()->bHit)
	{
		// キルラッシュ→コークスクリュー
		if (GetActionState() == BASE_ACTION_STATE::SKILL)
		{
			// 攻撃ボタン押したら
			if (isPushInputTRG(PLAYER_COMMAND_BIT::C, true) && GetActionFrame() == FRAME_STATE::FOLLOW)
			{
				// 先行入力リセット
				AheadCommandReset();

				// モーションに変える
				SetMotion(MOTION_TYPE::SKILL2);

				// アクションステート変更
				SetActionState(BASE_ACTION_STATE::SKILL2);
			}
		}
		else
		{
			// 横に移動
			if (isActiveFrame())
			{
				SetMove(Vector3(m_dir == DIR::LEFT ? -2.0f : 2.0f, 0, 0));
			}

			// 攻撃ボタン押したら
			if (isPushInputTRG(PLAYER_COMMAND_BIT::C, true))
			{
				// 先行入力リセット
				AheadCommandReset();

				// ゲージ50%以上で必殺技にキャンセルする
				// オーバードライブ中ならゲージなしで打てる
				if (isOverDrive())
				{
					// オーバードライブを終了して発動
					SetOverDriveFrame(0);
				}
				else if (GetOverDriveGage() < 50) return;

				// 50%消費
				AddOverDriveGage(-50);

				// 必殺ステートへ
				m_pStateMachine->ChangeState(BasePlayerState::HeavehoDrive::GetInstance());
			}
		}
	}
}

void Teki::ThirdRushExit()
{

}



void Teki::SkillInit()
{
	m_pSkillActions[(int)m_SkillActionType]->Enter();
}

bool Teki::SkillUpdate()
{
	const bool bEnd = m_pSkillActions[(int)m_SkillActionType]->Execute();
	return bEnd;
}

void Teki::SkillExit()
{
	m_pSkillActions[(int)m_SkillActionType]->Exit();
}


Teki::SkillAction::Base::Base(Teki *pTeki, BASE_ACTION_STATE state) :m_pTeki(pTeki)
{
	if (pTeki->m_ActionDatas[(int)state].isAttackData())
	{
		memcpy_s(&m_OrgHitSquare, sizeof(CollisionShape::Square), pTeki->m_ActionDatas[(int)state].pAttackData->pCollisionShape, sizeof(CollisionShape::Square));

		m_OrgDamage = pTeki->m_ActionDatas[(int)state].pAttackData->damage;
		m_fOrgComboRate = pTeki->m_ActionDatas[(int)state].pAttackData->fComboRate;
		m_OrgFlyVector[(int)AttackData::HIT_PLACE::LAND] = pTeki->m_ActionDatas[(int)state].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].FlyVector;
		m_OrgFlyVector[(int)AttackData::HIT_PLACE::AERIAL] = pTeki->m_ActionDatas[(int)state].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].FlyVector;
	}
}

Teki::SkillAction::Land::Land(Teki *pTeki) :Base(pTeki, BASE_ACTION_STATE::SKILL)
{}

void Teki::SkillAction::Land::Enter()
{
	// モーションに変える
	m_pTeki->SetMotion(MOTION_TYPE::SKILL_LAND);

	// アクションステート変更
	m_pTeki->SetActionState(BASE_ACTION_STATE::FRAMECOUNT);

	// 向き設定
	m_pTeki->SetDirAngle();

	// リセット
	m_iHitCount = 0;
}

bool Teki::SkillAction::Land::Execute()
{
	// アクション終了フラグ
	if (m_pTeki->GetActionState() == BASE_ACTION_STATE::NO_ACTION) return true;

	if (m_pTeki->GetActionState() == BASE_ACTION_STATE::SKILL)
	{
		// ヒット回数計測
		if (m_pTeki->m_ActionFrameList[(int)BASE_ACTION_STATE::SKILL][m_pTeki->m_iCurrentActionFrame] == FRAME_STATE::RECOVERY_HIT)
		{
			// 最後の一撃だけ強くする
			if (++m_iHitCount >= 3)
			{
				m_pTeki->m_ActionDatas[(int)BASE_ACTION_STATE::SKILL].pAttackData->damage = 400;
				m_pTeki->m_ActionDatas[(int)BASE_ACTION_STATE::SKILL].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].FlyVector.Set(1.5f, 0);
				m_pTeki->m_ActionDatas[(int)BASE_ACTION_STATE::SKILL].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].FlyVector.Set(1.5f, .5f);
			}

			// コンボレートを下げないようにする
			m_pTeki->m_ActionDatas[(int)BASE_ACTION_STATE::SKILL].pAttackData->fComboRate = 1.0f;
		}

		if (m_pTeki->isHitAttack() && m_pTeki->GetActionFrame() == FRAME_STATE::FOLLOW)
		{
			// 必殺技キャンセル
			if (HeaveHoCancel(m_pTeki)) return false;

			// コークスクリュールート
			if (m_pTeki->isPushInputTRG(PLAYER_COMMAND_BIT::D, true))
			{
				m_pTeki->SkillExit();
				m_pTeki->m_SkillActionType = SKILL_ACTION_TYPE::LAND2;
				m_pTeki->SkillInit();
				return false;
			}
		}
	}

	else
	{
		// 攻撃ステート終わったら
		if (m_pTeki->m_iCurrentActionFrame > 96)
		{
			return true;
		}

		// 走ってる間
		if (m_pTeki->m_iCurrentActionFrame < 57)
		{
			m_pTeki->SetMove(Vector3((m_pTeki->m_dir == DIR::LEFT) ? -m_pTeki->m_tagCharacterParam.fMaxSpeed : m_pTeki->m_tagCharacterParam.fMaxSpeed, 0, 0));

			// 接近したらキルラッシュモーション
			if (Math::Length(m_pTeki->GetPos(), m_pTeki->m_pTargetPlayer->GetPos()) < 16)
			{
				m_pTeki->SetActionState(BASE_ACTION_STATE::SKILL);
				m_pTeki->SetMotion(MOTION_TYPE::RUSH_ATTACK3);

				m_pTeki->AheadCommandReset();
			}
		}
	}

	return false;
}

void Teki::SkillAction::Land::Exit()
{
	// 攻撃情報を元に戻す
	m_pTeki->m_ActionDatas[(int)BASE_ACTION_STATE::SKILL].pAttackData->damage = m_OrgDamage;
	m_pTeki->m_ActionDatas[(int)BASE_ACTION_STATE::SKILL].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].FlyVector = m_OrgFlyVector[(int)AttackData::HIT_PLACE::LAND];
	m_pTeki->m_ActionDatas[(int)BASE_ACTION_STATE::SKILL].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].FlyVector = m_OrgFlyVector[(int)AttackData::HIT_PLACE::AERIAL];
	m_pTeki->m_ActionDatas[(int)BASE_ACTION_STATE::SKILL].pAttackData->fComboRate = m_fOrgComboRate;

	// 先行入力リセット
	m_pTeki->AheadCommandReset();
}


Teki::SkillAction::Squat::Squat(Teki *pTeki) :Base(pTeki, BASE_ACTION_STATE::SKILL_SQUAT)
{}

void Teki::SkillAction::Squat::Enter()
{
	// アクションセット
	m_pTeki->SetActionState(BASE_ACTION_STATE::FRAMECOUNT);

	// モーションセット
	m_pTeki->SetMotion(MOTION_TYPE::SKILL_SQUAT);
}

bool Teki::SkillAction::Squat::Execute()
{
	// 移動終わったら
	if (m_pTeki->m_iCurrentActionFrame > 27)
	{
		return true;
	}
	else if (m_pTeki->m_iCurrentActionFrame < 10)
	{
		// 移動するだけ
		m_pTeki->SetMove(Vector3((m_pTeki->m_dir == DIR::LEFT) ? -m_pTeki->m_tagCharacterParam.fMaxSpeed : m_pTeki->m_tagCharacterParam.fMaxSpeed, 0, 0));
	}

	else //if (m_pTeki->m_iCurrentActionFrame < 20)
	{
		/* キャンセルルート */

		// コークスクリュー
		if (m_pTeki->isPushInputTRG(PLAYER_COMMAND_BIT::D, true))
		{
			m_pTeki->SkillExit();
			m_pTeki->m_SkillActionType = SKILL_ACTION_TYPE::LAND2;
			m_pTeki->SkillInit();
			return false;
		}

		// 投げ
		if (ThrowCancel(m_pTeki)) return false;

		// ラッシュ攻撃
		if (AttackCancel(m_pTeki)) return false;
	}


	return false;
}

void Teki::SkillAction::Squat::Exit()
{}



Teki::SkillAction::Land2::Land2(Teki *pTeki) :Base(pTeki, BASE_ACTION_STATE::SKILL2)
{}

void Teki::SkillAction::Land2::Enter()
{
	// モーションに変える
	m_pTeki->SetMotion(MOTION_TYPE::SKILL2);

	// アクションステート変更
	m_pTeki->SetActionState(BASE_ACTION_STATE::SKILL2);

	// 向き変更
	m_pTeki->SetDirAngle();
}

bool Teki::SkillAction::Land2::Execute()
{
	// 攻撃ステート終わったら
	if (!m_pTeki->isAttackState())
	{
		return true;
	}

	// 横に移動
	if (m_pTeki->isActiveFrame())
	{
		m_pTeki->SetMove(Vector3(m_pTeki->m_dir == DIR::LEFT ? -4.0f : 4.0f, 0, 0));
	}
	else
	{
		m_pTeki->SetMove(m_pTeki->GetMove() * .8f);
	}

	// キャンセルルート
	if (m_pTeki->isHitAttack())
	{
		// 必殺技キャンセル
		HeaveHoCancel(m_pTeki);
	}

	return false;
}

void Teki::SkillAction::Land2::Exit(){}

void Teki::HeavehoDriveInit()
{
	// カットイン
	SIDE data(m_side);
	MsgMgr->Dispatch(0, ENTITY_ID::UI_MGR, ENTITY_ID::UI_MGR,
		MESSAGE_TYPE::OVER_DRIVE_CUTIN, &data);

	// モーションに変える
	SetMotion(MOTION_TYPE::HEAVEHO_DRIVE);

	// アクションステート変更
	SetActionState(BASE_ACTION_STATE::HEAVEHO_DRIVE);

	// 重力の影響受けるな！
	SetMoveUpdate(false);

	// ストップタイマー
	SetGameTimerStopFlag(true);

	m_iHeavehoStopTimer = 113;
}

bool Teki::HeavehoDriveUpdate()
{
	// 攻撃終わったら
	if (!isAttackState())
	{
		// ここに来たら終わり
		return true;
	}

	// ヒーホーストップ時間計測
	if (m_iHeavehoStopTimer > 0) if (--m_iHeavehoStopTimer == 0)	SetGameTimerStopFlag(false);

	return false;
}

void Teki::HeavehoDriveExit()
{
	// 重力早くもどって！
	SetMoveUpdate(true);

}

void Teki::HeavehoDriveHitEvent()
{
	// 基底クラスを呼んであげる
	BasePlayer::HeavehoDriveHitEvent();

}


void Teki::HeavehoDriveOverFlowInit()
{
	// 重力の影響受けるな！
	SetMoveUpdate(false);

	// ストップタイマー
	SetGameTimerStopFlag(true);

	m_iHeavehoStopTimer = 110;
}
void Teki::HeavehoDriveOverFlowExit()
{

}
void Teki::HeavehoDriveOverFlowUpdate()
{
	/* 実質、必殺がヒットするまでの間 */
	// ヒーホーストップ時間計測
	if (m_iHeavehoStopTimer > 0) if (--m_iHeavehoStopTimer == 0)	SetGameTimerStopFlag(false);
}
void Teki::HeavehoDriveOverFlowSuccessInit()
{
	// スピードラインON
	m_pSpeedLine->ActionRoop(Vector3(0, 0, 0), 1, 1, Vector3(0, 0, 0), Vector3(0, 0, 0));
}
void Teki::HeavehoDriveOverFlowSuccessUpdate()
{
	/* 必殺ヒットして演出中 */
	// ボーン追従
	{
		// ローカルマトリックスにモデルのトランスマトリックスを掛けて
		// ローカルからワールド座標へ返還する
		// マトリックスに代入したらアングルとか行けそう
		D3DXMATRIX WorldMat = *m_pObj->GetBone(19) * m_pObj->TransMatrix;

		// マトリックスから位置の部分のみをベクトルとして設定する
		m_pTargetPlayer->SetPos(Vector3(WorldMat._41, WorldMat._42, WorldMat._43));
		m_pTargetPlayer->SetAngleY(0);
		//m_pTarget->GetObj()->TransMatrix._11 = WorldMat._11;
		//m_pTarget->GetObj()->TransMatrix._12 = WorldMat._12;
		//m_pTarget->GetObj()->TransMatrix._13 = WorldMat._13;
		//m_pTarget->GetObj()->TransMatrix._21 = WorldMat._31;
		//m_pTarget->GetObj()->TransMatrix._22 = WorldMat._32;
		//m_pTarget->GetObj()->TransMatrix._23 = WorldMat._33;
		//m_pTarget->GetObj()->TransMatrix._31 = WorldMat._21;
		//m_pTarget->GetObj()->TransMatrix._32 = WorldMat._22;
		//m_pTarget->GetObj()->TransMatrix._33 = WorldMat._23;
		//m_pTarget->GetObj()->TransMatrix._41 = WorldMat._41;
		//m_pTarget->GetObj()->TransMatrix._42 = WorldMat._42;
		//m_pTarget->GetObj()->TransMatrix._43 = WorldMat._43;

		//m_pTekiPlayer->GetObj()->TransMatrix._31 = 0;
		//m_pTekiPlayer->GetObj()->TransMatrix._32 = 0;
		//m_pTekiPlayer->GetObj()->TransMatrix._33 = 1;
		//m_pTekiPlayer->GetObj()->TransMatrix._41 = 0;
		//m_pTekiPlayer->GetObj()->TransMatrix._42 = 0;
		//m_pTekiPlayer->GetObj()->TransMatrix._43 = 0;
	}

	// コンボ
	//COMBO_DESK comboDesk;

	//switch (m_iHeaveHoDriveOverFlowFrame++)
	//{
	//}

}

// AI
//PLAYER_INPUT Teki::AIHighAttackButton()
//{
//	m_pAI->Update();;
//}