#pragma once
// エンティティ関連のインクルード
#include "../BaseEntity/Message/Message.h"
#include "../BaseEntity/Entity/BaseGameEntity.h"
#include "../BaseEntity/Entity/EntityEnum.h"
#include "../BaseEntity/State/StateMachine.h"
#include "BasePlayerState.h"

//
#include "Data/SelectData.h"
#include "../Cutin/CutIn.h"

// 判定のインクルード
#include "../Collision/Collision.h"

// エフェクト関連
#include "../Effect/PanelEffect/PanelEffectManager.h"
#include "../Effect\UVEffect\UVEffectManager.h"

// UI
#include "../UI/ComboUI.h"

// カメラ
#include "../Camera/camera.h"

// コマンド表用
#include "Window\BaseWindow.h"


// 前方宣言
class AI;
namespace Stand
{
	class Base;
}
namespace Stage
{
	class Base;
}
struct SideData;

// 定数
enum class PLAYER_INPUT
{
	A,		// (ヒーホードライブキャンセル)→(弱攻撃)→どっこい攻撃(上下段)
	B,		// (攻撃)→(強攻撃)→必殺
	C,		// (ジャンプ)→(必殺技)→弱攻撃
	D,		// (キャラ固有)
	RIGHT,
	LEFT,
	UP,
	DOWN,
	R1,		// エスケープ
	R2,
	R3,
	L1,
	L2,
	L3,
	START,
	SELECT,
	MAX
};

enum class PLAYER_COMMAND_BIT
{
	NEUTRAL = 0,
	A = 1 << 1,
	B = 1 << 2,
	C = 1 << 3,
	D = 1 << 4,
	RIGHT = 1 << 5,
	LEFT = 1 << 6,
	UP = 1 << 7,
	DOWN = 1 << 8,
	R1 = 1 << 9,
	R2 = 1 << 10,
	R3 = 1 << 11,
	L1 = 1 << 12,
	L2 = 1 << 13,
	L3 = 1 << 14,
};

static const PLAYER_COMMAND_BIT c_BitList[(int)PLAYER_INPUT::MAX] =
{
	PLAYER_COMMAND_BIT::A,
	PLAYER_COMMAND_BIT::B,
	PLAYER_COMMAND_BIT::C,
	PLAYER_COMMAND_BIT::D,
	PLAYER_COMMAND_BIT::RIGHT,
	PLAYER_COMMAND_BIT::LEFT,
	PLAYER_COMMAND_BIT::UP,
	PLAYER_COMMAND_BIT::DOWN,
	PLAYER_COMMAND_BIT::R1,
	PLAYER_COMMAND_BIT::R2,
	PLAYER_COMMAND_BIT::R3,
	PLAYER_COMMAND_BIT::L1,
	PLAYER_COMMAND_BIT::L2,
	PLAYER_COMMAND_BIT::L3,

	// STARTとSELECT枠は無し
	PLAYER_COMMAND_BIT::NEUTRAL,
	PLAYER_COMMAND_BIT::NEUTRAL
};

enum class PLAYER_UPDATE
{
	CONTROL_NO,		// BasePlayerStateを通すけどコントロールはしない
	NO_FSM,			// コントロールは受け付けるけど、BasePlayerStateは発動しない
	CONTROL_OK		// どっちもOK
};

//static CommandList[(int)PLAYER_INPUT::MAX] = 
//{
//	
//	NANAME = PLAYER_INPUT::RIGHT + PLAYER_INPUT::LEFT;
//};


// 向き
enum class DIR { LEFT, RIGHT, MAX };
static const float DIR_ANGLE[(int)DIR::MAX] =
{
	PI * 1.5f,	// 左向き
	PI * .5,	// 右向き
};

enum class FRAME_STATE
{
	START,	// 発生
	ACTIVE,	// 持続(この間に攻撃判定)
	FOLLOW,	// 硬直(後隙)
	END,	// 終了(このフレームに来たら攻撃終了)
	RECOVERY_HIT,	// bHitが復活するフレーム
};
static const int FRAME_MAX = 256; // 攻撃のフレームの最大値(これより超えることはないだろう)

// 全キャラ固有で持つアクション
enum class BASE_ACTION_STATE
{
	NO_ACTION = -1,
	ESCAPE,				// 回避
	BACKSTEP,			// バクステ
	STAND,				// スタンド発動
	OVERDRIVE_ONEMORE,	// ワンモア覚醒
	OVERDRIVE_BURST,	// バースト覚醒
	RUSH1,				// 通常1段目
	RUSH2,				// 通常2段目
	RUSH3,				// 通常3段目
	ANTI_AIR,				// 対空攻撃
	DOKKOI_ATTACK,		// 中段攻撃
	SQUAT_ATTACK,		// しゃがみ攻撃
	DOWN_ATTACK,		// 足払い攻撃
	AERIAL_ATTACK,		// 空中攻撃
	AERIALDROP,			// 空中下攻撃
	INVINCIBLE_ATTACK,	// フィニッシュアーツ→無敵技
	SKILL,				// 地上キャラクター固有技
	SKILL2,				// 地上キャラクター固有技2
	SKILL3,				// 地上キャラクター固有技3
	SKILL4,				// 地上キャラクター固有技3
	SKILL_SQUAT,		// しゃがみキャラクター固有技
	SKILL_AERIAL,		// 空中下キャラクター固有技
	SKILL_AERIALDROP,	// 空中下キャラクター固有技
	THROW,				// 投げ
	THROW_SUCCESS,		// 投げ成功
	HEAVEHO_DRIVE,		// ヒーホードライブ
	HEAVEHO_DRIVE2,		// ヒーホードライブ2
	HEAVEHO_DRIVE3,		// ヒーホードライブ3
	HEAVEHO_DRIVE_OVERFLOW,// ヒーホードライブ_オーバーフロー
	FRAMECOUNT,			// ★とりあえず一定フレーム計りたいというときに、今使っているのは(投げ失敗の間だったり、投げ抜けの間だったり。)
	END					// 何もなし　(A列車)名前
};

enum class SKILL_ACTION_TYPE
{
	NO_ACTION = -1,
	LAND,
	SQUAT,
	AERIAL,
	AERIALDROP,

	// 追加はここに。ここ以前に書くとスタンドがおかしくなる
	LAND2,
	LAND3,
	LAND4,
	AERIAL2,
	MAX
};


enum class MOTION_TYPE
{
	WAIT,					// 待機
	WALK,					// 移動
	RUN,					// ダッシュ
	BACK_WALK,				// 後ろ歩き
	BACK_STEP,				// バクステ
	//BRAKE,				// ブレーキ
	//U_TURN,				// Uターン
	RUSH_ATTACK1,			// ラッシュ1
	RUSH_ATTACK2,			// ラッシュ2
	RUSH_ATTACK3,			// ラッシュ最後
	FINISH_ATTACK,			// フィニッシュアーツ
	SQUAT,					// しゃがみ
	SQUAT_ATTACK,		// しゃがみ攻撃
	ANTI_AIR_ATTACK,		// しゃがみ攻撃
	DOKKOI_ATTACK,			// 中段攻撃
	DOWN_ATTACK,			// 下段攻撃
	JUMP,					// ジャンプ
	FALL,					// 落下
	RAND,					// 着地
	AERIAL_ATTACK,			// 空中攻撃
	//AERIALDROP_ATTACK,
	AERIAL_DASH,			// 空中ダッシュ
	PERSONA,				// ペルソナ発動
	SKILL_LAND,				// キャラ固有地上
	SKILL_SQUAT,			// キャラ固有しゃがみ
	SKILL_AERIAL,			// キャラ固有空中
	SKILL_AERIALDROP,		// キャラ固有空中下
	SKILL2,					// スキル枠2
	SKILL3,					// スキル枠3
	SKILL4,					// スキル枠4
	HEAVEHO_DRIVE,			// DD
	HEAVEHO_DRIVE2,			// DD2
	HEAVEHO_DRIVE3,			// DD3
	BURST,					// バースト
	HEAVEHO_DRIVE_OVERFLOW,	// AH
	THROW,					// 投げ
	THROW_SUCCESS,			// 投げ成功
	THROW_DRAW,				// 投げはじき
	UP_GUARD,				// 上段ガード
	DOWN_GUARD,				// 下段ガード
	ESCAPE,					// 回避
	KNOCKBACK,
	KNOCKDOWN,				// 吹っ飛び
	KNOCKDOWN_DOWN,			// 足払いくらった
	//KNOCKDOWN_AERIAL,
	REVERSAL,				// 起き上がり
	//DIE,
	APPEAR,					// 登場(掛け合い)
	WIN,
	MAX
};

// エフェクトの種類
enum class EFFECT_TYPE
{
	DAMAGE,				// ダメージエフェクト
	WHIFF,				// 振り
	RECOVERY,			// リカバリー
	PERSONA,			// ペ...ル..ソ..ナ!!
	DROP_IMPACT,		// ドロップインパクト
	UPPER,				// アッパー
	FINISH_HIT,			// フィニッシュアーツヒット
	BURST,				// バースト
	ONEMORE_BURST,		// ワンモアバースト
	ONEMORE_BURST_START,// ワンモアバーストの前
	RUN,				// 走る瞬間
	BACK_STEP,			// バックステップ
	GUARD_BREAK,		// ガードブレイク
	AIROU_DRILL,		// アイルードリル
	AIROU_CIRCLE,		// アイルーサークル
	GUARD_WAVE,			// ガードウェーブ
	MULTIPLE_HIT,		// 多段ヒット
	OVER_DRIVE_ACTION,	// 必殺技アクション
	JUMP,				// 飛ぶときのエフェクト
	AERIAL_JUMP,		// 空中ジャンプのエフェクト
	ESCAPE,				// 回避
	INVINCIBLE_ATTACK,	// 逆切れ（無敵技）
	DOKKOI,				// 中段
	MUZZLE_FLASH,		// マズルフラッシュ
	THROW,				// 投げ
	WILL_POWER,			// 根性
	DROP_IMPACT_SUPER,	// てきの衝撃波
	CYCLONE_RUN,		// 竜巻を起こして走る
	MULTIPLE_HIT_BLOW,	// 多段ヒット（物理）
	THROW_HOLD,			// 掴まえた時
};

// ガードの種類
enum class GUARD_STATE
{
	NO_GUARD,	// ガードしてない
	UP_GUARD,	// 立ちガード
	DOWN_GUARD,	// しゃがみガード
};

// 攻撃のタイプ
enum class ANTIGUARD_ATTACK
{
	NONE,			// どのガードでも防げる普通の攻撃
	DOWN_ATTACK,	// 俗にいう下段
	UP_ATTACK,		// 俗にいう上段
	ALL_BREAK,		// ガード不可
};

// 攻撃属性
enum class ATTACK_ATTRIBUTE
{
	STRIKE,	// 打撃属性
	BULLET,	// 弾属性
	THROW,	// 投げ属性
};

/****************************************/
//		アタックデータ
/****************************************/
class AttackData
{
public:
	enum class HIT_PLACE{ LAND, AERIAL, MAX };	// 相手がどっちでヒットしたか。地上、空中

	// ここにあるエリアは地上ヒットと空中ヒットとか関係なく共通の情報
	CollisionShape::Square *pCollisionShape;	// ★あたり判定形状(四角にする)
	int damage;					// 与えるダメージ
	bool bHit;					// 当たったかどうか(★多段ヒット防止用)多段ヒットしてほしい攻撃だと、また考える必要がある
	bool bHitSuccess;			// 攻撃がガードされずに当たったかどうか
	int WhiffDelayFrame;		// 何フレーム後に空振りのSEを再生するか(WhiffSEを使わない(nullptr)なら別に設定しなくてもいい)
	EFFECT_TYPE HitEffectType;	// エフェクトのタイプ
	LPCSTR HitSE;				// 当たったときに鳴らすSE
	LPCSTR WhiffSE;				// 空振りSE
	EFFECT_TYPE WhiffEffectType;// 振りエフェクト
	int pierceLV;				// 貫通レベル
	bool bAntiAir;				// 対空攻撃かどうか　(追加)
	bool bFinish;				// フィニッシュになる攻撃かどうか
	ANTIGUARD_ATTACK AntiGuard;	// ガードを突き破る攻撃のタイプ
	ATTACK_ATTRIBUTE attribute;	// 攻撃の属性(弾、投げ、打撃)
	SHAKE_CAMERA_INFO ShakeCameraInfo;	// カメラ振動用構造体
	int GuardRecoveryFrame;		// ガードされたときに「ガードしている相手に与える」硬直時間
	float fGuardKnockBackPower;	// ガードさせたときのけぞり力
	float fComboRate;			// コンボ補正(0.8なら次の攻撃に0.8倍の補正がかかる、補正は掛け合わされる)
	float fRepeatAttackRate;	// 同技補正値

	// ★★★地上ヒットと空中ヒットで分けたい情報
	struct
	{
		bool bBeInvincible;			// 無敵になるかどうか(たとえば、通常の1.2段目ならfalseだし、3段目ならtrue)
		//Vector2 LandFlyVector;		// 当たって吹っ飛ばすベクトル(★基本的にxは+にすること)
		//Vector2 AerialFlyVector;	// ※相手が空中にいた時　当たって吹っ飛ばす空中ベクトル(★基本的にxは+にすること)
		Vector2 FlyVector;
		int iHitStopFrame;			// ヒットストップの時間
		int HitRecoveryFrame;		// 攻撃がヒットした際に「相手に与える」硬直の時間
		DAMAGE_MOTION DamageMotion;	// 喰らった時のモーション
	}places[(int)HIT_PLACE::MAX];
	AttackData() :
		fRepeatAttackRate(0.75f),
		fComboRate(1), 
		attribute(ATTACK_ATTRIBUTE::STRIKE), 
		HitSE(nullptr),
		WhiffSE(nullptr),
		bHit(false),
		bHitSuccess(false),
		damage(0),
		WhiffDelayFrame(0),
		pierceLV(0),
		bAntiAir(false),
		bFinish(true),
		AntiGuard(ANTIGUARD_ATTACK::NONE),
		pCollisionShape(new CollisionShape::Square),
		fGuardKnockBackPower(0),
		GuardRecoveryFrame(0),
		WhiffEffectType(EFFECT_TYPE::WHIFF){}
	~AttackData(){ SAFE_DELETE(pCollisionShape); }
};



// 覚醒の種類
enum class OVERDRIVE_TYPE
{
	//HEAVE_HO,	// どっこい通常の覚醒
	ONEMORE,	// コンボ中にさらなる追撃をするために発動
	BURST		// 相手の攻撃を受けているときにバーストとして発動
};

/***************************************************************/
//
//		ベースプレイヤー
//
/***************************************************************/
class BasePlayer : public BaseGameEntity
{
protected:

	//------------------------------------------------------
	//	ジャンプ用に使う構造体
	//------------------------------------------------------
	struct Jump
	{
		bool bHold;	// しゃがんでる状態
		bool bAttackPush;	// しゃがんでる最中に攻撃ボタンを押したか
		int HoldTimer;		// しゃがんでる時間
		int iAerialJumpCoolTime;	// 1フレーム空中ジャンプしないように、数フレームは待つ
		int LandTimer;		// 着地の時間
		void Clear()
		{
			bHold = true;
			bAttackPush = false;
			HoldTimer = iAerialJumpCoolTime = LandTimer = 0;
		}
	};
	Jump m_jump;

	//------------------------------------------------------
	//	キャラが持つアクションデータ
	//------------------------------------------------------
	class ActionData
	{
	public:
		AttackData *pAttackData;

		ActionData() :pAttackData(nullptr){}
		~ActionData() { SAFE_DELETE(pAttackData); }

		bool isAttackData() { return (pAttackData != nullptr); }	// アタックデータがあるということは、攻撃系のステートである
		void InstanceAttackData()
		{
			// isAttackDataでtrueが返るようになる
			if (!pAttackData) pAttackData = new AttackData;
		}
	};

public:
	//------------------------------------------------------
	//	コンストラクタ・初期化
	//------------------------------------------------------
	BasePlayer(SIDE side, const SideData &data);
	void InitAI(AI_TYPE type);	// AI用の初期化

	//------------------------------------------------------
	//	デストラクタ
	//------------------------------------------------------
	virtual ~BasePlayer();

	//------------------------------------------------------
	//	更新
	//------------------------------------------------------
	virtual void Update(PLAYER_UPDATE flag);	// 基本的な更新
	virtual void UpdateDrive();			// ヒーホードライブオーバーフローのときだけ通る更新
	void UpdatePos();			// 座標更新(判定後に呼び出す)
	void MoveUpdate();			// 動きの制御
	void InvincibleUpdate();	 // 無敵の制御
	void OverDriveUpdate();		// 覚醒の制御
	void ColorUpdate(); // 光色の制御

	//------------------------------------------------------
	//	コントロール制御
	//------------------------------------------------------
	void Control();			// プレイヤーからの入力を受け付ける
	void AIControl();

	//------------------------------------------------------
	//	描画
	//------------------------------------------------------
	virtual void Render();
	virtual void Render(tdnShader* shader, char* name);
	virtual void RenderShadow();
	virtual void RenderDrive();
	virtual void RenderDeferred();
	virtual void RenderUI();
	virtual void ShaderSendParam();
	void RenderCommandFrame(int x, int y);
	void RenderDebug();

	//------------------------------------------------------
	//	継承してるやつに強制的に呼ばせる系
	//------------------------------------------------------
	virtual void InitActionDatas() = 0;
	virtual void InitMotionDatas() = 0;

	/****************************/
	//	キャラクター固有の様々なポジション
	/****************************/
	virtual Vector3 GetCenterPos() = 0;	// プレイヤーの真ん中の場所
	virtual Vector3 GetFlontPos() = 0;   // つかみの相手の場所

	/****************************/
	// ラウンド跨ぎの初期化
	/****************************/
	virtual void Reset();

	/****************************/
	// 掛け合いのキャラごとの更新
	/****************************/
	virtual void KakeaiInit() = 0;
	virtual void KakeaiExit(){}
	virtual void KakeaiUpdate() = 0;

	/****************************/
	//	ラッシュ3段目
	/****************************/
	virtual void ThirdRushInit() = 0;		// スキル発動時に呼び出す
	virtual void ThirdRushExit() = 0;
	virtual void ThirdRushUpdate() = 0;//

	/****************************/
	//	足払い(兄貴用)
	/****************************/
	virtual void DownAttackInit()
	{
		// 足払いモーションに変える
		SetMotion(MOTION_TYPE::DOWN_ATTACK);

		// ★攻撃ステートを2段目に設定する　↓でHITフラグを消している
		SetActionState(BASE_ACTION_STATE::DOWN_ATTACK);

		SetDirAngle();
	}
	virtual void DownAttackExit()
	{
		// 先行入力リセット
		AheadCommandReset();
	}
	virtual bool DownAttackUpdate()
	{
		// 攻撃終了してたら
		if (!isAttackState())
		{
			return true;
		}

		// HITしていたらキャンセルできる
		if (GetAttackData()->bHit == true)
		{
			//////////////////////////////////////////////
			//	スキルキャンセル
			//============================================
			if (SkillCancel(this)) return false;

			//////////////////////////////////////////////
			//	スタンドキャンセル
			//============================================
			if (StandCancel(this)) return false;

			//////////////////////////////////////////////
			//	フィニッシュ攻撃ボタン
			//============================================
			if (InvincibleAttackCancel(this)) return false;

			//////////////////////////////////////////////
			//	ヒーホードライブキャンセル
			//============================================
			if (HeaveHoCancel(this)) return false;
		}
		return false;
	}

	/****************************/
	//	無敵攻撃カウンター成功したときのイベント
	/****************************/
	virtual void OnInvincibleCounterSuccess(){ MyAssert(0, "継承しよう。"); }

	/****************************/
	//	キャラクター固有スキル
	/****************************/
	virtual void SkillInit() = 0;		// スキル発動時に呼び出す
	virtual void SkillExit() = 0;
	virtual bool SkillUpdate() = 0;//

	/****************************/
	//	ヒーホードライブ
	/****************************/
	virtual void HeavehoDriveInit() = 0;		// スキル発動時に呼び出す
	virtual void HeavehoDriveExit() = 0;
	virtual bool HeavehoDriveUpdate() = 0;
	virtual void HeavehoDriveHitEvent()		// 最初の一撃が当たったときに呼び出される関数
	{
		// 相手のバースト使用不可に
		GetTargetPlayer()->ActionNotOverDrive();
		// m_bNotOverDrive = true; 
	}		

	/****************************/
	//	ヒーホードライブ_オーバーフロー
	/****************************/
	virtual void HeavehoDriveOverFlowInit() = 0;		// スキル発動時に呼び出す
	virtual void HeavehoDriveOverFlowExit() = 0;
	virtual void HeavehoDriveOverFlowUpdate() = 0;
	virtual void HeavehoDriveOverFlowSuccessInit() = 0;		// 必殺当てて演出中の初期化
	virtual void HeavehoDriveOverFlowSuccessUpdate() = 0;		// 必殺当てて演出中の更新

	/****************************/
	//	キャラクター個別のデータ
	/****************************/
	virtual void SetCharacterData(void* desk) {};
	virtual void CharacterDataReset() {};			//	キャラ毎の初期化情報
	virtual void CharacterDataUIRender() {};			//	キャラ毎の必要なUIの描画
	virtual void CharacterDataUIAction(int iDelayTimer) {};			//	キャラ毎の必要なUIの起動

	/*****************/
	// AIに必要な関数
	/*****************/
	//(考え)　AIもステートマシンを作ればいいのかも　考え->行動ってEnterとExecuteみたいな感じなので　

	// [メモ]まずAI用にステートマシンを作る
	// その中のおもな動きはボタンを押す。ただそれだけ　
	// enterで状況判断　executeでボタン操作　
	// ステートの種類　ふつう（敵をねらう）　自分のポイントが溜まってきた(相手にFAを決めにく)　など
	// 

	// それぞれのキャラクター毎のAIによる技のボタンを設定！
	virtual void AIHighAttackButton() = 0;	// このキャラクターの〆技(強い技)
	
	virtual bool AINearSkill() = 0;			// 各キャラクターの近距離のスキル攻撃
	virtual bool AIMiddleSkill() = 0;		// 各キャラクターの中距離のスキル攻撃
	virtual bool AIFarSkill() = 0;			// 各キャラクターの遠距離のスキル攻撃
	virtual bool AIAerialNearSkill() = 0;	// 各キャラクターの空中近距離のスキル攻撃

	virtual void AIAttackSkillButton() = 0;	// このキャラクターのスキル攻撃(攻撃系スキル発動)

	virtual float AIAttackRange() = 0;		// 攻撃範囲
	virtual float AIRunAttackRange() = 0;	// 走ってるときの攻撃範囲
	virtual float AIThrowRange() = 0;		// 投げの範囲

	//void AI_Brain();	// ここで考え->行動の処理を行う
	//void AI_Think();	// 考える関数
	//void AI_Execute();	// 


	//------------------------------------------------------
	//	メッセージ受信(BaseEntityのオーバーライド)
	//------------------------------------------------------
	bool HandleMessage(const Message& msg); //メッセージを受け取る


	//------------------------------------------------------
	//	ステートマシンのアクセサ
	//------------------------------------------------------
	StateMachine<BasePlayer>* GetFSM()const { return m_pStateMachine; }


	//------------------------------------------------------
	//	位置・移動量
	//------------------------------------------------------
	Vector3 &GetMove() { return m_vMove; }
	Vector3 *GetMoveAddress(){ return &m_vMove; }
	Vector3 &GetPos() { return m_vPos; }
	Vector3 *GetPosAddress() { return &m_vPos; }
	void AddMove(const Vector3 &v){ m_vMove += v; }
	void SetMove(const Vector3 &v){ m_vMove.Set(v.x, v.y, v.z); }
	void SetPos(const Vector3 &v) { m_vPos.Set(v.x, v.y, v.z); }
	void MoveClampX(float val) { m_vMove.x = Math::Clamp(m_vMove.x, -val, val); }
	void SetMoveUpdate(bool bMove){ m_bMoveUpdate = bMove; }
	void SetSinkingUpdate(bool bSinking){ m_bSinkingUpdate = bSinking; }
	bool isSinkingUpdateOK(){ return m_bSinkingUpdate; }

	//------------------------------------------------------
	//	向き・アングル
	//------------------------------------------------------
	DIR GetDir() { return m_dir; }
	void SetDir(DIR dir) { m_dir = dir; }
	void ReverseDir(){ m_dir = (m_dir == DIR::LEFT) ? DIR::RIGHT : DIR::LEFT; }
	DIR GetTargetDir(){ return m_TargetDir; }
	void SetTargetDir(DIR dir){ m_TargetDir = dir; }
	void SetAngleY(float a){ m_fAngleY = a; }
	void SetDirAngle(){ m_fAngleY = DIR_ANGLE[(int)m_dir]; }
	void TurnOverDir(){ m_dir = (m_dir == DIR::LEFT) ? DIR::RIGHT : DIR::LEFT; }
	float GetDirVecX(){ return (m_dir == DIR::LEFT) ? -1.0f : 1.0f; }
	float GetTargetDirVecX(){ return (m_TargetDir == DIR::LEFT) ? -1.0f : 1.0f; }

	//------------------------------------------------------
	//	プレイヤーアクション・攻撃アクション・ステート
	//------------------------------------------------------
	BASE_ACTION_STATE GetActionState(){ return m_eActionState; }
	AttackData *GetAttackData(BASE_ACTION_STATE state)
	{
		// [12/10]
		MyAssert(isAttackState(),"GetAttackDataでアタックデータがないことが判明");	// アタックデータがないステートでアタックデータを参照しようとしている}
		return m_ActionDatas[(int)state].pAttackData;
	}
	AttackData *GetAttackData(){ return GetAttackData(m_eActionState);}// 自分の状態の攻撃データを渡す
	bool isHitAttack() { return (isAttackState()) ? GetAttackData()->bHit : false; }
	bool isFrameAction() { return (m_eActionState != BASE_ACTION_STATE::NO_ACTION); }
	bool isAttackState()
	{
		// ★ステートが何もないのかそうでないか確認と、これがtrueならAttackDataがnull出ないことが保証される
		if (isFrameAction()){ return m_ActionDatas[(int)m_eActionState].isAttackData(); }// アクションデータがnullじゃないかどうかを返す
		else return false;
	}
	bool isActiveFrame()
	{
		if (!isFrameAction()) return false;
		return (m_ActionFrameList[(int)m_eActionState][m_iCurrentActionFrame] == FRAME_STATE::ACTIVE);
	}
	FRAME_STATE GetActionFrame()
	{
		if (!isFrameAction()) return FRAME_STATE::END;
		return m_ActionFrameList[(int)m_eActionState][m_iCurrentActionFrame];
	}
	FRAME_STATE GetActionFrame(BASE_ACTION_STATE state, int frame)
	{
		return m_ActionFrameList[(int)state][frame];
	}
	void SetActionState(BASE_ACTION_STATE state)
	{
		m_eActionState = state;

		// NO_ACTIONじゃなかったら
		if (isFrameAction())
		{
			m_iCurrentActionFrame = 0;				// フレーム0にセット

			// 攻撃だったら
			// ★↓でHITフラグを消している
			if (isAttackState())
			{
				m_ActionDatas[(int)state].pAttackData->bHit = false;	// ヒットフラグリセット
				m_ActionDatas[(int)state].pAttackData->bHitSuccess = false;
			}
		}
	}
	int GetCurrentFrame(){ return m_iCurrentActionFrame; }
	int GetRushStep(){ return m_iRushStep; }
	SKILL_ACTION_TYPE GetSkillActionType(){ return m_eSkillActionType; }
	void AddRushStep(){ m_iRushStep++; }
	void RushStepReset(){ m_iRushStep = 0; }
	void SetSkillActionType(SKILL_ACTION_TYPE type){ m_eSkillActionType = type; }
	bool isDownState(){ return (m_pStateMachine->isInState(*BasePlayerState::KnockDown::GetInstance())); }
	bool isHeavehoDriveState() { return (m_pStateMachine->isInState(*BasePlayerState::HeavehoDrive::GetInstance())); }


	//------------------------------------------------------
	//	地上・空中
	//------------------------------------------------------
	bool isLand() { return m_bLand; }
	bool isSquat() { return m_bSquat; }
	bool isAerialJump() { return m_bAerialJump; }
	bool isAerialDash() { return m_bAerialDash; }
	int  GetAerialDashFrame() { return m_iAerialDashFrame; }
	float GetAerialDashSpeed(){ return m_tagCharacterParam.fAerialDashSpeed; }
	float GetMaxJump() { return m_tagCharacterParam.fMaxJump; }
	Jump *GetJump() { return &m_jump; }
	void SetLand(bool bLand) { m_bLand = bLand; }
	void SetSquat(bool bSquat) { m_bSquat = bSquat; }
	void SetAerialJump(bool bAerialJump) { m_bAerialJump = bAerialJump; }
	void SetAerialDash(bool bAerialDash) { m_bAerialDash = bAerialDash; }
	void SetAerialDashFrame(int iAerialDashFrame) { m_iAerialDashFrame = iAerialDashFrame; }

	//------------------------------------------------------
	//	攻撃のヒットストップ・硬直
	//------------------------------------------------------
	int GetRecoveryFrame() { return m_iRecoveryFrame; }
	int GetHitStopFrame(){ return m_iHitStopFrame; }
	void SetHitStopFrame(int frame){m_iHitStopFrame = frame; }
	void SetRecoveryFrame(int frame) { m_iRecoveryFrame = frame; }
	std::list<BASE_ACTION_STATE> *GetRecoveryDamageCount(){ return &m_RecoveryDamageCount; }
	bool isDown() { return m_bDown; }
	void SetDown(bool down) { m_bDown = down; }

	//------------------------------------------------------
	//	ガード
	//------------------------------------------------------
	GUARD_STATE GetGuardState(){ return m_eGuardState; }
	void SetGuardState(GUARD_STATE state){ m_eGuardState = state; }
	bool isGuardSuccess() { return m_bGuardSuccess; }
	void SetGuardSuccess(bool flag) { m_bGuardSuccess = flag; }

	//------------------------------------------------------
	//	無敵関連
	//------------------------------------------------------
	int GetInvincibleLV(){ return m_iInvincibleLV; }
	int GetInvincibleTime(){ return m_iInvincibleTime; }
	bool isInvincible(){ return  (m_iInvincibleLV >= 1) ? true : false; }
	void SetInvincible(int time, int lv){ m_iInvincibleTime = time; m_iInvincibleLV = lv; }
	void SetInvincibleLV(int lv = 1){ m_iInvincibleLV = lv; }
	void SetInvincibleTime(int time){ m_iInvincibleTime = time; }
	void InvincibleOff(){ m_iInvincibleLV = 0; m_iInvincibleTime = 0; }
	bool isEscape() { return m_bEscape; }
	void SetEscapeFlag(bool bEscape)
	{
		m_bEscape = bEscape; 
		m_bSinkingUpdate = !bEscape;
	}
	bool isInvincibleCounterType(){ return m_tagCharacterParam.bInvincibleCoutner; }

	//------------------------------------------------------
	//	モーション
	//------------------------------------------------------
	void SetMotion(MOTION_TYPE type){ if (m_pObj) m_pObj->SetMotion(m_iMotionNumbers[(int)type]); }
	void SetMotion(int MotionNo)
	{
		if (m_pObj)
		{
			if (m_pObj->GetMotion() != MotionNo)
				m_pObj->SetMotion(MotionNo);
		}
	}


	//------------------------------------------------------
	//	HP・ダメージ
	//------------------------------------------------------
	int GetMaxHP() { return m_tagCharacterParam.iMaxHP; }
	int GetHP() { return m_iHP; }
	void SetHP(int hp) { m_iHP = hp; }
	void AddHP(int add) { m_iHP += add; }
	float GetDamageRate(){ return m_fDamageRate; }
	void AddDamageRate(float rate){ m_fDamageRate = min(m_fDamageRate + rate, 1);}		// 加算(1.0以上にならないようにする)
	void MultDamageRate(float rate){ m_fDamageRate = max(m_fDamageRate * rate, 0.2f); }	// 乗算(0.3以下にはならないようにする) // (12/31) コンボの幅がかなり広がってきたので少しさげます3.0->2.0
	//void MultOverDriveDamageRate(float rate) { m_fDamageRate = max(m_fDamageRate * rate, 0.5f); }	// 必殺技は最低保障値を上げる
	void ResetDamageRate(){ m_fDamageRate = 1.0f; }
	// HPの割合
	int  GetHPPercentage()
	{
		// HPのレートを計算
		MyAssert((m_tagCharacterParam.iMaxHP != 0), "最大HPが0だと0で割ることに");
		float HpRate = (float)m_iHP / (float)m_tagCharacterParam.iMaxHP;
		HpRate *= 100;//　繰り上げる 
		return (int)HpRate;
	};

	//------------------------------------------------------
	//	スコア
	//------------------------------------------------------
	int GetCollectScore(){ return m_iCollectScore; }
	void AddCollectScore(int score) { m_iCollectScore += score; }
	void ConversionScore(){ m_iScore += m_iCollectScore; m_iCollectScore = 0; }
	int GetScore(){ return m_iScore; }


	//------------------------------------------------------
	//	インプット・コマンド
	//------------------------------------------------------
	int GetInputList(PLAYER_INPUT input) { return m_iInputList[(int)input]; }
	void SetInputList(PLAYER_INPUT inputNo, int inputFlag)
	{
		m_iInputList[(int)inputNo] = inputFlag;
		m_wCommandHistory[0] |= (inputFlag | m_iInputList[(int)inputNo] & 0x01) ? (int)c_BitList[(int)inputNo] : 0;
	}
	bool isAnyPushKey(){ return (m_iInputList[(int)PLAYER_INPUT::A] == 3 || m_iInputList[(int)PLAYER_INPUT::B] == 3 || m_iInputList[(int)PLAYER_INPUT::C] == 3 || m_iInputList[(int)PLAYER_INPUT::D] == 3); }
	bool isPushInput(PLAYER_COMMAND_BIT CommandBit, bool bAhead = false) { if (bAhead){ if ((int)CommandBit & (int)m_wAheadCommand) return true; } return (m_wCommandHistory[0] & (int)CommandBit) ? true : false; }
	//PLAYER_COMMAND_BIT GetInputCommandBit(PLAYER_INPUT input)
	//{
	//	PLAYER_COMMAND_BIT bit(PLAYER_COMMAND_BIT::NEUTRAL);

	//	switch (input)
	//	{
	//	case PLAYER_INPUT::A:
	//		bit = PLAYER_COMMAND_BIT::A;
	//		break;
	//	case PLAYER_INPUT::B:
	//		bit = PLAYER_COMMAND_BIT::B;
	//		break;
	//	case PLAYER_INPUT::C:
	//		bit = PLAYER_COMMAND_BIT::C;
	//		break;
	//	case PLAYER_INPUT::D:
	//		bit = PLAYER_COMMAND_BIT::D;
	//		break;
	//	case PLAYER_INPUT::RIGHT:
	//		bit = PLAYER_COMMAND_BIT::RIGHT;
	//		break;
	//	case PLAYER_INPUT::LEFT:
	//		bit = PLAYER_COMMAND_BIT::LEFT;
	//		break;
	//	case PLAYER_INPUT::UP:
	//		bit = PLAYER_COMMAND_BIT::UP;
	//		break;
	//	case PLAYER_INPUT::DOWN:
	//		bit = PLAYER_COMMAND_BIT::DOWN;
	//		break;
	//	case PLAYER_INPUT::R1:
	//		bit = PLAYER_COMMAND_BIT::R1;
	//		break;
	//	case PLAYER_INPUT::R2:
	//		bit = PLAYER_COMMAND_BIT::R2;
	//		break;
	//	case PLAYER_INPUT::R3:
	//		bit = PLAYER_COMMAND_BIT::R3;
	//		break;
	//	case PLAYER_INPUT::L1:
	//		bit = PLAYER_COMMAND_BIT::L1;
	//		break;
	//	case PLAYER_INPUT::L2:
	//		bit = PLAYER_COMMAND_BIT::L2;
	//		break;
	//	case PLAYER_INPUT::L3:
	//		bit = PLAYER_COMMAND_BIT::L3;
	//		break;
	//	}
	//	return bit;
	//}
	bool isPushInputTRG(PLAYER_COMMAND_BIT CommandBit, bool bAhead = false)
	{
		// 先行入力フラグだったら先行入力も返す
		if (bAhead) if ((int)CommandBit & (int)m_wAheadCommand) return true;

		// まず、押した瞬間かどうかを確認
		if ((m_wCommandHistory[0] & (int)CommandBit) == 0) return false;

		// 押したかどうかは分かったので、押しっぱなしでないことを確認する
		if ((m_wCommandHistory[1] & (int)CommandBit)) return false;		// 押しっぱなしやんけ！

		return true;
	}
	WORD GetCommandHistory(int no = 0) { return m_wCommandHistory[no]; }
	WORD GetAheadCommand(){ return m_wAheadCommand; }
	void AheadCommandUpdate()
	{
		FOR((int)PLAYER_INPUT::MAX)
		{
			const PLAYER_COMMAND_BIT bit(c_BitList[i]);
			if (isPushInputTRG(bit)) m_wAheadCommand |= (int)bit;
		}
	}
	void AheadCommandReset(){ m_wAheadCommand &= 0; }
	void AheadCommandReset(PLAYER_COMMAND_BIT ResetBit){ m_wAheadCommand &= ((int)ResetBit ^ 0xffff); }
	bool isDoublePush(PLAYER_COMMAND_BIT InputBit, int JudgeFrame = c_COMMAND_FRAME_MAX)
	{
		// 押した瞬間かどうかを確認
		if (!isPushInputTRG(InputBit)) return false;

		// 2個前に押してたらOK！！
		int iCheckFrame(-1);
		for (int i = 2; i < c_COMMAND_FRAME_MAX; i++)
		{
			if ((m_wCommandHistory[i] & (int)InputBit))
			{
				if (iCheckFrame == -1 && i > 12) break;
				iCheckFrame++;
			}
			else
			{
				if (iCheckFrame >= 0) break;
			}
		}
		return (iCheckFrame != -1 && iCheckFrame < JudgeFrame);
	}

	//int GetCOMMAND_FRAME_MAX() { return c_COMMAND_FRAME_MAX; }
	
	//------------------------------------------------------
	//	スタンド
	//------------------------------------------------------
	Stand::Base *GetStand(){ return m_pStand; }


	//------------------------------------------------------
	//	対戦相手のポインタ
	//------------------------------------------------------
	void SetTargetPlayer(BasePlayer *pPlayer){ m_pTargetPlayer = pPlayer; }
	BasePlayer *GetTargetPlayer(){ return m_pTargetPlayer; }


	//------------------------------------------------------
	//	エフェクト
	//------------------------------------------------------
	PanelEffectManager *GetPanelEffectManager() { return m_pPanelEffectMGR; }
	UVEffectManager *GetUVEffectManager() { return m_pUVEffectMGR; }
	void AddEffectAction(Vector3 pos, EFFECT_TYPE effectType, Vector3 AttackVec = VECTOR_ZERO);
	void GuardEffectAction();
	void GuardEffectStop();
	void GuardEffectUpdate();
	tdn2DAnim* GetThrowMark() { return m_pThrowMark; }

	//------------------------------------------------------
	//	覚醒
	//------------------------------------------------------
	float GetOverDriveGage(){ return m_OverDriveGage; } // 覚醒ゲージ
	void AddOverDriveGage(float add) { m_OverDriveGage = max(0, min(m_OverDriveGage + add, 100)); }
	void SetOverDriveGage(float set) { m_OverDriveGage = set; }
	bool isOverDrive(){ return m_bOverDrive; } // 覚醒してるか
	int GetOverDriveFrame(){ return m_OverDriveFrame; } // 覚醒時間
	void SetOverDriveFrame(int frame) { m_OverDriveFrame = frame; } // 覚醒時間
	OVERDRIVE_TYPE GetOverDriveType(){ return m_OverDriveType; }	// 覚醒の種類
	void ActionOverDrive(OVERDRIVE_TYPE type);

	//------------------------------------------------------
	//	根性
	//------------------------------------------------------
	bool isWillPower() { return m_bWillPower; }
	void WillPowerUpdate(PLAYER_UPDATE updateFlag);

	//------------------------------------------------------
	//	ウィンドウ用 
	//------------------------------------------------------
	BaseWindow* GetCommandWindow() { return m_pCommandWindow; };

	//------------------------------------------------------
	//	キャラクターのタイプ
	//------------------------------------------------------
	CHARACTER GetCharacterType() { return m_eCharacterType; };

	//------------------------------------------------------
	//	その他
	//------------------------------------------------------
	int GetDeviceID() { return m_iDeviceID; }
	//bool isMaxSpeed() { return (abs(m_move.x) >= m_maxSpeed); }
	float GetMaxSpeed() { return m_tagCharacterParam.fMaxSpeed; }
	SIDE GetSide(){ return m_side; }
	CollisionShape::Square *GetHitSquare() { return &m_tagCharacterParam.HitSquare; }	// ステージのあたり判定用

	// UI
	//ComboUI* GetComboUI(){ return m_pComboUI; }

	// 最低ダッシュフレーム
	void AddDushFrame(int add) { m_iDashFrame += add; }
	void SetDushFrame(int set) { m_iDashFrame = set; }
	int  GetDushFrame() { return m_iDashFrame; }

	// 
	bool isGameTimerStopFlag() { return m_bGameTimerStopFlag; }
	void SetGameTimerStopFlag(bool flag) { m_bGameTimerStopFlag = flag; }

	// ひーほー
	EFFECT_CAMERA_ID GetOverFlowCameraID(){ return m_tagCharacterParam.eHeaveHoOverFlowCameraID; }

	void SetLastOneWin(){ m_bLastOneWin = true; }

	// 覚醒かつラウンド取ったら勝ち状態のチェック(一撃必殺)
	bool isHeaveHoDriveOverFlowOK(){ return (m_bLastOneWin && m_pTargetPlayer->isWillPower()); }

	CUTIN_TYPE_NAME GetCutInType(){ return m_tagCharacterParam.eCutInType; }

	iex3DObj *GetDefaultObj(){ return m_pDefaultObj; }
	iex3DObj *GetObj(){ return m_pObj; }

	// オブジェ切り替え
	void ChangeHeaveHoDriveOverFlowObj()
	{
		m_pObj = m_pHHDOFObj;
		m_pObj->SetMotion(0);
		m_pObj->SetFrame(0);
	}

	// 勝利数
	int GetWinNum() { return m_iWinNum; }
	void AddWinNum(int add) { m_iWinNum += add; }

	// 顔グラ
	tdn2DAnim* GetFacePic() { return m_pFacePic; }

	//
	tdn2DAnim* GetCutinPic() { return m_pCutinPic; };
	char* GetName() { return m_pName; };

	// オーバードライブゲージが使えない
	bool isNotOverDrive() { return m_bNotOverDrive; }
	void SetNotOverDrive(bool flag) { m_bNotOverDrive = flag; }
	void ActionNotOverDrive() { m_bNotOverDrive = true; /*m_pNotOverDriveMark->Action(); */}
	void StopNotOverDrive() {	m_bNotOverDrive = false;/* m_pNotOverDriveMark->Stop();*/}

	// AIか
	bool isAI() { return m_bAI; }
	AI* GetAI() { return m_pAI; }

	// カラー
	virtual void ChangeColor() = 0;// (12/28) 後で色変えれるようにしたい

	//_/_/_/_/_/_/__/_/__/_/__/_/__
	// 定数
	//_/_/_/_/__/_/__/_/__/_/__/_/_
	static const float	c_END_MOVE_LINE;		// 移動が終わって大気に戻る移動値ライン
	static const int	c_RUSH_AHEAD_START;		// ラッシュ中の先行入力受付開始フレーム
	static const float	c_GRAVITY;				// 全員が共通で持つ世界の重力
	static const float	c_GRAVITY_MAX;			// 重力の最大値
	//static const float	c_MAX_JUMP;				// ジャンプ最大ライン
	static const int	c_RECOVERY_FLAME;		// リカバリ―フレーム
	static const int	c_OVERDRIVE_MAX_GAGE;	// 覚醒ゲージの最大値
	static const int	c_OVERDRIVE_MAX_TIME;	// 覚醒が切れるまでの時間
	static const int	c_THROW_ESCAPE_FRAME;	// 投げぬけの猶予フレーム
	static const int	c_THROW_MISS_FRAME;		// 投げ失敗のフレーム(全キャラ共通だろうという考え)
	static const int	c_THROW_RELEASE_FRAME;	// 投げ抜けで、パシンてなってる間のフレーム(これも全キャラ共通だろう)
	static const int	c_WINNER_TIME;			// 勝ちモーション再生時間
	static const float	c_GUARD_DISTANCE;		// ガードが発動する距離
	static const int	c_FIRST_HIT_ADD_DAMAGE;	// 初段ヒット加算ダメージ
	static const int	c_COUNTER_BONUS_DAMAGE;	// カウンターヒット時のボーナス加算値(ダメージ)

protected:
	//------------------------------------------------------
	//	キャラが固有で持つ値
	//------------------------------------------------------
	struct CharacterParam
	{
		int iMaxHP;									// 最大HP
		float fMaxSpeed;							// 最大スピード(実質走る速さ)
		float fMaxJump;								// 最大ジャンプ力
		float fAerialDashSpeed;						// 空中ダッシュの速度
		bool bInvincibleCoutner;					// 無敵攻撃がカウンタータイプか
		CollisionShape::Square HitSquare;			// 四角判定(ステージ衝突で使う)
		EFFECT_CAMERA_ID eHeaveHoOverFlowCameraID;	// 一撃必殺カメラのID
		CUTIN_TYPE_NAME eCutInType;					// 一撃必殺カットインのID
	}m_tagCharacterParam;
	CharacterParam m_tagOrgCharacterParam;			// 初期の情報を保存
	
	//------------------------------------------------------
	//	位置・移動量
	//------------------------------------------------------
	Vector3 m_vPos;			// 座標
	Vector3 m_vMove;		// 移動値
	bool m_bMoveUpdate;		// これがtrueなら、重力とか空気抵抗とかを受ける(アイルードリルとかペルソナとかでfalseにする)
	bool m_bSinkingUpdate;	// これがtrueなら、キャラとキャラのめり込み判定を行う

	//------------------------------------------------------
	//	向き・アングル
	//------------------------------------------------------
	float m_fAngleY;		// Yの回転角度
	DIR m_dir;				// 左右のフラグ
	DIR m_TargetDir;		// 相手に対する左右のフラグ


	//------------------------------------------------------
	//	プレイヤーアクション・攻撃アクション・ステート
	//------------------------------------------------------
	FRAME_STATE m_ActionFrameList[(int)BASE_ACTION_STATE::END][FRAME_MAX];
	ActionData m_ActionDatas[(int)BASE_ACTION_STATE::END];
	BASE_ACTION_STATE m_eActionState;
	int m_iCurrentActionFrame;				// 攻撃フレームリストの中を再生しているフレーム
	int m_iRushStep;						// ラッシュアタックのステップ
	SKILL_ACTION_TYPE m_eSkillActionType;	// スキルアクション


	//------------------------------------------------------
	//	地上・空中
	//------------------------------------------------------
	bool m_bLand;			// 着地フラグ
	bool m_bSquat;			// しゃがみフラグ
	bool m_bAerialJump;		// 空中ジャンプのフラグ
	bool m_bAerialDash;		// 空中ダッシュのフラグ
	int m_iAerialDashFrame;	// 空中ダッシュの時間

	//------------------------------------------------------
	//	攻撃のヒットストップ・硬直
	//------------------------------------------------------
	std::list<BASE_ACTION_STATE> m_RecoveryDamageCount;	//「喰らい中」に攻撃をくらってるカウント
	int m_iRecoveryFrame;								// 1以上なら0になるまで操作を受け付けない
	int m_iHitStopFrame;									// 1以上なら0になるまでストップ
	bool m_bDown;										// ダウン状態

	//------------------------------------------------------
	//	ガード
	//------------------------------------------------------
	GUARD_STATE m_eGuardState;
	bool m_bGuardSuccess;

	//------------------------------------------------------
	//	無敵関連
	//------------------------------------------------------
	int m_iInvincibleLV;				// 無敵かどうか
	int m_iInvincibleTime;				// 無敵時間		無敵時は0以上の値が入り、0になるまでデクリメントされる
	float m_fInvincibleColRate;			// 無敵時間の点滅のレート
	int m_iInvincibleColRateFlame;		// 無敵点滅のフレーム(速さ）
	bool m_bInvincibleColRateUpFlag;	// 無敵点滅の上げ下げフラグ
	bool m_bEscape;						// エスケープ判定の時に立つフラグ


	//------------------------------------------------------
	//	モーション
	//------------------------------------------------------
	int m_iMotionNumbers[(int)MOTION_TYPE::MAX];


	//------------------------------------------------------
	//	HP・ダメージ
	//------------------------------------------------------
	int m_iHP;
	float m_fDamageRate;				// 自分にダメージのかかる補正


	//------------------------------------------------------
	//	スコア
	//------------------------------------------------------
	int m_iScore;				// 実体後の本当のスコア
	int m_iCollectScore;			// 実体前の貯めているスコア


	//------------------------------------------------------
	//	インプット・コマンド
	//------------------------------------------------------
	static const int c_COMMAND_FRAME_MAX = 30;
	int m_iInputList[(int)PLAYER_INPUT::MAX]; 	// 押しているキーを格納
	WORD m_wCommandHistory[c_COMMAND_FRAME_MAX];
	WORD m_wAheadCommand;	// 先行入力用

	//------------------------------------------------------
	//	スタンド
	//------------------------------------------------------
	Stand::Base *m_pStand;		// スタンドの基底クラスの実体


	//------------------------------------------------------
	//	対戦相手のポインタ
	//------------------------------------------------------
	BasePlayer *m_pTargetPlayer;		// 相手プレイヤーのポインタを持つ(好き勝手にいじる)

	//------------------------------------------------------
	//	オブジェクトの実体
	//------------------------------------------------------
	iex3DObj *m_pDefaultObj, *m_pHHDOFObj;	// メッシュ実体
	iex3DObj *m_pObj;				// 参照するだけのメッシュ実体
	BaseUVEffect *m_pSpeedLine;		// 超必殺技のスピードライン
	tdn2DAnim* m_pFacePic;			// 顔グラ


	//------------------------------------------------------
	//	エフェクト
	//------------------------------------------------------
	PanelEffectManager* m_pPanelEffectMGR;	// パラパラアニメエフェクト 
	UVEffectManager* m_pUVEffectMGR;			// UVエフェクト
	tdn2DAnim*		m_pThrowMark;			//	投げマーク

	//------------------------------------------------------
	//	覚醒
	//------------------------------------------------------
	float m_OverDriveGage;			// 覚醒ゲージ
	bool m_bOverDrive;				// 覚醒してるか
	int m_OverDriveFrame;			// 覚醒時間
	OVERDRIVE_TYPE m_OverDriveType;	// 覚醒の種類

	//------------------------------------------------------
	//	根性
	//------------------------------------------------------
	bool m_bWillPower;				// 根性値フラグ
	int m_iSpGettingFrame;			// SPが回復していく頻度 

	//------------------------------------------------------
	//	トレーニング用	(12/24) 外部のデータで設定するのでメンバ変数は作らない 
	//------------------------------------------------------

	//------------------------------------------------------
	//	ウィンドウ用 
	//------------------------------------------------------
	BaseWindow* m_pCommandWindow;

	//------------------------------------------------------
	//	キャラクターのタイプ
	//------------------------------------------------------
	CHARACTER m_eCharacterType;

	//------------------------------------------------------
	//	その他
	//------------------------------------------------------
	const int m_iDeviceID;		// 自分のコントローラーの番号(実質、スマブラのxPに値する)
	SIDE m_side;			// このキャラクターの所属してるチーム		
	StateMachine<BasePlayer>* m_pStateMachine; // ★ステートマシン
	AI*						  m_pAI;			// ★AI
	bool m_bAI;					// AIかどうか

	int m_iDashFrame;			//  ダッシュの最低時間

	// ヒーホー用の変数
	int m_iHeavehoStopTimer;			// ヒーホードライブ発動のザワールドの時間
	int m_iHeaveHoDriveOverFlowFrame;	// ヒーホーの必殺のSEとかエフェクト用に作る
	bool m_bLastOneWin;					// あと1ラウンドで勝利(一撃必殺のフラグで使う)

	float m_fOrangeColRate;		// 無敵技のオレンジの光レート
	float m_fMagentaColRate;	// 中段技のマゼンタの光レート

	// ストップフラグ
	bool m_bGameTimerStopFlag;

	// ラウンド獲得数
	int m_iWinNum;

	// オーバードライブゲージが使えない
	bool m_bNotOverDrive;		 


	// (メモ)　//////////////////////////////////////////////////////////
	// エフェクトマネージャーを一人ずつ持たせた理由は
	// 一つのマネージャーで追加していく形だと　可変長配列で増やしていくが
	// ゲーム中にnew Deleteはとても重かった
	// そもそもダメージエフェクトなどキャラクターに一つあればいいと思った
	//	4人対戦で5つもヒットエフェクトがでることはないので　
	// 初期ロードは重くなるがゲーム中おもくなるよりマシ。

	// 継承してるやつに強制的に呼ばせる
	void LoadCharacterParam(LPSTR filename);
	void LoadAttackFrameList(LPSTR filename);
	void LoadAttackDatas(LPSTR filename);

	// UI
	//ComboUI* m_pComboUI;

	/*****************************/ 
	// キャラクターの立ち絵＆名前
	tdn2DAnim* m_pCutinPic;
	char* m_pName;


};


