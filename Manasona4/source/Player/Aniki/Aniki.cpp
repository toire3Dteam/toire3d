#include "Aniki.h"
#include "../../Sound/SoundManager.h"
#include "UI/HeaveHoFinish/HeaveHoFinish.h"
#include "../../DeferredEx/DeferredEx.h"
#include "../../BaseEntity/Message/MessageDispatcher.h"
#include "Ketsudram.h"
#include "Window/Player/AnikiWindow.h"
#include "Stand\Stand.h"

Aniki::Aniki(SIDE side, const SideData &data) :BasePlayer(side, data), m_pKetsudram(nullptr), m_eHeaveHoType(HEAVEHO_TYPE::STRIKE)
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
	m_pCommandWindow = new AnikiWindow(l_vWindowPos);


	// エフェクトカメラID
	m_tagCharacterParam.eHeaveHoOverFlowCameraID = EFFECT_CAMERA_ID::TEKI_OVERFLOW;

	// カットインのID
	m_tagCharacterParam.eCutInType = CUTIN_TYPE_NAME::TEKI;

	// 無敵攻撃がかうんたーかどうか
	m_tagCharacterParam.bInvincibleCoutner = false;

	// キャラごとの設定を読み込む
	BasePlayer::LoadCharacterParam("DATA/CHR/Aniki/param.txt");

	m_pName = "Aniki";
	m_pCutinPic = new tdn2DAnim("DATA/UI/Game/OverDriveCutin/aniki.png");
	m_pCutinPic->OrderMoveAppeared(8, -400, +200);

	// フレーム読み込み
	BasePlayer::LoadAttackFrameList("DATA/CHR/Aniki/FrameList.txt");
	// ★フレーム独自改造
	{
		// 地上投げの後のケツドラム〆
		m_ActionFrameList[(int)BASE_ACTION_STATE::SKILL2][125] = FRAME_STATE::RECOVERY_HIT;

		// 対空投げの後のケツドラム〆
		m_ActionFrameList[(int)BASE_ACTION_STATE::SKILL3][125] = FRAME_STATE::RECOVERY_HIT;
	}
	// 兄貴メッシュ
	m_pObj = m_pDefaultObj;

	// 顔グラ
	m_pFacePic = new tdn2DAnim("Data/UI/Game/CharaIcon/AnikiIcon.png");
	//m_pFacePic->OrderNone();
	m_pFacePic->OrderShrink(8, 1, 1.25f);

	// スピードライン
	m_pSpeedLine = new SpeedLineGreenEffect;

	// エフェクト
	m_pSpearEffect = new SpearEffect();
	m_pRayEffect = new RayEffect();
	m_pHandLightEffect = new HandLightEffect();
	m_pAnikiCanonEffect = new AnikiCanonEffect();

	// リセットはなるべく最後に
	Reset();
}

void Aniki::Reset()
{
	// 必ず書く
	BasePlayer::Reset();

	m_bHeavehoHit = false;

	// リスタートすると残るので削除
	SAFE_DELETE(m_pKetsudram);

	m_pSpearEffect->Stop();
	m_pRayEffect->Stop();
	m_pHandLightEffect->Stop();
	m_pAnikiCanonEffect->Stop();

}

void Aniki::InitActionDatas()
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
	m_ActionDatas[(int)BASE_ACTION_STATE::RUSH1].pAttackData->pCollisionShape->pos.Set(6, 9, 0);

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
	m_ActionDatas[(int)BASE_ACTION_STATE::ANTI_AIR].pAttackData->damage = 700;
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
	m_ActionDatas[(int)BASE_ACTION_STATE::ANTI_AIR].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].FlyVector.Set(.2f, 1.9f);
	m_ActionDatas[(int)BASE_ACTION_STATE::ANTI_AIR].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].FlyVector.Set(.1f, 1.5f);
	m_ActionDatas[(int)BASE_ACTION_STATE::ANTI_AIR].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].iHitStopFrame = 4;
	m_ActionDatas[(int)BASE_ACTION_STATE::ANTI_AIR].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].iHitStopFrame = 8;
	m_ActionDatas[(int)BASE_ACTION_STATE::ANTI_AIR].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].HitRecoveryFrame = 24;
	m_ActionDatas[(int)BASE_ACTION_STATE::ANTI_AIR].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].HitRecoveryFrame = 40;
	m_ActionDatas[(int)BASE_ACTION_STATE::ANTI_AIR].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].DamageMotion = DAMAGE_MOTION::KNOCK_DOWN;
	m_ActionDatas[(int)BASE_ACTION_STATE::ANTI_AIR].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].DamageMotion = DAMAGE_MOTION::KNOCK_DOWN;
	// 判定形状
	m_ActionDatas[(int)BASE_ACTION_STATE::ANTI_AIR].pAttackData->pCollisionShape->width = 10;
	m_ActionDatas[(int)BASE_ACTION_STATE::ANTI_AIR].pAttackData->pCollisionShape->height = 9;
	m_ActionDatas[(int)BASE_ACTION_STATE::ANTI_AIR].pAttackData->pCollisionShape->pos.Set(4, 13, 0);

	//==============================================================================================================
	//	中断攻撃(どっこいアタック)
	// 地上ヒットも空中ヒットも共通の情報
	m_ActionDatas[(int)BASE_ACTION_STATE::DOKKOI_ATTACK].InstanceAttackData();	// アタックデータ作成
	m_ActionDatas[(int)BASE_ACTION_STATE::DOKKOI_ATTACK].pAttackData->damage = 1000;
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
	m_ActionDatas[(int)BASE_ACTION_STATE::DOKKOI_ATTACK].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].FlyVector.Set(1.2f, .25f);
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
	m_ActionDatas[(int)BASE_ACTION_STATE::DOWN_ATTACK].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].FlyVector.Set(.35f, .8f);
	m_ActionDatas[(int)BASE_ACTION_STATE::DOWN_ATTACK].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].FlyVector.Set(.25f, .85f);
	m_ActionDatas[(int)BASE_ACTION_STATE::DOWN_ATTACK].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].iHitStopFrame = 11;
	m_ActionDatas[(int)BASE_ACTION_STATE::DOWN_ATTACK].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].iHitStopFrame = 11;
	m_ActionDatas[(int)BASE_ACTION_STATE::DOWN_ATTACK].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].HitRecoveryFrame = 40;
	m_ActionDatas[(int)BASE_ACTION_STATE::DOWN_ATTACK].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].HitRecoveryFrame = 60;
	m_ActionDatas[(int)BASE_ACTION_STATE::DOWN_ATTACK].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].DamageMotion = DAMAGE_MOTION::DOWN_FALL;
	m_ActionDatas[(int)BASE_ACTION_STATE::DOWN_ATTACK].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].DamageMotion = DAMAGE_MOTION::DOWN_FALL;
	// 判定形状
	m_ActionDatas[(int)BASE_ACTION_STATE::DOWN_ATTACK].pAttackData->pCollisionShape->width = 8;
	m_ActionDatas[(int)BASE_ACTION_STATE::DOWN_ATTACK].pAttackData->pCollisionShape->height = 2;
	m_ActionDatas[(int)BASE_ACTION_STATE::DOWN_ATTACK].pAttackData->pCollisionShape->pos.Set(8, 2, 0);


	//==============================================================================================================
	//	空中攻撃
	// 地上ヒットも空中ヒットも共通の情報
	m_ActionDatas[(int)BASE_ACTION_STATE::AERIAL_ATTACK].InstanceAttackData();	// アタックデータ作成
	m_ActionDatas[(int)BASE_ACTION_STATE::AERIAL_ATTACK].pAttackData->damage = 400;
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
	m_ActionDatas[(int)BASE_ACTION_STATE::AERIAL_ATTACK].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].FlyVector.Set(.8f, .1f);
	m_ActionDatas[(int)BASE_ACTION_STATE::AERIAL_ATTACK].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].iHitStopFrame = 7;
	m_ActionDatas[(int)BASE_ACTION_STATE::AERIAL_ATTACK].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].iHitStopFrame = 7;
	m_ActionDatas[(int)BASE_ACTION_STATE::AERIAL_ATTACK].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].HitRecoveryFrame = 27;
	m_ActionDatas[(int)BASE_ACTION_STATE::AERIAL_ATTACK].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].HitRecoveryFrame = 22;
	m_ActionDatas[(int)BASE_ACTION_STATE::AERIAL_ATTACK].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].DamageMotion = DAMAGE_MOTION::KNOCK_BACK;
	m_ActionDatas[(int)BASE_ACTION_STATE::AERIAL_ATTACK].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].DamageMotion = DAMAGE_MOTION::KNOCK_DOWN;
	// 判定形状
	m_ActionDatas[(int)BASE_ACTION_STATE::AERIAL_ATTACK].pAttackData->pCollisionShape->width = 10;
	m_ActionDatas[(int)BASE_ACTION_STATE::AERIAL_ATTACK].pAttackData->pCollisionShape->height = 8;
	m_ActionDatas[(int)BASE_ACTION_STATE::AERIAL_ATTACK].pAttackData->pCollisionShape->pos.Set(7, 8, 0);

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
	m_ActionDatas[(int)BASE_ACTION_STATE::INVINCIBLE_ATTACK].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].FlyVector.Set(.5f, .5f);
	m_ActionDatas[(int)BASE_ACTION_STATE::INVINCIBLE_ATTACK].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].FlyVector.Set(.5f, .5f);
	m_ActionDatas[(int)BASE_ACTION_STATE::INVINCIBLE_ATTACK].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].iHitStopFrame = 25;
	m_ActionDatas[(int)BASE_ACTION_STATE::INVINCIBLE_ATTACK].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].iHitStopFrame = 25;
	m_ActionDatas[(int)BASE_ACTION_STATE::INVINCIBLE_ATTACK].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].HitRecoveryFrame = 60;
	m_ActionDatas[(int)BASE_ACTION_STATE::INVINCIBLE_ATTACK].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].HitRecoveryFrame = 60;
	m_ActionDatas[(int)BASE_ACTION_STATE::INVINCIBLE_ATTACK].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].DamageMotion = DAMAGE_MOTION::KNOCK_DOWN;
	m_ActionDatas[(int)BASE_ACTION_STATE::INVINCIBLE_ATTACK].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].DamageMotion = DAMAGE_MOTION::KNOCK_DOWN;
	// 判定形状
	m_ActionDatas[(int)BASE_ACTION_STATE::INVINCIBLE_ATTACK].pAttackData->pCollisionShape->width = 12;
	m_ActionDatas[(int)BASE_ACTION_STATE::INVINCIBLE_ATTACK].pAttackData->pCollisionShape->height = 8;
	m_ActionDatas[(int)BASE_ACTION_STATE::INVINCIBLE_ATTACK].pAttackData->pCollisionShape->pos.Set(0, 12, 0);


	//==============================================================================================================
	//	スキル攻撃(掴み)
	// 地上ヒットも空中ヒットも共通の情報
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL].InstanceAttackData();	// アタックデータ作成
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL].pAttackData->damage = 0;
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL].pAttackData->pierceLV = 0;
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL].pAttackData->HitSE = "掴み成功";
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL].pAttackData->WhiffSE = "空振り5";
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL].pAttackData->HitEffectType = EFFECT_TYPE::DAMAGE;
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL].pAttackData->WhiffEffectType = EFFECT_TYPE::BACK_STEP;
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL].pAttackData->bAntiAir = false;
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL].pAttackData->bFinish = false;
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL].pAttackData->AntiGuard = ANTIGUARD_ATTACK::ALL_BREAK;
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL].pAttackData->ShakeCameraInfo.Set(0, 0);
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL].pAttackData->GuardRecoveryFrame = 0;
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL].pAttackData->attribute = ATTACK_ATTRIBUTE::THROW;
	// 地上ヒットと空中ヒットで挙動が変わるもの
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].bBeInvincible = false;
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].bBeInvincible = false;
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].FlyVector.Set(.1f, .0f);
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].FlyVector.Set(.15f, .325f);
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].iHitStopFrame = 24;
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].iHitStopFrame = 24;
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].HitRecoveryFrame = 80;
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].HitRecoveryFrame = 110;
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].DamageMotion = DAMAGE_MOTION::KNOCK_BACK;
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].DamageMotion = DAMAGE_MOTION::KNOCK_BACK;
	// 判定形状
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL].pAttackData->pCollisionShape->width = 4;
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL].pAttackData->pCollisionShape->height = 2;
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL].pAttackData->pCollisionShape->pos.Set(4, 2, 0);

	//==============================================================================================================
	//	スキル攻撃(掴んだ後)
	// 地上ヒットも空中ヒットも共通の情報
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL2].InstanceAttackData();	// アタックデータ作成
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL2].pAttackData->damage = 1500;
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL2].pAttackData->pierceLV = 0;
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL2].pAttackData->HitSE = "ヒット3";
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL2].pAttackData->WhiffSE = "空振り5";
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL2].pAttackData->HitEffectType = EFFECT_TYPE::MULTIPLE_HIT_BLOW;
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL2].pAttackData->WhiffEffectType = EFFECT_TYPE::BACK_STEP;
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL2].pAttackData->bAntiAir = false;
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL2].pAttackData->bFinish = true;
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL2].pAttackData->AntiGuard = ANTIGUARD_ATTACK::ALL_BREAK;
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL2].pAttackData->ShakeCameraInfo.Set(.5f, 3);
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL2].pAttackData->GuardRecoveryFrame = 0;
	// 地上ヒットと空中ヒットで挙動が変わるもの
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL2].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].bBeInvincible = false;
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL2].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].bBeInvincible = false;
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL2].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].FlyVector.Set(.1f, .0f);
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL2].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].FlyVector.Set(.15f, .325f);
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL2].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].iHitStopFrame = 0;
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL2].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].iHitStopFrame = 0;
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL2].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].HitRecoveryFrame = 100;
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL2].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].HitRecoveryFrame = 100;
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL2].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].DamageMotion = DAMAGE_MOTION::KNOCK_DOWN;
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL2].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].DamageMotion = DAMAGE_MOTION::KNOCK_DOWN;
	// 判定形状
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL2].pAttackData->pCollisionShape->width = 6;
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL2].pAttackData->pCollisionShape->height = 5;
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL2].pAttackData->pCollisionShape->pos.Set(5, 5, 0);


	//==============================================================================================================
	//	しゃがみ固有スキル(対空)
	// 地上ヒットも空中ヒットも共通の情報
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL_SQUAT].InstanceAttackData();	// アタックデータ作成
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL_SQUAT].pAttackData->damage = 0;
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL_SQUAT].pAttackData->pierceLV = 0;
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL_SQUAT].pAttackData->HitSE = "掴み成功";
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL_SQUAT].pAttackData->WhiffSE = "空振り5";
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL_SQUAT].pAttackData->HitEffectType = EFFECT_TYPE::DAMAGE;
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL_SQUAT].pAttackData->WhiffEffectType = EFFECT_TYPE::WHIFF;
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL_SQUAT].pAttackData->bAntiAir = true;
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL_SQUAT].pAttackData->bFinish = true;
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL_SQUAT].pAttackData->AntiGuard = ANTIGUARD_ATTACK::NONE;
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL_SQUAT].pAttackData->ShakeCameraInfo.Set(0, 0);
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL_SQUAT].pAttackData->GuardRecoveryFrame = 10;
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL_SQUAT].pAttackData->fGuardKnockBackPower = .7f;
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL_SQUAT].pAttackData->fComboRate = 0.85f;
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL_SQUAT].pAttackData->attribute = ATTACK_ATTRIBUTE::AERIAL_THROW;
	// 地上ヒットと空中ヒットで挙動が変わるもの
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL_SQUAT].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].bBeInvincible = false;
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL_SQUAT].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].bBeInvincible = false;
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL_SQUAT].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].FlyVector.Set(-.1f, -.5f);
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL_SQUAT].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].FlyVector.Set(-.1f, -.5f);
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL_SQUAT].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].iHitStopFrame = 8;
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL_SQUAT].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].iHitStopFrame = 5;
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL_SQUAT].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].HitRecoveryFrame = 120;
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL_SQUAT].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].HitRecoveryFrame = 120;
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL_SQUAT].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].DamageMotion = DAMAGE_MOTION::KNOCK_DOWN;
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL_SQUAT].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].DamageMotion = DAMAGE_MOTION::KNOCK_DOWN;

	// 判定形状
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL_SQUAT].pAttackData->pCollisionShape->width = 8;
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL_SQUAT].pAttackData->pCollisionShape->height = 8;
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL_SQUAT].pAttackData->pCollisionShape->pos.Set(8, 27, 0);

	//==============================================================================================================
	//	スキル攻撃(対空で掴んだ後)
	// 地上ヒットも空中ヒットも共通の情報
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL3].InstanceAttackData();	// アタックデータ作成
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL3].pAttackData->damage = 1500;
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL3].pAttackData->pierceLV = 0;
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL3].pAttackData->HitSE = "ヒット3";
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL3].pAttackData->WhiffSE = "空振り5";
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL3].pAttackData->HitEffectType = EFFECT_TYPE::MULTIPLE_HIT_BLOW;
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL3].pAttackData->WhiffEffectType = EFFECT_TYPE::BACK_STEP;
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL3].pAttackData->bAntiAir = false;
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL3].pAttackData->bFinish = true;
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL3].pAttackData->AntiGuard = ANTIGUARD_ATTACK::ALL_BREAK;
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL3].pAttackData->ShakeCameraInfo.Set(.5f, 3);
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL3].pAttackData->GuardRecoveryFrame = 0;
	// 地上ヒットと空中ヒットで挙動が変わるもの
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL3].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].bBeInvincible = false;
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL3].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].bBeInvincible = false;
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL3].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].FlyVector.Set(-.1f, .0f);
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL3].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].FlyVector.Set(-.05f, .325f);
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL3].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].iHitStopFrame = 0;
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL3].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].iHitStopFrame = 0;
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL3].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].HitRecoveryFrame = 100;
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL3].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].HitRecoveryFrame = 100;
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL3].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].DamageMotion = DAMAGE_MOTION::KNOCK_DOWN;
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL3].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].DamageMotion = DAMAGE_MOTION::KNOCK_DOWN;
	// 判定形状
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL3].pAttackData->pCollisionShape->width = 6;
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL3].pAttackData->pCollisionShape->height = 5;
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL3].pAttackData->pCollisionShape->pos.Set(5, 5, 0);

	//==============================================================================================================
	//	スキル攻撃(〆ケツドラム)
	// 地上ヒットも空中ヒットも共通の情報
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL4].InstanceAttackData();	// アタックデータ作成
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL4].pAttackData->damage = 200;
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL4].pAttackData->pierceLV = 0;
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL4].pAttackData->HitSE = "ケツドラム1";
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL4].pAttackData->WhiffSE = "空振り5";
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL4].pAttackData->HitEffectType = EFFECT_TYPE::MULTIPLE_HIT_BLOW;
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL4].pAttackData->WhiffEffectType = EFFECT_TYPE::BACK_STEP;
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL4].pAttackData->bAntiAir = false;
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL4].pAttackData->bFinish = true;
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL4].pAttackData->AntiGuard = ANTIGUARD_ATTACK::ALL_BREAK;
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL4].pAttackData->ShakeCameraInfo.Set(.1f, 3);
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL4].pAttackData->GuardRecoveryFrame = 0;
	// 地上ヒットと空中ヒットで挙動が変わるもの
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL4].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].bBeInvincible = false;
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL4].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].bBeInvincible = false;
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL4].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].FlyVector.Set(-.1f, .0f);
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL4].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].FlyVector.Set(-.05f, .325f);
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL4].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].iHitStopFrame = 0;
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL4].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].iHitStopFrame = 0;
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL4].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].HitRecoveryFrame = 20;
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL4].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].HitRecoveryFrame = 20;
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL4].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].DamageMotion = DAMAGE_MOTION::KNOCK_DOWN;
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL4].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].DamageMotion = DAMAGE_MOTION::KNOCK_DOWN;
	// 判定形状
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL4].pAttackData->pCollisionShape->width = 12;
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL4].pAttackData->pCollisionShape->height = 12;
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL4].pAttackData->pCollisionShape->pos.Set(5, 5, 0);


	//==============================================================================================================
	//	キャラクター固有空中
	// 地上ヒットも空中ヒットも共通の情報
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL_AERIAL].InstanceAttackData();	// アタックデータ作成
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL_AERIAL].pAttackData->damage = 0;
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL_AERIAL].pAttackData->pierceLV = 0;
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL_AERIAL].pAttackData->HitSE = "掴み成功";
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL_AERIAL].pAttackData->WhiffSE = "空振り5";
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL_AERIAL].pAttackData->HitEffectType = EFFECT_TYPE::DAMAGE;
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL_AERIAL].pAttackData->WhiffEffectType = EFFECT_TYPE::WHIFF;
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL_AERIAL].pAttackData->bAntiAir = false;
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL_AERIAL].pAttackData->bFinish = false;
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL_AERIAL].pAttackData->ShakeCameraInfo.Set(.1f, 2);
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL_AERIAL].pAttackData->GuardRecoveryFrame = 15;
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL_AERIAL].pAttackData->fComboRate = 0.85f;
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL_AERIAL].pAttackData->attribute = ATTACK_ATTRIBUTE::AERIAL_THROW;	// 空中投げ属性
	// 地上ヒットと空中ヒットで挙動が変わるもの
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL_AERIAL].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].bBeInvincible = false;
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL_AERIAL].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].bBeInvincible = false;
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL_AERIAL].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].FlyVector.Set(0, 0);
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL_AERIAL].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].FlyVector.Set(0, 0);
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL_AERIAL].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].iHitStopFrame = 5;
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL_AERIAL].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].iHitStopFrame = 5;
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL_AERIAL].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].HitRecoveryFrame = 120;
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL_AERIAL].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].HitRecoveryFrame = 120;	// (12/26) 同技補正の影響で数値あげることに　ラッシュ系は補正は最初の一回だけでいいかも
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL_AERIAL].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].DamageMotion = DAMAGE_MOTION::KNOCK_BACK;
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL_AERIAL].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].DamageMotion = DAMAGE_MOTION::KNOCK_BACK;

	// 判定形状
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL_AERIAL].pAttackData->pCollisionShape->width = 4;
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL_AERIAL].pAttackData->pCollisionShape->height = 8;
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL_AERIAL].pAttackData->pCollisionShape->pos.Set(4, 10, 0);

	//==============================================================================================================
	//	空中スキル攻撃(空中掴んだ後)
	// 地上ヒットも空中ヒットも共通の情報
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL_AERIALDROP].InstanceAttackData();	// アタックデータ作成
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL_AERIALDROP].pAttackData->damage = 1000;
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL_AERIALDROP].pAttackData->pierceLV = 0;
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL_AERIALDROP].pAttackData->HitSE = "ヒット3";
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL_AERIALDROP].pAttackData->WhiffSE = "空振り5";
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL_AERIALDROP].pAttackData->HitEffectType = EFFECT_TYPE::MULTIPLE_HIT_BLOW;
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL_AERIALDROP].pAttackData->WhiffEffectType = EFFECT_TYPE::BACK_STEP;
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL_AERIALDROP].pAttackData->bAntiAir = false;
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL_AERIALDROP].pAttackData->bFinish = true;
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL_AERIALDROP].pAttackData->AntiGuard = ANTIGUARD_ATTACK::ALL_BREAK;
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL_AERIALDROP].pAttackData->ShakeCameraInfo.Set(.5f, 3);
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL_AERIALDROP].pAttackData->GuardRecoveryFrame = 0;
	// 地上ヒットと空中ヒットで挙動が変わるもの
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL_AERIALDROP].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].bBeInvincible = false;
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL_AERIALDROP].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].bBeInvincible = false;
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL_AERIALDROP].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].FlyVector.Set(.2f, -4.0f);
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL_AERIALDROP].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].FlyVector.Set(.3f, -4.325f);
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL_AERIALDROP].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].iHitStopFrame = 3;
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL_AERIALDROP].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].iHitStopFrame = 4;
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL_AERIALDROP].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].HitRecoveryFrame = 46;
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL_AERIALDROP].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].HitRecoveryFrame = 46;
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL_AERIALDROP].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].DamageMotion = DAMAGE_MOTION::KNOCK_DOWN;
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL_AERIALDROP].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].DamageMotion = DAMAGE_MOTION::KNOCK_DOWN;
	// 判定形状
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL_AERIALDROP].pAttackData->pCollisionShape->width = 16;
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL_AERIALDROP].pAttackData->pCollisionShape->height = 15;
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL_AERIALDROP].pAttackData->pCollisionShape->pos.Set(5, 5, 0);


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
	//	ヒーホードライブ(打撃)
	// 地上ヒットも空中ヒットも共通の情報
	m_ActionDatas[(int)BASE_ACTION_STATE::HEAVEHO_DRIVE].InstanceAttackData();	// アタックデータ作成
	m_ActionDatas[(int)BASE_ACTION_STATE::HEAVEHO_DRIVE].pAttackData->damage = 114;
	m_ActionDatas[(int)BASE_ACTION_STATE::HEAVEHO_DRIVE].pAttackData->pierceLV = 0;
	m_ActionDatas[(int)BASE_ACTION_STATE::HEAVEHO_DRIVE].pAttackData->HitSE = "フィニッシュ大";
	m_ActionDatas[(int)BASE_ACTION_STATE::HEAVEHO_DRIVE].pAttackData->WhiffSE = "空振り5";
	m_ActionDatas[(int)BASE_ACTION_STATE::HEAVEHO_DRIVE].pAttackData->HitEffectType = EFFECT_TYPE::FINISH_HIT;
	m_ActionDatas[(int)BASE_ACTION_STATE::HEAVEHO_DRIVE].pAttackData->WhiffEffectType = EFFECT_TYPE::WHIFF;
	m_ActionDatas[(int)BASE_ACTION_STATE::HEAVEHO_DRIVE].pAttackData->bAntiAir = false;
	m_ActionDatas[(int)BASE_ACTION_STATE::HEAVEHO_DRIVE].pAttackData->bFinish = false;
	m_ActionDatas[(int)BASE_ACTION_STATE::HEAVEHO_DRIVE].pAttackData->AntiGuard = ANTIGUARD_ATTACK::NONE;
	m_ActionDatas[(int)BASE_ACTION_STATE::HEAVEHO_DRIVE].pAttackData->ShakeCameraInfo.Set(.5f, 10);
	m_ActionDatas[(int)BASE_ACTION_STATE::HEAVEHO_DRIVE].pAttackData->GuardRecoveryFrame = 18;
	m_ActionDatas[(int)BASE_ACTION_STATE::HEAVEHO_DRIVE].pAttackData->attribute = ATTACK_ATTRIBUTE::STRIKE;
	m_ActionDatas[(int)BASE_ACTION_STATE::HEAVEHO_DRIVE].pAttackData->fComboRate = 0.85f;
	// 地上ヒットと空中ヒットで挙動が変わるもの
	m_ActionDatas[(int)BASE_ACTION_STATE::HEAVEHO_DRIVE].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].bBeInvincible = false;
	m_ActionDatas[(int)BASE_ACTION_STATE::HEAVEHO_DRIVE].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].bBeInvincible = false;
	m_ActionDatas[(int)BASE_ACTION_STATE::HEAVEHO_DRIVE].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].FlyVector.Set(0, 0);
	m_ActionDatas[(int)BASE_ACTION_STATE::HEAVEHO_DRIVE].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].FlyVector.Set(0, 0);
	m_ActionDatas[(int)BASE_ACTION_STATE::HEAVEHO_DRIVE].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].iHitStopFrame = 12;
	m_ActionDatas[(int)BASE_ACTION_STATE::HEAVEHO_DRIVE].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].iHitStopFrame = 12;
	m_ActionDatas[(int)BASE_ACTION_STATE::HEAVEHO_DRIVE].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].HitRecoveryFrame = 2400;
	m_ActionDatas[(int)BASE_ACTION_STATE::HEAVEHO_DRIVE].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].HitRecoveryFrame = 2400;
	m_ActionDatas[(int)BASE_ACTION_STATE::HEAVEHO_DRIVE].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].DamageMotion = DAMAGE_MOTION::KNOCK_DOWN;
	m_ActionDatas[(int)BASE_ACTION_STATE::HEAVEHO_DRIVE].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].DamageMotion = DAMAGE_MOTION::KNOCK_DOWN;
	// 判定形状
	m_ActionDatas[(int)BASE_ACTION_STATE::HEAVEHO_DRIVE].pAttackData->pCollisionShape->width = 6;
	m_ActionDatas[(int)BASE_ACTION_STATE::HEAVEHO_DRIVE].pAttackData->pCollisionShape->height = 8;
	m_ActionDatas[(int)BASE_ACTION_STATE::HEAVEHO_DRIVE].pAttackData->pCollisionShape->pos.Set(6, 6, 0);


	//==============================================================================================================
	//	ヒーホードライブ(投げ必殺)
	// 地上ヒットも空中ヒットも共通の情報
	m_ActionDatas[(int)BASE_ACTION_STATE::HEAVEHO_DRIVE2].InstanceAttackData();	// アタックデータ作成
	m_ActionDatas[(int)BASE_ACTION_STATE::HEAVEHO_DRIVE2].pAttackData->damage = 0;
	m_ActionDatas[(int)BASE_ACTION_STATE::HEAVEHO_DRIVE2].pAttackData->pierceLV = 0;
	m_ActionDatas[(int)BASE_ACTION_STATE::HEAVEHO_DRIVE2].pAttackData->HitSE = "掴み成功";
	m_ActionDatas[(int)BASE_ACTION_STATE::HEAVEHO_DRIVE2].pAttackData->WhiffSE = "空振り5";
	m_ActionDatas[(int)BASE_ACTION_STATE::HEAVEHO_DRIVE2].pAttackData->HitEffectType = EFFECT_TYPE::FINISH_HIT;
	m_ActionDatas[(int)BASE_ACTION_STATE::HEAVEHO_DRIVE2].pAttackData->WhiffEffectType = EFFECT_TYPE::WHIFF;
	m_ActionDatas[(int)BASE_ACTION_STATE::HEAVEHO_DRIVE2].pAttackData->bAntiAir = false;
	m_ActionDatas[(int)BASE_ACTION_STATE::HEAVEHO_DRIVE2].pAttackData->bFinish = false;
	m_ActionDatas[(int)BASE_ACTION_STATE::HEAVEHO_DRIVE2].pAttackData->AntiGuard = ANTIGUARD_ATTACK::ALL_BREAK;
	m_ActionDatas[(int)BASE_ACTION_STATE::HEAVEHO_DRIVE2].pAttackData->ShakeCameraInfo.Set(.5f, 10);
	m_ActionDatas[(int)BASE_ACTION_STATE::HEAVEHO_DRIVE2].pAttackData->GuardRecoveryFrame = 18;
	m_ActionDatas[(int)BASE_ACTION_STATE::HEAVEHO_DRIVE2].pAttackData->attribute = ATTACK_ATTRIBUTE::THROW;
	m_ActionDatas[(int)BASE_ACTION_STATE::HEAVEHO_DRIVE2].pAttackData->fComboRate = 1.0f;
	// 地上ヒットと空中ヒットで挙動が変わるもの
	m_ActionDatas[(int)BASE_ACTION_STATE::HEAVEHO_DRIVE2].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].bBeInvincible = false;
	m_ActionDatas[(int)BASE_ACTION_STATE::HEAVEHO_DRIVE2].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].bBeInvincible = false;
	m_ActionDatas[(int)BASE_ACTION_STATE::HEAVEHO_DRIVE2].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].FlyVector.Set(0, 0);
	m_ActionDatas[(int)BASE_ACTION_STATE::HEAVEHO_DRIVE2].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].FlyVector.Set(0, 0);
	m_ActionDatas[(int)BASE_ACTION_STATE::HEAVEHO_DRIVE2].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].iHitStopFrame = 12;
	m_ActionDatas[(int)BASE_ACTION_STATE::HEAVEHO_DRIVE2].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].iHitStopFrame = 12;
	m_ActionDatas[(int)BASE_ACTION_STATE::HEAVEHO_DRIVE2].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].HitRecoveryFrame = 114514;
	m_ActionDatas[(int)BASE_ACTION_STATE::HEAVEHO_DRIVE2].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].HitRecoveryFrame = 114514;
	m_ActionDatas[(int)BASE_ACTION_STATE::HEAVEHO_DRIVE2].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].DamageMotion = DAMAGE_MOTION::KNOCK_BACK;
	m_ActionDatas[(int)BASE_ACTION_STATE::HEAVEHO_DRIVE2].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].DamageMotion = DAMAGE_MOTION::KNOCK_BACK;
	// 判定形状
	m_ActionDatas[(int)BASE_ACTION_STATE::HEAVEHO_DRIVE2].pAttackData->pCollisionShape->width = 6;
	m_ActionDatas[(int)BASE_ACTION_STATE::HEAVEHO_DRIVE2].pAttackData->pCollisionShape->height = 4;
	m_ActionDatas[(int)BASE_ACTION_STATE::HEAVEHO_DRIVE2].pAttackData->pCollisionShape->pos.Set(6, 4, 0);


	//==============================================================================================================
	//	ヒーホードライブ(投げ必殺成功後)
	// 地上ヒットも空中ヒットも共通の情報
	m_ActionDatas[(int)BASE_ACTION_STATE::HEAVEHO_DRIVE3].InstanceAttackData();	// アタックデータ作成
	m_ActionDatas[(int)BASE_ACTION_STATE::HEAVEHO_DRIVE3].pAttackData->damage = 4000;
	m_ActionDatas[(int)BASE_ACTION_STATE::HEAVEHO_DRIVE3].pAttackData->pierceLV = 0;
	m_ActionDatas[(int)BASE_ACTION_STATE::HEAVEHO_DRIVE3].pAttackData->HitSE = "ケツドラム1";
	m_ActionDatas[(int)BASE_ACTION_STATE::HEAVEHO_DRIVE3].pAttackData->WhiffSE = "空振り5";
	m_ActionDatas[(int)BASE_ACTION_STATE::HEAVEHO_DRIVE3].pAttackData->HitEffectType = EFFECT_TYPE::FINISH_HIT;
	m_ActionDatas[(int)BASE_ACTION_STATE::HEAVEHO_DRIVE3].pAttackData->WhiffEffectType = EFFECT_TYPE::WHIFF;
	m_ActionDatas[(int)BASE_ACTION_STATE::HEAVEHO_DRIVE3].pAttackData->bAntiAir = false;
	m_ActionDatas[(int)BASE_ACTION_STATE::HEAVEHO_DRIVE3].pAttackData->bFinish = true;
	m_ActionDatas[(int)BASE_ACTION_STATE::HEAVEHO_DRIVE3].pAttackData->AntiGuard = ANTIGUARD_ATTACK::ALL_BREAK;
	m_ActionDatas[(int)BASE_ACTION_STATE::HEAVEHO_DRIVE3].pAttackData->ShakeCameraInfo.Set(1.25f, 25);
	m_ActionDatas[(int)BASE_ACTION_STATE::HEAVEHO_DRIVE3].pAttackData->GuardRecoveryFrame = 18;
	m_ActionDatas[(int)BASE_ACTION_STATE::HEAVEHO_DRIVE3].pAttackData->attribute = ATTACK_ATTRIBUTE::STRIKE;
	m_ActionDatas[(int)BASE_ACTION_STATE::HEAVEHO_DRIVE3].pAttackData->fComboRate = 0.85f;
	// 地上ヒットと空中ヒットで挙動が変わるもの
	m_ActionDatas[(int)BASE_ACTION_STATE::HEAVEHO_DRIVE3].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].bBeInvincible = false;
	m_ActionDatas[(int)BASE_ACTION_STATE::HEAVEHO_DRIVE3].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].bBeInvincible = false;
	m_ActionDatas[(int)BASE_ACTION_STATE::HEAVEHO_DRIVE3].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].FlyVector.Set(14, 2);
	m_ActionDatas[(int)BASE_ACTION_STATE::HEAVEHO_DRIVE3].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].FlyVector.Set(14, 2);
	m_ActionDatas[(int)BASE_ACTION_STATE::HEAVEHO_DRIVE3].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].iHitStopFrame = 36;
	m_ActionDatas[(int)BASE_ACTION_STATE::HEAVEHO_DRIVE3].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].iHitStopFrame = 36;
	m_ActionDatas[(int)BASE_ACTION_STATE::HEAVEHO_DRIVE3].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].HitRecoveryFrame = 90;
	m_ActionDatas[(int)BASE_ACTION_STATE::HEAVEHO_DRIVE3].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].HitRecoveryFrame = 90;
	m_ActionDatas[(int)BASE_ACTION_STATE::HEAVEHO_DRIVE3].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].DamageMotion = DAMAGE_MOTION::KNOCK_DOWN;
	m_ActionDatas[(int)BASE_ACTION_STATE::HEAVEHO_DRIVE3].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].DamageMotion = DAMAGE_MOTION::KNOCK_DOWN;
	// 判定形状
	m_ActionDatas[(int)BASE_ACTION_STATE::HEAVEHO_DRIVE3].pAttackData->pCollisionShape->width = 16;
	m_ActionDatas[(int)BASE_ACTION_STATE::HEAVEHO_DRIVE3].pAttackData->pCollisionShape->height = 14;
	m_ActionDatas[(int)BASE_ACTION_STATE::HEAVEHO_DRIVE3].pAttackData->pCollisionShape->pos.Set(6, 4, 0);


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
	m_pSkillActions[(int)SKILL_ACTION_TYPE::LAND3] = new SkillAction::Land3(this);
	m_pSkillActions[(int)SKILL_ACTION_TYPE::LAND4] = new SkillAction::Land4(this);
	m_pSkillActions[(int)SKILL_ACTION_TYPE::SQUAT] = new SkillAction::Squat(this);
	m_pSkillActions[(int)SKILL_ACTION_TYPE::AERIAL] = new SkillAction::Aerial(this);
	m_pSkillActions[(int)SKILL_ACTION_TYPE::AERIAL2] = new SkillAction::AerialDrop(this);
	m_pSkillActions[(int)SKILL_ACTION_TYPE::AERIALDROP] = new SkillAction::Aerial(this);

	m_pHeaveHoActions[(int)HEAVEHO_TYPE::STRIKE] = new HeaveHoAction::Strike(this);
	m_pHeaveHoActions[(int)HEAVEHO_TYPE::THROW] = new HeaveHoAction::Throw(this);
	m_pHeaveHoActions[(int)HEAVEHO_TYPE::THROW_SUCCESS] = new HeaveHoAction::ThrowSuccess(this);
}

Aniki::~Aniki()
{
	FOR((int)SKILL_ACTION_TYPE::MAX) SAFE_DELETE(m_pSkillActions[i]);
	FOR((int)HEAVEHO_TYPE::MAX)SAFE_DELETE(m_pHeaveHoActions[i]);
	SAFE_DELETE(m_pKetsudram);

	SAFE_DELETE(m_pSpearEffect);
	SAFE_DELETE(m_pRayEffect);
	SAFE_DELETE(m_pHandLightEffect);
	SAFE_DELETE(m_pAnikiCanonEffect);

}

void Aniki::Update(PLAYER_UPDATE flag)
{
	// 基底クラスの更新
	BasePlayer::Update(flag);

	// ケツドラム更新
	if (m_pKetsudram)m_pKetsudram->Update();

	// エフェクト
	m_pSpearEffect->Update();
	m_pRayEffect->Update();
	m_pHandLightEffect->Update();
	m_pAnikiCanonEffect->Update();

}

void Aniki::Render()
{
	// スタンド描画
	m_pStand->Render(shaderM, "Persona");

	// シェーダへ送るデータ
	ShaderSendParam();

	// 
	m_pObj->Render(shaderM, "PlayerToonNoRim");

	// エフェクトマネージャー描画
	m_pPanelEffectMGR->Render3D();
	m_pUVEffectMGR->Render();
	m_pPanelEffectMGR->Render();
	Vector2 vScreenPos = Math::WorldToScreen(m_vPos);// (TODO)頭のポジションの座標を使う
	m_pThrowMark->Render((int)vScreenPos.x - 56, (int)vScreenPos.y - 324, RS::COPY_NOZ);
	m_pExplosion->Render();

	// 専用エフェクト
	m_pSpearEffect->Render();
	m_pRayEffect->Render();
	m_pHandLightEffect->Render();
	m_pAnikiCanonEffect->Render();
	

}

void Aniki::RenderDrive()
{
	// 基底クラスの更新
	BasePlayer::RenderDrive();
}

void Aniki::ChangeColor(COLOR_TYPE eColType)
{
	LPSTR path = "none";

	switch (eColType)
	{
	case COLOR_TYPE::NORMAL:
		path = ("DATA/CHR/Aniki/Aniki.png");
		break;
	case COLOR_TYPE::EXTRA:
		path = ("DATA/CHR/Aniki/AnikiEx.png");
		break;
	case COLOR_TYPE::MIRROR:
		path = ("DATA/CHR/Aniki/Aniki2.png");
		break;
	default:
		MyAssert(0, "存在しないカラータイプ");
		break;
	}

	m_pDefaultObj->SetTexture(tdnTexture::Load(path), 0);
	m_pHHDOFObj->SetTexture(tdnTexture::Load(path), 0);

}

void Aniki::InitMotionDatas()
{
	m_iMotionNumbers[(int)MOTION_TYPE::WAIT] = 0;
	m_iMotionNumbers[(int)MOTION_TYPE::WALK] = 1;
	m_iMotionNumbers[(int)MOTION_TYPE::RUN] = 2;
	m_iMotionNumbers[(int)MOTION_TYPE::BACK_WALK] = 3;
	m_iMotionNumbers[(int)MOTION_TYPE::BACK_STEP] = 4;
	m_iMotionNumbers[(int)MOTION_TYPE::RUSH_ATTACK1] = 5;
	m_iMotionNumbers[(int)MOTION_TYPE::RUSH_ATTACK2] = 6;
	m_iMotionNumbers[(int)MOTION_TYPE::RUSH_ATTACK3] = 40;
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
	m_iMotionNumbers[(int)MOTION_TYPE::AERIAL_DASH] = 31;
	m_iMotionNumbers[(int)MOTION_TYPE::PERSONA] = 19;
	m_iMotionNumbers[(int)MOTION_TYPE::SKILL_LAND] = 20;
	m_iMotionNumbers[(int)MOTION_TYPE::SKILL2] = 21;
	m_iMotionNumbers[(int)MOTION_TYPE::SKILL3] = 44;
	m_iMotionNumbers[(int)MOTION_TYPE::SKILL4] = 40;
	m_iMotionNumbers[(int)MOTION_TYPE::SKILL_SQUAT] = 43;
	m_iMotionNumbers[(int)MOTION_TYPE::SKILL_AERIAL] = 20;
	m_iMotionNumbers[(int)MOTION_TYPE::SKILL_AERIALDROP] = 24;
	m_iMotionNumbers[(int)MOTION_TYPE::HEAVEHO_DRIVE] = 7;
	m_iMotionNumbers[(int)MOTION_TYPE::HEAVEHO_DRIVE2] = 39;
	m_iMotionNumbers[(int)MOTION_TYPE::HEAVEHO_DRIVE3] = 25;
	m_iMotionNumbers[(int)MOTION_TYPE::HEAVEHO_DRIVE_OVERFLOW] = 39;
	m_iMotionNumbers[(int)MOTION_TYPE::THROW] = 26;
	m_iMotionNumbers[(int)MOTION_TYPE::THROW_SUCCESS] = 27;
	m_iMotionNumbers[(int)MOTION_TYPE::THROW_DRAW] = 28;
	m_iMotionNumbers[(int)MOTION_TYPE::UP_GUARD] = 29;
	m_iMotionNumbers[(int)MOTION_TYPE::DOWN_GUARD] = 30;
	m_iMotionNumbers[(int)MOTION_TYPE::ESCAPE] = 31;
	m_iMotionNumbers[(int)MOTION_TYPE::KNOCKBACK] = 32;
	m_iMotionNumbers[(int)MOTION_TYPE::KNOCKDOWN] = 33;
	m_iMotionNumbers[(int)MOTION_TYPE::KNOCKDOWN_DOWN] = 34;
	m_iMotionNumbers[(int)MOTION_TYPE::REVERSAL] = 35;
	m_iMotionNumbers[(int)MOTION_TYPE::APPEAR] = 36;
	m_iMotionNumbers[(int)MOTION_TYPE::WIN] = 37;
	m_iMotionNumbers[(int)MOTION_TYPE::BURST] = 38;
}


void Aniki::KakeaiInit()
{

}

void Aniki::KakeaiUpdate()
{

}

void Aniki::KakeaiExit()
{

}

void Aniki::ThirdRushInit()
{
	// 足払い
	DownAttackInit();

	// 攻撃ボイスを再生する
	voice->Play(VOICE_TYPE::DOWN_ATTACK, CHARACTER::ANIKI);
}

void Aniki::ThirdRushUpdate()
{
	// アクション終了フラグ
	if (DownAttackUpdate()) return;

	// ヒットしてる状態
	if (GetAttackData()->bHit)
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

void Aniki::ThirdRushExit()
{
	DownAttackExit();
}

void Aniki::SkillInit()
{
	m_pSkillActions[(int)m_eSkillActionType]->Enter();
}

bool Aniki::SkillUpdate()
{
	const bool bEnd = m_pSkillActions[(int)m_eSkillActionType]->Execute();
	return bEnd;
}

void Aniki::SkillExit()
{
	m_pSkillActions[(int)m_eSkillActionType]->Exit();
}


Aniki::SkillAction::Base::Base(Aniki *pAniki, BASE_ACTION_STATE state) :m_pAniki(pAniki)
{
	if (pAniki->m_ActionDatas[(int)state].isAttackData())
	{
		memcpy_s(&m_OrgHitSquare, sizeof(CollisionShape::Square), pAniki->m_ActionDatas[(int)state].pAttackData->pCollisionShape, sizeof(CollisionShape::Square));

		m_OrgDamage = pAniki->m_ActionDatas[(int)state].pAttackData->damage;
		m_fOrgComboRate = pAniki->m_ActionDatas[(int)state].pAttackData->fComboRate;
		m_lpcOrgHitSE = pAniki->m_ActionDatas[(int)state].pAttackData->HitSE;
		m_iOrgRecoveryFrame = pAniki->m_ActionDatas[(int)state].pAttackData->places[1].HitRecoveryFrame;
		m_OrgFlyVector[(int)AttackData::HIT_PLACE::LAND] = pAniki->m_ActionDatas[(int)state].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].FlyVector;
		m_OrgFlyVector[(int)AttackData::HIT_PLACE::AERIAL] = pAniki->m_ActionDatas[(int)state].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].FlyVector;
	}
}

Aniki::SkillAction::Land::Land(Aniki *pAniki) :Base(pAniki, BASE_ACTION_STATE::SKILL)
{}

void Aniki::SkillAction::Land::Enter()
{
	// モーションに変える
	m_pAniki->SetMotion(MOTION_TYPE::SKILL_LAND);

	// アクションステート変更
	m_pAniki->SetActionState(BASE_ACTION_STATE::SKILL);

	// スキルボイスを再生する
	voice->Play(VOICE_TYPE::SKILL_LAND, CHARACTER::ANIKI);

	// 向き設定
	m_pAniki->SetDirAngle();

	// リセット
	m_iHitCount = 0;

	//D3DXMATRIX WorldMat = *m_pAniki->GetDefaultObj()->GetBone(46) * m_pAniki->GetDefaultObj()->TransMatrix;
	//m_pAniki->m_pRayEffect->Action(Vector3(WorldMat._41, WorldMat._42, WorldMat._43), 1, 1, Vector3(0, 0, 0), Vector3(1, 1, 1));
	//m_pAniki->m_pRayEffect->Action(m_pAniki->GetPos());
}

bool Aniki::SkillAction::Land::Execute()
{
	// アクション終了フラグ
	if (!m_pAniki->isAttackState()) return true;

	// 掴み成功したら、掴んだ後の攻撃ステートへ以降
	if (m_pAniki->isHitAttack())
	{
		m_pAniki->SkillExit();
		m_pAniki->m_eSkillActionType = SKILL_ACTION_TYPE::LAND2;
		m_pAniki->SkillInit();
		return false;
	}

	return false;
}

void Aniki::SkillAction::Land::Exit()
{
	// 先行入力リセット
	m_pAniki->AheadCommandReset();
}

Aniki::SkillAction::Land2::Land2(Aniki *pAniki) :Base(pAniki, BASE_ACTION_STATE::SKILL2)
{}

void Aniki::SkillAction::Land2::Enter()
{
	// モーションに変える
	m_pAniki->SetMotion(MOTION_TYPE::SKILL2);

	// アクションステート変更
	m_pAniki->SetActionState(BASE_ACTION_STATE::SKILL2);

	// スキルボイスを再生する
	voice->Play(VOICE_TYPE::SKILL2, CHARACTER::ANIKI);

	// 向き変更
	m_pAniki->SetDirAngle();


	// 相手の位置を手元に固定
	m_pAniki->GetTargetPlayer()->SetPos(m_pAniki->GetPos() + Vector3(m_pAniki->GetDirVecX() * 7.25f, 0, 0));
	m_pAniki->GetTargetPlayer()->GetObj()->SetPos(m_pAniki->GetTargetPlayer()->GetPos());
	m_pAniki->GetTargetPlayer()->GetObj()->Update();

	// 分かりやすいようにお互いヒットストップをかける
	const int l_ciHitStopFrame(m_pAniki->m_ActionDatas[(int)BASE_ACTION_STATE::SKILL].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].iHitStopFrame);
	m_pAniki->SetHitStopFrame(l_ciHitStopFrame);
	m_pAniki->GetTargetPlayer()->SetHitStopFrame(l_ciHitStopFrame);


	// オーバードライブ不可
	m_pAniki->SetNotOverDrive(true);

	// 元の攻撃データ
	m_pAniki->m_ActionDatas[(int)BASE_ACTION_STATE::SKILL2].pAttackData->damage = m_OrgDamage;
	m_pAniki->m_ActionDatas[(int)BASE_ACTION_STATE::SKILL2].pAttackData->HitSE = m_lpcOrgHitSE;
	m_pAniki->m_ActionDatas[(int)BASE_ACTION_STATE::SKILL2].pAttackData->places[1].HitRecoveryFrame = m_iOrgRecoveryFrame;
}

bool Aniki::SkillAction::Land2::Execute()
{
	// 攻撃ステート終わったら
	if (!m_pAniki->isAttackState())
	{
		// ケツドラム〆に行く
		m_pAniki->SkillExit();
		m_pAniki->SetSkillActionType(SKILL_ACTION_TYPE::LAND4);
		m_pAniki->SkillInit();
		return false;
	}

	// バースト不可
	m_pAniki->GetTargetPlayer()->SetNotOverDrive(true);

	return false;
}

void Aniki::SkillAction::Land2::Exit()
{
	// 先行入力リセット
	m_pAniki->AheadCommandReset();

	// オーバードライブ可能
	m_pAniki->SetNotOverDrive(false);
}


Aniki::SkillAction::Squat::Squat(Aniki *pAniki) :Base(pAniki, BASE_ACTION_STATE::SKILL_SQUAT)
{}

void Aniki::SkillAction::Squat::Enter()
{
	// アクションセット
	m_pAniki->SetActionState(BASE_ACTION_STATE::SKILL_SQUAT);

	// モーションセット
	m_pAniki->SetMotion(MOTION_TYPE::SKILL_SQUAT);

	// スキルボイスを再生する
	voice->Play(VOICE_TYPE::SKILL_SQUAT, CHARACTER::ANIKI);

	// 走るエフェクト
	m_pAniki->AddEffectAction( m_pAniki->GetFlontPos(), EFFECT_TYPE::RUN);


}

bool Aniki::SkillAction::Squat::Execute()
{
	if (m_pAniki->m_iCurrentActionFrame == 8)
	{
		float l_fSpearAngle = 0.0f;
		if (m_pAniki->GetDir() == DIR::LEFT)
		{
			l_fSpearAngle = 3.14f;
		}
		m_pAniki->m_pSpearEffect->Action(m_pAniki->GetFlontPos(), 1.1f, 1.5f, Vector3(0, l_fSpearAngle, 0), Vector3(0, l_fSpearAngle, 0));

	}

	// アクション終了フラグ
	if (!m_pAniki->isAttackState()) return true;

	// 掴み成功したら、掴んだ後の攻撃ステートへ以降
	if (m_pAniki->GetAttackData()->bHit)
	{
		m_pAniki->SkillExit();
		m_pAniki->m_eSkillActionType = SKILL_ACTION_TYPE::LAND3;
		m_pAniki->SkillInit();
		return false;
	}
	return false;
}

void Aniki::SkillAction::Squat::Exit()
{}


Aniki::SkillAction::Land3::Land3(Aniki *pAniki) :Base(pAniki, BASE_ACTION_STATE::SKILL3)
{}

void Aniki::SkillAction::Land3::Enter()
{
	// モーションに変える
	m_pAniki->SetMotion(MOTION_TYPE::SKILL3);

	// アクションステート変更
	m_pAniki->SetActionState(BASE_ACTION_STATE::SKILL3);

	// スキルボイスを再生する
	voice->Play(VOICE_TYPE::SKILL3, CHARACTER::ANIKI);

	// 向き変更
	m_pAniki->SetDirAngle();

	// 分かりやすいようにお互いヒットストップをかける
	const int l_ciHitStopFrame(m_pAniki->m_ActionDatas[(int)BASE_ACTION_STATE::SKILL].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].iHitStopFrame);
	m_pAniki->SetHitStopFrame(l_ciHitStopFrame);
	m_pAniki->GetTargetPlayer()->SetHitStopFrame(l_ciHitStopFrame);

	// 元の攻撃データ
	m_pAniki->m_ActionDatas[(int)BASE_ACTION_STATE::SKILL3].pAttackData->damage = m_OrgDamage;
	m_pAniki->m_ActionDatas[(int)BASE_ACTION_STATE::SKILL3].pAttackData->HitSE = m_lpcOrgHitSE;
	m_pAniki->m_ActionDatas[(int)BASE_ACTION_STATE::SKILL3].pAttackData->places[1].HitRecoveryFrame = m_iOrgRecoveryFrame;

	// オーバードライブ不可
	m_pAniki->SetNotOverDrive(true);
}

bool Aniki::SkillAction::Land3::Execute()
{
	// 攻撃ステート終わったら
	if (!m_pAniki->isAttackState())
	{
		// ケツドラム〆に行く
		m_pAniki->SkillExit();
		m_pAniki->SetSkillActionType(SKILL_ACTION_TYPE::LAND4);
		m_pAniki->SkillInit();
		return false;
	}

	// バースト不可
	m_pAniki->GetTargetPlayer()->SetNotOverDrive(true);

	// ボーン追従
	if (m_pAniki->GetActionFrame() == FRAME_STATE::START)
	{
		D3DXMATRIX WorldMat = *m_pAniki->GetDefaultObj()->GetBone(46) * m_pAniki->GetDefaultObj()->TransMatrix;
		m_pAniki->GetTargetPlayer()->SetPos(Vector3(WorldMat._41, WorldMat._42, WorldMat._43));
	}

	return false;
}

void Aniki::SkillAction::Land3::Exit()
{
	// 先行入力リセット
	m_pAniki->AheadCommandReset();

	// オーバードライブ可能
	m_pAniki->SetNotOverDrive(false);
}




Aniki::SkillAction::Land4::Land4(Aniki *pAniki) :Base(pAniki, BASE_ACTION_STATE::SKILL4)
{}

void Aniki::SkillAction::Land4::Enter()
{
	// モーションに変える
	m_pAniki->SetMotion(MOTION_TYPE::SKILL4);

	// アクションステート変更
	m_pAniki->SetActionState(BASE_ACTION_STATE::SKILL4);

	// スキルボイスを再生する
	voice->Play(VOICE_TYPE::SKILL4, CHARACTER::ANIKI);

	// 向き変更
	m_pAniki->SetDirAngle();
}

bool Aniki::SkillAction::Land4::Execute()
{
	// 攻撃ステート終わったら
	if (!m_pAniki->isAttackState())
	{
		return true;
	}

	//if (m_pAniki->GetAttackData()->bHit)
	//{
		// 強制起き上がらせ
	if (m_pAniki->GetCurrentFrame() == 24 && m_pAniki->GetTargetPlayer()->GetHP() > 0)
	{
		m_pAniki->GetTargetPlayer()->GetFSM()->ChangeState(BasePlayerState::LandRecovery::GetInstance());
	}
	//}

	return false;
}

void Aniki::SkillAction::Land4::Exit()
{
	// 先行入力リセット
	m_pAniki->AheadCommandReset();

	// オーバードライブ可能
	m_pAniki->GetTargetPlayer()->SetNotOverDrive(false);
}


Aniki::SkillAction::Aerial::Aerial(Aniki *pAniki) :Base(pAniki, BASE_ACTION_STATE::SKILL_AERIAL), m_ActiveFrame(0)
{}

void Aniki::SkillAction::Aerial::Enter()
{
	// モーションに変える
	m_pAniki->SetMotion(MOTION_TYPE::SKILL_AERIAL);

	// アクションステート変更
	m_pAniki->SetActionState(BASE_ACTION_STATE::SKILL_AERIAL);

	// スキルボイスを再生する
	voice->Play(VOICE_TYPE::SKILL_AERIAL, CHARACTER::ANIKI);

	// フレーム初期化
	m_ActiveFrame = 0;
}

bool Aniki::SkillAction::Aerial::Execute()
{
	// 着地するか、終了したら戻る
	if (!m_pAniki->isAttackState())
	{
		return true;
	}

	// 掴み成功したら、掴んだ後の攻撃ステートへ以降
	if (m_pAniki->GetAttackData()->bHit)
	{
		m_pAniki->SkillExit();
		m_pAniki->SetSkillActionType((m_pAniki->isLand()) ? SKILL_ACTION_TYPE::LAND2 : SKILL_ACTION_TYPE::AERIAL2);
		m_pAniki->SkillInit();
	}

	return false;
}

void Aniki::SkillAction::Aerial::Exit()
{}


Aniki::SkillAction::AerialDrop::AerialDrop(Aniki *pAniki) :Base(pAniki, BASE_ACTION_STATE::SKILL_AERIALDROP)
{}

void Aniki::SkillAction::AerialDrop::Enter()
{
	// モーションに変える
	m_pAniki->SetMotion(MOTION_TYPE::SKILL_AERIALDROP);

	// アクションステート変更
	m_pAniki->SetActionState(BASE_ACTION_STATE::SKILL_AERIALDROP);

	// スキルボイスを再生する
	voice->Play(VOICE_TYPE::SKILL_AERIALDROP, CHARACTER::ANIKI);

	// 重力無視
	m_pAniki->SetMoveUpdate(false);
	// 移動量を消す
	m_pAniki->SetMove(Vector3(m_pAniki->GetDirVecX()*0.9f, 2.0f, 0));

	// 分かりやすいようにお互いヒットストップをかける
	const int l_ciHitStopFrame(m_pAniki->m_ActionDatas[(int)BASE_ACTION_STATE::SKILL_AERIAL].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].iHitStopFrame);
	m_pAniki->SetHitStopFrame(l_ciHitStopFrame);
	m_pAniki->GetTargetPlayer()->SetHitStopFrame(l_ciHitStopFrame);

	// オーバードライブ不可
	m_pAniki->SetNotOverDrive(true);
}

bool Aniki::SkillAction::AerialDrop::Execute()
{
	// 攻撃ステート終わったら
	if (!m_pAniki->isAttackState())
	{
		return true;
	}

	// ボーン追従
	if (m_pAniki->GetActionFrame() == FRAME_STATE::START)
	{
		D3DXMATRIX WorldMat = *m_pAniki->GetDefaultObj()->GetBone(46) * m_pAniki->GetDefaultObj()->TransMatrix;
		m_pAniki->GetTargetPlayer()->SetPos(Vector3(WorldMat._41, WorldMat._42, WorldMat._43));
	}

	// ケツドラム〆のデータ
	if (m_pAniki->GetActionFrame() == FRAME_STATE::RECOVERY_HIT)
	{
		
	}

	// 常に相手の座標を操作する
	//const Vector3 l_vAnikiPos(m_pAniki->GetPos());
	//m_pAniki->GetTargetPlayer()->SetPos(Vector3(l_vAnikiPos.x + m_pAniki->GetDirVecX() * 5, l_vAnikiPos.y, 0));

	//if (m_pAniki->GetAttackData()->bHit)
	//{
	//	// 必殺技キャンセル
	//	if (HeaveHoCancel(m_pAniki)) return false;
	//}

	return false;
}

void Aniki::SkillAction::AerialDrop::Exit()
{
	// オーバードライブ可能
	m_pAniki->SetNotOverDrive(false);
}



void Aniki::HeavehoDriveInit()
{
	// カットイン
	SIDE data(m_side);
	MsgMgr->Dispatch(0, ENTITY_ID::UI_MGR, ENTITY_ID::UI_MGR, MESSAGE_TYPE::OVER_DRIVE_CUTIN, &data);

	m_bHeavehoHit = false;

	// Move値無効
	m_vMove = VECTOR_ZERO;

	// ストップタイマー
	SetGameTimerStopFlag(true);

	// ヒーホーのタイプ
	m_eHeaveHoType = (
		m_pStateMachine->isPrevState(*BasePlayerState::Wait::GetInstance()) ||
		m_pStateMachine->isPrevState(*BasePlayerState::Walk::GetInstance()) ||
		m_pStateMachine->isPrevState(*BasePlayerState::Squat::GetInstance()) ||
		m_pStateMachine->isPrevState(*BasePlayerState::BackWalk::GetInstance()) ||
		m_pStateMachine->isPrevState(*BasePlayerState::Run::GetInstance()) ||
		m_pStateMachine->isPrevState(*BasePlayerState::BackStep::GetInstance()) ||
		m_pStateMachine->isPrevState(*BasePlayerState::Guard::GetInstance())
		//&& !isPushInput(PLAYER_COMMAND_BIT::DOWN, true)
		) ?
		HEAVEHO_TYPE::THROW : HEAVEHO_TYPE::STRIKE;

	m_pHeaveHoActions[(int)m_eHeaveHoType]->Enter();
}

bool Aniki::HeavehoDriveUpdate()
{
	// ヒーホーストップ時間計測
	if (m_iHeavehoStopTimer > 0)
	{
		if (--m_iHeavehoStopTimer == 0) SetGameTimerStopFlag(false);
		else return false;
	}
	
	return m_pHeaveHoActions[(int)m_eHeaveHoType]->Execute();
}

void Aniki::HeavehoDriveExit()
{
	m_pHeaveHoActions[(int)m_eHeaveHoType]->Exit();

	// 重力もどって！
	SetMoveUpdate(true);
}

void Aniki::HeavehoDriveHitEvent()
{
	// 基底クラスを呼んであげる
	BasePlayer::HeavehoDriveHitEvent();

}


Aniki::HeaveHoAction::Strike::Strike(Aniki * pAniki) :Base(pAniki)
{}

void Aniki::HeaveHoAction::Strike::Enter()
{
	// モーションに変える
	m_pAniki->SetMotion(MOTION_TYPE::HEAVEHO_DRIVE);

	// アクションステート変更
	m_pAniki->SetActionState(BASE_ACTION_STATE::HEAVEHO_DRIVE);

	// 必殺ボイスを再生する
	voice->Play(VOICE_TYPE::HEAVEHO_DRIVE, CHARACTER::ANIKI);

	// 重力の影響受けるな！
	m_pAniki->SetMoveUpdate(false);

	m_pAniki->m_iHeavehoStopTimer = 10;
}

bool Aniki::HeaveHoAction::Strike::Execute()
{
	// 攻撃終わったら
	if (!m_pAniki->isAttackState())
	{
		// おしおきマネージャーがいたら
		if (m_pAniki->m_pKetsudram)
		{
			if (m_pAniki->m_pKetsudram->isAction())
			{
				return false;
			}
			delete m_pAniki->m_pKetsudram, m_pAniki->m_pKetsudram = nullptr;
		}

		// 攻撃が当たってたら
		else if (m_pAniki->m_bHeavehoHit)
		{
			//SetMotion(m_iMotionNumbers[(int)MOTION_TYPE::HEAVEHO_DRIVE] + 1);
			m_pAniki->m_pKetsudram = new Ketsudram::OverDrive(m_pAniki, m_pAniki->m_pTargetPlayer);
			m_pAniki->m_pKetsudram->Action();
			return false;
		}

		// ここに来たら終わり
		return true;
	}
	else
	{
		// 落ちる
		if(m_pAniki->GetActionFrame() == FRAME_STATE::FOLLOW)
			m_pAniki->SetMoveUpdate(true);
		if (m_pAniki->GetAttackData()->bHitSuccess) m_pAniki->m_bHeavehoHit = true;
	}

	return false;
}

void Aniki::HeaveHoAction::Strike::Exit()
{
}

Aniki::HeaveHoAction::Throw::Throw(Aniki * pAniki) :Base(pAniki)
{}

void Aniki::HeaveHoAction::Throw::Enter()
{
	// モーションに変える
	m_pAniki->SetMotion(MOTION_TYPE::HEAVEHO_DRIVE2);

	// アクションステート変更
	m_pAniki->SetActionState(BASE_ACTION_STATE::HEAVEHO_DRIVE2);

	// 必殺ボイスを再生する
	voice->Play(VOICE_TYPE::HEAVEHO_DRIVE2, CHARACTER::ANIKI);

	m_pAniki->m_iHeavehoStopTimer = 119;
}

bool Aniki::HeaveHoAction::Throw::Execute()
{
	// 投げモーション終わったら
	if (!m_pAniki->isAttackState())
	{
		return true;
	}

	// 移動
	if (m_pAniki->isActiveFrame())
	{
		m_pAniki->AddMove(Vector3(m_pAniki->GetDirVecX() * 0.5f, 0, 0));
	}

	// 掴み成功したら
	if (m_pAniki->isHitAttack())
	{
		// 掴み成功演出へ
		m_pAniki->m_eHeaveHoType = HEAVEHO_TYPE::THROW_SUCCESS;
		m_pAniki->m_pHeaveHoActions[(int)HEAVEHO_TYPE::THROW_SUCCESS]->Enter();
	}

	return false;
}

void Aniki::HeaveHoAction::Throw::Exit()
{
}


Aniki::HeaveHoAction::ThrowSuccess::ThrowSuccess(Aniki * pAniki) :Base(pAniki)
{}

void Aniki::HeaveHoAction::ThrowSuccess::Enter()
{
	// モーションに変える
	m_pAniki->SetMotion(MOTION_TYPE::HEAVEHO_DRIVE3);

	// アクションステート変更
	m_pAniki->SetActionState(BASE_ACTION_STATE::HEAVEHO_DRIVE3);

	// 必殺ボイスを再生する
	voice->Play(VOICE_TYPE::HEAVEHO_DRIVE3, CHARACTER::ANIKI);

	// 兄貴の動きを止める
	m_pAniki->SetMove(VECTOR_ZERO);

	// 上に打ち上げる
	m_pAniki->GetTargetPlayer()->SetMove(Vector3(m_pAniki->GetDirVecX() * .01f, 6, 0));

	// ぶんぶんみたいに座標をいじる
	m_pAniki->SetPos(VECTOR_ZERO);
	m_pAniki->GetTargetPlayer()->SetPos(Vector3(m_pAniki->GetDirVecX() * 5, 0, 0));

	D3DXMATRIX WorldMat = *m_pAniki->GetDefaultObj()->GetBone(46) * m_pAniki->GetDefaultObj()->TransMatrix;
	m_pAniki->m_pRayEffect->Action(Vector3(WorldMat._41, WorldMat._42, WorldMat._43), 1.2f, 0.7f, Vector3(0, 0, 0), Vector3(2, 4, 1));


}

bool Aniki::HeaveHoAction::ThrowSuccess::Execute()
{
	D3DXMATRIX WorldMat = *m_pAniki->GetDefaultObj()->GetBone(46) * m_pAniki->GetDefaultObj()->TransMatrix;
	m_pAniki->m_pRayEffect->SetPos(Vector3(WorldMat._41, WorldMat._42, WorldMat._43));
	m_pAniki->m_pHandLightEffect->SetPos(Vector3(WorldMat._41, WorldMat._42, WorldMat._43));

	// 攻撃終わったら
	if (!m_pAniki->isAttackState())
	{
		return true;
	}

	if (m_pAniki->GetCurrentFrame() == 60)
	{

		m_pAniki->m_pHandLightEffect->Action(Vector3(WorldMat._41, WorldMat._42, WorldMat._43), 1.0f, 4.5f);

	}



	// 落ちてくるやつ制御する
	const Vector3 l_vTargetMove(m_pAniki->GetTargetPlayer()->GetMove());
	if(l_vTargetMove.y < 0) m_pAniki->GetTargetPlayer()->SetMove(l_vTargetMove * 0.9725f);

	// 判定発生フレーム
	if (m_pAniki->isActiveFrame())
	{
		// アッーーーー
		voice->Play(VOICE_TYPE::DIE, CHARACTER::ANIKI);
		
		float m_fAngle = 0.0f;
		if (m_pAniki->GetDir() == DIR::RIGHT)m_fAngle = 3.14f;

		m_pAniki->m_pAnikiCanonEffect->Action(m_pAniki->GetTargetPlayer()->GetCenterPos(), 1.2f, 0.35f, Vector3(0, m_fAngle, 0), Vector3(0, m_fAngle, 0));

	}

	return false;
}

void Aniki::HeaveHoAction::ThrowSuccess::Exit()
{
}


void Aniki::HeavehoDriveOverFlowInit()
{
	// 重力の影響受けるな！
	//SetMoveUpdate(false);

	// ストップタイマー
	SetGameTimerStopFlag(true);

	m_iHeavehoStopTimer = 110;
}
void Aniki::HeavehoDriveOverFlowExit()
{

}
void Aniki::HeavehoDriveOverFlowUpdate()
{
	/* 実質、必殺がヒットするまでの間 */
	// ヒーホーストップ時間計測
	if (m_iHeavehoStopTimer > 0) if (--m_iHeavehoStopTimer == 0) SetGameTimerStopFlag(false);
}
void Aniki::HeavehoDriveOverFlowSuccessInit()
{
	// スピードラインON
	m_pSpeedLine->ActionRoop(Vector3(0, 0, 0), 1, 1, Vector3(0, 0, 0), Vector3(0, 0, 0));
}
void Aniki::HeavehoDriveOverFlowSuccessUpdate()
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

		//m_pAnikiPlayer->GetObj()->TransMatrix._31 = 0;
		//m_pAnikiPlayer->GetObj()->TransMatrix._32 = 0;
		//m_pAnikiPlayer->GetObj()->TransMatrix._33 = 1;
		//m_pAnikiPlayer->GetObj()->TransMatrix._41 = 0;
		//m_pAnikiPlayer->GetObj()->TransMatrix._42 = 0;
		//m_pAnikiPlayer->GetObj()->TransMatrix._43 = 0;
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
//PLAYER_INPUT Aniki::AIHighAttackButton()
//{
//	m_pAI->Update();;
//}