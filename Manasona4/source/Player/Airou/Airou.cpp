#include "Airou.h"
#include "../../Sound/SoundManager.h"
#include "UI/HeaveHoFinish/HeaveHoFinish.h"
#include "../../DeferredEx/DeferredEx.h"
#include "Oshioki.h"

static const int SKILL_AERIALDROP_FIRST(4);		// 初段
static const int SKILL_AERIALDROP_MIDDLE(9);	// 中間(判定無し)
static const int SKILL_AERIALDROP_FINISH(3);	// とどめ

Airou::Airou(SIDE side, const SideData &data) :BasePlayer(side, data)
, m_pOshiokiMgr(nullptr), m_bHeavehoHit(false), m_pAirouEntryEffect(new AirouEntryEffect), m_pAirouEntryCircleEffect(new AirouEntrySircleEffect)
{
	// エフェクトカメラID
	m_tagCharacterParam.eHeaveHoOverFlowCameraID = EFFECT_CAMERA_ID::AIROU_OVERFLOW;

	Reset();

	// キャラごとの設定を読み込む
	BasePlayer::LoadCharacterParam("DATA/CHR/Airou/param.txt");

	m_pName = "AIROU";
	m_pCutinPic = new tdn2DAnim("DATA/UI/Game/OverDriveCutin/Airou.png");
	m_pCutinPic->OrderMoveAppeared(8, -400, +200);

	// フレーム読み込み
	BasePlayer::LoadAttackFrameList("DATA/CHR/Airou/FrameList.txt");
	// ★フレーム独自改造
	{
		int StartActiveFrame(0);
		// アクティブフレームまでフレームを加算する
		for (; m_ActionFrameList[(int)BASE_ACTION_STATE::SKILL_AERIALDROP][StartActiveFrame] != FRAME_STATE::ACTIVE; StartActiveFrame++);

		// 中間の判定のない空間を作る
		FOR(SKILL_AERIALDROP_MIDDLE) m_ActionFrameList[(int)BASE_ACTION_STATE::SKILL_AERIALDROP][StartActiveFrame + SKILL_AERIALDROP_FIRST + i] = FRAME_STATE::FOLLOW;
	}
	// アイルーメッシュ
	m_pObj = m_pDefaultObj;

	// 顔グラ
	m_pFacePic = new tdn2DAnim("Data/UI/Game/CharaIcon/AirouIcon.png");
	//m_pFacePic->OrderNone();
	m_pFacePic->OrderShrink(8, 1, 1.25f);

	// スピードライン
	m_pSpeedLine = new SpeedLineGreenEffect;
}

void Airou::Reset()
{
	// 必ず書く
	BasePlayer::Reset();

	// おしおき出してリスタートすると残るので削除
	SAFE_DELETE(m_pOshiokiMgr);
}

void Airou::InitActionDatas()
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
	m_ActionDatas[(int)BASE_ACTION_STATE::OVERDRIVE_BURST].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].bBeInvincible = false;
	m_ActionDatas[(int)BASE_ACTION_STATE::OVERDRIVE_BURST].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].bBeInvincible = false;
	m_ActionDatas[(int)BASE_ACTION_STATE::OVERDRIVE_BURST].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].FlyVector.Set(3.25f, 3.0f);
	m_ActionDatas[(int)BASE_ACTION_STATE::OVERDRIVE_BURST].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].FlyVector.Set(3.25f, 3.0f);
	m_ActionDatas[(int)BASE_ACTION_STATE::OVERDRIVE_BURST].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].hitStopFlame = 0;
	m_ActionDatas[(int)BASE_ACTION_STATE::OVERDRIVE_BURST].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].hitStopFlame = 0;
	m_ActionDatas[(int)BASE_ACTION_STATE::OVERDRIVE_BURST].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].HitRecoveryFrame = 18;
	m_ActionDatas[(int)BASE_ACTION_STATE::OVERDRIVE_BURST].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].HitRecoveryFrame = 18;
	// 判定形状
	m_ActionDatas[(int)BASE_ACTION_STATE::OVERDRIVE_BURST].pAttackData->pCollisionShape->width = 18;
	m_ActionDatas[(int)BASE_ACTION_STATE::OVERDRIVE_BURST].pAttackData->pCollisionShape->height = 17;
	m_ActionDatas[(int)BASE_ACTION_STATE::OVERDRIVE_BURST].pAttackData->pCollisionShape->pos.Set(0, 3.5f, 0);


	//==============================================================================================================
	//	通常1段
	// 地上ヒットも空中ヒットも共通の情報
	m_ActionDatas[(int)BASE_ACTION_STATE::RUSH1].InstanceAttackData();	// アタックデータ作成
	m_ActionDatas[(int)BASE_ACTION_STATE::RUSH1].pAttackData->damage = 340;
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
	m_ActionDatas[(int)BASE_ACTION_STATE::RUSH1].pAttackData->fGuardKnockBackPower = .3f;
	m_ActionDatas[(int)BASE_ACTION_STATE::RUSH1].pAttackData->fComboRate = 0.85f;
	// 地上ヒットと空中ヒットで挙動が変わるもの
	m_ActionDatas[(int)BASE_ACTION_STATE::RUSH1].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].bBeInvincible = false;
	m_ActionDatas[(int)BASE_ACTION_STATE::RUSH1].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].bBeInvincible = false;
	m_ActionDatas[(int)BASE_ACTION_STATE::RUSH1].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].FlyVector.Set(.35f, .0f);
	m_ActionDatas[(int)BASE_ACTION_STATE::RUSH1].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].FlyVector.Set(.2f, .70f);
	m_ActionDatas[(int)BASE_ACTION_STATE::RUSH1].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].hitStopFlame = 12;
	m_ActionDatas[(int)BASE_ACTION_STATE::RUSH1].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].hitStopFlame = 13;
	m_ActionDatas[(int)BASE_ACTION_STATE::RUSH1].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].HitRecoveryFrame = 20;
	m_ActionDatas[(int)BASE_ACTION_STATE::RUSH1].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].HitRecoveryFrame = 20;
	m_ActionDatas[(int)BASE_ACTION_STATE::RUSH1].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].DamageMotion = DAMAGE_MOTION::KNOCK_BACK;
	m_ActionDatas[(int)BASE_ACTION_STATE::RUSH1].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].DamageMotion = DAMAGE_MOTION::KNOCK_DOWN;
	// 判定形状
	m_ActionDatas[(int)BASE_ACTION_STATE::RUSH1].pAttackData->pCollisionShape->width = 10;
	m_ActionDatas[(int)BASE_ACTION_STATE::RUSH1].pAttackData->pCollisionShape->height = 6.5f;
	m_ActionDatas[(int)BASE_ACTION_STATE::RUSH1].pAttackData->pCollisionShape->pos.Set(9, 4.5f, 0);

	//==============================================================================================================
	//	通常2段
	// 地上ヒットも空中ヒットも共通の情報
	m_ActionDatas[(int)BASE_ACTION_STATE::RUSH2].InstanceAttackData();	// アタックデータ作成
	m_ActionDatas[(int)BASE_ACTION_STATE::RUSH2].pAttackData->damage = 460;
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
	m_ActionDatas[(int)BASE_ACTION_STATE::RUSH2].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].FlyVector.Set(.45f, .0f);
	m_ActionDatas[(int)BASE_ACTION_STATE::RUSH2].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].FlyVector.Set(.3f, .75f);
	m_ActionDatas[(int)BASE_ACTION_STATE::RUSH2].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].hitStopFlame = 12;
	m_ActionDatas[(int)BASE_ACTION_STATE::RUSH2].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].hitStopFlame = 13;
	m_ActionDatas[(int)BASE_ACTION_STATE::RUSH2].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].HitRecoveryFrame = 26;
	m_ActionDatas[(int)BASE_ACTION_STATE::RUSH2].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].HitRecoveryFrame = 26;
	m_ActionDatas[(int)BASE_ACTION_STATE::RUSH2].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].DamageMotion = DAMAGE_MOTION::KNOCK_BACK;
	m_ActionDatas[(int)BASE_ACTION_STATE::RUSH2].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].DamageMotion = DAMAGE_MOTION::KNOCK_DOWN;
	// 判定形状
	m_ActionDatas[(int)BASE_ACTION_STATE::RUSH2].pAttackData->pCollisionShape->width = 11;
	m_ActionDatas[(int)BASE_ACTION_STATE::RUSH2].pAttackData->pCollisionShape->height = 8;
	m_ActionDatas[(int)BASE_ACTION_STATE::RUSH2].pAttackData->pCollisionShape->pos.Set(9, 5.5, 0);


	//==============================================================================================================
	//	対空攻撃
	// 地上ヒットも空中ヒットも共通の情報
	m_ActionDatas[(int)BASE_ACTION_STATE::ANTI_AIR].InstanceAttackData();	// アタックデータ作成
	m_ActionDatas[(int)BASE_ACTION_STATE::ANTI_AIR].pAttackData->damage = 360;
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
	m_ActionDatas[(int)BASE_ACTION_STATE::ANTI_AIR].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].HitRecoveryFrame = 32;
	m_ActionDatas[(int)BASE_ACTION_STATE::ANTI_AIR].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].HitRecoveryFrame = 55;
	m_ActionDatas[(int)BASE_ACTION_STATE::ANTI_AIR].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].DamageMotion = DAMAGE_MOTION::KNOCK_BACK;
	m_ActionDatas[(int)BASE_ACTION_STATE::ANTI_AIR].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].DamageMotion = DAMAGE_MOTION::KNOCK_DOWN;
	// 判定形状
	m_ActionDatas[(int)BASE_ACTION_STATE::ANTI_AIR].pAttackData->pCollisionShape->width = 8;
	m_ActionDatas[(int)BASE_ACTION_STATE::ANTI_AIR].pAttackData->pCollisionShape->height = 9;
	m_ActionDatas[(int)BASE_ACTION_STATE::ANTI_AIR].pAttackData->pCollisionShape->pos.Set(2.5f, 12, 0);

	//==============================================================================================================
	//	中断攻撃(どっこいアタック)
	// 地上ヒットも空中ヒットも共通の情報
	m_ActionDatas[(int)BASE_ACTION_STATE::DOKKOI_ATTACK].InstanceAttackData();	// アタックデータ作成
	m_ActionDatas[(int)BASE_ACTION_STATE::DOKKOI_ATTACK].pAttackData->damage = 800;
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
	m_ActionDatas[(int)BASE_ACTION_STATE::DOKKOI_ATTACK].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].FlyVector.Set(.6f, .25f);
	m_ActionDatas[(int)BASE_ACTION_STATE::DOKKOI_ATTACK].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].FlyVector.Set(.6f, 1.0f);
	m_ActionDatas[(int)BASE_ACTION_STATE::DOKKOI_ATTACK].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].hitStopFlame = 18;
	m_ActionDatas[(int)BASE_ACTION_STATE::DOKKOI_ATTACK].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].hitStopFlame = 18;
	m_ActionDatas[(int)BASE_ACTION_STATE::DOKKOI_ATTACK].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].HitRecoveryFrame = 64;
	m_ActionDatas[(int)BASE_ACTION_STATE::DOKKOI_ATTACK].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].HitRecoveryFrame = 55;
	m_ActionDatas[(int)BASE_ACTION_STATE::DOKKOI_ATTACK].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].DamageMotion = DAMAGE_MOTION::KNOCK_BACK;
	m_ActionDatas[(int)BASE_ACTION_STATE::DOKKOI_ATTACK].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].DamageMotion = DAMAGE_MOTION::KNOCK_DOWN;
	// 判定形状
	m_ActionDatas[(int)BASE_ACTION_STATE::DOKKOI_ATTACK].pAttackData->pCollisionShape->width = 11;
	m_ActionDatas[(int)BASE_ACTION_STATE::DOKKOI_ATTACK].pAttackData->pCollisionShape->height = 8;
	m_ActionDatas[(int)BASE_ACTION_STATE::DOKKOI_ATTACK].pAttackData->pCollisionShape->pos.Set(9, 5.5, 0);


	//==============================================================================================================
	//	しゃがみ下段攻撃
	// 地上ヒットも空中ヒットも共通の情報
	m_ActionDatas[(int)BASE_ACTION_STATE::SQUAT_ATTACK].InstanceAttackData();	// アタックデータ作成
	m_ActionDatas[(int)BASE_ACTION_STATE::SQUAT_ATTACK].pAttackData->damage = 360;
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
	m_ActionDatas[(int)BASE_ACTION_STATE::SQUAT_ATTACK].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].hitStopFlame = 7;
	m_ActionDatas[(int)BASE_ACTION_STATE::SQUAT_ATTACK].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].hitStopFlame = 7;
	m_ActionDatas[(int)BASE_ACTION_STATE::SQUAT_ATTACK].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].HitRecoveryFrame = 32;
	m_ActionDatas[(int)BASE_ACTION_STATE::SQUAT_ATTACK].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].HitRecoveryFrame = 55;
	m_ActionDatas[(int)BASE_ACTION_STATE::SQUAT_ATTACK].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].DamageMotion = DAMAGE_MOTION::KNOCK_BACK;
	m_ActionDatas[(int)BASE_ACTION_STATE::SQUAT_ATTACK].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].DamageMotion = DAMAGE_MOTION::KNOCK_DOWN;
	// 判定形状
	m_ActionDatas[(int)BASE_ACTION_STATE::SQUAT_ATTACK].pAttackData->pCollisionShape->width = 12;
	m_ActionDatas[(int)BASE_ACTION_STATE::SQUAT_ATTACK].pAttackData->pCollisionShape->height = 3;
	m_ActionDatas[(int)BASE_ACTION_STATE::SQUAT_ATTACK].pAttackData->pCollisionShape->pos.Set(10, 2, 0);



	//==============================================================================================================
	//	足払い攻撃
	// 地上ヒットも空中ヒットも共通の情報
	m_ActionDatas[(int)BASE_ACTION_STATE::DOWN_ATTACK].InstanceAttackData();	// アタックデータ作成
	m_ActionDatas[(int)BASE_ACTION_STATE::DOWN_ATTACK].pAttackData->damage = 360;
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
	m_ActionDatas[(int)BASE_ACTION_STATE::DOWN_ATTACK].pAttackData->fComboRate = 0.95f;
	// 地上ヒットと空中ヒットで挙動が変わるもの
	m_ActionDatas[(int)BASE_ACTION_STATE::DOWN_ATTACK].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].bBeInvincible = false;
	m_ActionDatas[(int)BASE_ACTION_STATE::DOWN_ATTACK].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].bBeInvincible = false;
	m_ActionDatas[(int)BASE_ACTION_STATE::DOWN_ATTACK].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].FlyVector.Set(.35f, .75f);
	m_ActionDatas[(int)BASE_ACTION_STATE::DOWN_ATTACK].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].FlyVector.Set(.25f, .85f);
	m_ActionDatas[(int)BASE_ACTION_STATE::DOWN_ATTACK].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].hitStopFlame = 9;
	m_ActionDatas[(int)BASE_ACTION_STATE::DOWN_ATTACK].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].hitStopFlame = 9;
	m_ActionDatas[(int)BASE_ACTION_STATE::DOWN_ATTACK].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].HitRecoveryFrame = 32;
	m_ActionDatas[(int)BASE_ACTION_STATE::DOWN_ATTACK].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].HitRecoveryFrame = 55;
	m_ActionDatas[(int)BASE_ACTION_STATE::DOWN_ATTACK].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].DamageMotion = DAMAGE_MOTION::DOWN_FALL;
	m_ActionDatas[(int)BASE_ACTION_STATE::DOWN_ATTACK].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].DamageMotion = DAMAGE_MOTION::DOWN_FALL;
	// 判定形状
	m_ActionDatas[(int)BASE_ACTION_STATE::DOWN_ATTACK].pAttackData->pCollisionShape->width = 12;
	m_ActionDatas[(int)BASE_ACTION_STATE::DOWN_ATTACK].pAttackData->pCollisionShape->height = 3;
	m_ActionDatas[(int)BASE_ACTION_STATE::DOWN_ATTACK].pAttackData->pCollisionShape->pos.Set(10, 2, 0);


	//==============================================================================================================
	//	空中攻撃
	// 地上ヒットも空中ヒットも共通の情報
	m_ActionDatas[(int)BASE_ACTION_STATE::AERIAL].InstanceAttackData();	// アタックデータ作成
	m_ActionDatas[(int)BASE_ACTION_STATE::AERIAL].pAttackData->damage = 360;
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
	m_ActionDatas[(int)BASE_ACTION_STATE::AERIAL].pAttackData->pCollisionShape->height = 10;
	m_ActionDatas[(int)BASE_ACTION_STATE::AERIAL].pAttackData->pCollisionShape->pos.Set(6.5f, 5, 0);

	//==============================================================================================================
	//	空中下攻撃
	// 地上ヒットも空中ヒットも共通の情報
	m_ActionDatas[(int)BASE_ACTION_STATE::AERIALDROP].InstanceAttackData();	// アタックデータ作成
	m_ActionDatas[(int)BASE_ACTION_STATE::AERIALDROP].pAttackData->damage = 360;
	m_ActionDatas[(int)BASE_ACTION_STATE::AERIALDROP].pAttackData->pierceLV = 0;
	m_ActionDatas[(int)BASE_ACTION_STATE::AERIALDROP].pAttackData->HitSE = "ヒット5";
	m_ActionDatas[(int)BASE_ACTION_STATE::AERIALDROP].pAttackData->WhiffSE = "空振り5";
	m_ActionDatas[(int)BASE_ACTION_STATE::AERIALDROP].pAttackData->HitEffectType = EFFECT_TYPE::DAMAGE;
	m_ActionDatas[(int)BASE_ACTION_STATE::AERIALDROP].pAttackData->WhiffEffectType = EFFECT_TYPE::WHIFF;
	m_ActionDatas[(int)BASE_ACTION_STATE::AERIALDROP].pAttackData->bAntiAir = false;
	m_ActionDatas[(int)BASE_ACTION_STATE::AERIALDROP].pAttackData->bFinish = true;
	m_ActionDatas[(int)BASE_ACTION_STATE::AERIALDROP].pAttackData->AntiGuard = ANTIGUARD_ATTACK::UP_ATTACK;		// ★しゃがみガードをつぶす
	m_ActionDatas[(int)BASE_ACTION_STATE::AERIALDROP].pAttackData->ShakeCameraInfo.Set(.3f, 3);
	m_ActionDatas[(int)BASE_ACTION_STATE::AERIALDROP].pAttackData->GuardRecoveryFrame = 25;
	m_ActionDatas[(int)BASE_ACTION_STATE::AERIALDROP].pAttackData->fComboRate = 0.85f;
	// 地上ヒットと空中ヒットで挙動が変わるもの
	m_ActionDatas[(int)BASE_ACTION_STATE::AERIALDROP].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].bBeInvincible = false;
	m_ActionDatas[(int)BASE_ACTION_STATE::AERIALDROP].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].bBeInvincible = false;
	m_ActionDatas[(int)BASE_ACTION_STATE::AERIALDROP].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].FlyVector.Set(1.0f, .0f);
	m_ActionDatas[(int)BASE_ACTION_STATE::AERIALDROP].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].FlyVector.Set(1.5f, -2.5f);
	m_ActionDatas[(int)BASE_ACTION_STATE::AERIALDROP].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].hitStopFlame = 8;
	m_ActionDatas[(int)BASE_ACTION_STATE::AERIALDROP].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].hitStopFlame = 8;
	m_ActionDatas[(int)BASE_ACTION_STATE::AERIALDROP].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].HitRecoveryFrame = 45;
	m_ActionDatas[(int)BASE_ACTION_STATE::AERIALDROP].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].HitRecoveryFrame = 50;
	m_ActionDatas[(int)BASE_ACTION_STATE::AERIALDROP].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].DamageMotion = DAMAGE_MOTION::KNOCK_BACK;
	m_ActionDatas[(int)BASE_ACTION_STATE::AERIALDROP].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].DamageMotion = DAMAGE_MOTION::KNOCK_BACK;
	// 判定形状
	m_ActionDatas[(int)BASE_ACTION_STATE::AERIALDROP].pAttackData->pCollisionShape->width = 10;
	m_ActionDatas[(int)BASE_ACTION_STATE::AERIALDROP].pAttackData->pCollisionShape->height = 9;
	m_ActionDatas[(int)BASE_ACTION_STATE::AERIALDROP].pAttackData->pCollisionShape->pos.Set(5, -3.0f, 0);

	//==============================================================================================================
	//	逆切れ攻撃
	// 地上ヒットも空中ヒットも共通の情報
	m_ActionDatas[(int)BASE_ACTION_STATE::INVINCIBLE_ATTACK].InstanceAttackData();	// アタックデータ作成
	m_ActionDatas[(int)BASE_ACTION_STATE::INVINCIBLE_ATTACK].pAttackData->damage = 514;
	m_ActionDatas[(int)BASE_ACTION_STATE::INVINCIBLE_ATTACK].pAttackData->pierceLV = 0;
	m_ActionDatas[(int)BASE_ACTION_STATE::INVINCIBLE_ATTACK].pAttackData->HitSE = "フィニッシュ大";
	m_ActionDatas[(int)BASE_ACTION_STATE::INVINCIBLE_ATTACK].pAttackData->WhiffSE = "空振り5";
	m_ActionDatas[(int)BASE_ACTION_STATE::INVINCIBLE_ATTACK].pAttackData->HitEffectType = EFFECT_TYPE::DAMAGE;
	m_ActionDatas[(int)BASE_ACTION_STATE::INVINCIBLE_ATTACK].pAttackData->WhiffEffectType = EFFECT_TYPE::WHIFF;
	m_ActionDatas[(int)BASE_ACTION_STATE::INVINCIBLE_ATTACK].pAttackData->bAntiAir = false;
	m_ActionDatas[(int)BASE_ACTION_STATE::INVINCIBLE_ATTACK].pAttackData->bFinish = true;
	m_ActionDatas[(int)BASE_ACTION_STATE::INVINCIBLE_ATTACK].pAttackData->AntiGuard = ANTIGUARD_ATTACK::NONE;
	m_ActionDatas[(int)BASE_ACTION_STATE::INVINCIBLE_ATTACK].pAttackData->ShakeCameraInfo.Set(.5f, 10);
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
	m_ActionDatas[(int)BASE_ACTION_STATE::INVINCIBLE_ATTACK].pAttackData->pCollisionShape->width = 11;
	m_ActionDatas[(int)BASE_ACTION_STATE::INVINCIBLE_ATTACK].pAttackData->pCollisionShape->height = 6.5f;
	m_ActionDatas[(int)BASE_ACTION_STATE::INVINCIBLE_ATTACK].pAttackData->pCollisionShape->pos.Set(9, 4.5f, 0);

	//==============================================================================================================
	//	キャラクター固有スキル
	// 地上ヒットも空中ヒットも共通の情報
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL].InstanceAttackData();	// アタックデータ作成
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL].pAttackData->damage = 120;
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL].pAttackData->pierceLV = 0;
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL].pAttackData->HitSE = "ぐちょヒット";
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL].pAttackData->WhiffSE = "アイルードリル";
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL].pAttackData->HitEffectType = EFFECT_TYPE::MULTIPLE_HIT;
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL].pAttackData->WhiffEffectType = EFFECT_TYPE::WHIFF;
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL].pAttackData->bAntiAir = false;
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL].pAttackData->bFinish = true;
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL].pAttackData->AntiGuard = ANTIGUARD_ATTACK::NONE;
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL].pAttackData->ShakeCameraInfo.Set(.1f, 2);
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL].pAttackData->GuardRecoveryFrame = 15;
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL].pAttackData->fComboRate = 0.85f;
	// 地上ヒットと空中ヒットで挙動が変わるもの
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].bBeInvincible = false;
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].bBeInvincible = false;
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].FlyVector.Set(1.1f, .37f);
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].FlyVector.Set(1.2f, .47f);
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].hitStopFlame = 1;
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].hitStopFlame = 1;
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].HitRecoveryFrame = 80;
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].HitRecoveryFrame = 80;
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].DamageMotion = DAMAGE_MOTION::KNOCK_BACK;
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].DamageMotion = DAMAGE_MOTION::KNOCK_DOWN;

	// 判定形状
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL].pAttackData->pCollisionShape->width = 11;
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL].pAttackData->pCollisionShape->height = 6;
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL].pAttackData->pCollisionShape->pos.Set(9, 4, 0);

	//==============================================================================================================
	//	キャラクター固有下攻撃
	// 地上ヒットも空中ヒットも共通の情報
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL_SQUAT].InstanceAttackData();	// アタックデータ作成
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL_SQUAT].pAttackData->damage = 700;
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL_SQUAT].pAttackData->pierceLV = 0;
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL_SQUAT].pAttackData->HitSE = "ヒット6";
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL_SQUAT].pAttackData->WhiffSE = "空振り5";
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL_SQUAT].pAttackData->HitEffectType = EFFECT_TYPE::DAMAGE;
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL_SQUAT].pAttackData->WhiffEffectType = EFFECT_TYPE::WHIFF;
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL_SQUAT].pAttackData->bAntiAir = false;
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL_SQUAT].pAttackData->bFinish = true;
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL_SQUAT].pAttackData->AntiGuard = ANTIGUARD_ATTACK::NONE;
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL_SQUAT].pAttackData->ShakeCameraInfo.Set(.2f, 3);
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL_SQUAT].pAttackData->GuardRecoveryFrame = 18;
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL_SQUAT].pAttackData->fComboRate = 0.85f;
	// 地上ヒットと空中ヒットで挙動が変わるもの
	// [メモ] アイル―は最後吹き飛ばす　棒人間は三段目ものけぞりで隙が短い
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL_SQUAT].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].bBeInvincible = false;
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL_SQUAT].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].bBeInvincible = false;
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL_SQUAT].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].FlyVector.Set(1.5f, 1.9f);
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL_SQUAT].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].FlyVector.Set(1.5f, 1.9f);
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL_SQUAT].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].hitStopFlame = 6;
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL_SQUAT].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].hitStopFlame = 8;
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL_SQUAT].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].HitRecoveryFrame = 50;
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL_SQUAT].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].HitRecoveryFrame = 50;
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL_SQUAT].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].DamageMotion = DAMAGE_MOTION::KNOCK_DOWN;
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL_SQUAT].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].DamageMotion = DAMAGE_MOTION::KNOCK_DOWN;
	// 判定形状
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL_SQUAT].pAttackData->pCollisionShape->width = 18;
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL_SQUAT].pAttackData->pCollisionShape->height = 7;
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL_SQUAT].pAttackData->pCollisionShape->pos.Set(9, 5, 0);

	//==============================================================================================================
	//	キャラクター固有空中下攻撃
	// 地上ヒットも空中ヒットも共通の情報
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL_AERIALDROP].InstanceAttackData();	// アタックデータ作成
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL_AERIALDROP].pAttackData->damage = 360;
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL_AERIALDROP].pAttackData->pierceLV = 0;
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL_AERIALDROP].pAttackData->HitSE = "ヒット4";
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL_AERIALDROP].pAttackData->WhiffSE = "空振り5";
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL_AERIALDROP].pAttackData->HitEffectType = EFFECT_TYPE::DAMAGE;
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL_AERIALDROP].pAttackData->WhiffEffectType = EFFECT_TYPE::AIROU_CIRCLE;
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL_AERIALDROP].pAttackData->bAntiAir = false;
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL_AERIALDROP].pAttackData->bFinish = true;
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL_AERIALDROP].pAttackData->AntiGuard = ANTIGUARD_ATTACK::UP_ATTACK;	// ★しゃがみガードをつぶす
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL_AERIALDROP].pAttackData->ShakeCameraInfo.Set(.3f, 3);
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL_AERIALDROP].pAttackData->GuardRecoveryFrame = 25;
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL_AERIALDROP].pAttackData->fComboRate = 0.85f;
	// 地上ヒットと空中ヒットで挙動が変わるもの
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL_AERIALDROP].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].bBeInvincible = false;
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL_AERIALDROP].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].bBeInvincible = false;
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL_AERIALDROP].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].FlyVector.Set(1.0f, .0f);
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL_AERIALDROP].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].FlyVector.Set(-.05f, .75f);
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL_AERIALDROP].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].hitStopFlame = 8;
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL_AERIALDROP].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].hitStopFlame = 8;
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL_AERIALDROP].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].HitRecoveryFrame = 65;
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL_AERIALDROP].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].HitRecoveryFrame = 50;
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL_AERIALDROP].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].DamageMotion = DAMAGE_MOTION::KNOCK_BACK;
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL_AERIALDROP].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].DamageMotion = DAMAGE_MOTION::KNOCK_DOWN;
	// 判定形状
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL_AERIALDROP].pAttackData->pCollisionShape->width = 15;
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL_AERIALDROP].pAttackData->pCollisionShape->height = 15;
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL_AERIALDROP].pAttackData->pCollisionShape->pos.Set(5, -1.0f, 0);

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
	m_ActionDatas[(int)BASE_ACTION_STATE::THROW].pAttackData->pCollisionShape->pos.Set(2, 4, 0);

	//==============================================================================================================
	//	投げ成功
	// 地上ヒットも空中ヒットも共通の情報
	m_ActionDatas[(int)BASE_ACTION_STATE::THROW_SUCCESS].InstanceAttackData();	// アタックデータ作成
	m_ActionDatas[(int)BASE_ACTION_STATE::THROW_SUCCESS].pAttackData->damage = 1200;
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
	m_ActionDatas[(int)BASE_ACTION_STATE::HEAVEHO_DRIVE].pAttackData->damage = 114;
	m_ActionDatas[(int)BASE_ACTION_STATE::HEAVEHO_DRIVE].pAttackData->pierceLV = 0;
	m_ActionDatas[(int)BASE_ACTION_STATE::HEAVEHO_DRIVE].pAttackData->HitSE = "フィニッシュ大";
	m_ActionDatas[(int)BASE_ACTION_STATE::HEAVEHO_DRIVE].pAttackData->WhiffSE = "空振り5";
	m_ActionDatas[(int)BASE_ACTION_STATE::HEAVEHO_DRIVE].pAttackData->HitEffectType = EFFECT_TYPE::FINISH_HIT;
	m_ActionDatas[(int)BASE_ACTION_STATE::HEAVEHO_DRIVE].pAttackData->WhiffEffectType = EFFECT_TYPE::WHIFF;
	m_ActionDatas[(int)BASE_ACTION_STATE::HEAVEHO_DRIVE].pAttackData->bAntiAir = false;
	m_ActionDatas[(int)BASE_ACTION_STATE::HEAVEHO_DRIVE].pAttackData->bFinish = false;
	m_ActionDatas[(int)BASE_ACTION_STATE::HEAVEHO_DRIVE].pAttackData->AntiGuard = ANTIGUARD_ATTACK::NONE;
	m_ActionDatas[(int)BASE_ACTION_STATE::HEAVEHO_DRIVE].pAttackData->ShakeCameraInfo.Set(1.0f, 20);
	m_ActionDatas[(int)BASE_ACTION_STATE::HEAVEHO_DRIVE].pAttackData->GuardRecoveryFrame = 18;
	m_ActionDatas[(int)BASE_ACTION_STATE::HEAVEHO_DRIVE].pAttackData->fComboRate = 0.85f;
	// 地上ヒットと空中ヒットで挙動が変わるもの
	m_ActionDatas[(int)BASE_ACTION_STATE::HEAVEHO_DRIVE].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].bBeInvincible = true;
	m_ActionDatas[(int)BASE_ACTION_STATE::HEAVEHO_DRIVE].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].bBeInvincible = true;
	m_ActionDatas[(int)BASE_ACTION_STATE::HEAVEHO_DRIVE].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].FlyVector.Set(3.0f, 2.5f);
	m_ActionDatas[(int)BASE_ACTION_STATE::HEAVEHO_DRIVE].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].FlyVector.Set(3.5f, 2.5f);
	m_ActionDatas[(int)BASE_ACTION_STATE::HEAVEHO_DRIVE].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].hitStopFlame = 25;
	m_ActionDatas[(int)BASE_ACTION_STATE::HEAVEHO_DRIVE].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].hitStopFlame = 25;
	m_ActionDatas[(int)BASE_ACTION_STATE::HEAVEHO_DRIVE].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].HitRecoveryFrame = 120;
	m_ActionDatas[(int)BASE_ACTION_STATE::HEAVEHO_DRIVE].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].HitRecoveryFrame = 120;
	m_ActionDatas[(int)BASE_ACTION_STATE::HEAVEHO_DRIVE].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].DamageMotion = DAMAGE_MOTION::KNOCK_DOWN;
	m_ActionDatas[(int)BASE_ACTION_STATE::HEAVEHO_DRIVE].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].DamageMotion = DAMAGE_MOTION::KNOCK_DOWN;
	// 判定形状
	m_ActionDatas[(int)BASE_ACTION_STATE::HEAVEHO_DRIVE].pAttackData->pCollisionShape->width = 12;
	m_ActionDatas[(int)BASE_ACTION_STATE::HEAVEHO_DRIVE].pAttackData->pCollisionShape->height = 9.5f;
	m_ActionDatas[(int)BASE_ACTION_STATE::HEAVEHO_DRIVE].pAttackData->pCollisionShape->pos.Set(12, 8.0f, 0);

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
	m_ActionDatas[(int)BASE_ACTION_STATE::HEAVEHO_DRIVE_OVERFLOW].pAttackData->AntiGuard = ANTIGUARD_ATTACK::NONE;
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
	m_ActionDatas[(int)BASE_ACTION_STATE::HEAVEHO_DRIVE_OVERFLOW].pAttackData->pCollisionShape->height = 9.5f;
	m_ActionDatas[(int)BASE_ACTION_STATE::HEAVEHO_DRIVE_OVERFLOW].pAttackData->pCollisionShape->pos.Set(10, 6.5f, 0);

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
	m_pSkillActions[(int)SKILL_ACTION_TYPE::SQUAT] = new SkillAction::Squat(this);
	m_pSkillActions[(int)SKILL_ACTION_TYPE::AERIAL] = new SkillAction::Land(this);
	m_pSkillActions[(int)SKILL_ACTION_TYPE::AERIALDROP] = new SkillAction::AerialDrop(this);
}

Airou::~Airou()
{
	SAFE_DELETE(m_pOshiokiMgr);
	SAFE_DELETE(m_pAirouEntryEffect); 
	SAFE_DELETE(m_pAirouEntryCircleEffect);
	FOR((int)SKILL_ACTION_TYPE::MAX) SAFE_DELETE(m_pSkillActions[i])
}

void Airou::Update(PLAYER_UPDATE flag)
{
	// 基底クラスの更新
	BasePlayer::Update(flag);

	// おしおき更新
	if (m_pOshiokiMgr)
	{
		//if (KeyBoardTRG(KB_ENTER)) m_pOshiokiMgr->Explosion();
		m_pOshiokiMgr->Update();
	}
}

void Airou::Render()
{
	// 闇の輪っか
	if (m_pAirouEntryEffect) m_pAirouEntryEffect->Render();
	if (m_pAirouEntryCircleEffect) m_pAirouEntryCircleEffect->Render();

	// 基底クラスの更新
	BasePlayer::Render();

	// おしおき描画
	if (m_pOshiokiMgr) m_pOshiokiMgr->Render();
}

void Airou::RenderDrive()
{
	// 基底クラスの更新
	BasePlayer::RenderDrive();

	// おしおき描画
	if (m_pOshiokiMgr) m_pOshiokiMgr->Render();
}

void Airou::InitMotionDatas()
{
	m_iMotionNumbers[(int)MOTION_TYPE::WAIT] = 0;
	m_iMotionNumbers[(int)MOTION_TYPE::WALK] = 1;
	m_iMotionNumbers[(int)MOTION_TYPE::RUN] = 2;
	m_iMotionNumbers[(int)MOTION_TYPE::BACK_WALK] = 3;
	m_iMotionNumbers[(int)MOTION_TYPE::BACK_STEP] = 4;
	m_iMotionNumbers[(int)MOTION_TYPE::RUSH_ATTACK1] = 5;
	m_iMotionNumbers[(int)MOTION_TYPE::RUSH_ATTACK2] = 6;
	m_iMotionNumbers[(int)MOTION_TYPE::RUSH_ATTACK3] = 7;
	m_iMotionNumbers[(int)MOTION_TYPE::FINISH_ATTACK] = 8;
	m_iMotionNumbers[(int)MOTION_TYPE::SQUAT] = 9;
	m_iMotionNumbers[(int)MOTION_TYPE::ANTI_AIR_ATTACK] = 10;
	m_iMotionNumbers[(int)MOTION_TYPE::DOKKOI_ATTACK] = 11;
	m_iMotionNumbers[(int)MOTION_TYPE::SQUAT_ATTACK] = 12;
	m_iMotionNumbers[(int)MOTION_TYPE::DOWN_ATTACK] = 13;
	m_iMotionNumbers[(int)MOTION_TYPE::JUMP] = 14;
	m_iMotionNumbers[(int)MOTION_TYPE::FALL] = 15;
	m_iMotionNumbers[(int)MOTION_TYPE::RAND] = 16;
	m_iMotionNumbers[(int)MOTION_TYPE::AERIAL_ATTACK] = 17;
	m_iMotionNumbers[(int)MOTION_TYPE::AERIAL_DASH] = 18;
	m_iMotionNumbers[(int)MOTION_TYPE::PERSONA] = 19;
	m_iMotionNumbers[(int)MOTION_TYPE::CHARA_ATTACK_LAND] = 20;
	m_iMotionNumbers[(int)MOTION_TYPE::CHARA_ATTACK_SQUAT] = 7;
	m_iMotionNumbers[(int)MOTION_TYPE::CHARA_ATTACK_AERIAL] = 21;
	m_iMotionNumbers[(int)MOTION_TYPE::CHARA_ATTACK_AERIALDROP] = 22;
	m_iMotionNumbers[(int)MOTION_TYPE::HEAVEHO_DRIVE] = 23;
	m_iMotionNumbers[(int)MOTION_TYPE::HEAVEHO_DRIVE_OVERFLOW] = 25;
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
	//m_iMotionNumbers[(int)MOTION_TYPE::DIE] = 35;
	m_iMotionNumbers[(int)MOTION_TYPE::APPEAR] = 36;
	m_iMotionNumbers[(int)MOTION_TYPE::WIN] = 37;
	m_iMotionNumbers[(int)MOTION_TYPE::BURST] = 38;
}


void Airou::KakeaiInit()
{
	m_pAirouEntryEffect->Action(m_vPos + Vector3(0, 1, 0));
	m_pAirouEntryCircleEffect->Action(m_vPos);
}

void Airou::KakeaiUpdate()
{
	m_pAirouEntryEffect->Update();
	m_pAirouEntryEffect->SetAngleAnimation(Vector3(0, 0, 0), Vector3(0, 3.14f, 0));
	m_pAirouEntryCircleEffect->Update();
	m_pAirouEntryCircleEffect->SetAngleAnimation(Vector3(0, 0, 0), Vector3(0, 3.14f, 0));
}

void Airou::KakeaiExit()
{
	SAFE_DELETE(m_pAirouEntryEffect);
	SAFE_DELETE(m_pAirouEntryCircleEffect);
}

void Airou::ThirdRushInit()
{
	m_pSkillActions[(int)SKILL_ACTION_TYPE::LAND]->Enter();
}

void Airou::ThirdRushUpdate()
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

void Airou::ThirdRushExit()
{
	m_pSkillActions[(int)SKILL_ACTION_TYPE::LAND]->Exit();
}



void Airou::SkillInit()
{
	m_pSkillActions[(int)m_SkillActionType]->Enter();
}

bool Airou::SkillUpdate()
{
	const bool bEnd = m_pSkillActions[(int)m_SkillActionType]->Execute();
	return bEnd;
}

void Airou::SkillExit()
{
	m_pSkillActions[(int)m_SkillActionType]->Exit();
}


Airou::SkillAction::Base::Base(Airou *pAirou, BASE_ACTION_STATE state) :m_pAirou(pAirou)
{
	memcpy_s(&m_OrgHitSquare, sizeof(CollisionShape::Square), pAirou->m_ActionDatas[(int)state].pAttackData->pCollisionShape, sizeof(CollisionShape::Square));

	m_OrgDamage = pAirou->m_ActionDatas[(int)state].pAttackData->damage;
	m_fOrgComboRate = pAirou->m_ActionDatas[(int)state].pAttackData->fComboRate;
	m_OrgFlyVector[(int)AttackData::HIT_PLACE::LAND] = pAirou->m_ActionDatas[(int)state].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].FlyVector;
	m_OrgFlyVector[(int)AttackData::HIT_PLACE::AERIAL] = pAirou->m_ActionDatas[(int)state].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].FlyVector;
}

Airou::SkillAction::Land::Land(Airou *pAirou) :Base(pAirou, BASE_ACTION_STATE::SKILL), m_DrillFrame(0), m_DrillHitCount(0), m_bHit(false)
{}

void Airou::SkillAction::Land::Enter()
{
	// モーションに変える
	m_pAirou->SetMotion(MOTION_TYPE::CHARA_ATTACK_LAND);

	// アクションステート変更
	m_pAirou->SetActionState(BASE_ACTION_STATE::SKILL);

	// 重力の影響受けるな！
	m_pAirou->SetMoveUpdate(false);

	// ★★★ここで終了するとやばい
	m_pAirou->m_tagCharacterParam.HitSquare.width = 5.5f;

	// アイル―ドリルエフェクト発動！
	m_pAirou->AddEffectAction(m_pAirou->m_vPos, EFFECT_TYPE::AIROU_DRILL);

	// アイルー正面向きながらドリル防止
	m_pAirou->SetDirAngle();

	m_DrillFrame = 0;
	m_DrillHitCount = 0;
	m_bHit = false;
}

bool Airou::SkillAction::Land::Execute()
{
	// 攻撃ステート終わったら
	if (!m_pAirou->isAttackState())
	{
		return true;
	}
	
	if (m_pAirou->GetActionFrame() == FRAME_STATE::FOLLOW) 	m_pAirou->m_pUVEffectMGR->StopEffect(UV_EFFECT_TYPE::AIROU_DRILL);// ドリルエフェクトおしまい

	if (m_pAirou->GetAttackData()->bHit)
	{
		m_bHit = true;
		// 最後当たったら先行入力
		if (m_DrillHitCount > 4)m_pAirou->AheadCommandUpdate();
	}

	// 1回でも当たったらフラグ
	if (m_bHit)
	{
		// 必殺技キャンセル
		if (HeaveHoCancel(m_pAirou)) return false;

		m_pAirou->m_ActionDatas[(int)BASE_ACTION_STATE::SKILL].pAttackData->fComboRate = 1;
	}

	// xフレーム後に移動
	if (m_DrillFrame > 8)
	{
		const float speed((m_pAirou->GetActionFrame() == FRAME_STATE::FOLLOW) ? .75f : 1.75f);
		m_pAirou->m_vMove.x = (m_pAirou->m_dir == DIR::RIGHT) ? speed : -speed;
	}


	if (++m_DrillFrame % 6 == 0)
	{
		m_pAirou->m_ActionDatas[(int)BASE_ACTION_STATE::SKILL].pAttackData->bHit = false;	// ヒットフラグリセット
		if (++m_DrillHitCount >= 5)	// フィニッシュ
		{
			// 最後だけやや強い攻撃力にする
			m_pAirou->m_ActionDatas[(int)BASE_ACTION_STATE::SKILL].pAttackData->damage = (int)(m_OrgDamage * 1.5f);
			m_pAirou->m_ActionDatas[(int)BASE_ACTION_STATE::SKILL].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].FlyVector.Set(2.0f, 2.0f);
			m_pAirou->m_ActionDatas[(int)BASE_ACTION_STATE::SKILL].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].FlyVector.Set(2.0f, 1.25f);
			m_pAirou->m_ActionDatas[(int)BASE_ACTION_STATE::SKILL].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].hitStopFlame = 8;
			m_pAirou->m_ActionDatas[(int)BASE_ACTION_STATE::SKILL].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].hitStopFlame = 8;

			m_pAirou->m_ActionDatas[(int)BASE_ACTION_STATE::SKILL_AERIALDROP].pAttackData->pCollisionShape->width = 10;
			m_pAirou->m_ActionDatas[(int)BASE_ACTION_STATE::SKILL_AERIALDROP].pAttackData->pCollisionShape->height = 10;
			m_pAirou->m_ActionDatas[(int)BASE_ACTION_STATE::SKILL_AERIALDROP].pAttackData->pCollisionShape->pos.Set(5, 0.0f, 0);
		}
	}

	// エフェクト
	m_pAirou->m_pUVEffectMGR->SetPos(m_pAirou->m_vPos, UV_EFFECT_TYPE::AIROU_DRILL);// ドリルの場所をキャラに追従

	return false;
}

void Airou::SkillAction::Land::Exit()
{
	// 移動更新無効を解除しておく
	m_pAirou->SetMoveUpdate(true);

	m_pAirou->m_tagCharacterParam.HitSquare.width = 1.5f;
	m_pAirou->m_pUVEffectMGR->StopEffect(UV_EFFECT_TYPE::AIROU_DRILL);// ドリルエフェクトおしまい

	// 攻撃情報を元に戻す
	m_pAirou->m_ActionDatas[(int)BASE_ACTION_STATE::SKILL].pAttackData->damage = m_OrgDamage;
	m_pAirou->m_ActionDatas[(int)BASE_ACTION_STATE::SKILL].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].FlyVector = m_OrgFlyVector[(int)AttackData::HIT_PLACE::LAND];
	m_pAirou->m_ActionDatas[(int)BASE_ACTION_STATE::SKILL].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].FlyVector = m_OrgFlyVector[(int)AttackData::HIT_PLACE::AERIAL];
	m_pAirou->m_ActionDatas[(int)BASE_ACTION_STATE::SKILL].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].hitStopFlame = 2;
	m_pAirou->m_ActionDatas[(int)BASE_ACTION_STATE::SKILL].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].hitStopFlame = 2;
	m_pAirou->m_ActionDatas[(int)BASE_ACTION_STATE::SKILL].pAttackData->fComboRate = m_fOrgComboRate;

	// 先行入力リセット
	m_pAirou->AheadCommandReset();
}


Airou::SkillAction::Squat::Squat(Airou *pAirou) :Base(pAirou, BASE_ACTION_STATE::SKILL_SQUAT)
{}

void Airou::SkillAction::Squat::Enter()
{
	// アクションセット
	m_pAirou->SetActionState(BASE_ACTION_STATE::SKILL_SQUAT);

	// モーションセット
	m_pAirou->SetMotion(MOTION_TYPE::CHARA_ATTACK_SQUAT);
}

bool Airou::SkillAction::Squat::Execute()
{
	// 攻撃ステート終わったら
	if (!m_pAirou->isAttackState())
	{
		return true;
	}

	return false;
}



Airou::SkillAction::AerialDrop::AerialDrop(Airou *pAirou) :Base(pAirou, BASE_ACTION_STATE::SKILL_AERIALDROP), m_ActiveFrame(0)
{}

void Airou::SkillAction::AerialDrop::Enter()
{
	// モーションに変える
	m_pAirou->SetMotion(MOTION_TYPE::CHARA_ATTACK_AERIALDROP);

	// アクションステート変更
	m_pAirou->SetActionState(BASE_ACTION_STATE::SKILL_AERIALDROP);

	// 重力の影響受けるな！
	m_pAirou->SetMoveUpdate(false);

	// アイルー正面向きながらドリル防止
	m_pAirou->SetDirAngle();

	// フレーム初期化
	m_ActiveFrame = 0;
}

bool Airou::SkillAction::AerialDrop::Execute()
{
	// 攻撃ステート終わったら
	if (!m_pAirou->isAttackState())
	{
		return true;
	}
	
	if (m_pAirou->GetAttackData()->bHit)
	{
		// 必殺技キャンセル
		if (HeaveHoCancel(m_pAirou)) return false;
	}

	if (m_pAirou->GetActionFrame() == FRAME_STATE::ACTIVE)
	{
		// ヒットストップ中は無視
		if (m_pAirou->m_iHitStopFrame > 0)return false;

		static const int SECOND_ATTACK_FRAME(SKILL_AERIALDROP_FIRST);
		// 初段
		if (m_ActiveFrame < SECOND_ATTACK_FRAME)
		{

		}

		// 2段目のヒットフラグリセット
		else if (m_ActiveFrame == SECOND_ATTACK_FRAME)
		{
			m_pAirou->m_ActionDatas[(int)BASE_ACTION_STATE::SKILL_AERIALDROP].pAttackData->bHit = false;

			// 強い吹っ飛びと長いヒットストップにする
			m_pAirou->m_ActionDatas[(int)BASE_ACTION_STATE::SKILL_AERIALDROP].pAttackData->damage = (int)(m_OrgDamage * 1.5f);
			m_pAirou->m_ActionDatas[(int)BASE_ACTION_STATE::SKILL_AERIALDROP].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].FlyVector.Set(1.0f, .0f);
			m_pAirou->m_ActionDatas[(int)BASE_ACTION_STATE::SKILL_AERIALDROP].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].FlyVector.Set(1.5f, -2.5f);
			m_pAirou->m_ActionDatas[(int)BASE_ACTION_STATE::SKILL_AERIALDROP].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].hitStopFlame = 6;
			m_pAirou->m_ActionDatas[(int)BASE_ACTION_STATE::SKILL_AERIALDROP].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].hitStopFlame = 12;
			m_pAirou->m_ActionDatas[(int)BASE_ACTION_STATE::SKILL_AERIALDROP].pAttackData->fComboRate = 1;

			//m_pAirou->m_ActionDatas[(int)BASE_ACTION_STATE::SKILL_AERIALDROP].pAttackData->pCollisionShape->width = 12;
			//m_pAirou->m_ActionDatas[(int)BASE_ACTION_STATE::SKILL_AERIALDROP].pAttackData->pCollisionShape->height = 11;
			m_pAirou->m_ActionDatas[(int)BASE_ACTION_STATE::SKILL_AERIALDROP].pAttackData->pCollisionShape->pos.Set(10, 5.0f, 0);

			// エフェクト
			m_pAirou->AddEffectAction(m_pAirou->GetFlontPos(), EFFECT_TYPE::AIROU_CIRCLE);
		}

		// 二段
		else if (m_ActiveFrame > SECOND_ATTACK_FRAME)
		{

		}

		m_ActiveFrame++;
	}

	return false;
}

void Airou::SkillAction::AerialDrop::Exit()
{
	// 攻撃情報を元に戻す
	memcpy_s(m_pAirou->m_ActionDatas[(int)BASE_ACTION_STATE::SKILL_AERIALDROP].pAttackData->pCollisionShape, sizeof(CollisionShape::Square), &m_OrgHitSquare, sizeof(CollisionShape::Square));

	m_pAirou->m_ActionDatas[(int)BASE_ACTION_STATE::SKILL_AERIALDROP].pAttackData->damage = m_OrgDamage;
	m_pAirou->m_ActionDatas[(int)BASE_ACTION_STATE::SKILL_AERIALDROP].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].FlyVector = m_OrgFlyVector[(int)AttackData::HIT_PLACE::LAND];
	m_pAirou->m_ActionDatas[(int)BASE_ACTION_STATE::SKILL_AERIALDROP].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].FlyVector=m_OrgFlyVector[(int)AttackData::HIT_PLACE::AERIAL];
	m_pAirou->m_ActionDatas[(int)BASE_ACTION_STATE::SKILL_AERIALDROP].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].hitStopFlame = 4;
	m_pAirou->m_ActionDatas[(int)BASE_ACTION_STATE::SKILL_AERIALDROP].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].hitStopFlame = 8;
	m_pAirou->m_ActionDatas[(int)BASE_ACTION_STATE::SKILL_AERIALDROP].pAttackData->fComboRate = m_fOrgComboRate;
}


void Airou::HeavehoDriveInit()
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

	// ヒットフラグ
	m_bHeavehoHit = false;

	m_iHeavehoStopTimer = 85;
}

bool Airou::HeavehoDriveUpdate()
{
	// 攻撃終わったら
	if (!isAttackState())
	{
		// おしおきマネージャーがいたら
		if (m_pOshiokiMgr)
		{
			if (m_pOshiokiMgr->isAction())
			{
				// こっちみんな
				m_fAngleY = PI;

				m_pOshiokiMgr->Update();
				return false;
			}
			delete m_pOshiokiMgr, m_pOshiokiMgr = nullptr;
		}

		// 攻撃が当たってたら
		else if (m_bHeavehoHit)
		{
			SetMotion(m_iMotionNumbers[(int)MOTION_TYPE::HEAVEHO_DRIVE] + 1);
			m_pOshiokiMgr = new OshiokiManager::OverDrive(this, m_pTargetPlayer);
			m_pOshiokiMgr->Action();
			return false;
		}

		// ここに来たら終わり
		return true;
	}

	// ヒーホーストップ時間計測
	if (m_iHeavehoStopTimer > 0) if (--m_iHeavehoStopTimer == 0)	SetGameTimerStopFlag(false);

	return false;
}

void Airou::HeavehoDriveExit()
{
	// 重力早くもどって！
	SetMoveUpdate(true);

}

void Airou::HeavehoDriveHitEvent()
{
	// 基底クラスを呼んであげる
	BasePlayer::HeavehoDriveHitEvent();

	// 
	m_bHeavehoHit = true;
}


void Airou::HeavehoDriveOverFlowInit()
{
	// 重力の影響受けるな！
	SetMoveUpdate(false);

	// ストップタイマー
	SetGameTimerStopFlag(true);

	m_iHeavehoStopTimer = 100;
}
void Airou::HeavehoDriveOverFlowExit()
{

}
void Airou::HeavehoDriveOverFlowUpdate()
{
	/* 実質、必殺がヒットするまでの間 */
	// ヒーホーストップ時間計測
	if (m_iHeavehoStopTimer > 0) if (--m_iHeavehoStopTimer == 0)	SetGameTimerStopFlag(false);
}
void Airou::HeavehoDriveOverFlowSuccessInit()
{
	if (m_pOshiokiMgr) delete m_pOshiokiMgr;

	// オーバーフロー用のおしおきくんを生成
	m_pOshiokiMgr = new OshiokiManager::OverFlow(this, m_pTargetPlayer);

	// スピードラインON
	m_pSpeedLine->ActionRoop(Vector3(0, 0, 0), 1, 1, Vector3(0, 0, 0), Vector3(0, 0, 0));
}
void Airou::HeavehoDriveOverFlowSuccessUpdate()
{
	/* 必殺ヒットして演出中 */
	// おしおき更新
	if (m_pOshiokiMgr)
	{
		m_pOshiokiMgr->Update();
	}


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

		//m_pAirouPlayer->GetObj()->TransMatrix._31 = 0;
		//m_pAirouPlayer->GetObj()->TransMatrix._32 = 0;
		//m_pAirouPlayer->GetObj()->TransMatrix._33 = 1;
		//m_pAirouPlayer->GetObj()->TransMatrix._41 = 0;
		//m_pAirouPlayer->GetObj()->TransMatrix._42 = 0;
		//m_pAirouPlayer->GetObj()->TransMatrix._43 = 0;
	}

	// コンボ
	COMBO_DESK comboDesk;

	switch (m_iHeaveHoDriveOverFlowFrame++)
	{
	case 60:	// 打ち上げ
		m_vMove.Set(0, .5f, -.5f);
		BasePlayer::AddEffectAction(Vector3(0, 7, -15), EFFECT_TYPE::DAMAGE);
		se->Play("フィニッシュ小");
		se->Play("通常3段ヒット");

		comboDesk.side = m_pTargetPlayer->GetSide();
		comboDesk.damage = 4274;
		comboDesk.recoveryFrame = 10000;
		MsgMgr->Dispatch(0, ENTITY_ID::PLAYER_MGR, ENTITY_ID::UI_MGR, MESSAGE_TYPE::COMBO_COUNT, &comboDesk);

		m_pTargetPlayer->SetRecoveryFrame(10000);
		m_pTargetPlayer->SetMotion(MOTION_TYPE::KNOCKDOWN);
		break;

	case 170:	// 叩きおとし
		m_vMove.Set(0, -1.0f, 0);
		BasePlayer::AddEffectAction(Vector3(0, 51, -68), EFFECT_TYPE::DAMAGE);
		DeferredManagerEx.SetRadialBlur(Vector2(0, 0), 9);
		se->Play("フィニッシュ小");
		se->Play("通常3段ヒット");

		comboDesk.side = m_pTargetPlayer->GetSide();
		comboDesk.damage = 4274;
		comboDesk.recoveryFrame = 10000;
		MsgMgr->Dispatch(0, ENTITY_ID::PLAYER_MGR, ENTITY_ID::UI_MGR, MESSAGE_TYPE::COMBO_COUNT, &comboDesk);

		m_pTargetPlayer->SetRecoveryFrame(10000);
		break;

	case 195:	// 相手が地面にたたき落ちる
		m_vMove.Set(0, 1.0f, 0);
		BasePlayer::AddEffectAction(Vector3(0, 10, -68), EFFECT_TYPE::DAMAGE);
		DeferredManagerEx.SetRadialBlur(Vector2(0, 0), 7);
		se->Play("撃ち落とし");

		comboDesk.side = m_pTargetPlayer->GetSide();
		comboDesk.damage = 4274;
		comboDesk.recoveryFrame = 10000;
		MsgMgr->Dispatch(0, ENTITY_ID::PLAYER_MGR, ENTITY_ID::UI_MGR, MESSAGE_TYPE::COMBO_COUNT, &comboDesk);

		m_pTargetPlayer->SetRecoveryFrame(10000);
		break;

	case 220:	// アイルー流星群開始
		m_pSpeedLine->SetAngleAnimation(Vector3(PI*-.25f, 0, 0), Vector3(PI*-.25f, 0, 0));
		m_pOshiokiMgr->Action();
		break;

	case 525:	// 爆発してアイルーたちが飛び散る
		se->Play("フィニッシュ大");
		m_pOshiokiMgr->Explosion();

		// コンボ
		comboDesk.side = m_pTargetPlayer->GetSide();
		comboDesk.damage = 4274;
		comboDesk.recoveryFrame = 10000;
		MsgMgr->Dispatch(0, ENTITY_ID::PLAYER_MGR, ENTITY_ID::UI_MGR, MESSAGE_TYPE::COMBO_COUNT, &comboDesk);

		m_pTargetPlayer->SetRecoveryFrame(10000);
		break;

	case 710:	// スピードラインの向きを変える
		m_pSpeedLine->SetAngleAnimation(Vector3(PI*-.15f, 0, 0), Vector3(PI*-.15f, 0, 0));
		break;

	case 750:	// アイルー貫き爆発
		se->Play("フィニッシュ大");
		m_pOshiokiMgr->Explosion();
		DeferredManagerEx.SetRadialBlur(Vector2(0, 0), 10);

		// コンボ
		comboDesk.side = m_pTargetPlayer->GetSide();
		comboDesk.damage = 114514;
		comboDesk.recoveryFrame = 10000;
		MsgMgr->Dispatch(0, ENTITY_ID::PLAYER_MGR, ENTITY_ID::UI_MGR, MESSAGE_TYPE::COMBO_COUNT, &comboDesk);

		m_pTargetPlayer->SetRecoveryFrame(10000);
		break;

	case 760:
		HeaveHoFinishUI->Action();	// ふぃにっしゅ”
		break;
	}

}

// AI
//PLAYER_INPUT Airou::AIHighAtackButton()
//{
//	m_pAI->Update();;
//}
