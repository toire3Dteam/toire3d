#include "Balance.h"
#include "../../Sound/SoundManager.h"
#include "UI/HeaveHoFinish/HeaveHoFinish.h"
#include "../../DeferredEx/DeferredEx.h"
#include "../../BaseEntity/Message/MessageDispatcher.h"
#include "../../Shot/BaseShot.h"
#include "PointLight\PointLight.h"
#include "DeferredEx\DeferredEx.h"

#include "Window\Player\TekiWindow.h"

Balance::Balance(SIDE side, const SideData &data) :BasePlayer(side, data)
{
	// コマンドウィンドウ
	// 右と左でWindowの場所を変える
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
	m_pCommandWindow = new TekiWindow(l_vWindowPos);


	// エフェクトカメラID
	m_tagCharacterParam.eHeaveHoOverFlowCameraID = EFFECT_CAMERA_ID::TEKI_OVERFLOW;

	// カットインのID
	m_tagCharacterParam.eCutInType = CUTIN_TYPE_NAME::TEKI;

	// 無敵攻撃がかうんたーかどうか
	m_tagCharacterParam.bInvincibleCoutner = false;

	// キャラごとの設定を読み込む
	BasePlayer::LoadCharacterParam("DATA/CHR/Balance/param.txt");

	m_pName = "Balance";
	m_pCutinPic = new tdn2DAnim("DATA/UI/Game/OverDriveCutin/balance.png");
	m_pCutinPic->OrderMoveAppeared(8, -400, +200);

	// フレーム読み込み
	BasePlayer::LoadAttackFrameList("DATA/CHR/Balance/FrameList.txt");
	// ★フレーム独自改造
	// アクティブ時になるフレーム
	m_iBlueImpactActFrame = 0;
	for (int i = 0;; i++)
	{
		if (m_ActionFrameList[(int)BASE_ACTION_STATE::SKILL_SQUAT][i] == FRAME_STATE::ACTIVE)
		{
			m_iBlueImpactActFrame = i;
			break;
		}
	}

	//{
	//	// 2段目は2HITじゃなくなったので、ボツニナリマシタ
	//	//m_ActionFrameList[(int)BASE_ACTION_STATE::RUSH2][13] = FRAME_STATE::RECOVERY_HIT;
	//	//m_ActionFrameList[(int)BASE_ACTION_STATE::RUSH2][14] = FRAME_STATE::ACTIVE;
	//	//m_ActionFrameList[(int)BASE_ACTION_STATE::RUSH2][15] = FRAME_STATE::ACTIVE;

	//	// キルラ
	//	m_ActionFrameList[(int)BASE_ACTION_STATE::SKILL][5 + 7] = FRAME_STATE::RECOVERY_HIT;
	//	m_ActionFrameList[(int)BASE_ACTION_STATE::SKILL][5 + 13] = FRAME_STATE::RECOVERY_HIT;
	//	m_ActionFrameList[(int)BASE_ACTION_STATE::SKILL][5 + 19] = FRAME_STATE::RECOVERY_HIT;

	//	// アサルトダイブ
	//	int ActiveStartFrame;
	//	for (int i = 0;; i++) if (m_ActionFrameList[(int)BASE_ACTION_STATE::SKILL_AERIAL][i] == FRAME_STATE::ACTIVE){ ActiveStartFrame = i; break; }
	//	for (int i = ActiveStartFrame; m_ActionFrameList[(int)BASE_ACTION_STATE::SKILL_AERIAL][i] != FRAME_STATE::FOLLOW; i++)
	//	{
	//		// 小刻みにいくよおおおおっ！！！！！
	//		static const int kankaku(3);
	//		if ((i - ActiveStartFrame) % kankaku == kankaku - 1) m_ActionFrameList[(int)BASE_ACTION_STATE::SKILL_AERIAL][i] = FRAME_STATE::RECOVERY_HIT;
	//	}
	//}
	// てきメッシュ
	m_pObj = m_pDefaultObj;

	// 顔グラ
	m_pFacePic = new tdn2DAnim("Data/UI/Game/CharaIcon/balanceIcon.png");
	//m_pFacePic->OrderNone();
	m_pFacePic->OrderShrink(8, 1, 1.25f);

	// スピードライン
	m_pSpeedLine = new SpeedLineGreenEffect;

	// 敵専用エフェクト 
	m_pCanonEffect = new AramitamaCanonEffect();
	m_pNozzleFlashEffect = new AramitamaNozzleFlashEffect();
	m_pBlueImpact = new BlueImpactEffect();
	// m_pCounterEffect = new TekiCounterEffect();
	// m_pStepEffect = new StepEffect();

	m_pHeartEffect = new  HeartEffect();

	 m_pNinjin = new iex3DObj("Data/UVeffect/Balance/ninjin.IEM");


	// リセットはなるべく最後に
	Reset();
}

void Balance::Reset()
{
	// 必ず書く
	BasePlayer::Reset();

	m_pCanonEffect->Stop();
	m_pNozzleFlashEffect->Stop();
	m_pBlueImpact->Stop();
	m_pHeartEffect->Stop();
	//m_pCounterEffect->Stop();
	//m_pStepEffect->Stop();

}

void Balance::InitActionDatas()
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
	m_ActionDatas[(int)BASE_ACTION_STATE::RUSH1].pAttackData->fComboRate = .875f;
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
	m_ActionDatas[(int)BASE_ACTION_STATE::RUSH1].pAttackData->pCollisionShape->pos.Set(6, 7, 0);

	//==============================================================================================================
	//	通常2段
	// 地上ヒットも空中ヒットも共通の情報
	m_ActionDatas[(int)BASE_ACTION_STATE::RUSH2].InstanceAttackData();	// アタックデータ作成
	m_ActionDatas[(int)BASE_ACTION_STATE::RUSH2].pAttackData->damage = 360;
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
	m_ActionDatas[(int)BASE_ACTION_STATE::RUSH2].pAttackData->fComboRate = 0.875f;
	// 地上ヒットと空中ヒットで挙動が変わるもの
	m_ActionDatas[(int)BASE_ACTION_STATE::RUSH2].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].bBeInvincible = false;
	m_ActionDatas[(int)BASE_ACTION_STATE::RUSH2].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].bBeInvincible = false;
	m_ActionDatas[(int)BASE_ACTION_STATE::RUSH2].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].FlyVector.Set(.3f, .0f);
	m_ActionDatas[(int)BASE_ACTION_STATE::RUSH2].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].FlyVector.Set(.3f, .75f);
	m_ActionDatas[(int)BASE_ACTION_STATE::RUSH2].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].iHitStopFrame = 12;
	m_ActionDatas[(int)BASE_ACTION_STATE::RUSH2].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].iHitStopFrame = 13;
	m_ActionDatas[(int)BASE_ACTION_STATE::RUSH2].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].HitRecoveryFrame = 24;
	m_ActionDatas[(int)BASE_ACTION_STATE::RUSH2].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].HitRecoveryFrame = 24;
	m_ActionDatas[(int)BASE_ACTION_STATE::RUSH2].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].DamageMotion = DAMAGE_MOTION::KNOCK_BACK;
	m_ActionDatas[(int)BASE_ACTION_STATE::RUSH2].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].DamageMotion = DAMAGE_MOTION::KNOCK_DOWN;
	// 判定形状
	m_ActionDatas[(int)BASE_ACTION_STATE::RUSH2].pAttackData->pCollisionShape->width = 8;
	m_ActionDatas[(int)BASE_ACTION_STATE::RUSH2].pAttackData->pCollisionShape->height = 10;
	m_ActionDatas[(int)BASE_ACTION_STATE::RUSH2].pAttackData->pCollisionShape->pos.Set(7, 8, 0);

	//==============================================================================================================
	//	対空攻撃
	// 地上ヒットも空中ヒットも共通の情報
	m_ActionDatas[(int)BASE_ACTION_STATE::ANTI_AIR].InstanceAttackData();	// アタックデータ作成
	m_ActionDatas[(int)BASE_ACTION_STATE::ANTI_AIR].pAttackData->damage = 500;
	m_ActionDatas[(int)BASE_ACTION_STATE::ANTI_AIR].pAttackData->pierceLV = 0;
	m_ActionDatas[(int)BASE_ACTION_STATE::ANTI_AIR].pAttackData->HitSE = "ヒット6";
	m_ActionDatas[(int)BASE_ACTION_STATE::ANTI_AIR].pAttackData->WhiffSE = "空振り5";
	m_ActionDatas[(int)BASE_ACTION_STATE::ANTI_AIR].pAttackData->HitEffectType = EFFECT_TYPE::DAMAGE;
	m_ActionDatas[(int)BASE_ACTION_STATE::ANTI_AIR].pAttackData->WhiffEffectType = EFFECT_TYPE::ANITI_AIR;
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
	m_ActionDatas[(int)BASE_ACTION_STATE::ANTI_AIR].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].iHitStopFrame = 4;
	m_ActionDatas[(int)BASE_ACTION_STATE::ANTI_AIR].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].iHitStopFrame = 8;
	m_ActionDatas[(int)BASE_ACTION_STATE::ANTI_AIR].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].HitRecoveryFrame = 24;
	m_ActionDatas[(int)BASE_ACTION_STATE::ANTI_AIR].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].HitRecoveryFrame = 40;
	m_ActionDatas[(int)BASE_ACTION_STATE::ANTI_AIR].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].DamageMotion = DAMAGE_MOTION::KNOCK_BACK;
	m_ActionDatas[(int)BASE_ACTION_STATE::ANTI_AIR].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].DamageMotion = DAMAGE_MOTION::KNOCK_DOWN;
	// 判定形状
	m_ActionDatas[(int)BASE_ACTION_STATE::ANTI_AIR].pAttackData->pCollisionShape->width = 8;
	m_ActionDatas[(int)BASE_ACTION_STATE::ANTI_AIR].pAttackData->pCollisionShape->height = 8;
	m_ActionDatas[(int)BASE_ACTION_STATE::ANTI_AIR].pAttackData->pCollisionShape->pos.Set(2.5f, 11, 0);

	//==============================================================================================================
	//	中断攻撃(どっこいアタック)
	// 地上ヒットも空中ヒットも共通の情報
	m_ActionDatas[(int)BASE_ACTION_STATE::DOKKOI_ATTACK].InstanceAttackData();	// アタックデータ作成
	m_ActionDatas[(int)BASE_ACTION_STATE::DOKKOI_ATTACK].pAttackData->damage = 700;
	m_ActionDatas[(int)BASE_ACTION_STATE::DOKKOI_ATTACK].pAttackData->pierceLV = 0;
	m_ActionDatas[(int)BASE_ACTION_STATE::DOKKOI_ATTACK].pAttackData->HitSE = "ヒット4";
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
	m_ActionDatas[(int)BASE_ACTION_STATE::DOKKOI_ATTACK].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].HitRecoveryFrame = 45;
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
	m_ActionDatas[(int)BASE_ACTION_STATE::SQUAT_ATTACK].pAttackData->damage = 400;
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
	m_ActionDatas[(int)BASE_ACTION_STATE::SQUAT_ATTACK].pAttackData->fComboRate = 0.9f;
	// 地上ヒットと空中ヒットで挙動が変わるもの
	m_ActionDatas[(int)BASE_ACTION_STATE::SQUAT_ATTACK].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].bBeInvincible = false;
	m_ActionDatas[(int)BASE_ACTION_STATE::SQUAT_ATTACK].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].bBeInvincible = false;
	m_ActionDatas[(int)BASE_ACTION_STATE::SQUAT_ATTACK].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].FlyVector.Set(.25f, .25f);
	m_ActionDatas[(int)BASE_ACTION_STATE::SQUAT_ATTACK].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].FlyVector.Set(.1f, .25f);
	m_ActionDatas[(int)BASE_ACTION_STATE::SQUAT_ATTACK].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].iHitStopFrame = 7;
	m_ActionDatas[(int)BASE_ACTION_STATE::SQUAT_ATTACK].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].iHitStopFrame = 7;
	m_ActionDatas[(int)BASE_ACTION_STATE::SQUAT_ATTACK].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].HitRecoveryFrame = 22;
	m_ActionDatas[(int)BASE_ACTION_STATE::SQUAT_ATTACK].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].HitRecoveryFrame = 24;
	m_ActionDatas[(int)BASE_ACTION_STATE::SQUAT_ATTACK].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].DamageMotion = DAMAGE_MOTION::KNOCK_BACK;
	m_ActionDatas[(int)BASE_ACTION_STATE::SQUAT_ATTACK].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].DamageMotion = DAMAGE_MOTION::KNOCK_DOWN;
	// 判定形状
	m_ActionDatas[(int)BASE_ACTION_STATE::SQUAT_ATTACK].pAttackData->pCollisionShape->width = 6;
	m_ActionDatas[(int)BASE_ACTION_STATE::SQUAT_ATTACK].pAttackData->pCollisionShape->height = 3;
	m_ActionDatas[(int)BASE_ACTION_STATE::SQUAT_ATTACK].pAttackData->pCollisionShape->pos.Set(6, 5, 0);



	//==============================================================================================================
	//	足払い攻撃
	// 地上ヒットも空中ヒットも共通の情報
	m_ActionDatas[(int)BASE_ACTION_STATE::DOWN_ATTACK].InstanceAttackData();	// アタックデータ作成
	m_ActionDatas[(int)BASE_ACTION_STATE::DOWN_ATTACK].pAttackData->damage = 420;
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
	m_ActionDatas[(int)BASE_ACTION_STATE::DOWN_ATTACK].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].FlyVector.Set(.35f, 1.0f);
	m_ActionDatas[(int)BASE_ACTION_STATE::DOWN_ATTACK].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].FlyVector.Set(.25f, 1.05f);
	m_ActionDatas[(int)BASE_ACTION_STATE::DOWN_ATTACK].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].iHitStopFrame = 11;
	m_ActionDatas[(int)BASE_ACTION_STATE::DOWN_ATTACK].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].iHitStopFrame = 11;
	m_ActionDatas[(int)BASE_ACTION_STATE::DOWN_ATTACK].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].HitRecoveryFrame = 40;
	m_ActionDatas[(int)BASE_ACTION_STATE::DOWN_ATTACK].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].HitRecoveryFrame = 60;
	m_ActionDatas[(int)BASE_ACTION_STATE::DOWN_ATTACK].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].DamageMotion = DAMAGE_MOTION::DOWN_FALL;
	m_ActionDatas[(int)BASE_ACTION_STATE::DOWN_ATTACK].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].DamageMotion = DAMAGE_MOTION::DOWN_FALL;
	// 判定形状
	m_ActionDatas[(int)BASE_ACTION_STATE::DOWN_ATTACK].pAttackData->pCollisionShape->width = 8;
	m_ActionDatas[(int)BASE_ACTION_STATE::DOWN_ATTACK].pAttackData->pCollisionShape->height = 3;
	m_ActionDatas[(int)BASE_ACTION_STATE::DOWN_ATTACK].pAttackData->pCollisionShape->pos.Set(8, 2, 0);


	//==============================================================================================================
	//	空中攻撃
	// 地上ヒットも空中ヒットも共通の情報
	m_ActionDatas[(int)BASE_ACTION_STATE::AERIAL_ATTACK].InstanceAttackData();	// アタックデータ作成
	m_ActionDatas[(int)BASE_ACTION_STATE::AERIAL_ATTACK].pAttackData->damage = 300;
	m_ActionDatas[(int)BASE_ACTION_STATE::AERIAL_ATTACK].pAttackData->pierceLV = 0;
	m_ActionDatas[(int)BASE_ACTION_STATE::AERIAL_ATTACK].pAttackData->HitSE = "ヒット5";
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
	m_ActionDatas[(int)BASE_ACTION_STATE::AERIAL_ATTACK].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].FlyVector.Set(.35f, 1.75f);
	m_ActionDatas[(int)BASE_ACTION_STATE::AERIAL_ATTACK].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].iHitStopFrame = 7;
	m_ActionDatas[(int)BASE_ACTION_STATE::AERIAL_ATTACK].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].iHitStopFrame = 7;
	m_ActionDatas[(int)BASE_ACTION_STATE::AERIAL_ATTACK].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].HitRecoveryFrame = 35;
	m_ActionDatas[(int)BASE_ACTION_STATE::AERIAL_ATTACK].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].HitRecoveryFrame = 50;
	m_ActionDatas[(int)BASE_ACTION_STATE::AERIAL_ATTACK].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].DamageMotion = DAMAGE_MOTION::KNOCK_BACK;
	m_ActionDatas[(int)BASE_ACTION_STATE::AERIAL_ATTACK].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].DamageMotion = DAMAGE_MOTION::KNOCK_DOWN;
	// 判定形状
	m_ActionDatas[(int)BASE_ACTION_STATE::AERIAL_ATTACK].pAttackData->pCollisionShape->width = 7;
	m_ActionDatas[(int)BASE_ACTION_STATE::AERIAL_ATTACK].pAttackData->pCollisionShape->height = 8;
	m_ActionDatas[(int)BASE_ACTION_STATE::AERIAL_ATTACK].pAttackData->pCollisionShape->pos.Set(5.5f, 9, 0);

	//==============================================================================================================
	//	逆切れ攻撃
	// 地上ヒットも空中ヒットも共通の情報
	m_ActionDatas[(int)BASE_ACTION_STATE::INVINCIBLE_ATTACK].InstanceAttackData();	// アタックデータ作成
	m_ActionDatas[(int)BASE_ACTION_STATE::INVINCIBLE_ATTACK].pAttackData->damage = 740;
	m_ActionDatas[(int)BASE_ACTION_STATE::INVINCIBLE_ATTACK].pAttackData->pierceLV = 0;
	m_ActionDatas[(int)BASE_ACTION_STATE::INVINCIBLE_ATTACK].pAttackData->HitSE = "フィニッシュ小";
	m_ActionDatas[(int)BASE_ACTION_STATE::INVINCIBLE_ATTACK].pAttackData->WhiffSE = "空振り5";
	m_ActionDatas[(int)BASE_ACTION_STATE::INVINCIBLE_ATTACK].pAttackData->HitEffectType = EFFECT_TYPE::DAMAGE;
	m_ActionDatas[(int)BASE_ACTION_STATE::INVINCIBLE_ATTACK].pAttackData->WhiffEffectType = EFFECT_TYPE::BOOST;
	m_ActionDatas[(int)BASE_ACTION_STATE::INVINCIBLE_ATTACK].pAttackData->bAntiAir = false;
	m_ActionDatas[(int)BASE_ACTION_STATE::INVINCIBLE_ATTACK].pAttackData->bFinish = true;
	m_ActionDatas[(int)BASE_ACTION_STATE::INVINCIBLE_ATTACK].pAttackData->AntiGuard = ANTIGUARD_ATTACK::NONE;
	m_ActionDatas[(int)BASE_ACTION_STATE::INVINCIBLE_ATTACK].pAttackData->ShakeCameraInfo.Set(1.0f, 20);
	m_ActionDatas[(int)BASE_ACTION_STATE::INVINCIBLE_ATTACK].pAttackData->GuardRecoveryFrame = 10;
	m_ActionDatas[(int)BASE_ACTION_STATE::INVINCIBLE_ATTACK].pAttackData->fComboRate = 0.85f;
	// 地上ヒットと空中ヒットで挙動が変わるもの
	m_ActionDatas[(int)BASE_ACTION_STATE::INVINCIBLE_ATTACK].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].bBeInvincible = false;
	m_ActionDatas[(int)BASE_ACTION_STATE::INVINCIBLE_ATTACK].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].bBeInvincible = false;
	m_ActionDatas[(int)BASE_ACTION_STATE::INVINCIBLE_ATTACK].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].FlyVector.Set(2.0f, 1.85f);
	m_ActionDatas[(int)BASE_ACTION_STATE::INVINCIBLE_ATTACK].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].FlyVector.Set(2.2f, 1.85f);
	m_ActionDatas[(int)BASE_ACTION_STATE::INVINCIBLE_ATTACK].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].iHitStopFrame = 8;
	m_ActionDatas[(int)BASE_ACTION_STATE::INVINCIBLE_ATTACK].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].iHitStopFrame = 8;
	m_ActionDatas[(int)BASE_ACTION_STATE::INVINCIBLE_ATTACK].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].HitRecoveryFrame = 60;
	m_ActionDatas[(int)BASE_ACTION_STATE::INVINCIBLE_ATTACK].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].HitRecoveryFrame = 60;
	m_ActionDatas[(int)BASE_ACTION_STATE::INVINCIBLE_ATTACK].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].DamageMotion = DAMAGE_MOTION::KNOCK_DOWN;
	m_ActionDatas[(int)BASE_ACTION_STATE::INVINCIBLE_ATTACK].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].DamageMotion = DAMAGE_MOTION::KNOCK_DOWN;
	// 判定形状
	m_ActionDatas[(int)BASE_ACTION_STATE::INVINCIBLE_ATTACK].pAttackData->pCollisionShape->width = 6;
	m_ActionDatas[(int)BASE_ACTION_STATE::INVINCIBLE_ATTACK].pAttackData->pCollisionShape->height = 10;
	m_ActionDatas[(int)BASE_ACTION_STATE::INVINCIBLE_ATTACK].pAttackData->pCollisionShape->pos.Set(4, 6, 0);


	//==============================================================================================================
	//	スキル攻撃
	// 地上ヒットも空中ヒットも共通の情報
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL].InstanceAttackData();	// アタックデータ作成
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL].pAttackData->damage = 120;
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL].pAttackData->pierceLV = 0;
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL].pAttackData->HitSE = "ヒット3";
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL].pAttackData->WhiffSE = "空振り5";
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL].pAttackData->HitEffectType = EFFECT_TYPE::MULTIPLE_HIT_BLOW;
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL].pAttackData->WhiffEffectType = EFFECT_TYPE::BACK_STEP;
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL].pAttackData->bAntiAir = false;
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL].pAttackData->bFinish = true;
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL].pAttackData->AntiGuard = ANTIGUARD_ATTACK::NONE;
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL].pAttackData->ShakeCameraInfo.Set(.2f, 3);
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL].pAttackData->GuardRecoveryFrame = 18;
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL].pAttackData->fGuardKnockBackPower = .2f;
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL].pAttackData->fComboRate = 0.85f;
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL].pAttackData->fRepeatAttackRate = .95f;// 同技補正の掛かる値

	// 地上ヒットと空中ヒットで挙動が変わるもの
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].bBeInvincible = false;
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].bBeInvincible = false;
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].FlyVector.Set(.1f, .0f);
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].FlyVector.Set(.15f, .325f);
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].iHitStopFrame = 1;
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].iHitStopFrame = 2;
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].HitRecoveryFrame = 40;
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].HitRecoveryFrame = 50;// (12/26) 同技補正の影響で数値あげることに　ラッシュ系は補正は最初の一回だけでいいかも
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].DamageMotion = DAMAGE_MOTION::KNOCK_BACK;
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].DamageMotion = DAMAGE_MOTION::KNOCK_DOWN;
	// 判定形状
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL].pAttackData->pCollisionShape->width = 6;
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL].pAttackData->pCollisionShape->height = 5;
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL].pAttackData->pCollisionShape->pos.Set(5, 5, 0);


	//==============================================================================================================
	//	第2キャラクター固有スキル(突進攻撃)
	// 地上ヒットも空中ヒットも共通の情報
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL_SQUAT].InstanceAttackData();	// アタックデータ作成
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL_SQUAT].pAttackData->damage = 900;
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL_SQUAT].pAttackData->pierceLV = 0;
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL_SQUAT].pAttackData->HitSE = "フィニッシュ小";
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL_SQUAT].pAttackData->WhiffSE = "空振り5";
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL_SQUAT].pAttackData->HitEffectType = EFFECT_TYPE::DAMAGE;
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL_SQUAT].pAttackData->WhiffEffectType = EFFECT_TYPE::CYCLONE_RUN;
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL_SQUAT].pAttackData->bAntiAir = false;
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL_SQUAT].pAttackData->bFinish = true;
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL_SQUAT].pAttackData->AntiGuard = ANTIGUARD_ATTACK::DOWN_ATTACK;
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL_SQUAT].pAttackData->ShakeCameraInfo.Set(.75f, 10);
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL_SQUAT].pAttackData->GuardRecoveryFrame = 10;
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL_SQUAT].pAttackData->fGuardKnockBackPower = .7f;
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL_SQUAT].pAttackData->fComboRate = 0.85f;
	// 地上ヒットと空中ヒットで挙動が変わるもの
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL_SQUAT].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].bBeInvincible = false;
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL_SQUAT].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].bBeInvincible = false;
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL_SQUAT].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].FlyVector.Set(3.0f, 2.0f);
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL_SQUAT].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].FlyVector.Set(3.0f, 2.2f);
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL_SQUAT].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].iHitStopFrame = 8;
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL_SQUAT].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].iHitStopFrame = 5;
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL_SQUAT].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].HitRecoveryFrame = 50;
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL_SQUAT].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].HitRecoveryFrame = 50;
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL_SQUAT].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].DamageMotion = DAMAGE_MOTION::KNOCK_DOWN;
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL_SQUAT].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].DamageMotion = DAMAGE_MOTION::KNOCK_DOWN;

	// 判定形状
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL_SQUAT].pAttackData->pCollisionShape->width = 12;
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL_SQUAT].pAttackData->pCollisionShape->height = 6;
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL_SQUAT].pAttackData->pCollisionShape->pos.Set(9, 9, 0);

	////==============================================================================================================
	////	第2キャラクター固有スキル(突進攻撃)
	//// 地上ヒットも空中ヒットも共通の情報
	//m_ActionDatas[(int)BASE_ACTION_STATE::SKILL2].InstanceAttackData();	// アタックデータ作成
	//m_ActionDatas[(int)BASE_ACTION_STATE::SKILL2].pAttackData->damage = 700;
	//m_ActionDatas[(int)BASE_ACTION_STATE::SKILL2].pAttackData->pierceLV = 0;
	//m_ActionDatas[(int)BASE_ACTION_STATE::SKILL2].pAttackData->HitSE = "フィニッシュ小";
	//m_ActionDatas[(int)BASE_ACTION_STATE::SKILL2].pAttackData->WhiffSE = "空振り5";
	//m_ActionDatas[(int)BASE_ACTION_STATE::SKILL2].pAttackData->HitEffectType = EFFECT_TYPE::DAMAGE;
	//m_ActionDatas[(int)BASE_ACTION_STATE::SKILL2].pAttackData->WhiffEffectType = EFFECT_TYPE::CYCLONE_RUN;
	//m_ActionDatas[(int)BASE_ACTION_STATE::SKILL2].pAttackData->bAntiAir = false;
	//m_ActionDatas[(int)BASE_ACTION_STATE::SKILL2].pAttackData->bFinish = true;
	//m_ActionDatas[(int)BASE_ACTION_STATE::SKILL2].pAttackData->AntiGuard = ANTIGUARD_ATTACK::NONE;
	//m_ActionDatas[(int)BASE_ACTION_STATE::SKILL2].pAttackData->ShakeCameraInfo.Set(.75f, 10);
	//m_ActionDatas[(int)BASE_ACTION_STATE::SKILL2].pAttackData->GuardRecoveryFrame = 10;
	//m_ActionDatas[(int)BASE_ACTION_STATE::SKILL2].pAttackData->fGuardKnockBackPower = .7f;
	//m_ActionDatas[(int)BASE_ACTION_STATE::SKILL2].pAttackData->fComboRate = 0.85f;
	//// 地上ヒットと空中ヒットで挙動が変わるもの
	//m_ActionDatas[(int)BASE_ACTION_STATE::SKILL2].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].bBeInvincible = false;
	//m_ActionDatas[(int)BASE_ACTION_STATE::SKILL2].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].bBeInvincible = false;
	//m_ActionDatas[(int)BASE_ACTION_STATE::SKILL2].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].FlyVector.Set(3.1f, 1.25f);
	//m_ActionDatas[(int)BASE_ACTION_STATE::SKILL2].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].FlyVector.Set(3.1f, 1.5f);
	//m_ActionDatas[(int)BASE_ACTION_STATE::SKILL2].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].iHitStopFrame = 8;
	//m_ActionDatas[(int)BASE_ACTION_STATE::SKILL2].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].iHitStopFrame = 5;
	//m_ActionDatas[(int)BASE_ACTION_STATE::SKILL2].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].HitRecoveryFrame = 40;
	//m_ActionDatas[(int)BASE_ACTION_STATE::SKILL2].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].HitRecoveryFrame = 40;
	//m_ActionDatas[(int)BASE_ACTION_STATE::SKILL2].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].DamageMotion = DAMAGE_MOTION::KNOCK_DOWN;
	//m_ActionDatas[(int)BASE_ACTION_STATE::SKILL2].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].DamageMotion = DAMAGE_MOTION::KNOCK_DOWN;

	//// 判定形状
	//m_ActionDatas[(int)BASE_ACTION_STATE::SKILL2].pAttackData->pCollisionShape->width = 12;
	//m_ActionDatas[(int)BASE_ACTION_STATE::SKILL2].pAttackData->pCollisionShape->height = 6;
	//m_ActionDatas[(int)BASE_ACTION_STATE::SKILL2].pAttackData->pCollisionShape->pos.Set(9, 9, 0);

	//==============================================================================================================
	//	キャラクター固有空中
	// 地上ヒットも空中ヒットも共通の情報
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL_AERIAL].InstanceAttackData();	// アタックデータ作成
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL_AERIAL].pAttackData->damage = 890;
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL_AERIAL].pAttackData->pierceLV = 0;
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL_AERIAL].pAttackData->HitSE = "ヒット3";
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL_AERIAL].pAttackData->WhiffSE = "空振り5";
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL_AERIAL].pAttackData->HitEffectType = EFFECT_TYPE::MULTIPLE_HIT_BLOW;
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL_AERIAL].pAttackData->WhiffEffectType = EFFECT_TYPE::UPPER;
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL_AERIAL].pAttackData->bAntiAir = false;
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL_AERIAL].pAttackData->bFinish = true;
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL_AERIAL].pAttackData->AntiGuard = ANTIGUARD_ATTACK::UP_ATTACK;	// ★しゃがみガードをつぶす
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL_AERIAL].pAttackData->ShakeCameraInfo.Set(.1f, 2);
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL_AERIAL].pAttackData->GuardRecoveryFrame = 15;
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL_AERIAL].pAttackData->fComboRate = 0.85f;
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL_AERIAL].pAttackData->fGuardKnockBackPower = .2f;
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL_AERIAL].pAttackData->fRepeatAttackRate = .99f;// 同技補正の掛かる値

	// 地上ヒットと空中ヒットで挙動が変わるもの
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL_AERIAL].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].bBeInvincible = false;
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL_AERIAL].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].bBeInvincible = false;
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL_AERIAL].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].FlyVector.Set(2.5f, 0.0f);
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL_AERIAL].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].FlyVector.Set(1.2f, 1.85f);
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL_AERIAL].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].iHitStopFrame = 5;
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL_AERIAL].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].iHitStopFrame = 5;
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL_AERIAL].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].HitRecoveryFrame = 60;
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL_AERIAL].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].HitRecoveryFrame = 60;	// (12/26) 同技補正の影響で数値あげることに　ラッシュ系は補正は最初の一回だけでいいかも
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL_AERIAL].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].DamageMotion = DAMAGE_MOTION::KNOCK_BACK;
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL_AERIAL].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].DamageMotion = DAMAGE_MOTION::KNOCK_DOWN;

	// 判定形状
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL_AERIAL].pAttackData->pCollisionShape->width = 12;
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL_AERIAL].pAttackData->pCollisionShape->height = 11;
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL_AERIAL].pAttackData->pCollisionShape->pos.Set(6, 12, 0);

	////==============================================================================================================
	////	キャラクター固有空中攻撃の2段目
	//// 地上ヒットも空中ヒットも共通の情報
	//m_ActionDatas[(int)BASE_ACTION_STATE::SKILL_AERIALDROP].InstanceAttackData();	// アタックデータ作成
	//m_ActionDatas[(int)BASE_ACTION_STATE::SKILL_AERIALDROP].pAttackData->damage = 600;
	//m_ActionDatas[(int)BASE_ACTION_STATE::SKILL_AERIALDROP].pAttackData->pierceLV = 0;
	//m_ActionDatas[(int)BASE_ACTION_STATE::SKILL_AERIALDROP].pAttackData->HitSE = "ヒット6";
	//m_ActionDatas[(int)BASE_ACTION_STATE::SKILL_AERIALDROP].pAttackData->WhiffSE = "空振り5";
	//m_ActionDatas[(int)BASE_ACTION_STATE::SKILL_AERIALDROP].pAttackData->HitEffectType = EFFECT_TYPE::DAMAGE;
	//m_ActionDatas[(int)BASE_ACTION_STATE::SKILL_AERIALDROP].pAttackData->WhiffEffectType = EFFECT_TYPE::DROP_IMPACT_SUPER;
	//m_ActionDatas[(int)BASE_ACTION_STATE::SKILL_AERIALDROP].pAttackData->bAntiAir = false;
	//m_ActionDatas[(int)BASE_ACTION_STATE::SKILL_AERIALDROP].pAttackData->bFinish = true;
	//m_ActionDatas[(int)BASE_ACTION_STATE::SKILL_AERIALDROP].pAttackData->ShakeCameraInfo.Set(.3f, 3);
	//m_ActionDatas[(int)BASE_ACTION_STATE::SKILL_AERIALDROP].pAttackData->GuardRecoveryFrame = 25;
	//m_ActionDatas[(int)BASE_ACTION_STATE::SKILL_AERIALDROP].pAttackData->fComboRate = 0.85f;
	//// 地上ヒットと空中ヒットで挙動が変わるもの
	//m_ActionDatas[(int)BASE_ACTION_STATE::SKILL_AERIALDROP].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].bBeInvincible = false;
	//m_ActionDatas[(int)BASE_ACTION_STATE::SKILL_AERIALDROP].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].bBeInvincible = false;
	//m_ActionDatas[(int)BASE_ACTION_STATE::SKILL_AERIALDROP].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].FlyVector.Set(1.5f, 1.5f);
	//m_ActionDatas[(int)BASE_ACTION_STATE::SKILL_AERIALDROP].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].FlyVector.Set(1.5f, 1.5f);
	//m_ActionDatas[(int)BASE_ACTION_STATE::SKILL_AERIALDROP].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].iHitStopFrame = 8;
	//m_ActionDatas[(int)BASE_ACTION_STATE::SKILL_AERIALDROP].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].iHitStopFrame = 8;
	//m_ActionDatas[(int)BASE_ACTION_STATE::SKILL_AERIALDROP].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].HitRecoveryFrame = 45;
	//m_ActionDatas[(int)BASE_ACTION_STATE::SKILL_AERIALDROP].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].HitRecoveryFrame = 45;
	//m_ActionDatas[(int)BASE_ACTION_STATE::SKILL_AERIALDROP].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].DamageMotion = DAMAGE_MOTION::KNOCK_BACK;
	//m_ActionDatas[(int)BASE_ACTION_STATE::SKILL_AERIALDROP].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].DamageMotion = DAMAGE_MOTION::KNOCK_DOWN;
	//// 判定形状
	//m_ActionDatas[(int)BASE_ACTION_STATE::SKILL_AERIALDROP].pAttackData->pCollisionShape->width = 12;
	//m_ActionDatas[(int)BASE_ACTION_STATE::SKILL_AERIALDROP].pAttackData->pCollisionShape->height = 12;
	//m_ActionDatas[(int)BASE_ACTION_STATE::SKILL_AERIALDROP].pAttackData->pCollisionShape->pos.Set(0, .25f, 0);


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
	m_ActionDatas[(int)BASE_ACTION_STATE::THROW].pAttackData->pCollisionShape->width = 4;
	m_ActionDatas[(int)BASE_ACTION_STATE::THROW].pAttackData->pCollisionShape->height = 4;
	m_ActionDatas[(int)BASE_ACTION_STATE::THROW].pAttackData->pCollisionShape->pos.Set(2, 8, 0);

	//==============================================================================================================
	//	投げ成功
	// 地上ヒットも空中ヒットも共通の情報
	m_ActionDatas[(int)BASE_ACTION_STATE::THROW_SUCCESS].InstanceAttackData();	// アタックデータ作成
	m_ActionDatas[(int)BASE_ACTION_STATE::THROW_SUCCESS].pAttackData->damage = 800;
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
	m_ActionDatas[(int)BASE_ACTION_STATE::THROW_SUCCESS].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].FlyVector.Set(1.2f, 2.4f);
	m_ActionDatas[(int)BASE_ACTION_STATE::THROW_SUCCESS].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].FlyVector.Set(1.3f, 2.5f);
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
	m_ActionDatas[(int)BASE_ACTION_STATE::HEAVEHO_DRIVE].pAttackData->damage = 2000;
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
	m_ActionDatas[(int)BASE_ACTION_STATE::HEAVEHO_DRIVE].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].FlyVector.Set(2.5f, 1.85f);
	m_ActionDatas[(int)BASE_ACTION_STATE::HEAVEHO_DRIVE].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].FlyVector.Set(2.5f, 1.85f);
	m_ActionDatas[(int)BASE_ACTION_STATE::HEAVEHO_DRIVE].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].iHitStopFrame = 12;
	m_ActionDatas[(int)BASE_ACTION_STATE::HEAVEHO_DRIVE].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].iHitStopFrame = 12;
	m_ActionDatas[(int)BASE_ACTION_STATE::HEAVEHO_DRIVE].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].HitRecoveryFrame = 80;
	m_ActionDatas[(int)BASE_ACTION_STATE::HEAVEHO_DRIVE].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].HitRecoveryFrame = 80;
	m_ActionDatas[(int)BASE_ACTION_STATE::HEAVEHO_DRIVE].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].DamageMotion = DAMAGE_MOTION::KNOCK_DOWN;
	m_ActionDatas[(int)BASE_ACTION_STATE::HEAVEHO_DRIVE].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].DamageMotion = DAMAGE_MOTION::KNOCK_DOWN;
	// 判定形状
	m_ActionDatas[(int)BASE_ACTION_STATE::HEAVEHO_DRIVE].pAttackData->pCollisionShape->width = 8;
	m_ActionDatas[(int)BASE_ACTION_STATE::HEAVEHO_DRIVE].pAttackData->pCollisionShape->height = 8;
	m_ActionDatas[(int)BASE_ACTION_STATE::HEAVEHO_DRIVE].pAttackData->pCollisionShape->pos.Set(4, 4, 0);

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
	m_ActionDatas[(int)BASE_ACTION_STATE::HEAVEHO_DRIVE_OVERFLOW].pAttackData->AntiGuard = ANTIGUARD_ATTACK::NONE;	// 中断攻撃
	m_ActionDatas[(int)BASE_ACTION_STATE::HEAVEHO_DRIVE_OVERFLOW].pAttackData->ShakeCameraInfo.Set(0, 0);
	m_ActionDatas[(int)BASE_ACTION_STATE::HEAVEHO_DRIVE_OVERFLOW].pAttackData->GuardRecoveryFrame = 18;
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
	m_ActionDatas[(int)BASE_ACTION_STATE::HEAVEHO_DRIVE_OVERFLOW].pAttackData->pCollisionShape->width = 15;
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
			for (int frame = 0;; frame++)if (m_ActionFrameList[i][frame] == FRAME_STATE::ACTIVE) { m_ActionDatas[i].pAttackData->WhiffDelayFrame = frame-1; break; }
		}
	}


	// スキルアクションの初期化(★★★ここに書くのは、この中でAttackDataを参照するから)
	m_pSkillActions[(int)SKILL_ACTION_TYPE::LAND] = new SkillAction::Land(this);
	m_pSkillActions[(int)SKILL_ACTION_TYPE::LAND2] = nullptr;//new SkillAction::Land2(this);
	m_pSkillActions[(int)SKILL_ACTION_TYPE::LAND3] = nullptr;
	m_pSkillActions[(int)SKILL_ACTION_TYPE::LAND4] = nullptr;
	m_pSkillActions[(int)SKILL_ACTION_TYPE::SQUAT] = new SkillAction::Squat(this);
	m_pSkillActions[(int)SKILL_ACTION_TYPE::AERIAL] = new SkillAction::Land(this);
	m_pSkillActions[(int)SKILL_ACTION_TYPE::AERIAL2] = nullptr;// new SkillAction::AerialDrop(this);
	m_pSkillActions[(int)SKILL_ACTION_TYPE::AERIALDROP] = new SkillAction::Aerial(this);

	//==============================================================================================================
	//	ショット
	// 地上ヒットも空中ヒットも共通の情報
	m_pShotData = new AttackData;
	m_pShotData->damage = 300;
	m_pShotData->pierceLV = 0;
	m_pShotData->HitSE = "被弾";
	m_pShotData->WhiffSE = "空振り1";
	m_pShotData->HitEffectType = EFFECT_TYPE::DAMAGE;
	m_pShotData->WhiffEffectType = EFFECT_TYPE::WHIFF;
	m_pShotData->bAntiAir = true;
	m_pShotData->bFinish = true;
	m_pShotData->AntiGuard = ANTIGUARD_ATTACK::NONE;
	m_pShotData->ShakeCameraInfo.Set(.2f, 2);
	m_pShotData->GuardRecoveryFrame = 20;
	m_pShotData->fGuardKnockBackPower = .25f;
	m_pShotData->attribute = ATTACK_ATTRIBUTE::BULLET;
	m_pShotData->fComboRate = 1.0f;
	// 地上ヒットと空中ヒットで挙動が変わるもの
	m_pShotData->places[(int)AttackData::HIT_PLACE::LAND].bBeInvincible = false;
	m_pShotData->places[(int)AttackData::HIT_PLACE::AERIAL].bBeInvincible = false;
	m_pShotData->places[(int)AttackData::HIT_PLACE::LAND].FlyVector.Set(.75f, 0);
	m_pShotData->places[(int)AttackData::HIT_PLACE::AERIAL].FlyVector.Set(1.5f, 1.5f);
	m_pShotData->places[(int)AttackData::HIT_PLACE::LAND].iHitStopFrame = 8;
	m_pShotData->places[(int)AttackData::HIT_PLACE::AERIAL].iHitStopFrame = 10;
	m_pShotData->places[(int)AttackData::HIT_PLACE::LAND].HitRecoveryFrame = 35;
	m_pShotData->places[(int)AttackData::HIT_PLACE::AERIAL].HitRecoveryFrame = 45;
	m_pShotData->places[(int)AttackData::HIT_PLACE::LAND].DamageMotion = DAMAGE_MOTION::KNOCK_BACK;
	m_pShotData->places[(int)AttackData::HIT_PLACE::AERIAL].DamageMotion = DAMAGE_MOTION::KNOCK_DOWN;
	// 判定形状
	m_pShotData->pCollisionShape->width = 4;
	m_pShotData->pCollisionShape->height = 4;
	m_pShotData->pCollisionShape->pos.Set(0, 0, 0);

	m_pBullet = new  NinjinShotEffect();
	m_pUSABullet = new  USAShotEffect();
}

Balance::~Balance()
{
	FOR((int)SKILL_ACTION_TYPE::MAX) SAFE_DELETE(m_pSkillActions[i])

		SAFE_DELETE(m_pCanonEffect);
		SAFE_DELETE(m_pNozzleFlashEffect);
		SAFE_DELETE(m_pBlueImpact);
		SAFE_DELETE(m_pHeartEffect);

		//SAFE_DELETE(m_pCounterEffect);
		//SAFE_DELETE(m_pStepEffect);

	SAFE_DELETE(m_pShotData);
	SAFE_DELETE(m_pBullet);
	SAFE_DELETE(m_pUSABullet);

	SAFE_DELETE(m_pNinjin);
}

void Balance::Update(PLAYER_UPDATE flag)
{
	// 基底クラスの更新
	BasePlayer::Update(flag);

	// 専用エフェクト
	m_pCanonEffect->Update();
	m_pNozzleFlashEffect->Update();
	m_pBlueImpact->Update();
	m_pHeartEffect->Update();
	//m_pCounterEffect->Update();
	//m_pStepEffect->Update();

}

void Balance::Render()
{
	// 基底クラスの更新
	BasePlayer::Render();

	// 専用エフェクト
	m_pCanonEffect->Render();
	m_pNozzleFlashEffect->Render();
	m_pBlueImpact->Render();
	m_pHeartEffect->Render3D();
	//m_pCounterEffect->Render();
	//m_pStepEffect->Render();

}

void Balance::RenderDrive()
{
	// 基底クラスの更新
	BasePlayer::RenderDrive();
}

void Balance::ChangeColor(COLOR_TYPE eColType)
{
	LPSTR path = "none";

	switch (eColType)
	{
	case COLOR_TYPE::NORMAL:
		path = ("DATA/CHR/Balance/MiMiko_D.png");
		break;
	case COLOR_TYPE::EXTRA:
		path = ("DATA/CHR/Balance/MiMiko_Dot2.png");
		break;
	case COLOR_TYPE::MIRROR:
		path = ("DATA/CHR/Balance/NMiMiko_D3.png");
		break;
	default:
		MyAssert(0, "存在しないカラータイプ");
		break;
	}

	m_pDefaultObj->SetTexture(tdnTexture::Load(path), 0);
	m_pHHDOFObj->SetTexture(tdnTexture::Load(path), 0);
	
}

void Balance::InitMotionDatas()
{
	m_iMotionNumbers[(int)MOTION_TYPE::WAIT] = 0;
	m_iMotionNumbers[(int)MOTION_TYPE::WALK] = 1;
	m_iMotionNumbers[(int)MOTION_TYPE::RUN] = 2;
	m_iMotionNumbers[(int)MOTION_TYPE::BACK_WALK] = 3;
	m_iMotionNumbers[(int)MOTION_TYPE::BACK_STEP] = 4;
	m_iMotionNumbers[(int)MOTION_TYPE::RUSH_ATTACK1] = 5;
	m_iMotionNumbers[(int)MOTION_TYPE::RUSH_ATTACK2] = 6;
	m_iMotionNumbers[(int)MOTION_TYPE::RUSH_ATTACK3] = 7;
	//m_iMotionNumbers[(int)MOTION_TYPE::SKILL2] = 8;
	m_iMotionNumbers[(int)MOTION_TYPE::FINISH_ATTACK] = 7;
	m_iMotionNumbers[(int)MOTION_TYPE::SQUAT] = 8;//10;
	m_iMotionNumbers[(int)MOTION_TYPE::ANTI_AIR_ATTACK] = 9;
	m_iMotionNumbers[(int)MOTION_TYPE::DOKKOI_ATTACK] = 10;
	m_iMotionNumbers[(int)MOTION_TYPE::SQUAT_ATTACK] = 11;
	m_iMotionNumbers[(int)MOTION_TYPE::DOWN_ATTACK] = 12;
	m_iMotionNumbers[(int)MOTION_TYPE::JUMP] = 13;
	m_iMotionNumbers[(int)MOTION_TYPE::FALL] = 14;
	m_iMotionNumbers[(int)MOTION_TYPE::RAND] = 15;
	m_iMotionNumbers[(int)MOTION_TYPE::AERIAL_ATTACK] = 16;
	m_iMotionNumbers[(int)MOTION_TYPE::AERIAL_DASH] = 29;
	m_iMotionNumbers[(int)MOTION_TYPE::PERSONA] = 17;
	m_iMotionNumbers[(int)MOTION_TYPE::SKILL_LAND] = 18;
	m_iMotionNumbers[(int)MOTION_TYPE::SKILL_SQUAT] = 20;
	m_iMotionNumbers[(int)MOTION_TYPE::SKILL_AERIAL] = 21;
	//m_iMotionNumbers[(int)MOTION_TYPE::SKILL_AERIALDROP] = 23;
	m_iMotionNumbers[(int)MOTION_TYPE::HEAVEHO_DRIVE] = 23;
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
	//m_iMotionNumbers[(int)MOTION_TYPE::APPEAR] = 37;
	m_iMotionNumbers[(int)MOTION_TYPE::WIN] = 35;
	m_iMotionNumbers[(int)MOTION_TYPE::BURST] = 37;
}


void Balance::KakeaiInit()
{

}

void Balance::KakeaiUpdate()
{

}

void Balance::KakeaiExit()
{

}

void Balance::ThirdRushInit()
{
	m_eSkillActionType = SKILL_ACTION_TYPE::SQUAT;
	SkillInit();
}

void Balance::ThirdRushUpdate()
{
	// アクション終了フラグ
	if (GetActionState() == BASE_ACTION_STATE::NO_ACTION) return;

	//// ラッシュブロウ
	//if (m_eSkillActionType == SKILL_ACTION_TYPE::LAND)
	//{
	//	// 走り終わってたら
	//	if (m_pSkillActions[(int)SKILL_ACTION_TYPE::LAND]->Execute())
	//	{
	//		// 無理やり走るのを終わらせる
	//		SetActionState(BASE_ACTION_STATE::NO_ACTION);
	//		return;
	//	}

	//	// ヒットしてる状態
	//	if (
	//		//GetAttackData()->bHit && 
	//		GetActionFrame() == FRAME_STATE::FOLLOW)
	//	{
	//		// 攻撃ボタン押したら
	//		if (isPushInputTRG(PLAYER_COMMAND_BIT::C, true))
	//		{
	//			SkillExit();
	//			m_eSkillActionType = SKILL_ACTION_TYPE::LAND2;
	//			SkillInit();
	//		}
	//	}
	//}

	//// 突進攻撃
	//else
	{
		m_pSkillActions[(int)SKILL_ACTION_TYPE::SQUAT]->Execute();

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

}

void Balance::ThirdRushExit()
{
	if (m_eSkillActionType != SKILL_ACTION_TYPE::NO_ACTION)
	{
		m_pSkillActions[(int)m_eSkillActionType]->Exit();
		m_eSkillActionType = SKILL_ACTION_TYPE::NO_ACTION;
	}
}


void Balance::OnInvincibleCounterSuccess()
{
	// カウンター成功のSEとかエフェクトとか
	se->Play("カウンター成功");

	// 突進攻撃設定
	m_eSkillActionType = SKILL_ACTION_TYPE::LAND2;

	// スキルステートへ
	m_pStateMachine->ChangeState(BasePlayerState::Skill::GetInstance());

	// 一定時間無敵にする(だいたい突進攻撃のフレーム)
	SetInvincible(40, 1);

}

void Balance::SkillInit()
{
	m_pSkillActions[(int)m_eSkillActionType]->Enter();
}

bool Balance::SkillUpdate()
{
	const bool bEnd = m_pSkillActions[(int)m_eSkillActionType]->Execute();
	return bEnd;
}

//+---------------------------------
//	無敵用
//+---------------------------------
void Balance::InvincibleAttackInit()
{
	se->Play("昇竜");

}

void Balance::InvincibleAttackExit()
{
}

bool Balance::InvincibleAttackUpdate()
{
	if (m_iCurrentActionFrame == 6)
	{
		m_vMove.y = 2.5f;
		m_vMove.x = GetDirVecX()*0.45f;

		// アッパーラインエフェクト
		Vector3 l_vMoveVec = m_vMove;
		l_vMoveVec.Normalize();
		float l_fZAngle = atan2(-l_vMoveVec.x, l_vMoveVec.y);
		//m_pUpperLineEffect->Action(GetFlontPos(), 1, 1, Vector3(0, 0, l_fZAngle), Vector3(0, 0, l_fZAngle));
		//m_pCycloneEffect->Action(GetFlontPos(), 1.5f, 3.5f, Vector3(0, 0, l_fZAngle), Vector3(0, 0, l_fZAngle));

	}
	
	if (m_iCurrentActionFrame >= 50)
	{

		if (GetObj()->GetMotion() != m_iMotionNumbers[(int)MOTION_TYPE::RAND])
		{
			if (isLand() == true)
			{
				// 着地エフェクト！
				AddEffectAction(GetPos(), EFFECT_TYPE::DROP_IMPACT);

				SetMotion(m_iMotionNumbers[(int)MOTION_TYPE::RAND]);
			}
		}
	}

	//// 昇竜から必殺キャンセル
	//if (GetAttackData()->bHit)
	//{
	//	// 必殺技キャンセル
	//	if (HeaveHoCancel(this)) return false;

	//}

	return false;
}

void Balance::SkillExit()
{
	m_pSkillActions[(int)m_eSkillActionType]->Exit();
}


Balance::SkillAction::Base::Base(Balance *pBalance, BASE_ACTION_STATE state) :m_pBalance(pBalance)
{
	if (pBalance->m_ActionDatas[(int)state].isAttackData())
	{
		memcpy_s(&m_OrgHitSquare, sizeof(CollisionShape::Square), pBalance->m_ActionDatas[(int)state].pAttackData->pCollisionShape, sizeof(CollisionShape::Square));

		//m_OrgDamage = pBalance->m_ActionDatas[(int)state].pAttackData->damage;
		//m_fOrgComboRate = pBalance->m_ActionDatas[(int)state].pAttackData->fComboRate;
		//m_OrgFlyVector[(int)AttackData::HIT_PLACE::LAND] = pBalance->m_ActionDatas[(int)state].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].FlyVector;
		//m_OrgFlyVector[(int)AttackData::HIT_PLACE::AERIAL] = pBalance->m_ActionDatas[(int)state].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].FlyVector;
	}
}

Balance::SkillAction::Land::Land(Balance *pBalance) :Base(pBalance, BASE_ACTION_STATE::SKILL)
{}

void Balance::SkillAction::Land::Enter()
{
	// モーションに変える
	m_pBalance->SetMotion(MOTION_TYPE::SKILL_LAND);

	// アクションステート変更
	m_pBalance->SetActionState(BASE_ACTION_STATE::FRAMECOUNT);

	// 向き設定
	m_pBalance->SetDirAngle();

	// リセット
	m_iHitCount = 0;

	// 重力の影響受けるな！
	m_pBalance->SetMoveUpdate(false);


}

bool Balance::SkillAction::Land::Execute()
{
	static const int IMPACT_FRAME = 28;

	// 移動終わったら
	if (m_pBalance->m_iCurrentActionFrame > 42)
	{
		return true;
	}
	else if (m_pBalance->m_iCurrentActionFrame == IMPACT_FRAME - 2)
	{
		Vector3 ShotVec;
		// キャラが空中にいるかいないかで向きを変える
		ShotVec.Set(0.5f, 0, 0);
		// ショットの向き逆
		ShotVec.x *= m_pBalance->GetDirVecX();
		// ハートエフェクト
		m_pBalance->m_pHeartEffect->Action(m_pBalance->GetCenterPos() + Vector3(ShotVec.x * 3, 3, -5));
	}
	else if (m_pBalance->m_iCurrentActionFrame == IMPACT_FRAME)
	{
		Vector3 ShotVec;

		// キャラが空中にいるかいないかで向きを変える
		ShotVec.Set(0.5f, 0, 0);
		//if (m_pBalance->isLand() == false)
		//{
		//	ShotVec.Set(0.5f, -0.5f, 0);
		//}
		
	
		// ショットの向き逆
		ShotVec.x *= m_pBalance->GetDirVecX();

		// マズルエフェクト発動！
		m_pBalance->AddEffectAction(m_pBalance->GetCenterPos(), EFFECT_TYPE::MUZZLE_FLASH, ShotVec);

		Shot::Base *pNewShot = nullptr;

		// 玉の情報を格納
		pNewShot = new Shot::Ninjin(m_pBalance, m_pBalance->m_pShotData, m_pBalance->m_pBullet,
			m_pBalance->GetCenterPos() + Vector3(ShotVec.x * 3, 0, 0), ShotVec);

		MsgMgr->Dispatch(0, m_pBalance->GetID(), ENTITY_ID::SHOT_MGR, MESSAGE_TYPE::ADD_SHOT, pNewShot);

	}


	return false;
}

void Balance::SkillAction::Land::Exit()
{
	// 攻撃情報を元に戻す
	//m_pBalance->m_ActionDatas[(int)BASE_ACTION_STATE::SKILL].pAttackData->damage = m_OrgDamage;
	//m_pBalance->m_ActionDatas[(int)BASE_ACTION_STATE::SKILL].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].FlyVector = m_OrgFlyVector[(int)AttackData::HIT_PLACE::LAND];
	//m_pBalance->m_ActionDatas[(int)BASE_ACTION_STATE::SKILL].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].FlyVector = m_OrgFlyVector[(int)AttackData::HIT_PLACE::AERIAL];
	//m_pBalance->m_ActionDatas[(int)BASE_ACTION_STATE::SKILL].pAttackData->fComboRate = m_fOrgComboRate;

	// 先行入力リセット
	m_pBalance->AheadCommandReset();


	// 重力のもどって！
	m_pBalance->SetMoveUpdate(true);


}


Balance::SkillAction::Squat::Squat(Balance *pBalance) :Base(pBalance, BASE_ACTION_STATE::SKILL_SQUAT)
{}

void Balance::SkillAction::Squat::Enter()
{
	// アクションセット
	m_pBalance->SetActionState(BASE_ACTION_STATE::SKILL_SQUAT);

	// モーションセット
	m_pBalance->SetMotion(MOTION_TYPE::SKILL_SQUAT);

	// 自体の横幅を増やす
	m_pBalance->m_tagCharacterParam.HitSquare.width = 5.5f;

	// 走るエフェクト
	//m_pBalance->AddEffectAction(m_pBalance->GetPos(), EFFECT_TYPE::RUN);
	


	// ステップエフェクト
	float l_fStepAngle = 0.0f;
	if (m_pBalance->GetDir() == DIR::RIGHT) 
	{
		l_fStepAngle = 3.14f;
	}

	//m_pBalance->m_pStepEffect->Action(m_pBalance->GetFlontPos(),
	//	0.85f, 1.0f, Vector3(0, 0, l_fStepAngle), Vector3(0, 0, l_fStepAngle));

}

bool Balance::SkillAction::Squat::Execute()
{
	// 攻撃ステート終わったら
	if (!m_pBalance->isAttackState())
	{
		return true;
	}

	// 青いブースト発動タイミング
	if (m_pBalance->m_iCurrentActionFrame == m_pBalance->m_iBlueImpactActFrame) 
	{
		Vector3 l_vMoveVec = Vector3(-(m_pBalance->GetDirVecX()), 0, 0);
		l_vMoveVec.Normalize();
		float l_fZAngle = atan2(-l_vMoveVec.x, l_vMoveVec.y);
		m_pBalance->m_pBlueImpact->Action(m_pBalance->GetCenterPos() + Vector3(m_pBalance->GetDirVecX()*-2, 1, 0), 1, 2, Vector3(0, 0, l_fZAngle), Vector3(0, 0, l_fZAngle));

		// 青いポイントライト追加
		PointLightMgr->AddPointLight(m_pBalance->GetCenterPos(),
			Vector3(.0f, 0.25f, .95f), 60, 10, 10, 0, 6);// ポイントライトエフェクト！

	}

	// フォロー以降、横幅をもどす
	if (m_pBalance->GetActionFrame() == FRAME_STATE::FOLLOW)
		m_pBalance->m_tagCharacterParam.HitSquare.width = m_pBalance->m_tagOrgCharacterParam.HitSquare.width;


	// 横に移動
	if (m_pBalance->isActiveFrame())
	{
		m_pBalance->SetMove(Vector3(m_pBalance->m_dir == DIR::LEFT ? -4.5f : 4.5f, 0, 0));
	}
	else
	{
		// 移動量をぐいーって戻す
		if (m_pBalance->isLand() == true)
		{
			m_pBalance->SetMove(m_pBalance->GetMove() * .85f);
		}
		else 
		{
			// 空中は慣性そのまま
			m_pBalance->SetMove(Vector3(m_pBalance->GetMove().x * 0.93f, m_pBalance->GetMove().y, m_pBalance->GetMove().z));
		}

	}

	// キャンセルルート
	if (m_pBalance->isHitAttack())
	{
		// 必殺技キャンセル
		HeaveHoCancel(m_pBalance);
	}

	return false;
}

void Balance::SkillAction::Squat::Exit()
{
	// 横幅をもどす
	m_pBalance->m_tagCharacterParam.HitSquare.width = m_pBalance->m_tagOrgCharacterParam.HitSquare.width;

}




Balance::SkillAction::Aerial::Aerial(Balance *pBalance) :Base(pBalance, BASE_ACTION_STATE::SKILL_AERIAL), m_ActiveFrame(0)
{}

void Balance::SkillAction::Aerial::Enter()
{
	// モーションに変える
	m_pBalance->SetMotion(MOTION_TYPE::SKILL_AERIAL);

	// アクションステート変更
	m_pBalance->SetActionState(BASE_ACTION_STATE::SKILL_AERIAL);

	// 移動量を消す
	//m_pBalance->SetMove(Vector3((m_pBalance->GetDir() == DIR::LEFT) ? -1.8f : 1.8f, 6.0f, 0));

	// 重力の影響受けるな！
	m_pBalance->SetMoveUpdate(false);

	m_pBalance->AddMove(Vector3(0.0f, .5f, 0.0f));

	// フレーム初期化
	m_ActiveFrame = 0;

	// 攻撃情報を元に戻す
	//m_pBalance->m_ActionDatas[(int)BASE_ACTION_STATE::SKILL_AERIAL].pAttackData->damage = m_OrgDamage;
	//m_pBalance->m_ActionDatas[(int)BASE_ACTION_STATE::SKILL_AERIAL].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].FlyVector = m_OrgFlyVector[(int)AttackData::HIT_PLACE::LAND];
	//m_pBalance->m_ActionDatas[(int)BASE_ACTION_STATE::SKILL_AERIAL].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].FlyVector = m_OrgFlyVector[(int)AttackData::HIT_PLACE::AERIAL];
	//m_pBalance->m_ActionDatas[(int)BASE_ACTION_STATE::SKILL_AERIAL].pAttackData->fComboRate = m_fOrgComboRate;

	// 初段だけヒットストップ多め
	//m_pBalance->m_ActionDatas[(int)BASE_ACTION_STATE::SKILL_AERIAL].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].iHitStopFrame = 5;
	//m_pBalance->m_ActionDatas[(int)BASE_ACTION_STATE::SKILL_AERIAL].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].iHitStopFrame = 5;
}

bool Balance::SkillAction::Aerial::Execute()
{
	// 攻撃ステート終わったら
	if (!m_pBalance->isAttackState())
	{
		return true;
	}

	if (m_pBalance->GetAttackData()->bHit)
	{
		// 必殺技キャンセル
		if (HeaveHoCancel(m_pBalance)) return false;

	}


	return false;
}

void Balance::SkillAction::Aerial::Exit()
{}

//
//Balance::SkillAction::AerialDrop::AerialDrop(Balance *pBalance) :Base(pBalance, BASE_ACTION_STATE::SKILL_AERIALDROP)
//{}
//
//void Balance::SkillAction::AerialDrop::Enter()
//{
//	// モーションに変える
//	m_pBalance->SetMotion(MOTION_TYPE::SKILL_AERIALDROP);
//
//	// アクションステート変更
//	m_pBalance->SetActionState(BASE_ACTION_STATE::SKILL_AERIALDROP);
//
//	se->Play("空振り6");
//}
//
//bool Balance::SkillAction::AerialDrop::Execute()
//{
//	// 攻撃ステート終わったら
//	if (!m_pBalance->isAttackState())
//	{
//		return true;
//	}
//
//	if (m_pBalance->GetAttackData()->bHit)
//	{
//		// 必殺技キャンセル
//		if (HeaveHoCancel(m_pBalance)) return false;
//	}
//
//	return false;
//}
//
//void Balance::SkillAction::AerialDrop::Exit()
//{}



void Balance::HeavehoDriveInit()
{
	// カットイン
	SIDE data(m_side);
	MsgMgr->Dispatch(0, ENTITY_ID::UI_MGR, ENTITY_ID::UI_MGR,
		MESSAGE_TYPE::OVER_DRIVE_CUTIN, &data);

	// モーションに変える
	SetMotion(MOTION_TYPE::HEAVEHO_DRIVE);

	// アクションステート変更
	SetActionState(BASE_ACTION_STATE::FRAMECOUNT);

	// 重力の影響受けるな！
	SetMoveUpdate(false);

	// Move値無効
	m_vMove = VECTOR_ZERO;
	float l_fXVec;
	l_fXVec = m_pTargetPlayer->GetPos().x - this->GetPos().x;
	// Normalize();

	// 逆方向に下がりながら発射
	if (abs(l_fXVec) <= 25)
	{
		m_vMove.x += -GetDirVecX()*3.25f;
	}

	// ストップタイマー
	SetGameTimerStopFlag(true);

	m_iHeavehoStopTimer = 113;
}

bool Balance::HeavehoDriveUpdate()
{
	static const int EF_STOP_FRAME = 117;

	// 移動終わったら
	if (m_iCurrentActionFrame > 180)
	{
		return true;
	}


	// 時が止まっている間
	if (m_iHeavehoStopTimer > 0)
	{

		// 時止めが終わった瞬間
		if (--m_iHeavehoStopTimer == 0)
		{
			Shot::Base *pNewShot = nullptr;

			Vector3 ShotVec;

			// キャラが空中にいるかいないかで向きを変える
			ShotVec.Set(1.0f, 0, 0);
			// ショットの向き逆
			ShotVec.x = GetDirVecX();

			// 玉の情報を格納
			pNewShot = new Shot::USAShot(
				this,
				m_ActionDatas[(int)BASE_ACTION_STATE::HEAVEHO_DRIVE].pAttackData,
				m_pUSABullet,
				GetCenterPos(), ShotVec);

			MsgMgr->Dispatch(0, GetID(), ENTITY_ID::SHOT_MGR, MESSAGE_TYPE::ADD_SHOT, pNewShot);


			Vector3 l_vMoveVec = Vector3(GetDirVecX(), 0, 0);
			l_vMoveVec.Normalize();
			float l_fZAngle = atan2(-l_vMoveVec.x, l_vMoveVec.y);
			m_pNozzleFlashEffect->Action(GetCenterPos(), 1.0f, 1.25f, Vector3(0, 0, l_fZAngle), Vector3(0, 0, l_fZAngle));

			// マズルエフェクト発動！
			AddEffectAction(GetCenterPos(), EFFECT_TYPE::MUZZLE_FLASH, ShotVec);


			SetGameTimerStopFlag(false);
		}
	}

	if (m_iCurrentActionFrame == EF_STOP_FRAME)
	{
		Vector3 ShotVec;
		// キャラが空中にいるかいないかで向きを変える
		ShotVec.Set(0.5f, 0, 0);
		// ショットの向き逆
		ShotVec.x *= GetDirVecX();
		// ハートエフェクト
		m_pHeartEffect->Action(GetCenterPos() + Vector3(ShotVec.x * 14, 3, -5));


		m_pNozzleFlashEffect->Stop();
	}


	return false;


	//// 攻撃終わったら
	//if (!isAttackState())
	//{
	//	// ここに来たら終わり
	//	return true;
	//}
	//
	//if (m_iCurrentActionFrame == 132)
	//{
	//	m_pCanonEffect->Stop();
	//	m_pNozzleFlashEffect->Stop();
	//}

	//// 時が止まっている間
	//if (m_iHeavehoStopTimer > 0)
	//{

	//	// 時止めが終わった瞬間
	//	if (--m_iHeavehoStopTimer == 0)
	//	{
	//		// しょおおおおおおりゅううううけんん！！！！！ウーハー！小杉
	//		//m_vMove.Set((m_dir == DIR::LEFT) ? -.5f : .5f, 2.5f, 0);

	//		// アッパーラインエフェクト
	//		Vector3 l_vMoveVec = Vector3(GetDirVecX() , 0, 0);
	//		l_vMoveVec.Normalize();
	//		float l_fZAngle = atan2(-l_vMoveVec.x, l_vMoveVec.y);
	//		m_pCanonEffect->Action(GetCenterPos(), 1.25, 1.25, Vector3(0, 0, l_fZAngle), Vector3(0, 0, l_fZAngle));
	//		m_pNozzleFlashEffect->Action(GetCenterPos(), 1.0f, 1.25f, Vector3(0, 0, l_fZAngle), Vector3(0, 0, l_fZAngle));


	//		Shot::Base *pNewShot = nullptr;

	//		Vector3 ShotVec;

	//		// キャラが空中にいるかいないかで向きを変える
	//		ShotVec.Set(0.5f, 0, 0);
	//		// ショットの向き逆
	//		ShotVec.x = GetDirVecX();

	//		// 玉の情報を格納
	//		pNewShot = new Shot::USAShot(this, m_ActionDatas[(int)BASE_ACTION_STATE::HEAVEHO_DRIVE].pAttackData, m_pUSABullet,
	//			GetPos() + Vector3(-ShotVec.x * 2, 5, 0), ShotVec);

	//		MsgMgr->Dispatch(0, GetID(), ENTITY_ID::SHOT_MGR, MESSAGE_TYPE::ADD_SHOT, pNewShot);


	//		SetGameTimerStopFlag(false);
	//	}
	//}

	//return false;
}

void Balance::HeavehoDriveExit()
{
	// 重力早くもどって！
	SetMoveUpdate(true);

}

void Balance::HeavehoDriveHitEvent()
{
	// 基底クラスを呼んであげる
	BasePlayer::HeavehoDriveHitEvent();

}


void Balance::HeavehoDriveOverFlowInit()
{
	// 重力の影響受けるな！
	SetMoveUpdate(false);

	// ストップタイマー
	SetGameTimerStopFlag(true);

	m_iHeavehoStopTimer = 110;
}
void Balance::HeavehoDriveOverFlowExit()
{

}
void Balance::HeavehoDriveOverFlowUpdate()
{
	/* 実質、必殺がヒットするまでの間 */
	// ヒーホーストップ時間計測
	if (m_iHeavehoStopTimer > 0) if (--m_iHeavehoStopTimer == 0) SetGameTimerStopFlag(false);
}
void Balance::HeavehoDriveOverFlowSuccessInit()
{
	// スピードラインON
	m_pSpeedLine->ActionRoop(Vector3(0, 0, 0), 1, 1, Vector3(0, 0, 0), Vector3(0, 0, 0));
}
void Balance::HeavehoDriveOverFlowSuccessUpdate()
{
	/* 必殺ヒットして演出中 */
	// ボーン追従
	{
		// ローカルマトリックスにモデルのトランスマトリックスを掛けて
		// ローカルからワールド座標へ返還する
		// マトリックスに代入したらアングルとか行けそう
		D3DXMATRIX WorldMat = *m_pObj->GetBone(22) * m_pObj->TransMatrix;

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

		//m_pBalancePlayer->GetObj()->TransMatrix._31 = 0;
		//m_pBalancePlayer->GetObj()->TransMatrix._32 = 0;
		//m_pBalancePlayer->GetObj()->TransMatrix._33 = 1;
		//m_pBalancePlayer->GetObj()->TransMatrix._41 = 0;
		//m_pBalancePlayer->GetObj()->TransMatrix._42 = 0;
		//m_pBalancePlayer->GetObj()->TransMatrix._43 = 0;
	}

	// コンボ
	//COMBO_DESK comboDesk;

	switch (m_iHeaveHoDriveOverFlowFrame++)
	{
	case 170:	// HIT
		se->Play("ヒット3");
		break;

	case 220:	// HIT
		se->Play("ヒット3");
		break;

	case 270:	// HIT
		se->Play("ヒット3");
		break;

	case 300:	// HIT
		se->Play("ヒット3");
		break;

	case 330:	// HIT
		se->Play("ヒット3");
		break;

	case 360:	// HIT
		se->Play("ヒット3");
		break;

	case 390:	// HIT
		se->Play("ヒット3");
		break;

	case 400:	// HIT
		se->Play("ヒット3");
		break;

	case 410:	// HIT
		se->Play("ヒット3");
		break;
	case 420:	// HIT
		se->Play("ヒット3");
		break;
	case 430:	// HIT
		se->Play("ヒット3");
		break;
	case 440:	// HIT
		se->Play("ヒット3");
		break;
	case 450:	// HIT
		se->Play("ヒット3");
		break;
	case 460:	// HIT
		se->Play("ヒット3");
		break;
	case 470:	// HIT
		se->Play("ヒット3");
		break;
	case 480:	// HIT
		se->Play("ヒット3");
		break;
	case 490:	// HIT
		se->Play("ヒット3");
		break;
	case 500:	// HIT
		se->Play("ヒット3");
		break;
	case 510:	// HIT
		se->Play("ヒット3");
		break;
	case 520:	// HIT
		se->Play("ヒット3");
		break;
	case 530:	// HIT
		se->Play("ヒット3");
		break;
	case 540:	// HIT
		se->Play("ヒット3");
		break;
	case 550:	// HIT
		se->Play("ヒット3");
		break;
	case 560:	// HIT
		se->Play("ヒット3");
		break;
	case 570:	// HIT
		se->Play("ヒット3");
		break;
	case 710:
		HeaveHoFinishUI->Action();	// ふぃにっしゅ
		break;
	}

}

// AI
//PLAYER_INPUT Balance::AIHighAttackButton()
//{
//	m_pAI->Update();;
//}