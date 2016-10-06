#pragma once
// エンティティ関連のインクルード
#include "../BaseEntity/Message/Message.h"
#include "../BaseEntity/Entity/BaseGameEntity.h"
#include "../BaseEntity/Entity/EntityEnum.h"
#include "../BaseEntity/State/StateMachine.h"
#include "BasePlayerState.h"

// 判定のインクルード
#include "../Collision/Collision.h"

// エフェクト関連
#include "../Effect/PanelEffect/PanelEffectManager.h"
#include "../Effect\UVEffect\UVEffectManager.h"

// UI
#include "../UI/ComboUI.h"


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

// 定数
enum class PLAYER_INPUT
{
	A,		// ヒーホードライブキャンセル
	B,		// 攻撃
	C,		// ジャンプ
	D,		// キャラ固有
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

// キーボード表
static const KEYCODE KeyCodeList[(int)PLAYER_INPUT::MAX] = {
	KEY_A,
	KEY_B,
	KEY_C,
	KEY_D,
	KEY_RIGHT,
	KEY_LEFT,
	KEY_UP,
	KEY_DOWN,
	KEY_R1,
	KEY_R2,
	KEY_R3,
	KEY_L1,
	KEY_L2,
	KEY_L3,
	KEY_START,
	KEY_SELECT,
};

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
	END	// 終了(このフレームに来たら攻撃終了)
};
static const int FRAME_MAX = 256; // 攻撃のフレームの最大値(これより超えることはないだろう)

// 全キャラ固有で持つアクション
enum class BASE_ACTION_STATE
{
	NO_ACTION = -1,
	ESCAPE,		// 回避
	STAND,		// スタンド発動
	OVERDRIVE_ONEMORE,	// ワンモア覚醒
	OVERDRIVE_BURST,	// バースト覚醒
	RUSH1,		// 通常1段目
	RUSH2,		// 通常2段目
	RUSH3,		// 通常3段目
	SQUAT,		// 下段攻撃
	AERIAL,		// 空中攻撃
	AERIALDROP,	// 空中下攻撃
	FINISH,		// フィニッシュアーツ
	SKILL,		// 地上キャラクター固有技
	THROW,		// 投げ
	HEAVEHO_DRIVE,// ヒーホードライブ
	HEAVEHO_DRIVE_OVERFLOW,// ヒーホードライブ_オーバーフロー
	FRAMECOUNT,	// ★とりあえず一定フレーム計りたいというときに、今使っているのは(投げ失敗の間だったり、投げ抜けの間だったり。)
	END		// 何もなし　(A列車)名前
};

enum class MOTION_TYPE
{
	WAIT,
	WALK,
	RUN,
	JUMP,
	FALL,
	RAND,
	BRAKE,
	U_TURN,
	SQUAT,
	RUSH_ATTACK1,
	RUSH_ATTACK2,
	RUSH_ATTACK3,
	SQUAT_ATTACK,
	AERIAL_ATTACK,
	AERIALDROP_ATTACK,
	FINISH_ATTACK,
	CHARA_ATTACK_LAND,
	CHARA_ATTACK_SQUAT,
	CHARA_ATTACK_AERIAL,
	CHARA_ATTACK_AERIALDROP,
	BURST,
	ESCAPE,
	PERSONA,
	KNOCKBACK,
	KNOCKDOWN_RAND,
	KNOCKDOWN_AERIAL,
	HEAVEHO_DRIVE,
	HEAVEHO_DRIVE_OVERFLOW,
	DIE,
	WIN,
	MAX
};

// エフェクトの種類
enum class EFFECT_TYPE
{
	DAMAGE,	// ダメージエフェクト
	WHIFF,	//	振り
	RECOVERY, //リカバリー
	PERSONA,  //　ペ...ル..ソ..ナ!!
	DROP_IMPACT,// ドロップインパクト
	UPPER,		// アッパー
	FINISH_HIT,		// フィニッシュアーツヒット
	BURST,		// バースト
	ONEMORE_BURST,// ワンモアバースト
	ONEMORE_BURST_START,// ワンモアバーストの前
	RUN,			// 走る瞬間
	GUARD_BREAK,		// ガードブレイク
 	//AIROU_CIRCLE,// アイルーサークル
	

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
	int HitScore;				// 加算されるスコア
	int damage;					// 与えるダメージ
	bool bHit;					// 当たったかどうか(★多段ヒット防止用)多段ヒットしてほしい攻撃だと、また考える必要がある
	int WhiffDelayFrame;		// 何フレーム後に空振りのSEを再生するか(WhiffSEを使わない(nullptr)なら別に設定しなくてもいい)
	EFFECT_TYPE HitEffectType;	// エフェクトのタイプ
	LPCSTR HitSE;				// 当たったときに鳴らすSE
	LPCSTR WhiffSE;				// 空振りSE
	EFFECT_TYPE WhiffEffectType;	// 振りエフェクト
	int pierceLV;				// 貫通レベル
	bool bAntiAir;				// 対空攻撃かどうか　(追加)
	SHAKE_CAMERA_INFO ShakeCameraInfo;	// カメラ振動用構造体

	// ★★★地上ヒットと空中ヒットで分けたい情報
	struct
	{
		bool bBeInvincible;			// 無敵になるかどうか(たとえば、通常の1.2段目ならfalseだし、3段目ならtrue)
		//Vector2 LandFlyVector;		// 当たって吹っ飛ばすベクトル(★基本的にxは+にすること)
		//Vector2 AerialFlyVector;	// ※相手が空中にいた時　当たって吹っ飛ばす空中ベクトル(★基本的にxは+にすること)
		Vector2 FlyVector;
		int hitStopFlame;			// ヒットストップの時間
		int recoveryFlame;			// 硬直の時間
	}places[(int)HIT_PLACE::MAX];
	AttackData() :HitSE(nullptr), WhiffSE(nullptr), bHit(false), pCollisionShape(new CollisionShape::Square){}
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
	struct Jump
	{
		bool bHold;	// しゃがんでる状態
		bool bAttackPush;	// しゃがんでる最中に攻撃ボタンを押したか
		int HoldTimer;		// しゃがんでる時間
		int PlayerHoldTimer;	// しゃがんでる間にプレイヤーからジャンプボタンを受け付ける時間
		int LandTimer;		// 着地の時間
		void Clear()
		{
			bHold = true;
			bAttackPush = false;
			HoldTimer = PlayerHoldTimer = LandTimer = 0;
		}
	};
	Jump m_jump;

	struct RushAttack
	{
		int step;	// 0,1,2で進める
		//bool bNextOK;	// これがtrueの時にボタンを押すと次の攻撃に行く
		void Clear()
		{
			step = 0;
			//bNextOK = false;
		}
	};
	RushAttack m_RushAttack;

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
	}m_ActionDatas[(int)BASE_ACTION_STATE::END];

public:
	BasePlayer(int deviceID, SIDE side, bool bAI);
	~BasePlayer();
	void InitAI();
	void Control();			// プレイヤーからの入力を受け付ける
	void AIControl();
	virtual void Update(bool bControl);	// 基本的な更新
	void UpdatePos();		// 座標更新(判定後に呼び出す)
	void Move();			// 動きの制御
	void InvincibleUpdate(); // 無敵の制御
	void OverDriveUpdate(); // 覚醒の制御
	void ActionOverDrive(OVERDRIVE_TYPE type); // 覚醒発動

	virtual void Render();
	virtual void Render(tdnShader* shader, char* name);
	virtual void RenderDeferred();
	virtual void RenderUI();
	
	/****************************/
	//	キャラクター固有の様々なポジション
	/****************************/
	virtual Vector3 GetCenterPos() = 0;	// プレイヤーの真ん中の場所
	virtual Vector3 GetHoldPos() = 0;   // つかみの相手の場所

	/****************************/
	//	キャラクター固有スキル
	/****************************/
	virtual void SkillInit() = 0;		// スキル発動時に呼び出す
	virtual void SkillExit() = 0;
	virtual void SkillUpdate() = 0;//

	/****************************/
	//	ヒーホードライブ
	/****************************/
	virtual void HeavehoDriveInit() = 0;		// スキル発動時に呼び出す
	virtual void HeavehoDriveExit() = 0;
	virtual void HeavehoDriveUpdate() = 0;

	/****************************/
	//	ヒーホードライブ_オーバーフロー
	/****************************/
	virtual void HeavehoDriveOverFlowInit() = 0;		// スキル発動時に呼び出す
	virtual void HeavehoDriveOverFlowExit() = 0;
	virtual void HeavehoDriveOverFlowUpdate() = 0;
	virtual void HeavehoDriveOverFlowSuccess() = 0;		// 必殺当てて演出中の更新

		
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
	virtual void AIHighAtackButton() = 0;	// このキャラクターの〆技(強い技)
	virtual void AIAtackSkillButton() = 0;	// このキャラクターのスキル攻撃(攻撃系スキル発動)

	virtual float AIAtackRange() = 0;		// 攻撃範囲
	virtual float AIRunAtackRange() = 0;	// 走ってるときの攻撃範囲

	//void AI_Brain();	// ここで考え->行動の処理を行う
	//void AI_Think();	// 考える関数
	//void AI_Execute();	// 


	// 継承してるやつに強制的に呼ばせる
	virtual void InitActionDatas() = 0;
	virtual void InitMotionDatas() = 0;

	bool HandleMessage(const Message& msg); //メッセージを受け取る

	// ゲッター
	int GetDeviceID() { return m_deviceID; }
	Vector3 &GetMove() { return m_move; }
	Vector3 *GetMoveAddress(){ return &m_move; }
	//Vector3 &GetStandSaveMove(){ return m_StandSaveMove; }
	Vector3 &GetPos() { return m_pos; }
	Vector3 *GetPosAddress() { return &m_pos; }
	int GetInputList(PLAYER_INPUT input) { return m_InputList[(int)input]; }
	bool isPushInput(PLAYER_INPUT input) { return (m_InputList[(int)input] == 1 || m_InputList[(int)input] == 3); }	// 押した瞬間と押している間どちらかだったらtrue
	StateMachine<BasePlayer>* GetFSM()const { return m_pStateMachine; }// ★ステートマシンのアクセサを作る
	DIR GetDir() { return m_dir; }
	bool isMaxSpeed() { return (abs(m_move.x) >= m_maxSpeed); }
	bool isLand() { return m_bLand; }
	bool isAerialJump() { return m_bAerialJump; }
	Jump *GetJump() { return &m_jump; }
	RushAttack *GetRushAttack() { return &m_RushAttack; }
	float GetMaxSpeed() { return m_maxSpeed; }
	BASE_ACTION_STATE GetActionState(){ return m_ActionState; }
	SIDE GetSide(){ return m_side; }

	int GetMaxHP() {return m_MaxHP;	}
	
	int GetHP() {	return m_HP; }
	void SetHP(int hp) { m_HP = hp; }


	void SetInputList(PLAYER_INPUT inputNo, int inputFlag){ m_InputList[(int)inputNo] = inputFlag; }

	AttackData *GetAttackData(BASE_ACTION_STATE state)
	{
		if (!isAttackState())
		{
			int i = 0;
			assert(isAttackState());	// アタックデータがないステートでアタックデータを参照しようとしている
		}
		return m_ActionDatas[(int)state].pAttackData;
	}

	AttackData *GetAttackData()
	{
		// 自分の状態の攻撃データを渡す
		return GetAttackData(m_ActionState);
	}

	int GetRecoveryCount(){ return m_recoveryCount; }

	//int GetPierceLV(){ return m_ActionDatas[(int)m_ActionState].pierceLV; }
	//LPCSTR GetAttackSE(){ return m_ActionDatas[(int)m_ActionState].SE_ID; }
	bool isFrameAction() { return (m_ActionState != BASE_ACTION_STATE::NO_ACTION); }
	bool isAttackState()	// ★ステートが何もないのかそうでないか確認と、これがtrueならAttackDataがnull出ないことが保証される
	{
		// 何かしらアクションしてたあ
		if (isFrameAction())
		{
			// アクションデータがnullじゃないかどうかを返す
			return m_ActionDatas[(int)m_ActionState].isAttackData();
		}
		else return false;
	}
	int GetInvincibleLV(){ return m_InvincibleLV; }
	int GetInvincibleTime(){ return m_InvincibleTime; }
	bool isInvincible(){ return  (m_InvincibleLV >= 1) ? true : false; }

	bool isActiveFrame()
	{
		if (!isFrameAction()) return false;
		return (m_ActionFrameList[(int)m_ActionState][m_CurrentActionFrame] == FRAME_STATE::ACTIVE);
	}
	FRAME_STATE GetActionFrame()
	{
		if (!isFrameAction()) return FRAME_STATE::END;
		return m_ActionFrameList[(int)m_ActionState][m_CurrentActionFrame];
	}
	int GetRecoveryFrame() { return m_RecoveryFlame; }
	bool isEscape() { return m_bEscape; }
	Stand::Base *GetStand(){ return m_pStand; }
	std::list<BASE_ACTION_STATE> *GetRecoveryDamageCount(){ return &m_RecoveryDamageCount; }
	bool isDownState(){ return (m_pStateMachine->isInState(*BasePlayerState::KnockDown::GetInstance())); }

	// セッター
	void SetMove(const Vector3 &v)
	{
		if (v.y > 0)
			int i = 0;
		m_move.Set(v.x, v.y, v.z);
	}
	void SetPos(const Vector3 &v) { m_pos.Set(v.x, v.y, v.z); }
	void SetDir(DIR dir) { m_dir = dir; }
	void SetMotion(MOTION_TYPE type)
	{
		if (!m_pObj) return;
		m_pObj->SetMotion(m_MotionNumbers[(int)type]);
		//SetMotion(m_MotionNumbers[(int)type]); 
	}
	void SetMotion(int MotionNo) { if (m_pObj) { if (m_pObj->GetMotion() != MotionNo)m_pObj->SetMotion(MotionNo); } }
	void SetLand(bool bLand) { m_bLand = bLand; }
	void SetAerialJump(bool bAerialJump) { m_bAerialJump = bAerialJump; }
	void SetHitStopFrame(int frame) { m_HitStopFrame = frame; }
	void SetRecoveryFrame(int frame) { m_RecoveryFlame = frame; }
	void SetEscapeFlag(bool bEscape) { m_bEscape = bEscape; }
	void SetInvincible(int time, int lv){ m_InvincibleTime = time; m_InvincibleLV = lv; }
	void SetInvincibleLV(int lv = 1){ m_InvincibleLV = lv; }
	void SetInvincibleTime(int time){ m_InvincibleTime = time; }
	void InvincibleOff(){ m_InvincibleLV = 0; m_InvincibleTime = 0; }
	void SetActionState(BASE_ACTION_STATE state)
	{
		m_ActionState = state;

		// NO_ACTIONじゃなかったら
		if (isFrameAction())
		{
			m_CurrentActionFrame = 0;				// フレーム0にセット

			// 攻撃だったら
			// ★↓でHITフラグを消している
			if (isAttackState())
			{
				m_ActionDatas[(int)state].pAttackData->bHit = false;	// ヒットフラグリセット
			}
		}
	}
	int GetCurrentFrame(){ return m_CurrentActionFrame; }
	//void SetStandSaveMove(const Vector3 &move){ m_StandSaveMove = move; }
	void SetRecoveryCount(int recoverycount){ m_recoveryCount = recoverycount; }
	void SetMoveUpdate(bool bMove){ m_bMoveUpdate = bMove; }
	void TurnOverDir(){ m_dir = (m_dir == DIR::LEFT) ? DIR::RIGHT : DIR::LEFT; }

	// アクセサー
	void AddMove(const Vector3 &v)
	{
		m_move += v;
	}
	int GetCollectScore(){ return m_CollectScore; }
	void AddCollectScore(int score) { m_CollectScore += score; }
	void ConversionScore()
	{
		m_score += m_CollectScore;
		m_CollectScore = 0;
	}

	int GetScore(){ return m_score; }
	//void SetScore(){ return m_score; }

	void AddRecoveryCount(int add){ m_recoveryCount += add; }

	void MoveClampX(float val) { m_move.x = Math::Clamp(m_move.x, -val, val); }

	// ガードしているか？ 
	bool isGuard(){ return m_bGuard; }
	int GetGuardFollowFrame(){ return m_GuardFollowFrame; }

	void SetGuard(bool isguard){ m_bGuard = isguard; }
	void SetGuardFollowFrame(int followFrame){ m_GuardFollowFrame = followFrame; }

	void AddGuardFollowFrame(){ m_GuardFollowFrame++; }

	// エフェクト
	PanelEffectManager* GetPanelEffectManager() { return m_PanelEffectMGR; }
	UVEffectManager* GetUVEffectManager() { return m_UVEffectMGR; }
	void AddEffectAction(Vector3 pos,EFFECT_TYPE effectType);
	void GuardEffectAction();
	void GuardEffectStop();
	void GuardEffectUpdate();

	// ステージのあたり判定用
	CollisionShape::Square *GetHitSquare() { return m_pHitSquare; }

	// 覚醒
	int GetOverDriveGage(){ return m_OverDriveGage; } // 覚醒ゲージ
	bool isOverDrive(){ return m_bOverDrive; } // 覚醒してるか
	int GetOverDriveFrame(){ return m_OverDriveFrame; } // 覚醒時間
	OVERDRIVE_TYPE GetOverDriveType(){ return m_OverDriveType; }	// 覚醒の種類

	//  投げてる人のID
	ENTITY_ID  GetThrowPlayerID() { return m_ThrowPlayerID; }
	void SetThrowPlayerID(ENTITY_ID id){ m_ThrowPlayerID = id; }
	bool isThrowSuccess(){ return m_bThrowSuccess; }
	void SetThrowSuccess(bool bSuccess){ m_bThrowSuccess = bSuccess; }

	// UI
	ComboUI* GetComboUI(){ return m_pComboUI; }

	// 
	bool isGameTimerStopFlag() { return m_bGameTimerStopFlag; }
	void SetGameTimerStopFlag(bool flag) { m_bGameTimerStopFlag = flag; }

	//_/_/_/_/_/_/__/_/__/_/__/_/__
	// 定数
	//_/_/_/_/__/_/__/_/__/_/__/_/_
	static const float	c_END_MOVE_LINE;		// 移動が終わって大気に戻る移動値ライン
	static const float	c_FRONT_BRAKE_LINE;		// 走り中に操作を離してブレーキに移行するまでの移動値ライン
	static const float	c_GRAVITY;				// 全員が共通で持つ世界の重力
	static const float	c_MAX_JUMP;				// ジャンプ最大ライン
	static const int	c_RECOVERY_FLAME;		// リカバリ―フレーム
	static const int	c_OVERDRIVE_MAX_GAGE;	// 覚醒ゲージの最大値
	static const int	c_OVERDRIVE_MAX_TIME;	// 覚醒が切れるまでの時間
	static const int	c_THROW_ESCAPE_FRAME;	// 投げぬけの猶予フレーム
	static const int	c_THROW_MISS_FRAME;		// 投げ失敗のフレーム(全キャラ共通だろうという考え)
	static const int	c_THROW_RELEASE_FRAME;	// 投げ抜けで、パシンてなってる間のフレーム(これも全キャラ共通だろう)

protected:
	const int m_deviceID;		// 自分のコントローラーの番号(実質、スマブラのxPに値する)
	const SIDE m_side;			// このキャラクターの所属してるチーム

	iex3DObj *m_pObj;	// メッシュ実体
	Vector3 m_pos;	// 座標
	Vector3 m_move;	// 移動値
	CollisionShape::Square *m_pHitSquare;	// 四角判定(ステージ衝突で使う)
	DIR m_dir;				// 左右のフラグ
	//float m_Gravity;		// 重力値
	float m_maxSpeed;		// キャラクターの最大スピード 
	float m_angleY;
	bool m_bLand;			// 着地フラグ
	bool m_bAerialJump;		// 空中ジャンプのフラグ
	int m_InputList[(int)PLAYER_INPUT::MAX]; 	// 押しているキーを格納
	StateMachine<BasePlayer>* m_pStateMachine; // ★ステートマシン
	AI*						  m_pAI;			// ★AI
	int m_InvincibleLV;			// 無敵かどうか
	int m_InvincibleTime;		// 無敵時間		無敵時は0以上の値が入り、0になるまでデクリメントされる
	float m_InvincibleColRate;	// 無敵時間の点滅のレート
	int m_InvincibleColRateFlame;		// 無敵点滅のフレーム(速さ）
	bool m_bInvincibleColRateUpFlag;		// 無敵点滅の上げ下げフラグ
	int m_CurrentActionFrame;	// 攻撃フレームリストの中を再生しているフレーム
	int m_HitStopFrame;			// 1以上なら0になるまでストップ
	int m_RecoveryFlame;		// 1以上なら0になるまで操作を受け付けない
	bool m_bEscape;				// エスケープ判定の時に立つフラグ
	bool m_bAI;					// AIかどうか
	int m_score;				// 実体後の本当のスコア
	int m_CollectScore;			// 実体前の貯めているスコア
	//Vector3 m_StandSaveMove;	// スタンド発動の保存移動量
	int m_recoveryCount;		// リカバリーステートのいる時間をカウント
	bool m_bMoveUpdate;			// これがtrueなら、重力とか空気抵抗とかを受ける(アイルードリルとかペルソナとかでfalseにする)
	// ガードに必要なやつ
	bool m_bGuard;				// ガードしてるか
	int m_GuardFollowFrame;		// ガード解除の隙
	// HP
	int m_MaxHP;
	int m_HP;
	// ヒーホー用の変数
	int m_HeavehoStopTimer;			// ヒーホードライブ発動のザワールドの時間

	// ストップフラグ
	bool m_bGameTimerStopFlag;

	// モーション
	int m_MotionNumbers[(int)MOTION_TYPE::MAX];

	// 覚醒
	int m_OverDriveGage;			// 覚醒ゲージ
	bool m_bOverDrive;				// 覚醒してるか
	int m_OverDriveFrame;			// 覚醒時間
	OVERDRIVE_TYPE m_OverDriveType;	// 覚醒の種類

	// 「喰らい中」に攻撃をくらってるカウント
	std::list<BASE_ACTION_STATE> m_RecoveryDamageCount;

	Stand::Base *m_pStand;		// スタンドの基底クラスの実体

	FRAME_STATE m_ActionFrameList[(int)BASE_ACTION_STATE::END][FRAME_MAX];
	BASE_ACTION_STATE m_ActionState;

	// (A列車)　//////////////////////////////////////////////////////////
	// エフェクトマネージャーを一人ずつ持たせた理由は
	// 一つのマネージャーで追加していく形だと　可変長配列で増やしていくが
	// ゲーム中にnew Deleteはとても重かった
	// そもそもダメージエフェクトなどキャラクターに一つあればいいと思った
	//	4人対戦で5つもヒットエフェクトがでることはないので　
	// 初期ロードは重くなるがゲーム中おもくなるよりマシ。

	PanelEffectManager* m_PanelEffectMGR;	// パラパラアニメエフェクト 
	UVEffectManager* m_UVEffectMGR;			// UVエフェクト

	// 継承してるやつに強制的に呼ばせる
	void LoadAttackFrameList(char *filename);

	// 投げてきた相手のIDを保存
	ENTITY_ID m_ThrowPlayerID;

	// 投げ成功フラグ(投げ抜け猶予終了時にこのフラグがたってなかったら投げ失敗ステートへ)
	bool m_bThrowSuccess;

	// UI
	ComboUI* m_pComboUI;

};


