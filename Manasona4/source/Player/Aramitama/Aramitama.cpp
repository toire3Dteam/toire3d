#include "Aramitama.h"
#include "../../Sound/SoundManager.h"
#include "UI/HeaveHoFinish/HeaveHoFinish.h"
#include "../../DeferredEx/DeferredEx.h"
#include "../../BaseEntity/Message/MessageDispatcher.h"
#include "Window\Player\AirouWindow.h"
#include "../../Shot/BaseShot.h"

Aramitama::Aramitama(SIDE side, const SideData &data) :BasePlayer(side, data), m_iWassyoiGauge(0), m_bWassyoi(false)
{
	// コマンドウィンドウ
	// 右か左でWindowの場所を変える
	Vector2 l_vWindowPos;
	if (side == SIDE::LEFT)
	{
		l_vWindowPos.x = 100;
		l_vWindowPos.y = 100;
	}
	else
	{
		l_vWindowPos.x = 550;
		l_vWindowPos.y = 100;
	}
	m_pCommandWindow = new AirouWindow(l_vWindowPos);

	// エフェクトカメラID
	m_tagCharacterParam.eHeaveHoOverFlowCameraID = EFFECT_CAMERA_ID::NAZENARA_OVERFLOW;

	// カットインのID
	m_tagCharacterParam.eCutInType = CUTIN_TYPE_NAME::NAZENARABA;

	// 無敵攻撃がかうんたーかどうか
	m_tagCharacterParam.bInvincibleCoutner = false;

	// キャラごとの設定を読み込む
	BasePlayer::LoadCharacterParam("DATA/CHR/Aramitama/param.txt");


	m_pName = "アラミタマ";
	m_pCutinPic = new tdn2DAnim("DATA/UI/Game/OverDriveCutin/nazenaraba.png");
	m_pCutinPic->OrderMoveAppeared(8, -400, +200);

	// フレーム読み込み
	BasePlayer::LoadAttackFrameList("DATA/CHR/Aramitama/FrameList.txt");
	// ★フレーム独自改造
	{
		// コロマル
		m_ActionFrameList[(int)BASE_ACTION_STATE::SKILL][24] = FRAME_STATE::RECOVERY_HIT;
		m_ActionFrameList[(int)BASE_ACTION_STATE::SKILL][34] = FRAME_STATE::RECOVERY_HIT;

		// 車輪
		int ActiveStartFrame;
		for (int i = 0;; i++) if (m_ActionFrameList[(int)BASE_ACTION_STATE::SKILL_AERIAL][i] == FRAME_STATE::ACTIVE){ ActiveStartFrame = i; break; }
		for (int i = ActiveStartFrame; m_ActionFrameList[(int)BASE_ACTION_STATE::SKILL_AERIAL][i] != FRAME_STATE::FOLLOW; i++)
		{
			// 小刻みにいくよおおおおっ！！！！！
			static const int kankaku(3);
			if ((i - ActiveStartFrame) % kankaku == kankaku - 1) m_ActionFrameList[(int)BASE_ACTION_STATE::SKILL_AERIAL][i] = FRAME_STATE::RECOVERY_HIT;
		}

		// 字小田院
		for (int i = 0;; i++) if (m_ActionFrameList[(int)BASE_ACTION_STATE::HEAVEHO_DRIVE][i] == FRAME_STATE::ACTIVE){ ActiveStartFrame = i; break; }
		for (int i = ActiveStartFrame; m_ActionFrameList[(int)BASE_ACTION_STATE::HEAVEHO_DRIVE][i] != FRAME_STATE::FOLLOW; i++)
		{
			// 小刻みにいくよおおおおっ！！！！！
			static const int kankaku(3);
			if ((i - ActiveStartFrame) % kankaku == kankaku - 1) m_ActionFrameList[(int)BASE_ACTION_STATE::HEAVEHO_DRIVE][i] = FRAME_STATE::RECOVERY_HIT;
		}
	}

	// 顔グラ
	m_pFacePic = new tdn2DAnim("Data/UI/Game/CharaIcon/nazenarabaIcon.png");
	//m_pFacePic->OrderNone();
	m_pFacePic->OrderShrink(8, 1, 1.25f);

	// スピードライン
	m_pSpeedLine = new SpeedLineGreenEffect;

	// 虫データ初期化
	InitMushiDatas();
	// エフェクト
	m_pAChargeWave		= new AramitamaChargeWaveEffect();
	m_pAChargeAura		= new AramitamaChargeAuraEffect();
	m_pACircle			= new AramitamaCircleEffect();
	m_pACanon			= new AramitamaCanonEffect();
	m_pANozzleFlash		= new AramitamaNozzleFlashEffect();

	// 最後に初期化
	Reset();
}

void Aramitama::Reset()
{
	// 必ず書く
	BasePlayer::Reset();

	m_bWassyoi = true;
	m_iWassyoiGauge = 0;
	m_pAChargeWave->Stop();
	m_pAChargeAura->Stop();
	m_pACircle->Stop();
	m_pACanon->Stop();
	m_pANozzleFlash->Stop();
}

void Aramitama::InitActionDatas()
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
	m_ActionDatas[(int)BASE_ACTION_STATE::OVERDRIVE_BURST].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].iHitStopFrame = 4;
	m_ActionDatas[(int)BASE_ACTION_STATE::OVERDRIVE_BURST].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].iHitStopFrame = 4;
	m_ActionDatas[(int)BASE_ACTION_STATE::OVERDRIVE_BURST].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].HitRecoveryFrame = 60;
	m_ActionDatas[(int)BASE_ACTION_STATE::OVERDRIVE_BURST].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].HitRecoveryFrame = 60;
	m_ActionDatas[(int)BASE_ACTION_STATE::OVERDRIVE_BURST].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].DamageMotion = DAMAGE_MOTION::KNOCK_DOWN;
	m_ActionDatas[(int)BASE_ACTION_STATE::OVERDRIVE_BURST].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].DamageMotion = DAMAGE_MOTION::KNOCK_DOWN;
	// 判定形状
	m_ActionDatas[(int)BASE_ACTION_STATE::OVERDRIVE_BURST].pAttackData->pCollisionShape->width = 20;
	m_ActionDatas[(int)BASE_ACTION_STATE::OVERDRIVE_BURST].pAttackData->pCollisionShape->height = 20;
	m_ActionDatas[(int)BASE_ACTION_STATE::OVERDRIVE_BURST].pAttackData->pCollisionShape->pos.Set(0, 3.5f, 0);


	//==============================================================================================================
	//	通常1段
	// 地上ヒットも空中ヒットも共通の情報
	m_ActionDatas[(int)BASE_ACTION_STATE::RUSH1].InstanceAttackData();	// アタックデータ作成
	m_ActionDatas[(int)BASE_ACTION_STATE::RUSH1].pAttackData->damage = 140;
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
	m_ActionDatas[(int)BASE_ACTION_STATE::RUSH1].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].iHitStopFrame = 12;
	m_ActionDatas[(int)BASE_ACTION_STATE::RUSH1].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].iHitStopFrame = 13;
	m_ActionDatas[(int)BASE_ACTION_STATE::RUSH1].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].HitRecoveryFrame = 20;
	m_ActionDatas[(int)BASE_ACTION_STATE::RUSH1].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].HitRecoveryFrame = 20;
	m_ActionDatas[(int)BASE_ACTION_STATE::RUSH1].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].DamageMotion = DAMAGE_MOTION::KNOCK_BACK;
	m_ActionDatas[(int)BASE_ACTION_STATE::RUSH1].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].DamageMotion = DAMAGE_MOTION::KNOCK_DOWN;
	// 判定形状
	m_ActionDatas[(int)BASE_ACTION_STATE::RUSH1].pAttackData->pCollisionShape->width = 7;
	m_ActionDatas[(int)BASE_ACTION_STATE::RUSH1].pAttackData->pCollisionShape->height = 6;
	m_ActionDatas[(int)BASE_ACTION_STATE::RUSH1].pAttackData->pCollisionShape->pos.Set(6, 7.0f, 0);

	//==============================================================================================================
	//	通常2段
	// 地上ヒットも空中ヒットも共通の情報
	m_ActionDatas[(int)BASE_ACTION_STATE::RUSH2].InstanceAttackData();	// アタックデータ作成
	m_ActionDatas[(int)BASE_ACTION_STATE::RUSH2].pAttackData->damage = 260;
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
	m_ActionDatas[(int)BASE_ACTION_STATE::RUSH2].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].FlyVector.Set(.6f, .0f);
	m_ActionDatas[(int)BASE_ACTION_STATE::RUSH2].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].FlyVector.Set(.6f, .75f);
	m_ActionDatas[(int)BASE_ACTION_STATE::RUSH2].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].iHitStopFrame = 12;
	m_ActionDatas[(int)BASE_ACTION_STATE::RUSH2].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].iHitStopFrame = 13;
	m_ActionDatas[(int)BASE_ACTION_STATE::RUSH2].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].HitRecoveryFrame = 28;
	m_ActionDatas[(int)BASE_ACTION_STATE::RUSH2].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].HitRecoveryFrame = 28;
	m_ActionDatas[(int)BASE_ACTION_STATE::RUSH2].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].DamageMotion = DAMAGE_MOTION::KNOCK_BACK;
	m_ActionDatas[(int)BASE_ACTION_STATE::RUSH2].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].DamageMotion = DAMAGE_MOTION::KNOCK_DOWN;
	// 判定形状
	m_ActionDatas[(int)BASE_ACTION_STATE::RUSH2].pAttackData->pCollisionShape->width = 8;
	m_ActionDatas[(int)BASE_ACTION_STATE::RUSH2].pAttackData->pCollisionShape->height = 8;
	m_ActionDatas[(int)BASE_ACTION_STATE::RUSH2].pAttackData->pCollisionShape->pos.Set(7, 6.0, 0);


	//==============================================================================================================
	//	対空攻撃
	// 地上ヒットも空中ヒットも共通の情報
	m_ActionDatas[(int)BASE_ACTION_STATE::ANTI_AIR].InstanceAttackData();	// アタックデータ作成
	m_ActionDatas[(int)BASE_ACTION_STATE::ANTI_AIR].pAttackData->damage = 300;
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
	m_ActionDatas[(int)BASE_ACTION_STATE::ANTI_AIR].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].FlyVector.Set(.25f, 1.7f);
	m_ActionDatas[(int)BASE_ACTION_STATE::ANTI_AIR].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].iHitStopFrame = 4;
	m_ActionDatas[(int)BASE_ACTION_STATE::ANTI_AIR].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].iHitStopFrame = 8;
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
	m_ActionDatas[(int)BASE_ACTION_STATE::DOKKOI_ATTACK].pAttackData->HitSE = "斬撃1";
	m_ActionDatas[(int)BASE_ACTION_STATE::DOKKOI_ATTACK].pAttackData->WhiffSE = "空振り5";
	m_ActionDatas[(int)BASE_ACTION_STATE::DOKKOI_ATTACK].pAttackData->HitEffectType = EFFECT_TYPE::DAMAGE;
	m_ActionDatas[(int)BASE_ACTION_STATE::DOKKOI_ATTACK].pAttackData->WhiffEffectType = EFFECT_TYPE::WHIFF;
	m_ActionDatas[(int)BASE_ACTION_STATE::DOKKOI_ATTACK].pAttackData->bAntiAir = false;
	m_ActionDatas[(int)BASE_ACTION_STATE::DOKKOI_ATTACK].pAttackData->bFinish = true;
	m_ActionDatas[(int)BASE_ACTION_STATE::DOKKOI_ATTACK].pAttackData->AntiGuard = ANTIGUARD_ATTACK::UP_ATTACK;	// ★しゃがみガードをつぶす
	m_ActionDatas[(int)BASE_ACTION_STATE::DOKKOI_ATTACK].pAttackData->ShakeCameraInfo.Set(.1f, 2);
	m_ActionDatas[(int)BASE_ACTION_STATE::DOKKOI_ATTACK].pAttackData->GuardRecoveryFrame = 10;
	m_ActionDatas[(int)BASE_ACTION_STATE::DOKKOI_ATTACK].pAttackData->fGuardKnockBackPower = .45f;
	m_ActionDatas[(int)BASE_ACTION_STATE::DOKKOI_ATTACK].pAttackData->fComboRate = 0.85f;
	// 地上ヒットと空中ヒットで挙動が変わるもの
	m_ActionDatas[(int)BASE_ACTION_STATE::DOKKOI_ATTACK].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].bBeInvincible = false;
	m_ActionDatas[(int)BASE_ACTION_STATE::DOKKOI_ATTACK].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].bBeInvincible = false;
	m_ActionDatas[(int)BASE_ACTION_STATE::DOKKOI_ATTACK].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].FlyVector.Set(1.6f, .25f);
	m_ActionDatas[(int)BASE_ACTION_STATE::DOKKOI_ATTACK].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].FlyVector.Set(2.4f, 1.85f);
	m_ActionDatas[(int)BASE_ACTION_STATE::DOKKOI_ATTACK].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].iHitStopFrame = 16;
	m_ActionDatas[(int)BASE_ACTION_STATE::DOKKOI_ATTACK].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].iHitStopFrame = 16;
	m_ActionDatas[(int)BASE_ACTION_STATE::DOKKOI_ATTACK].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].HitRecoveryFrame = 55;
	m_ActionDatas[(int)BASE_ACTION_STATE::DOKKOI_ATTACK].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].HitRecoveryFrame = 25;
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
	m_ActionDatas[(int)BASE_ACTION_STATE::SQUAT_ATTACK].pAttackData->damage = 260;
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
	m_ActionDatas[(int)BASE_ACTION_STATE::SQUAT_ATTACK].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].FlyVector.Set(.5f, .25f);
	m_ActionDatas[(int)BASE_ACTION_STATE::SQUAT_ATTACK].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].FlyVector.Set(.25f, .25f);
	m_ActionDatas[(int)BASE_ACTION_STATE::SQUAT_ATTACK].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].iHitStopFrame = 7;
	m_ActionDatas[(int)BASE_ACTION_STATE::SQUAT_ATTACK].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].iHitStopFrame = 7;
	m_ActionDatas[(int)BASE_ACTION_STATE::SQUAT_ATTACK].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].HitRecoveryFrame = 26;
	m_ActionDatas[(int)BASE_ACTION_STATE::SQUAT_ATTACK].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].HitRecoveryFrame = 32;
	m_ActionDatas[(int)BASE_ACTION_STATE::SQUAT_ATTACK].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].DamageMotion = DAMAGE_MOTION::KNOCK_BACK;
	m_ActionDatas[(int)BASE_ACTION_STATE::SQUAT_ATTACK].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].DamageMotion = DAMAGE_MOTION::KNOCK_DOWN;
	// 判定形状
	m_ActionDatas[(int)BASE_ACTION_STATE::SQUAT_ATTACK].pAttackData->pCollisionShape->width = 8;
	m_ActionDatas[(int)BASE_ACTION_STATE::SQUAT_ATTACK].pAttackData->pCollisionShape->height = 3;
	m_ActionDatas[(int)BASE_ACTION_STATE::SQUAT_ATTACK].pAttackData->pCollisionShape->pos.Set(6, 2, 0);



	//==============================================================================================================
	//	足払い攻撃
	// 地上ヒットも空中ヒットも共通の情報
	m_ActionDatas[(int)BASE_ACTION_STATE::DOWN_ATTACK].InstanceAttackData();	// アタックデータ作成
	m_ActionDatas[(int)BASE_ACTION_STATE::DOWN_ATTACK].pAttackData->damage = 400;
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
	m_ActionDatas[(int)BASE_ACTION_STATE::DOWN_ATTACK].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].FlyVector.Set(.35f, .85f);
	m_ActionDatas[(int)BASE_ACTION_STATE::DOWN_ATTACK].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].FlyVector.Set(.25f, .95f);
	m_ActionDatas[(int)BASE_ACTION_STATE::DOWN_ATTACK].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].iHitStopFrame = 11;
	m_ActionDatas[(int)BASE_ACTION_STATE::DOWN_ATTACK].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].iHitStopFrame = 11;
	m_ActionDatas[(int)BASE_ACTION_STATE::DOWN_ATTACK].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].HitRecoveryFrame = 32;
	m_ActionDatas[(int)BASE_ACTION_STATE::DOWN_ATTACK].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].HitRecoveryFrame = 45;
	m_ActionDatas[(int)BASE_ACTION_STATE::DOWN_ATTACK].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].DamageMotion = DAMAGE_MOTION::DOWN_FALL;
	m_ActionDatas[(int)BASE_ACTION_STATE::DOWN_ATTACK].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].DamageMotion = DAMAGE_MOTION::DOWN_FALL;
	// 判定形状
	m_ActionDatas[(int)BASE_ACTION_STATE::DOWN_ATTACK].pAttackData->pCollisionShape->width = 12;
	m_ActionDatas[(int)BASE_ACTION_STATE::DOWN_ATTACK].pAttackData->pCollisionShape->height = 3;
	m_ActionDatas[(int)BASE_ACTION_STATE::DOWN_ATTACK].pAttackData->pCollisionShape->pos.Set(10, 2, 0);


	//==============================================================================================================
	//	空中攻撃
	// 地上ヒットも空中ヒットも共通の情報
	m_ActionDatas[(int)BASE_ACTION_STATE::AERIAL_ATTACK].InstanceAttackData();	// アタックデータ作成
	m_ActionDatas[(int)BASE_ACTION_STATE::AERIAL_ATTACK].pAttackData->damage = 300;
	m_ActionDatas[(int)BASE_ACTION_STATE::AERIAL_ATTACK].pAttackData->pierceLV = 0;
	m_ActionDatas[(int)BASE_ACTION_STATE::AERIAL_ATTACK].pAttackData->HitSE = "斬撃1";
	m_ActionDatas[(int)BASE_ACTION_STATE::AERIAL_ATTACK].pAttackData->WhiffSE = "空振り5";
	m_ActionDatas[(int)BASE_ACTION_STATE::AERIAL_ATTACK].pAttackData->HitEffectType = EFFECT_TYPE::DAMAGE;
	m_ActionDatas[(int)BASE_ACTION_STATE::AERIAL_ATTACK].pAttackData->WhiffEffectType = EFFECT_TYPE::WHIFF;
	m_ActionDatas[(int)BASE_ACTION_STATE::AERIAL_ATTACK].pAttackData->bAntiAir = false;
	m_ActionDatas[(int)BASE_ACTION_STATE::AERIAL_ATTACK].pAttackData->bFinish = true;
	m_ActionDatas[(int)BASE_ACTION_STATE::AERIAL_ATTACK].pAttackData->AntiGuard = ANTIGUARD_ATTACK::UP_ATTACK;		// ★しゃがみガードをつぶす
	m_ActionDatas[(int)BASE_ACTION_STATE::AERIAL_ATTACK].pAttackData->ShakeCameraInfo.Set(.1f, 3);
	m_ActionDatas[(int)BASE_ACTION_STATE::AERIAL_ATTACK].pAttackData->GuardRecoveryFrame = 20;
	m_ActionDatas[(int)BASE_ACTION_STATE::AERIAL_ATTACK].pAttackData->fGuardKnockBackPower = .2f;
	m_ActionDatas[(int)BASE_ACTION_STATE::AERIAL_ATTACK].pAttackData->fComboRate = 0.85f;
	// 地上ヒットと空中ヒットで挙動が変わるもの
	m_ActionDatas[(int)BASE_ACTION_STATE::AERIAL_ATTACK].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].bBeInvincible = false;
	m_ActionDatas[(int)BASE_ACTION_STATE::AERIAL_ATTACK].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].bBeInvincible = false;
	m_ActionDatas[(int)BASE_ACTION_STATE::AERIAL_ATTACK].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].FlyVector.Set(.5f, .0f);
	m_ActionDatas[(int)BASE_ACTION_STATE::AERIAL_ATTACK].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].FlyVector.Set(.45f, 1.65f);
	m_ActionDatas[(int)BASE_ACTION_STATE::AERIAL_ATTACK].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].iHitStopFrame = 7;
	m_ActionDatas[(int)BASE_ACTION_STATE::AERIAL_ATTACK].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].iHitStopFrame = 7;
	m_ActionDatas[(int)BASE_ACTION_STATE::AERIAL_ATTACK].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].HitRecoveryFrame = 35;
	m_ActionDatas[(int)BASE_ACTION_STATE::AERIAL_ATTACK].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].HitRecoveryFrame = 50;
	m_ActionDatas[(int)BASE_ACTION_STATE::AERIAL_ATTACK].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].DamageMotion = DAMAGE_MOTION::KNOCK_BACK;
	m_ActionDatas[(int)BASE_ACTION_STATE::AERIAL_ATTACK].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].DamageMotion = DAMAGE_MOTION::KNOCK_DOWN;
	// 判定形状
	m_ActionDatas[(int)BASE_ACTION_STATE::AERIAL_ATTACK].pAttackData->pCollisionShape->width = 8;
	m_ActionDatas[(int)BASE_ACTION_STATE::AERIAL_ATTACK].pAttackData->pCollisionShape->height = 12;
	m_ActionDatas[(int)BASE_ACTION_STATE::AERIAL_ATTACK].pAttackData->pCollisionShape->pos.Set(6, 14, 0);

	//==============================================================================================================
	//	逆切れ攻撃
	// 地上ヒットも空中ヒットも共通の情報
	m_ActionDatas[(int)BASE_ACTION_STATE::INVINCIBLE_ATTACK].InstanceAttackData();	// アタックデータ作成
	m_ActionDatas[(int)BASE_ACTION_STATE::INVINCIBLE_ATTACK].pAttackData->damage = 240;
	m_ActionDatas[(int)BASE_ACTION_STATE::INVINCIBLE_ATTACK].pAttackData->pierceLV = 0;
	m_ActionDatas[(int)BASE_ACTION_STATE::INVINCIBLE_ATTACK].pAttackData->HitSE = "フィニッシュ小";
	m_ActionDatas[(int)BASE_ACTION_STATE::INVINCIBLE_ATTACK].pAttackData->WhiffSE = "空振り5";
	m_ActionDatas[(int)BASE_ACTION_STATE::INVINCIBLE_ATTACK].pAttackData->HitEffectType = EFFECT_TYPE::DAMAGE;
	m_ActionDatas[(int)BASE_ACTION_STATE::INVINCIBLE_ATTACK].pAttackData->WhiffEffectType = EFFECT_TYPE::WHIFF;
	m_ActionDatas[(int)BASE_ACTION_STATE::INVINCIBLE_ATTACK].pAttackData->bAntiAir = false;
	m_ActionDatas[(int)BASE_ACTION_STATE::INVINCIBLE_ATTACK].pAttackData->bFinish = true;
	m_ActionDatas[(int)BASE_ACTION_STATE::INVINCIBLE_ATTACK].pAttackData->AntiGuard = ANTIGUARD_ATTACK::NONE;
	m_ActionDatas[(int)BASE_ACTION_STATE::INVINCIBLE_ATTACK].pAttackData->ShakeCameraInfo.Set(1.0f, 20);
	m_ActionDatas[(int)BASE_ACTION_STATE::INVINCIBLE_ATTACK].pAttackData->GuardRecoveryFrame = 10;
	m_ActionDatas[(int)BASE_ACTION_STATE::INVINCIBLE_ATTACK].pAttackData->fComboRate = 0.85f;
	// 地上ヒットと空中ヒットで挙動が変わるもの
	m_ActionDatas[(int)BASE_ACTION_STATE::INVINCIBLE_ATTACK].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].bBeInvincible = true;
	m_ActionDatas[(int)BASE_ACTION_STATE::INVINCIBLE_ATTACK].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].bBeInvincible = true;
	m_ActionDatas[(int)BASE_ACTION_STATE::INVINCIBLE_ATTACK].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].FlyVector.Set(3.0f, 2.5f);
	m_ActionDatas[(int)BASE_ACTION_STATE::INVINCIBLE_ATTACK].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].FlyVector.Set(3.5f, 2.5f);
	m_ActionDatas[(int)BASE_ACTION_STATE::INVINCIBLE_ATTACK].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].iHitStopFrame = 25;
	m_ActionDatas[(int)BASE_ACTION_STATE::INVINCIBLE_ATTACK].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].iHitStopFrame = 25;
	m_ActionDatas[(int)BASE_ACTION_STATE::INVINCIBLE_ATTACK].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].HitRecoveryFrame = 60;
	m_ActionDatas[(int)BASE_ACTION_STATE::INVINCIBLE_ATTACK].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].HitRecoveryFrame = 60;
	m_ActionDatas[(int)BASE_ACTION_STATE::INVINCIBLE_ATTACK].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].DamageMotion = DAMAGE_MOTION::KNOCK_DOWN;
	m_ActionDatas[(int)BASE_ACTION_STATE::INVINCIBLE_ATTACK].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].DamageMotion = DAMAGE_MOTION::KNOCK_DOWN;
	// 判定形状
	m_ActionDatas[(int)BASE_ACTION_STATE::INVINCIBLE_ATTACK].pAttackData->pCollisionShape->width = 10;
	m_ActionDatas[(int)BASE_ACTION_STATE::INVINCIBLE_ATTACK].pAttackData->pCollisionShape->height = 12;
	m_ActionDatas[(int)BASE_ACTION_STATE::INVINCIBLE_ATTACK].pAttackData->pCollisionShape->pos.Set(4, 10, 0);

	//==============================================================================================================
	//	キャラクター固有スキル
	// 地上ヒットも空中ヒットも共通の情報
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL].InstanceAttackData();	// アタックデータ作成
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL].pAttackData->damage = 270;
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL].pAttackData->pierceLV = 0;
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL].pAttackData->HitSE = "斬撃2";
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL].pAttackData->WhiffSE = "空振り5";
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL].pAttackData->HitEffectType = EFFECT_TYPE::DAMAGE;
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL].pAttackData->WhiffEffectType = EFFECT_TYPE::WHIFF;
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL].pAttackData->bAntiAir = false;
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL].pAttackData->bFinish = true;
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL].pAttackData->AntiGuard = ANTIGUARD_ATTACK::NONE;
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL].pAttackData->ShakeCameraInfo.Set(.1f, 2);
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL].pAttackData->GuardRecoveryFrame = 15;
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL].pAttackData->fComboRate = 0.85f;
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL].pAttackData->fRepeatAttackRate = 0.8f;
	// 地上ヒットと空中ヒットで挙動が変わるもの
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].bBeInvincible = false;
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].bBeInvincible = false;
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].FlyVector.Set(.5f, 0);
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].FlyVector.Set(.5f, .5f);
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].iHitStopFrame = 4;
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].iHitStopFrame = 4;
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].HitRecoveryFrame = 40;
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].HitRecoveryFrame = 40;
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].DamageMotion = DAMAGE_MOTION::KNOCK_DOWN;
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].DamageMotion = DAMAGE_MOTION::KNOCK_DOWN;

	// 判定形状
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL].pAttackData->pCollisionShape->width = 11;
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL].pAttackData->pCollisionShape->height = 6;
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL].pAttackData->pCollisionShape->pos.Set(9, 4, 0);


	//==============================================================================================================
	//	キャラクター固有空中攻撃
	// 地上ヒットも空中ヒットも共通の情報
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL_AERIAL].InstanceAttackData();	// アタックデータ作成
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL_AERIAL].pAttackData->damage = 30;
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL_AERIAL].pAttackData->pierceLV = 0;
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL_AERIAL].pAttackData->HitSE = "斬撃2";
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL_AERIAL].pAttackData->WhiffSE = "空振り5";
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL_AERIAL].pAttackData->HitEffectType = EFFECT_TYPE::MULTIPLE_HIT;
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL_AERIAL].pAttackData->WhiffEffectType = EFFECT_TYPE::WHIFF;
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL_AERIAL].pAttackData->bAntiAir = false;
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL_AERIAL].pAttackData->bFinish = true;
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL_AERIAL].pAttackData->AntiGuard = ANTIGUARD_ATTACK::NONE;
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL_AERIAL].pAttackData->ShakeCameraInfo.Set(.1f, 2);
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL_AERIAL].pAttackData->GuardRecoveryFrame = 15;
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL_AERIAL].pAttackData->GuardRecoveryFrame = 30;
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL_AERIAL].pAttackData->fGuardKnockBackPower = .6f;
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL_AERIAL].pAttackData->fComboRate = 0.85f;
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL_AERIAL].pAttackData->fRepeatAttackRate = 0.9f;
	// 地上ヒットと空中ヒットで挙動が変わるもの
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL_AERIAL].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].bBeInvincible = false;
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL_AERIAL].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].bBeInvincible = false;
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL_AERIAL].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].FlyVector.Set(.2f, 0);
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL_AERIAL].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].FlyVector.Set(.2f, 0);
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL_AERIAL].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].iHitStopFrame = 2;
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL_AERIAL].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].iHitStopFrame = 2;
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL_AERIAL].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].HitRecoveryFrame = 50;
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL_AERIAL].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].HitRecoveryFrame = 50;
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL_AERIAL].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].DamageMotion = DAMAGE_MOTION::KNOCK_BACK;
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL_AERIAL].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].DamageMotion = DAMAGE_MOTION::KNOCK_DOWN;

	// 判定形状								   
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL_AERIAL].pAttackData->pCollisionShape->width = 12;
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL_AERIAL].pAttackData->pCollisionShape->height = 12;
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL_AERIAL].pAttackData->pCollisionShape->pos.Set(2, 5, 0);


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
	m_ActionDatas[(int)BASE_ACTION_STATE::THROW].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].iHitStopFrame = c_THROW_ESCAPE_FRAME;
	m_ActionDatas[(int)BASE_ACTION_STATE::THROW].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].HitRecoveryFrame = 60;

	// 判定形状
	m_ActionDatas[(int)BASE_ACTION_STATE::THROW].pAttackData->pCollisionShape->width = 5;
	m_ActionDatas[(int)BASE_ACTION_STATE::THROW].pAttackData->pCollisionShape->height = 4;
	m_ActionDatas[(int)BASE_ACTION_STATE::THROW].pAttackData->pCollisionShape->pos.Set(2, 4, 0);

	//==============================================================================================================
	//	投げ成功
	// 地上ヒットも空中ヒットも共通の情報
	m_ActionDatas[(int)BASE_ACTION_STATE::THROW_SUCCESS].InstanceAttackData();	// アタックデータ作成
	m_ActionDatas[(int)BASE_ACTION_STATE::THROW_SUCCESS].pAttackData->damage = 500;
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
	m_ActionDatas[(int)BASE_ACTION_STATE::THROW_SUCCESS].pAttackData->fComboRate = 0.65f;// (12/27) 投げからあんまりダメージを伸ばせない
	// 地上ヒットと空中ヒットで挙動が変わるもの
	m_ActionDatas[(int)BASE_ACTION_STATE::THROW_SUCCESS].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].bBeInvincible = false;
	m_ActionDatas[(int)BASE_ACTION_STATE::THROW_SUCCESS].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].bBeInvincible = false;
	m_ActionDatas[(int)BASE_ACTION_STATE::THROW_SUCCESS].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].FlyVector.Set(1.1f, 2.2f);
	m_ActionDatas[(int)BASE_ACTION_STATE::THROW_SUCCESS].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].FlyVector.Set(1.2f, 2.3f);
	m_ActionDatas[(int)BASE_ACTION_STATE::THROW_SUCCESS].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].iHitStopFrame = 1;
	m_ActionDatas[(int)BASE_ACTION_STATE::THROW_SUCCESS].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].iHitStopFrame = 1;
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
	m_ActionDatas[(int)BASE_ACTION_STATE::HEAVEHO_DRIVE].pAttackData->damage = 30;
	m_ActionDatas[(int)BASE_ACTION_STATE::HEAVEHO_DRIVE].pAttackData->pierceLV = 0;
	m_ActionDatas[(int)BASE_ACTION_STATE::HEAVEHO_DRIVE].pAttackData->HitSE = "斬撃2";
	m_ActionDatas[(int)BASE_ACTION_STATE::HEAVEHO_DRIVE].pAttackData->WhiffSE = "空振り5";
	m_ActionDatas[(int)BASE_ACTION_STATE::HEAVEHO_DRIVE].pAttackData->HitEffectType = EFFECT_TYPE::FINISH_HIT;
	m_ActionDatas[(int)BASE_ACTION_STATE::HEAVEHO_DRIVE].pAttackData->WhiffEffectType = EFFECT_TYPE::WHIFF;
	m_ActionDatas[(int)BASE_ACTION_STATE::HEAVEHO_DRIVE].pAttackData->bAntiAir = false;
	m_ActionDatas[(int)BASE_ACTION_STATE::HEAVEHO_DRIVE].pAttackData->bFinish = true;
	m_ActionDatas[(int)BASE_ACTION_STATE::HEAVEHO_DRIVE].pAttackData->AntiGuard = ANTIGUARD_ATTACK::NONE;
	m_ActionDatas[(int)BASE_ACTION_STATE::HEAVEHO_DRIVE].pAttackData->ShakeCameraInfo.Set(.1f, 5);
	m_ActionDatas[(int)BASE_ACTION_STATE::HEAVEHO_DRIVE].pAttackData->GuardRecoveryFrame = 18;
	m_ActionDatas[(int)BASE_ACTION_STATE::HEAVEHO_DRIVE].pAttackData->attribute = ATTACK_ATTRIBUTE::BULLET;	// 弾属性
	m_ActionDatas[(int)BASE_ACTION_STATE::HEAVEHO_DRIVE].pAttackData->fComboRate = 0.85f;
	m_ActionDatas[(int)BASE_ACTION_STATE::HEAVEHO_DRIVE].pAttackData->fRepeatAttackRate = 1;
	// 地上ヒットと空中ヒットで挙動が変わるもの
	m_ActionDatas[(int)BASE_ACTION_STATE::HEAVEHO_DRIVE].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].bBeInvincible = false;
	m_ActionDatas[(int)BASE_ACTION_STATE::HEAVEHO_DRIVE].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].bBeInvincible = false;
	m_ActionDatas[(int)BASE_ACTION_STATE::HEAVEHO_DRIVE].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].FlyVector.Set(0, 0);
	m_ActionDatas[(int)BASE_ACTION_STATE::HEAVEHO_DRIVE].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].FlyVector.Set(0, 0);
	m_ActionDatas[(int)BASE_ACTION_STATE::HEAVEHO_DRIVE].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].iHitStopFrame = 0;
	m_ActionDatas[(int)BASE_ACTION_STATE::HEAVEHO_DRIVE].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].iHitStopFrame = 0;
	m_ActionDatas[(int)BASE_ACTION_STATE::HEAVEHO_DRIVE].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].HitRecoveryFrame = 60;
	m_ActionDatas[(int)BASE_ACTION_STATE::HEAVEHO_DRIVE].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].HitRecoveryFrame = 60;
	m_ActionDatas[(int)BASE_ACTION_STATE::HEAVEHO_DRIVE].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].DamageMotion = DAMAGE_MOTION::KNOCK_DOWN;
	m_ActionDatas[(int)BASE_ACTION_STATE::HEAVEHO_DRIVE].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].DamageMotion = DAMAGE_MOTION::KNOCK_DOWN;
	// 判定形状
	m_ActionDatas[(int)BASE_ACTION_STATE::HEAVEHO_DRIVE].pAttackData->pCollisionShape->width = 150;
	m_ActionDatas[(int)BASE_ACTION_STATE::HEAVEHO_DRIVE].pAttackData->pCollisionShape->height = 20;
	m_ActionDatas[(int)BASE_ACTION_STATE::HEAVEHO_DRIVE].pAttackData->pCollisionShape->pos.Set(75, 20, 0);

	//==============================================================================================================
	//	ヒーホードライブ_オーバーフロー
	// 地上ヒットも空中ヒットも共通の情報
	m_ActionDatas[(int)BASE_ACTION_STATE::HEAVEHO_DRIVE_OVERFLOW].InstanceAttackData();	// アタックデータ作成
	m_ActionDatas[(int)BASE_ACTION_STATE::HEAVEHO_DRIVE_OVERFLOW].pAttackData->damage = 514;
	m_ActionDatas[(int)BASE_ACTION_STATE::HEAVEHO_DRIVE_OVERFLOW].pAttackData->pierceLV = 0;
	m_ActionDatas[(int)BASE_ACTION_STATE::HEAVEHO_DRIVE_OVERFLOW].pAttackData->HitSE = "∵ヒーホーヒット";
	m_ActionDatas[(int)BASE_ACTION_STATE::HEAVEHO_DRIVE_OVERFLOW].pAttackData->WhiffSE = "空振り5";
	m_ActionDatas[(int)BASE_ACTION_STATE::HEAVEHO_DRIVE_OVERFLOW].pAttackData->HitEffectType = EFFECT_TYPE::FINISH_HIT;
	m_ActionDatas[(int)BASE_ACTION_STATE::HEAVEHO_DRIVE_OVERFLOW].pAttackData->WhiffEffectType = EFFECT_TYPE::WHIFF;
	m_ActionDatas[(int)BASE_ACTION_STATE::HEAVEHO_DRIVE_OVERFLOW].pAttackData->bAntiAir = false;
	m_ActionDatas[(int)BASE_ACTION_STATE::HEAVEHO_DRIVE_OVERFLOW].pAttackData->bFinish = false;
	m_ActionDatas[(int)BASE_ACTION_STATE::HEAVEHO_DRIVE_OVERFLOW].pAttackData->AntiGuard = ANTIGUARD_ATTACK::NONE;
	m_ActionDatas[(int)BASE_ACTION_STATE::HEAVEHO_DRIVE_OVERFLOW].pAttackData->ShakeCameraInfo.Set(0, 0);
	m_ActionDatas[(int)BASE_ACTION_STATE::HEAVEHO_DRIVE_OVERFLOW].pAttackData->GuardRecoveryFrame = 18;
	m_ActionDatas[(int)BASE_ACTION_STATE::HEAVEHO_DRIVE_OVERFLOW].pAttackData->attribute = ATTACK_ATTRIBUTE::THROW;	// 投げ属性(ガードできない)
	m_ActionDatas[(int)BASE_ACTION_STATE::HEAVEHO_DRIVE_OVERFLOW].pAttackData->fComboRate = 0.85f;
	// 地上ヒットと空中ヒットで挙動が変わるもの
	m_ActionDatas[(int)BASE_ACTION_STATE::HEAVEHO_DRIVE_OVERFLOW].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].bBeInvincible = true;
	m_ActionDatas[(int)BASE_ACTION_STATE::HEAVEHO_DRIVE_OVERFLOW].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].bBeInvincible = true;
	m_ActionDatas[(int)BASE_ACTION_STATE::HEAVEHO_DRIVE_OVERFLOW].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].FlyVector.Set(3.0f, 2.5f);
	m_ActionDatas[(int)BASE_ACTION_STATE::HEAVEHO_DRIVE_OVERFLOW].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].FlyVector.Set(3.5f, 2.5f);
	m_ActionDatas[(int)BASE_ACTION_STATE::HEAVEHO_DRIVE_OVERFLOW].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].iHitStopFrame = 25;
	m_ActionDatas[(int)BASE_ACTION_STATE::HEAVEHO_DRIVE_OVERFLOW].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].iHitStopFrame = 25;
	m_ActionDatas[(int)BASE_ACTION_STATE::HEAVEHO_DRIVE_OVERFLOW].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].HitRecoveryFrame = 1000;
	m_ActionDatas[(int)BASE_ACTION_STATE::HEAVEHO_DRIVE_OVERFLOW].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].HitRecoveryFrame = 1000;
	m_ActionDatas[(int)BASE_ACTION_STATE::HEAVEHO_DRIVE_OVERFLOW].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].DamageMotion = DAMAGE_MOTION::KNOCK_BACK;
	m_ActionDatas[(int)BASE_ACTION_STATE::HEAVEHO_DRIVE_OVERFLOW].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].DamageMotion = DAMAGE_MOTION::KNOCK_BACK;
	// 判定形状
	m_ActionDatas[(int)BASE_ACTION_STATE::HEAVEHO_DRIVE_OVERFLOW].pAttackData->pCollisionShape->width = 16;
	m_ActionDatas[(int)BASE_ACTION_STATE::HEAVEHO_DRIVE_OVERFLOW].pAttackData->pCollisionShape->height = 14;
	m_ActionDatas[(int)BASE_ACTION_STATE::HEAVEHO_DRIVE_OVERFLOW].pAttackData->pCollisionShape->pos.Set(8, 6, 0);

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
			for (int frame = 0;; frame++)if (m_ActionFrameList[i][frame] == FRAME_STATE::ACTIVE){ m_ActionDatas[i].pAttackData->WhiffDelayFrame = frame; break; }
		}
	}


	// スキルアクションの初期化(★★★ここに書くのは、この中でAttackDataを参照するから)
	m_pSkillActions[(int)SKILL_ACTION_TYPE::LAND] = new SkillAction::Land(this);
	m_pSkillActions[(int)SKILL_ACTION_TYPE::LAND2] = nullptr;
	m_pSkillActions[(int)SKILL_ACTION_TYPE::SQUAT] = new SkillAction::Squat(this);
	m_pSkillActions[(int)SKILL_ACTION_TYPE::AERIAL] = new SkillAction::Aerial(this);
	m_pSkillActions[(int)SKILL_ACTION_TYPE::AERIAL2] = nullptr;
	m_pSkillActions[(int)SKILL_ACTION_TYPE::AERIALDROP] = new SkillAction::AerialDrop(this);
}

void Aramitama::InitMushiDatas()
{
	//==============================================================================================================
	//	トスミタマ
	m_pMushi[(int)MUSHI_TYPE::LAND] = new MushiData("DATA/CHR/Aramitama/aramitama_mushi1.IEM", 40);
	// 地上ヒットも空中ヒットも共通の情報
	m_pMushi[(int)MUSHI_TYPE::LAND]->tagpAttackData->damage = 800;
	m_pMushi[(int)MUSHI_TYPE::LAND]->tagpAttackData->pierceLV = 0;
	m_pMushi[(int)MUSHI_TYPE::LAND]->tagpAttackData->HitSE = "ヒット6";
	m_pMushi[(int)MUSHI_TYPE::LAND]->tagpAttackData->WhiffSE = "空振り1";
	m_pMushi[(int)MUSHI_TYPE::LAND]->tagpAttackData->HitEffectType = EFFECT_TYPE::DAMAGE;
	m_pMushi[(int)MUSHI_TYPE::LAND]->tagpAttackData->WhiffEffectType = EFFECT_TYPE::WHIFF;
	m_pMushi[(int)MUSHI_TYPE::LAND]->tagpAttackData->bAntiAir = true;
	m_pMushi[(int)MUSHI_TYPE::LAND]->tagpAttackData->bFinish = true;
	m_pMushi[(int)MUSHI_TYPE::LAND]->tagpAttackData->AntiGuard = ANTIGUARD_ATTACK::NONE;
	m_pMushi[(int)MUSHI_TYPE::LAND]->tagpAttackData->ShakeCameraInfo.Set(.2f, 2);
	m_pMushi[(int)MUSHI_TYPE::LAND]->tagpAttackData->GuardRecoveryFrame = 20;
	m_pMushi[(int)MUSHI_TYPE::LAND]->tagpAttackData->fGuardKnockBackPower = .25f;
	m_pMushi[(int)MUSHI_TYPE::LAND]->tagpAttackData->attribute = ATTACK_ATTRIBUTE::BULLET;
	m_pMushi[(int)MUSHI_TYPE::LAND]->tagpAttackData->fComboRate = 1.0f;
	m_pMushi[(int)MUSHI_TYPE::LAND]->tagpAttackData->fRepeatAttackRate = 0.9f;
	// 地上ヒットと空中ヒットで挙動が変わるもの
	m_pMushi[(int)MUSHI_TYPE::LAND]->tagpAttackData->places[(int)AttackData::HIT_PLACE::LAND].bBeInvincible = false;
	m_pMushi[(int)MUSHI_TYPE::LAND]->tagpAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].bBeInvincible = false;
	m_pMushi[(int)MUSHI_TYPE::LAND]->tagpAttackData->places[(int)AttackData::HIT_PLACE::LAND].FlyVector.Set(-.05f, 1.5f);
	m_pMushi[(int)MUSHI_TYPE::LAND]->tagpAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].FlyVector.Set(-.05f, 1.5f);
	m_pMushi[(int)MUSHI_TYPE::LAND]->tagpAttackData->places[(int)AttackData::HIT_PLACE::LAND].iHitStopFrame = 20;
	m_pMushi[(int)MUSHI_TYPE::LAND]->tagpAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].iHitStopFrame = 20;
	m_pMushi[(int)MUSHI_TYPE::LAND]->tagpAttackData->places[(int)AttackData::HIT_PLACE::LAND].HitRecoveryFrame = 60;
	m_pMushi[(int)MUSHI_TYPE::LAND]->tagpAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].HitRecoveryFrame = 60;
	m_pMushi[(int)MUSHI_TYPE::LAND]->tagpAttackData->places[(int)AttackData::HIT_PLACE::LAND].DamageMotion = DAMAGE_MOTION::KNOCK_DOWN;
	m_pMushi[(int)MUSHI_TYPE::LAND]->tagpAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].DamageMotion = DAMAGE_MOTION::KNOCK_DOWN;
	// 判定形状
	m_pMushi[(int)MUSHI_TYPE::LAND]->tagpAttackData->pCollisionShape->width = 5;
	m_pMushi[(int)MUSHI_TYPE::LAND]->tagpAttackData->pCollisionShape->height = 5;
	m_pMushi[(int)MUSHI_TYPE::LAND]->tagpAttackData->pCollisionShape->pos.Set(0, 0, 0);


	//==============================================================================================================
	//	隕石ミタマ
	m_pMushi[(int)MUSHI_TYPE::SQUAT] = new MushiData("DATA/CHR/Aramitama/aramitama_mushi3.IEM", 0);
	// 地上ヒットも空中ヒットも共通の情報
	m_pMushi[(int)MUSHI_TYPE::SQUAT]->tagpAttackData->damage = 1000;
	m_pMushi[(int)MUSHI_TYPE::SQUAT]->tagpAttackData->pierceLV = 0;
	m_pMushi[(int)MUSHI_TYPE::SQUAT]->tagpAttackData->HitSE = "ヒット6";
	m_pMushi[(int)MUSHI_TYPE::SQUAT]->tagpAttackData->WhiffSE = "空振り1";
	m_pMushi[(int)MUSHI_TYPE::SQUAT]->tagpAttackData->HitEffectType = EFFECT_TYPE::DAMAGE;
	m_pMushi[(int)MUSHI_TYPE::SQUAT]->tagpAttackData->WhiffEffectType = EFFECT_TYPE::WHIFF;
	m_pMushi[(int)MUSHI_TYPE::SQUAT]->tagpAttackData->bAntiAir = true;
	m_pMushi[(int)MUSHI_TYPE::SQUAT]->tagpAttackData->bFinish = true;
	m_pMushi[(int)MUSHI_TYPE::SQUAT]->tagpAttackData->AntiGuard = ANTIGUARD_ATTACK::NONE;
	m_pMushi[(int)MUSHI_TYPE::SQUAT]->tagpAttackData->ShakeCameraInfo.Set(.2f, 2);
	m_pMushi[(int)MUSHI_TYPE::SQUAT]->tagpAttackData->GuardRecoveryFrame = 20;
	m_pMushi[(int)MUSHI_TYPE::SQUAT]->tagpAttackData->fGuardKnockBackPower = .25f;
	m_pMushi[(int)MUSHI_TYPE::SQUAT]->tagpAttackData->attribute = ATTACK_ATTRIBUTE::BULLET;
	m_pMushi[(int)MUSHI_TYPE::SQUAT]->tagpAttackData->fComboRate = 1.0f;
	m_pMushi[(int)MUSHI_TYPE::SQUAT]->tagpAttackData->fRepeatAttackRate = .75f;
	// 地上ヒットと空中ヒットで挙動が変わるもの
	m_pMushi[(int)MUSHI_TYPE::SQUAT]->tagpAttackData->places[(int)AttackData::HIT_PLACE::LAND].bBeInvincible = false;
	m_pMushi[(int)MUSHI_TYPE::SQUAT]->tagpAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].bBeInvincible = false;
	m_pMushi[(int)MUSHI_TYPE::SQUAT]->tagpAttackData->places[(int)AttackData::HIT_PLACE::LAND].FlyVector.Set(2.75f, 1.5f);
	m_pMushi[(int)MUSHI_TYPE::SQUAT]->tagpAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].FlyVector.Set(1.5f, 1.5f);
	m_pMushi[(int)MUSHI_TYPE::SQUAT]->tagpAttackData->places[(int)AttackData::HIT_PLACE::LAND].iHitStopFrame = 20;
	m_pMushi[(int)MUSHI_TYPE::SQUAT]->tagpAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].iHitStopFrame = 20;
	m_pMushi[(int)MUSHI_TYPE::SQUAT]->tagpAttackData->places[(int)AttackData::HIT_PLACE::LAND].HitRecoveryFrame = 60;
	m_pMushi[(int)MUSHI_TYPE::SQUAT]->tagpAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].HitRecoveryFrame = 60;
	m_pMushi[(int)MUSHI_TYPE::SQUAT]->tagpAttackData->places[(int)AttackData::HIT_PLACE::LAND].DamageMotion = DAMAGE_MOTION::KNOCK_DOWN;
	m_pMushi[(int)MUSHI_TYPE::SQUAT]->tagpAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].DamageMotion = DAMAGE_MOTION::KNOCK_DOWN;
	// 判定形状
	m_pMushi[(int)MUSHI_TYPE::SQUAT]->tagpAttackData->pCollisionShape->width = 5;
	m_pMushi[(int)MUSHI_TYPE::SQUAT]->tagpAttackData->pCollisionShape->height = 5;
	m_pMushi[(int)MUSHI_TYPE::SQUAT]->tagpAttackData->pCollisionShape->pos.Set(0, 0, 0);



	//==============================================================================================================
	//	D虫ミタマ
	m_pMushi[(int)MUSHI_TYPE::AERIAL] = new MushiData("DATA/CHR/Aramitama/aramitama_mushi2.IEM", Shot::AramitamaMushi::Aerial::c_SOJOURN_TIME);
	// 地上ヒットも空中ヒットも共通の情報
	m_pMushi[(int)MUSHI_TYPE::AERIAL]->tagpAttackData->damage = 50;
	m_pMushi[(int)MUSHI_TYPE::AERIAL]->tagpAttackData->pierceLV = 0;
	m_pMushi[(int)MUSHI_TYPE::AERIAL]->tagpAttackData->HitSE = "斬撃2";
	m_pMushi[(int)MUSHI_TYPE::AERIAL]->tagpAttackData->WhiffSE = "空振り1";
	m_pMushi[(int)MUSHI_TYPE::AERIAL]->tagpAttackData->HitEffectType = EFFECT_TYPE::MULTIPLE_HIT;
	m_pMushi[(int)MUSHI_TYPE::AERIAL]->tagpAttackData->WhiffEffectType = EFFECT_TYPE::WHIFF;
	m_pMushi[(int)MUSHI_TYPE::AERIAL]->tagpAttackData->bAntiAir = true;
	m_pMushi[(int)MUSHI_TYPE::AERIAL]->tagpAttackData->bFinish = true;
	m_pMushi[(int)MUSHI_TYPE::AERIAL]->tagpAttackData->AntiGuard = ANTIGUARD_ATTACK::DOWN_ATTACK;
	m_pMushi[(int)MUSHI_TYPE::AERIAL]->tagpAttackData->ShakeCameraInfo.Set(.2f, 2);
	m_pMushi[(int)MUSHI_TYPE::AERIAL]->tagpAttackData->GuardRecoveryFrame = 20;
	m_pMushi[(int)MUSHI_TYPE::AERIAL]->tagpAttackData->fGuardKnockBackPower = .25f;
	m_pMushi[(int)MUSHI_TYPE::AERIAL]->tagpAttackData->attribute = ATTACK_ATTRIBUTE::BULLET;
	m_pMushi[(int)MUSHI_TYPE::AERIAL]->tagpAttackData->fComboRate = 1.0f;
	m_pMushi[(int)MUSHI_TYPE::AERIAL]->tagpAttackData->fRepeatAttackRate = .99f;
	// 地上ヒットと空中ヒットで挙動が変わるもの
	m_pMushi[(int)MUSHI_TYPE::AERIAL]->tagpAttackData->places[(int)AttackData::HIT_PLACE::LAND].bBeInvincible = false;
	m_pMushi[(int)MUSHI_TYPE::AERIAL]->tagpAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].bBeInvincible = false;
	m_pMushi[(int)MUSHI_TYPE::AERIAL]->tagpAttackData->places[(int)AttackData::HIT_PLACE::LAND].FlyVector.Set(.01f, 0.75f);
	m_pMushi[(int)MUSHI_TYPE::AERIAL]->tagpAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].FlyVector.Set(.01f, 0.75f);
	m_pMushi[(int)MUSHI_TYPE::AERIAL]->tagpAttackData->places[(int)AttackData::HIT_PLACE::LAND].iHitStopFrame = 1;
	m_pMushi[(int)MUSHI_TYPE::AERIAL]->tagpAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].iHitStopFrame = 1;
	m_pMushi[(int)MUSHI_TYPE::AERIAL]->tagpAttackData->places[(int)AttackData::HIT_PLACE::LAND].HitRecoveryFrame = 60;
	m_pMushi[(int)MUSHI_TYPE::AERIAL]->tagpAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].HitRecoveryFrame = 60;
	m_pMushi[(int)MUSHI_TYPE::AERIAL]->tagpAttackData->places[(int)AttackData::HIT_PLACE::LAND].DamageMotion = DAMAGE_MOTION::KNOCK_DOWN;
	m_pMushi[(int)MUSHI_TYPE::AERIAL]->tagpAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].DamageMotion = DAMAGE_MOTION::KNOCK_DOWN;
	// 判定形状
	m_pMushi[(int)MUSHI_TYPE::AERIAL]->tagpAttackData->pCollisionShape->width = 5;
	m_pMushi[(int)MUSHI_TYPE::AERIAL]->tagpAttackData->pCollisionShape->height = 5;
	m_pMushi[(int)MUSHI_TYPE::AERIAL]->tagpAttackData->pCollisionShape->pos.Set(0, 0, 0);
}

Aramitama::~Aramitama()
{
	FOR((int)SKILL_ACTION_TYPE::MAX) SAFE_DELETE(m_pSkillActions[i]);
	FOR((int)MUSHI_TYPE::MAX)SAFE_DELETE(m_pMushi[i]);

	SAFE_DELETE(m_pAChargeWave);
	SAFE_DELETE(m_pAChargeAura);
	SAFE_DELETE(m_pACircle);
	SAFE_DELETE(m_pACanon);
	SAFE_DELETE(m_pANozzleFlash);
}

void Aramitama::Update(PLAYER_UPDATE flag)
{
	// 基底クラスの更新
	BasePlayer::Update(flag);

	// 虫更新
	FOR((int)MUSHI_TYPE::MAX)m_pMushi[i]->Update();
	// エフェクト
	m_pAChargeWave->Update();
	m_pAChargeAura->Update();
	m_pACircle->Update();
	m_pACanon->Update();
	m_pANozzleFlash->Update();
}

void Aramitama::Render()
{
	// 基底クラスの更新
	BasePlayer::Render();
	// エフェクト
	m_pAChargeWave->Render();
	m_pAChargeAura->Render();
	m_pACircle->Render();
	m_pACanon->Render();
	m_pANozzleFlash->Render();
}

void Aramitama::RenderDrive()
{
	// 基底クラスの更新
	BasePlayer::RenderDrive();
}

void Aramitama::ChangeColor()
{
	LPSTR path("DATA/CHR/Aramitama/tex_aramitama_blue.png");
	m_pDefaultObj->SetTexture(tdnTexture::Load(path), 0);
	m_pHHDOFObj->SetTexture(tdnTexture::Load(path), 0);
}

void Aramitama::InitMotionDatas()
{
	m_iMotionNumbers[(int)MOTION_TYPE::WAIT] = 0;
	m_iMotionNumbers[(int)MOTION_TYPE::WALK] = 1;
	m_iMotionNumbers[(int)MOTION_TYPE::RUN] = 2;
	m_iMotionNumbers[(int)MOTION_TYPE::BACK_WALK] = 3;
	m_iMotionNumbers[(int)MOTION_TYPE::BACK_STEP] = 4;
	m_iMotionNumbers[(int)MOTION_TYPE::RUSH_ATTACK1] = 5;
	m_iMotionNumbers[(int)MOTION_TYPE::RUSH_ATTACK2] = 6;
	m_iMotionNumbers[(int)MOTION_TYPE::RUSH_ATTACK3] = 7;
	m_iMotionNumbers[(int)MOTION_TYPE::FINISH_ATTACK] = 7;
	m_iMotionNumbers[(int)MOTION_TYPE::SQUAT] = 8;
	m_iMotionNumbers[(int)MOTION_TYPE::ANTI_AIR_ATTACK] = 9;
	m_iMotionNumbers[(int)MOTION_TYPE::DOKKOI_ATTACK] = 10;
	m_iMotionNumbers[(int)MOTION_TYPE::SQUAT_ATTACK] = 11;
	m_iMotionNumbers[(int)MOTION_TYPE::DOWN_ATTACK] = 12;
	m_iMotionNumbers[(int)MOTION_TYPE::JUMP] = 13;
	m_iMotionNumbers[(int)MOTION_TYPE::FALL] = 14;
	m_iMotionNumbers[(int)MOTION_TYPE::RAND] = 15;
	m_iMotionNumbers[(int)MOTION_TYPE::AERIAL_ATTACK] = 16;
	m_iMotionNumbers[(int)MOTION_TYPE::AERIAL_DASH] = 17;
	m_iMotionNumbers[(int)MOTION_TYPE::PERSONA] = 18;
	m_iMotionNumbers[(int)MOTION_TYPE::SKILL_LAND] = 19;
	m_iMotionNumbers[(int)MOTION_TYPE::SKILL_SQUAT] = 20;
	m_iMotionNumbers[(int)MOTION_TYPE::SKILL_AERIAL] = 21;
	m_iMotionNumbers[(int)MOTION_TYPE::SKILL_AERIALDROP] = 37;
	m_iMotionNumbers[(int)MOTION_TYPE::HEAVEHO_DRIVE] = 22;
	m_iMotionNumbers[(int)MOTION_TYPE::BURST] = 23;
	m_iMotionNumbers[(int)MOTION_TYPE::HEAVEHO_DRIVE_OVERFLOW] = 24;
	m_iMotionNumbers[(int)MOTION_TYPE::THROW] = 25;
	m_iMotionNumbers[(int)MOTION_TYPE::THROW_SUCCESS] = 26;
	m_iMotionNumbers[(int)MOTION_TYPE::THROW_DRAW] = 27;
	m_iMotionNumbers[(int)MOTION_TYPE::UP_GUARD] = 28;
	m_iMotionNumbers[(int)MOTION_TYPE::DOWN_GUARD] = 29;
	m_iMotionNumbers[(int)MOTION_TYPE::ESCAPE] = 30;
	m_iMotionNumbers[(int)MOTION_TYPE::KNOCKBACK] = 31;
	m_iMotionNumbers[(int)MOTION_TYPE::KNOCKDOWN] = 32;
	m_iMotionNumbers[(int)MOTION_TYPE::KNOCKDOWN_DOWN] = 33;
	m_iMotionNumbers[(int)MOTION_TYPE::REVERSAL] = 34;
	m_iMotionNumbers[(int)MOTION_TYPE::APPEAR] = 35;
	m_iMotionNumbers[(int)MOTION_TYPE::WIN] = 36;
}


void Aramitama::KakeaiInit()
{
	
}

void Aramitama::KakeaiUpdate()
{

}

void Aramitama::KakeaiExit()
{

}

void Aramitama::ThirdRushInit()
{
	m_pSkillActions[(int)SKILL_ACTION_TYPE::LAND]->Enter();
}

void Aramitama::ThirdRushUpdate()
{
	if (!isAttackState()) return;

	m_pSkillActions[(int)SKILL_ACTION_TYPE::LAND]->Execute();

	// ヒットしてる状態
	if (
		//GetAttackData()->bHit && 
		GetActionFrame() == FRAME_STATE::FOLLOW)
	{
		// 攻撃ボタン押したら
		if (isPushInputTRG(PLAYER_COMMAND_BIT::C, true))
		{
			// 先行入力リセット
			AheadCommandReset();
			// ゲージ50%以上で必殺技にキャンセルする
			//if (GetActionFrame() == FRAME_STATE::FOLLOW)
			{
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

void Aramitama::ThirdRushExit()
{
	m_pSkillActions[(int)SKILL_ACTION_TYPE::LAND]->Exit();
}



void Aramitama::SkillInit()
{
	m_pSkillActions[(int)m_eSkillActionType]->Enter();
}

bool Aramitama::SkillUpdate()
{
	const bool bEnd = m_pSkillActions[(int)m_eSkillActionType]->Execute();
	return bEnd;
}

void Aramitama::SkillExit()
{
	m_pSkillActions[(int)m_eSkillActionType]->Exit();
}


Aramitama::SkillAction::Base::Base(Aramitama *pAramitama, BASE_ACTION_STATE state) :m_pAramitama(pAramitama)
{
	if (!pAramitama->m_ActionDatas[(int)state].isAttackData()) return;

	memcpy_s(&m_OrgHitSquare, sizeof(CollisionShape::Square), pAramitama->m_ActionDatas[(int)state].pAttackData->pCollisionShape, sizeof(CollisionShape::Square));
	m_iOrgHitRecoveryFrame[(int)AttackData::HIT_PLACE::LAND] = pAramitama->m_ActionDatas[(int)state].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].HitRecoveryFrame;
	m_iOrgHitRecoveryFrame[(int)AttackData::HIT_PLACE::AERIAL] = pAramitama->m_ActionDatas[(int)state].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].HitRecoveryFrame;
	m_iOrgHitStopFrame[(int)AttackData::HIT_PLACE::LAND] = pAramitama->m_ActionDatas[(int)state].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].iHitStopFrame;
	m_iOrgHitStopFrame[(int)AttackData::HIT_PLACE::AERIAL] = pAramitama->m_ActionDatas[(int)state].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].iHitStopFrame;
	m_OrgDamage = pAramitama->m_ActionDatas[(int)state].pAttackData->damage;
	m_fOrgComboRate = pAramitama->m_ActionDatas[(int)state].pAttackData->fComboRate;
	m_OrgFlyVector[(int)AttackData::HIT_PLACE::LAND] = pAramitama->m_ActionDatas[(int)state].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].FlyVector;
	m_OrgFlyVector[(int)AttackData::HIT_PLACE::AERIAL] = pAramitama->m_ActionDatas[(int)state].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].FlyVector;
}

Aramitama::SkillAction::Land::Land(Aramitama *pAramitama) :Base(pAramitama, BASE_ACTION_STATE::SKILL), m_iHitCount(0)
{}

void Aramitama::SkillAction::Land::Enter()
{
	// モーションに変える
	m_pAramitama->SetMotion(MOTION_TYPE::SKILL_LAND);

	// アクションステート変更
	m_pAramitama->SetActionState(BASE_ACTION_STATE::SKILL);

	// 向き設定
	m_pAramitama->SetDirAngle();

	m_iHitCount = 0;
}

bool Aramitama::SkillAction::Land::Execute()
{
	// 攻撃ステート終わったら
	if (!m_pAramitama->isAttackState())
	{
		return true;
	}

	// 1回でも当たったらフラグ
	if (m_pAramitama->isHitAttack())
	{
		// 必殺技キャンセル
		if (HeaveHoCancel(m_pAramitama)) return false;
	}

	// 烙印時の処理
	if (m_pAramitama->m_bWassyoi)
	{
		// トスミタマ出現フレーム
		if (m_pAramitama->GetCurrentFrame() == 52)
		{
			// 発動！
			m_pAramitama->m_pMushi[(int)MUSHI_TYPE::LAND]->Action(m_pAramitama, MUSHI_TYPE::LAND);
		}
	}

	switch (m_pAramitama->GetActionFrame())
	{
	case FRAME_STATE::ACTIVE:
		m_pAramitama->AddMove(Vector3((m_pAramitama->m_dir == DIR::LEFT) ? -1.0f : 1.0f, 0, 0));
		m_pAramitama->MoveClampX(1.5f);

		// アラミタマ自体の横幅を増やす
		m_pAramitama->m_tagCharacterParam.HitSquare.width = m_pAramitama->m_tagOrgCharacterParam.HitSquare.width * 4;
		break;
	case FRAME_STATE::RECOVERY_HIT:
		m_pAramitama->AddMove(Vector3((m_pAramitama->m_dir == DIR::LEFT) ? -1.0f : 1.0f, 0, 0));
		m_pAramitama->MoveClampX(1.5f);

		// ヒット回数計測
		if (++m_iHitCount >= 2)
		{
			// 最後の一撃だけ強くする
			m_pAramitama->m_ActionDatas[(int)BASE_ACTION_STATE::SKILL].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].FlyVector.Set(1.75f, 2.0f);
			m_pAramitama->m_ActionDatas[(int)BASE_ACTION_STATE::SKILL].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].FlyVector.Set(1.75f, 1.8f);

			m_pAramitama->m_ActionDatas[(int)BASE_ACTION_STATE::SKILL].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].iHitStopFrame = 12;
			m_pAramitama->m_ActionDatas[(int)BASE_ACTION_STATE::SKILL].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].iHitStopFrame = 12;
		}

		// コンボレートを下げないようにする
		m_pAramitama->m_ActionDatas[(int)BASE_ACTION_STATE::SKILL].pAttackData->fComboRate = 1.0f;
		break;

	case FRAME_STATE::FOLLOW:
		m_pAramitama->SetMove(m_pAramitama->GetMove() * .5f);
		break;
	}

	return false;
}

void Aramitama::SkillAction::Land::Exit()
{
	// 横幅を戻す
	m_pAramitama->m_tagCharacterParam.HitSquare.width = m_pAramitama->m_tagOrgCharacterParam.HitSquare.width;

	// 攻撃情報を元に戻す
	m_pAramitama->m_ActionDatas[(int)BASE_ACTION_STATE::SKILL].pAttackData->damage = m_OrgDamage;
	m_pAramitama->m_ActionDatas[(int)BASE_ACTION_STATE::SKILL].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].FlyVector = m_OrgFlyVector[(int)AttackData::HIT_PLACE::LAND];
	m_pAramitama->m_ActionDatas[(int)BASE_ACTION_STATE::SKILL].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].FlyVector = m_OrgFlyVector[(int)AttackData::HIT_PLACE::AERIAL];
	m_pAramitama->m_ActionDatas[(int)BASE_ACTION_STATE::SKILL].pAttackData->fComboRate = m_fOrgComboRate;
	m_pAramitama->m_ActionDatas[(int)BASE_ACTION_STATE::SKILL].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].iHitStopFrame = m_iOrgHitStopFrame[(int)AttackData::HIT_PLACE::LAND];
	m_pAramitama->m_ActionDatas[(int)BASE_ACTION_STATE::SKILL].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].iHitStopFrame = m_iOrgHitStopFrame[(int)AttackData::HIT_PLACE::AERIAL];
	m_pAramitama->m_ActionDatas[(int)BASE_ACTION_STATE::SKILL].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].HitRecoveryFrame = m_iOrgHitRecoveryFrame[(int)AttackData::HIT_PLACE::LAND];
	m_pAramitama->m_ActionDatas[(int)BASE_ACTION_STATE::SKILL].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].HitRecoveryFrame = m_iOrgHitRecoveryFrame[(int)AttackData::HIT_PLACE::AERIAL];

	// 先行入力リセット
	m_pAramitama->AheadCommandReset();
}


Aramitama::SkillAction::Squat::Squat(Aramitama *pAramitama) :Base(pAramitama, BASE_ACTION_STATE::SKILL_SQUAT)
{}

void Aramitama::SkillAction::Squat::Enter()
{
	// アクションセット
	m_pAramitama->SetActionState(BASE_ACTION_STATE::SKILL_SQUAT);

	// モーションセット
	m_pAramitama->SetMotion(MOTION_TYPE::SKILL_SQUAT);


}

bool Aramitama::SkillAction::Squat::Execute()
{
	// ステート終わったら
	if (!m_pAramitama->isFrameAction())
	{
		return true;
	}

	// 烙印時の処理
	if (m_pAramitama->m_bWassyoi)
	{
		// 隕石ミタマ出現フレーム
		if (m_pAramitama->GetCurrentFrame() == 30)
		{
			// 発動！
			m_pAramitama->m_pMushi[(int)MUSHI_TYPE::SQUAT]->Action(m_pAramitama, MUSHI_TYPE::SQUAT);
		}
	}

	// チャージして、わっしょいゲージが増加される瞬間
	if (m_pAramitama->isActiveFrame())
	{
		m_pAramitama->m_iWassyoiGauge += 10;
		
		// チャージエフェクト発動
		m_pAramitama->m_pAChargeWave->Action(m_pAramitama->GetPos());
		m_pAramitama->m_pAChargeAura->Action(m_pAramitama->GetPos());

	}
	

	return false;
}

void Aramitama::SkillAction::Squat::Exit()
{
}


Aramitama::SkillAction::Aerial::Aerial(Aramitama *pAramitama) :Base(pAramitama, BASE_ACTION_STATE::SKILL_AERIAL), m_bMoved(false)
{}

void Aramitama::SkillAction::Aerial::Enter()
{
	// モーションに変える
	m_pAramitama->SetMotion(MOTION_TYPE::SKILL_AERIAL);

	// アクションステート変更
	m_pAramitama->SetActionState(BASE_ACTION_STATE::SKILL_AERIAL);

	// 重力の影響受けるな！
	//m_pAramitama->SetMoveUpdate(false);
	if (m_pAramitama->GetMove().y < 0) m_pAramitama->AddMove(Vector3(0, -m_pAramitama->GetMove().y * 2, 0));

	// 向き変更
	m_pAramitama->SetDirAngle();

	m_bMoved = false;
}

bool Aramitama::SkillAction::Aerial::Execute()
{
	// 着地したら
	if (m_pAramitama->isLand())
	{
		m_pAramitama->SetMove(VECTOR_ZERO);
		m_pAramitama->SetActionState(BASE_ACTION_STATE::NO_ACTION);
		return true;
	}

	// 烙印時の処理
	if (m_pAramitama->m_bWassyoi)
	{
		// D虫ミタマ出現フレーム
		if (m_pAramitama->GetCurrentFrame() == 30)
		{
			// 発動！
			m_pAramitama->m_pMushi[(int)MUSHI_TYPE::AERIAL]->Action(m_pAramitama, MUSHI_TYPE::AERIAL);
		}
	}

	// まだ動いてなかったら
	if (!m_bMoved)
	{
		// 攻撃判定出たら
		if (m_pAramitama->isActiveFrame())
		{
			//m_pAramitama->SetMoveUpdate(true);
			m_pAramitama->SetMove(Vector3(m_pAramitama->GetDirVecX() * 1.5f, 2, 0));

			// 動いたフラグを立てて、もう入らないようにする
			m_bMoved = true;
		}
	}

	if (m_pAramitama->GetActionFrame() == FRAME_STATE::RECOVERY_HIT)
	{
		m_pAramitama->m_ActionDatas[(int)BASE_ACTION_STATE::SKILL_AERIAL].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].FlyVector.x = m_pAramitama->GetDirVecX() * -0.2f;
		m_pAramitama->m_ActionDatas[(int)BASE_ACTION_STATE::SKILL_AERIAL].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].FlyVector.x = m_pAramitama->GetDirVecX() * -0.2f;
	}

	if (m_pAramitama->GetAttackData()->bHit)
	{
		// 必殺技キャンセル
		if (HeaveHoCancel(m_pAramitama)) return false;

		// コンボレート減らないように
		m_pAramitama->m_ActionDatas[(int)BASE_ACTION_STATE::SKILL_AERIAL].pAttackData->fComboRate = 1;
	}

	return false;
}

void Aramitama::SkillAction::Aerial::Exit()
{
	// 攻撃情報を元に戻す
	memcpy_s(m_pAramitama->m_ActionDatas[(int)BASE_ACTION_STATE::SKILL_AERIAL].pAttackData->pCollisionShape, sizeof(CollisionShape::Square), &m_OrgHitSquare, sizeof(CollisionShape::Square));

	m_pAramitama->m_ActionDatas[(int)BASE_ACTION_STATE::SKILL_AERIAL].pAttackData->damage = m_OrgDamage;
	m_pAramitama->m_ActionDatas[(int)BASE_ACTION_STATE::SKILL_AERIAL].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].FlyVector = m_OrgFlyVector[(int)AttackData::HIT_PLACE::LAND];
	m_pAramitama->m_ActionDatas[(int)BASE_ACTION_STATE::SKILL_AERIAL].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].FlyVector = m_OrgFlyVector[(int)AttackData::HIT_PLACE::AERIAL];
	m_pAramitama->m_ActionDatas[(int)BASE_ACTION_STATE::SKILL_AERIAL].pAttackData->fComboRate = m_fOrgComboRate;
}


Aramitama::SkillAction::AerialDrop::AerialDrop(Aramitama *pAramitama) :Base(pAramitama, BASE_ACTION_STATE::SKILL_AERIALDROP), m_bMoved(false)
{}

void Aramitama::SkillAction::AerialDrop::Enter()
{
	// モーションに変える
	m_pAramitama->SetMotion(MOTION_TYPE::SKILL_AERIALDROP);

	// アクションステート変更
	m_pAramitama->SetActionState(BASE_ACTION_STATE::SKILL_AERIALDROP);

	// 重力の影響受けるな！
	//m_pAramitama->SetMoveUpdate(false);
	if (m_pAramitama->GetMove().y < 0) m_pAramitama->AddMove(Vector3(0, -m_pAramitama->GetMove().y * 2, 0));

	// 向き変更
	m_pAramitama->SetDirAngle();

	m_bMoved = false;
}

bool Aramitama::SkillAction::AerialDrop::Execute()
{
	// アクションが終わったら
	if (!m_pAramitama->isFrameAction())
	{	
		return true;
	}

	// 烙印時の処理
	if (m_pAramitama->m_bWassyoi)
	{
		// D虫ミタマ出現フレーム
		if (m_pAramitama->GetCurrentFrame() == 28)
		{
			// 発動！
			m_pAramitama->m_pMushi[(int)MUSHI_TYPE::AERIAL]->Action(m_pAramitama, MUSHI_TYPE::AERIAL);
		}
	}

	// 着地したら
	else if (m_pAramitama->isLand())
	{
		m_pAramitama->SetMove(VECTOR_ZERO);
		m_pAramitama->SetActionState(BASE_ACTION_STATE::NO_ACTION);
		return true;
	}

	// まだ動いてなかったら
	if (!m_bMoved)
	{
		// 判定出たら
		if (m_pAramitama->isActiveFrame())
		{
			//m_pAramitama->SetMoveUpdate(true);
			m_pAramitama->SetMove(Vector3(m_pAramitama->GetDirVecX() * -1.0f, 1.5f, 0));

			// 動いたフラグを立てて、もう入らないようにする
			m_bMoved = true;

			// 無敵フラグをオン
			m_pAramitama->SetEscapeFlag(true);
		}
	}

	if (m_pAramitama->GetActionFrame() == FRAME_STATE::FOLLOW)
	{
		// 無敵フラグをオフ
		m_pAramitama->SetEscapeFlag(false);
	}

	return false;
}

void Aramitama::SkillAction::AerialDrop::Exit()
{
	// 無敵フラグをオフ
	m_pAramitama->SetEscapeFlag(false);
}


void Aramitama::HeavehoDriveInit()
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
	m_iHeavehoStopTimer = 97;
}

bool Aramitama::HeavehoDriveUpdate()
{
	// 攻撃終わったら
	if (!isAttackState())
	{
		// ここに来たら終わり
		return true;
	}

	// エフェクト発生時間
	if (m_iCurrentActionFrame == 49)
	{
		// キャノンエフェクト発動
		m_pACanon->Action(m_vPos);
		m_pANozzleFlash->Action(m_vPos);

	}
	// エフェクト終了時間
	else if (m_iCurrentActionFrame == 155)
	{
		// キャノンエフェクト止める
		m_pACanon->Stop();
		m_pANozzleFlash->Stop();
	}

	if (GetAttackData()->bHit)
	{
		// コンボレート減らないように
		m_ActionDatas[(int)BASE_ACTION_STATE::HEAVEHO_DRIVE].pAttackData->fComboRate = 1;
	}

	// ヒーホーストップ時間計測
	if (m_iHeavehoStopTimer > 0) if (--m_iHeavehoStopTimer == 0)	SetGameTimerStopFlag(false);

	return false;
}

void Aramitama::HeavehoDriveExit()
{
	// 重力早くもどって！
	SetMoveUpdate(true);

}

void Aramitama::HeavehoDriveHitEvent()
{
	// 基底クラスを呼んであげる
	BasePlayer::HeavehoDriveHitEvent();

}


void Aramitama::HeavehoDriveOverFlowInit()
{
	// 重力の影響受けるな！
	SetMoveUpdate(false);

	// ストップタイマー
	SetGameTimerStopFlag(true);

	m_iHeavehoStopTimer = 90;
}
void Aramitama::HeavehoDriveOverFlowExit()
{

}
void Aramitama::HeavehoDriveOverFlowUpdate()
{
	/* 実質、必殺がヒットするまでの間 */

	// ヒーホーストップ時間計測
	if (m_iHeavehoStopTimer > 0)
	{
		if (--m_iHeavehoStopTimer == 0)	SetGameTimerStopFlag(false);
	}
}
void Aramitama::HeavehoDriveOverFlowSuccessInit()
{
	// スピードラインON
	m_pSpeedLine->ActionRoop(Vector3(0, 0, 0), 1, 1, Vector3(0, 0, 0), Vector3(0, 0, 0));
}
void Aramitama::HeavehoDriveOverFlowSuccessUpdate()
{
	// 一撃必殺更新

	/* 必殺ヒットして演出中 */
	// ボーン追従
	{
		// ローカルマトリックスにモデルのトランスマトリックスを掛けて
		// ローカルからワールド座標へ返還する
		// マトリックスに代入したらアングルとか行けそう
		D3DXMATRIX WorldMat = *m_pObj->GetBone(2) * m_pObj->TransMatrix;

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

		//m_pAramitamaPlayer->GetObj()->TransMatrix._31 = 0;
		//m_pAramitamaPlayer->GetObj()->TransMatrix._32 = 0;
		//m_pAramitamaPlayer->GetObj()->TransMatrix._33 = 1;
		//m_pAramitamaPlayer->GetObj()->TransMatrix._41 = 0;
		//m_pAramitamaPlayer->GetObj()->TransMatrix._42 = 0;
		//m_pAramitamaPlayer->GetObj()->TransMatrix._43 = 0;
	}

	// コンボ
	//COMBO_DESK comboDesk;

	switch (m_iHeaveHoDriveOverFlowFrame++)
	{
	case 950:
		HeaveHoFinishUI->Action();	// ふぃにっしゅ
		break;
	}

}

void Aramitama::MushiData::Action(Aramitama *pAramitama, MUSHI_TYPE type)
{
	// クールタイム残ってたらスルー
	if (iCoolTime > 0) return;

	// クールタイム設定
	iCoolTime = c_COOL_TIME;

	// ショット追加
	Shot::Base *pNewShot(nullptr);
	
	switch (type)
	{
	case MUSHI_TYPE::LAND:
		pNewShot = new Shot::AramitamaMushi::Land(pAramitama, tagpAttackData, pBullet);
		break;

	case MUSHI_TYPE::SQUAT:
		pNewShot = new Shot::AramitamaMushi::Squat(pAramitama, tagpAttackData, pBullet);
		break;

	case MUSHI_TYPE::AERIAL:
		pNewShot = new Shot::AramitamaMushi::Aerial(pAramitama, tagpAttackData, pBullet);
		break;
	default:
		assert(0);
		break;
	}
	MsgMgr->Dispatch(0, pAramitama->GetID(), ENTITY_ID::SHOT_MGR, MESSAGE_TYPE::ADD_SHOT, pNewShot);
}