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
	A,
	B,
	C,
	D,
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
enum class DIR { LEFT, RIGHT };

	class BasePlayer: public BaseGameEntity
	{
	public:
		BasePlayer(int deviceID);
		~BasePlayer();
		void Control();			// プレイヤーからの入力を受け付ける
		virtual void Update();	// 基本的な更新
		void UpdatePos();		// 座標更新(判定後に呼び出す)
		virtual void Render();
		
		bool HandleMessage(const Message& msg); //メッセージを受け取る

		// ゲッター
		Vector3 &GetMove() { return m_move; }
		Vector3 &GetPos() { return m_pos; }
		Vector3 *GetPosAddress() { return &m_pos; }
		int GetInputList(PLAYER_INPUT input) { return m_InputList[(int)input]; }	
		StateMachine<BasePlayer>* GetFSM()const { return m_pStateMachine; }// ★ステートマシンのアクセサを作る

		// セッター
		void SetMove(const Vector3 &v) { m_move.Set(v.x, v.y, v.z); }
		void SetPos(const Vector3 &v) { m_pos.Set(v.x, v.y, v.z); }

		// ステージのあたり判定用
		CollisionShape::Square *GetHitSquare() { return m_pHitSquare; }

		
		

	protected:
		const int m_deviceID;		// 自分のコントローラーの番号(実質、スマブラのxPに値する)
		iex3DObj *m_pObj;	// メッシュ実体
		Vector3 m_pos;	// 座標
		Vector3 m_move;	// 移動値
		CollisionShape::Square *m_pHitSquare;	// 四角判定(ステージ衝突で使う)
		DIR m_dir;				// 左右のフラグ
		const float m_Gravity;	// 重力値

		int m_InputList[(int)PLAYER_INPUT::MAX]; 	// 押しているキーを格納
		StateMachine<BasePlayer>* m_pStateMachine; // ★ステートマシン
		
	};


