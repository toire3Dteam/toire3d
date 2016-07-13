#pragma once

// 前方宣言
namespace CollisionShape
{
	class Square;
}
namespace Stage
{
	class Base;
}

// 向き
enum class DIR{ LEFT, RIGHT };

namespace Fighter
{
	class Base
	{
	public:
		Base(int deviceID);
		~Base();
		void Control();			// プレイヤーからの入力を受け付ける
		virtual void Update();	// 基本的な更新
		void UpdatePos();		// 座標更新(判定後に呼び出す)
		virtual void Render();

		// ゲッター
		Vector3 &GetMove(){ return m_move; }
		Vector3 &GetPos(){ return m_pos; }
		Vector3 *GetPosAddress(){ return &m_pos; }

		// セッター
		void SetMove(const Vector3 &v){ m_move.Set(v.x, v.y, v.z); }
		void SetPos(const Vector3 &v){ m_pos.Set(v.x, v.y, v.z); }

		// ステージのあたり判定用
		CollisionShape::Square *GetHitSquare(){ return m_pHitSquare; }

	protected:
		const int m_deviceID;		// 自分のコントローラーの番号(実質、スマブラのxPに値する)
		iex3DObj *m_pObj;	// メッシュ実体
		Vector3 m_pos;	// 座標
		Vector3 m_move;	// 移動値
		CollisionShape::Square *m_pHitSquare;	// 四角判定(ステージ衝突で使う)
		DIR m_dir;				// 左右のフラグ
		const float m_Gravity;	// 重力値
	};

	class Airou: public Base
	{
	public:
		Airou(int deviceID);

	};
}

class PlayerManager
{
public:
	PlayerManager(int NumPlayer);
	~PlayerManager();
	void Update(Stage::Base *stage);
	void Render();

	int GetNumPlayer(){ return m_NumPlayer; }
	Fighter::Base *GetPlayer(int no){ return m_pPlayers[no]; }

private:
	const int m_NumPlayer;
	Fighter::Base **m_pPlayers;
};