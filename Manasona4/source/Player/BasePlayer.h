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

class BasePlayer : public BaseGameEntity
{
protected:
	struct Jump
	{
		bool bHold;	// しゃがんでる状態
		int HoldTimer;		// しゃがんでる時間
		int PlayerHoldTimer;	// しゃがんでる間にプレイヤーからジャンプボタンを受け付ける時間
		int LandTimer;		// 着地の時間
	};
	Jump m_jump;

public:
	BasePlayer(int deviceID);
	~BasePlayer();
	void Control();			// プレイヤーからの入力を受け付ける
	virtual void Update();	// 基本的な更新
	void UpdatePos();		// 座標更新(判定後に呼び出す)
	void Move(); // 動きの制御
	virtual void Render();

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
	float GetMaxSpeed() { return m_maxSpeed; }

	// セッター
	void SetMove(const Vector3 &v) { m_move.Set(v.x, v.y, v.z); }
	void SetPos(const Vector3 &v) { m_pos.Set(v.x, v.y, v.z); }
	void SetDir(DIR dir) { m_dir = dir; }
	void SetMotion(int MotionNo) { if (m_pObj) { if (m_pObj->GetMotion() != MotionNo)m_pObj->SetMotion(MotionNo); } }
	void SetLand(bool bLand) { m_bLand = bLand; }

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



};


