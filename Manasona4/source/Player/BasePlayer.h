#pragma once
// エンティティ関連のインクルード
#include "../BaseEntity/Entity/BaseGameEntity.h"
#include "../BaseEntity/State/StateMachine.h"

// 判定のインクルード
#include "../Collision/Collision.h"

// エフェクト関連
#include "../Effect/PanelEffect/PanelEffectManager.h"
#include "../Effect\UVEffect\UVEffectManager.h"

// 前方宣言
namespace Stage
{
	class Base;
}

enum class PLAYER_INPUT
{
	A,		// 
	B,		// 攻撃
	C,		// ジャンプ
	D,		// 
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

enum class FRAME_STATE
{
	START,	// 発生
	ACTIVE,	// 持続(この間に攻撃判定)
	FOLLOW,	// 硬直(後隙)
	END	// 終了(このフレームに来たら攻撃終了)
};

// 全キャラ固有で持つ攻撃タイプ
enum BASE_ACTION_STATE
{
	NO_ACTION = -1,
	ESCAPE,		// 回避
	RUSH1,		// 通常1段目
	RUSH2,		// 通常2段目
	RUSH3,		// 通常3段目
	SQUAT,		// 下段攻撃
	AERIAL,		// 空中攻撃
	AERIALDROP,	// 空中下攻撃
	END		// 何もなし　(A列車)名前
};

// エフェクトの種類
enum class EFFECT_TYPE
{
	DAMAGE,	// ダメージエフェクト 
};


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
		bool bNextOK;	// これがtrueの時にボタンを押すと次の攻撃に行く
		void Clear()
		{
			step = 0;
			bNextOK = false;
		}
	};
	RushAttack m_RushAttack;

	class ActionData
	{
	public:
		class AttackData
		{
		public:
			//enum class HIT_TYPE{ LAND, AERIAL, MAX };	// 相手がどっちでヒットしたか。地上、空中
			//struct
			//{
				CollisionShape::Square *pCollisionShape;	// ★あたり判定形状(四角にする)
				bool bBeInvincible;			// 無敵になるかどうか(たとえば、通常の1.2段目ならfalseだし、3段目ならtrue)
				int damage;					// 与えるダメージ(スコア？)
				int pierceLV;				// 貫通レベル
				bool bHit;					// 当たったかどうか(★多段ヒット防止用)多段ヒットしてほしい攻撃だと、また考える必要がある
				Vector2 LandFlyVector;		// 当たって吹っ飛ばすベクトル(★基本的にxは+にすること)
				Vector2 AerialFlyVector;	// ※相手が空中にいた時　当たって吹っ飛ばす空中ベクトル(★基本的にxは+にすること)
				LPCSTR HitSE;				// 当たったときに鳴らすSE
				LPCSTR WhiffSE;				// 空振りSE
				int WhiffDelayFrame;		// 何フレーム後に空振りのSEを再生するか(WhiffSEを使わない(nullptr)なら別に設定しなくてもいい)
				int hitStopFlame;			// ヒットストップの時間
				int recoveryFlame;			// 硬直の時間
				EFFECT_TYPE effectType;		// エフェクトのタイプ
			//};
				AttackData() :HitSE(nullptr), WhiffSE(nullptr), bHit(false), pCollisionShape(new CollisionShape::Square){}
				~AttackData(){ SAFE_DELETE(pCollisionShape); }
		}*pAttackData;

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
	BasePlayer(int deviceID);
	~BasePlayer();
	void Control();			// プレイヤーからの入力を受け付ける
	virtual void Update();	// 基本的な更新
	void UpdatePos();		// 座標更新(判定後に呼び出す)
	void Move();			// 動きの制御
	virtual void Render();

	// 継承してるやつに強制的に呼ばせる
	virtual void InitActionDatas() = 0;

	bool HandleMessage(const Message& msg); //メッセージを受け取る

	// ゲッター
	int GetDeviceID() { return m_deviceID; }
	Vector3 &GetMove() { return m_move; }
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

	ActionData::AttackData *GetAttackData(BASE_ACTION_STATE state)
	{
		assert(isAttackState());	// アタックデータがないステートでアタックデータを参照しようとしている
		return m_ActionDatas[(int)state].pAttackData; 
	}

	ActionData::AttackData *GetAttackData()
	{
		// 自分の状態の攻撃データを渡す
		return GetAttackData(m_ActionState);
	}

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
	bool isActiveFrame()
	{
		if (!isFrameAction()) return false;
		return (m_ActionFrameList[m_ActionState][m_CurrentActionFrame] == FRAME_STATE::ACTIVE);
	}
	FRAME_STATE GetActionFrame()
	{
		if (!isFrameAction()) return FRAME_STATE::END;
		return m_ActionFrameList[(int)m_ActionState][m_CurrentActionFrame]; 
	}
	int GetRecoveryFrame() { return m_RecoveryFlame; }
	bool isEscape() { return m_bEscape; }


	// セッター
	void SetMove(const Vector3 &v) { m_move.Set(v.x, v.y, v.z); }
	void SetPos(const Vector3 &v) { m_pos.Set(v.x, v.y, v.z); }
	void SetDir(DIR dir) { m_dir = dir; }
	void SetMotion(int MotionNo) { if (m_pObj) { if (m_pObj->GetMotion() != MotionNo)m_pObj->SetMotion(MotionNo); } }
	void SetLand(bool bLand) { m_bLand = bLand; }
	void SetAerialJump(bool bAerialJump) { m_bAerialJump = bAerialJump; }
	void SetHitStopFrame(int frame) { m_HitStopFrame = frame; }
	void SetRecoveryFrame(int frame) { m_RecoveryFlame = frame; }
	void SetEscapeFlag(bool bEscape) { m_bEscape = bEscape; }
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


	// アクセサー
	void AddMove(const Vector3 &v) { m_move += v; }
	void MoveClampX(float val) { m_move.x = Math::Clamp(m_move.x, -val, val); }

	// エフェクト
	PanelEffectManager* GetPanelEffectManager() { return m_PanelEffectMGR; }
	UVEffectManager* GetUVEffectManager() { return m_UVEffectMGR; }
	void AddEffectAction(Vector3 pos,EFFECT_TYPE effectType);

	// ステージのあたり判定用
	CollisionShape::Square *GetHitSquare() { return m_pHitSquare; }


	//_/_/_/_/_/_/__/_/__/_/__/_/__
	// 定数
	//_/_/_/_/__/_/__/_/__/_/__/_/_
	static const float c_END_MOVE_LINE;		// 移動が終わって大気に戻る移動値ライン
	static const float c_FRONT_BRAKE_LINE;	// 走り中に操作を離してブレーキに移行するまでの移動値ライン
	static const float c_GRAVITY;			// 全員が共通で持つ世界の重力
	static const float c_MAX_JUMP;			// ジャンプ最大ライン

protected:
	const int m_deviceID;		// 自分のコントローラーの番号(実質、スマブラのxPに値する)
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
	int m_InvincibleLV;			// 無敵かどうか
	int m_InvincibleTime;		// 無敵時間		無敵時は0以上の値が入り、0になるまでデクリメントされる
	int m_CurrentActionFrame;	// 攻撃フレームリストの中を再生しているフレーム
	int m_HitStopFrame;			// 1以上なら0になるまでストップ
	int m_RecoveryFlame;		// 1以上なら0になるまで操作を受け付けない
	bool m_bEscape;				// エスケープ判定の時に立つフラグ

	static const int FRAME_MAX = 256; // 攻撃のフレームの最大値(これより超えることはないだろう)
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
};


