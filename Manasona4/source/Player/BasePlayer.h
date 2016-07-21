#pragma once
// エンティティ関連のインクルード
#include "../BaseEntity/Entity/BaseGameEntity.h"
#include "../BaseEntity/State/StateMachine.h"

// 前方宣言
namespace CollisionShape
{
	class Square;
}
namespace Stage
{
	class Base;
}

enum class PLAYER_INPUT
{
	A,		// 
	B,		// 
	C,		// ジャンプ
	D,		// 
	RIGHT,
	LEFT,
	UP,
	DOWN,
	R1,
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

enum class ATTACK_FRAME
{
	START,	// 発生
	ATTACK,	// 持続(この間に攻撃判定)
	FOLLOW,	// 硬直(後隙)
	END	// 終了(このフレームに来たら攻撃終了)
};

// 全キャラ固有で持つ攻撃タイプ
enum BASE_ATTACK_STATE
{
	RUSH1,	// 通常1段目
	RUSH2,	// 通常2段目
	RUSH3,	// 通常3段目
	END		// 何もなし
};

class BasePlayer : public BaseGameEntity
{
protected:
	struct Jump
	{
		bool bHold;	// しゃがんでる状態
		int HoldTimer;		// しゃがんでる時間
		int PlayerHoldTimer;	// しゃがんでる間にプレイヤーからジャンプボタンを受け付ける時間
		int LandTimer;		// 着地の時間
		void Clear()
		{
			bHold = true;
			HoldTimer = PlayerHoldTimer = LandTimer = 0;
		}
	};
	Jump m_jump;

	struct RushAttack
	{
		int step;	// 0,1,2で進める
		bool bHit;	// これがtrueの時にボタンを押すと次の攻撃に行く
		void Clear()
		{
			step = 0;
			bHit = false;
		}
	};
	RushAttack m_RushAttack;

	struct AttackData
	{
		//Attack_collision shape;		// ★あたり判定の形
		int damage;	// 与えるダメージ(スコア？)
		int pierceLV;		// 貫通レベル
		bool bBeInvincible;	// 無敵になるかどうか(たとえば、通常の1.2段目ならfalseだし、3段目ならtrue)
		bool bHit;			// 当たったかどうか(★多段ヒット防止用)多段ヒットしてほしい攻撃だと、また考える必要がある
		Vector2 FlyVector;	// 当たって吹っ飛ばすベクトル(★基本的にxは+にすること)
		LPCSTR SE_ID;		// 当たったときに鳴らすSE
	}m_AttackDatas[(int)BASE_ATTACK_STATE::END];

public:
	BasePlayer(int deviceID);
	~BasePlayer();
	void Control();			// プレイヤーからの入力を受け付ける
	virtual void Update();	// 基本的な更新
	void UpdatePos();		// 座標更新(判定後に呼び出す)
	void Move(); // 動きの制御
	virtual void Render();

	// 継承してるやつに強制的に呼ばせる
	virtual void InitAttackDatas() = 0;

	bool HandleMessage(const Message& msg); //メッセージを受け取る

	// ゲッター
	Vector3 &GetMove() { return m_move; }
	Vector3 &GetPos() { return m_pos; }
	Vector3 *GetPosAddress() { return &m_pos; }
	int GetInputList(PLAYER_INPUT input) { return m_InputList[(int)input]; }
	bool isPushInput(PLAYER_INPUT input) { return (m_InputList[(int)input] == 1 || m_InputList[(int)input] == 3); }	// 押した瞬間と押している間どちらかだったらtrue
	StateMachine<BasePlayer>* GetFSM()const { return m_pStateMachine; }// ★ステートマシンのアクセサを作る
	DIR GetDir() { return m_dir; }
	bool isMaxSpeed() { return (abs(m_move.x) >= m_maxSpeed); }
	bool isLand() { return m_bLand; }
	Jump *GetJump() { return &m_jump; }
	RushAttack *GetRushAttack() { return &m_RushAttack; }
	float GetMaxSpeed() { return m_maxSpeed; }
	BASE_ATTACK_STATE GetAttackState(){ return m_AttackState; }
	AttackData *GetAttackData(){ return &m_AttackDatas[(int)m_AttackState]; }
	int GetPierceLV(){ return m_AttackDatas[(int)m_AttackState].pierceLV; }
	LPCSTR GetAttackSE(){ return m_AttackDatas[(int)m_AttackState].SE_ID; }
	bool isAttackState(){ return (m_AttackState != BASE_ATTACK_STATE::END); }
	bool isAttackFrame()
	{
		if (!isAttackState()) return false;
		return (m_AttackFrameList[m_AttackState][m_CurrentAttackFrame] == ATTACK_FRAME::ATTACK);
	}
	int GetInvincibleLV(){ return m_InvincibleLV; }

	// セッター
	void SetMove(const Vector3 &v) { m_move.Set(v.x, v.y, v.z); }
	void SetPos(const Vector3 &v) { m_pos.Set(v.x, v.y, v.z); }
	void SetDir(DIR dir) { m_dir = dir; }
	void SetMotion(int MotionNo) { if (m_pObj) { if (m_pObj->GetMotion() != MotionNo)m_pObj->SetMotion(MotionNo); } }
	void SetLand(bool bLand) { m_bLand = bLand; }
	void SetAttackState(BASE_ATTACK_STATE state)
	{
		m_AttackState = state; 

		// 攻撃だったら
		if (isAttackState())
		{
			m_AttackDatas[(int)state].bHit = false;	// ヒットフラグリセット
			m_CurrentAttackFrame = 0;				// フレーム0にセット
		}
	}


	// アクセサー
	void AddMove(const Vector3 &v) { m_move += v; }
	void MoveClampX(float val) { m_move.x = Math::Clamp(m_move.x, -val, val); }

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
	int m_InputList[(int)PLAYER_INPUT::MAX]; 	// 押しているキーを格納
	StateMachine<BasePlayer>* m_pStateMachine; // ★ステートマシン
	int m_InvincibleLV;			// 無敵かどうか
	int m_InvincibleTime;		// 無敵時間		無敵時は0以上の値が入り、0になるまでデクリメントされる
	int m_CurrentAttackFrame;	// 攻撃フレームリストの中を再生しているフレーム

	static const int FRAME_MAX = 256; // 攻撃のフレームの最大値(これより超えることはないだろう)
	ATTACK_FRAME m_AttackFrameList[(int)BASE_ATTACK_STATE::END][FRAME_MAX];
	BASE_ATTACK_STATE m_AttackState;

	// 継承してるやつに強制的に呼ばせる
	void LoadAttackFrameList(char *filename);
};


