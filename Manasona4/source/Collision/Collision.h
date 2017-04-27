#pragma once

// 前方宣言
class BasePlayer;
class PlayerManager;
class ShotManager;
namespace Stand{class Base;}
namespace Stage{class Base;}
namespace Shot{class Base;}
struct HIT_DAMAGE_INFO;

// 形状
namespace CollisionShape
{
	class Base
	{
	public:

		Vector3 pos; // 中心

		Base() :pos(0, 0, 0){}
		virtual ~Base(){}

		virtual void Render(){}
	};

	class Circle : public Base
	{
	public:

		float radius; // 半径

		Circle() :Base(), radius(0.0f){}
		~Circle(){}
	};

	class Square : public Base
	{
	public:

		float height, // 高さ＜半分＞
			width; // 幅＜半分＞

		Square() :Base(), height(0.0f), width(0.0f){}
		~Square(){}

		virtual void Render(){}
	};

	// キャラクター用の当たり判定
	class SquareChara : public Base
	{
	public:

		float height,	// ★★★高さ＜半分ではなく、下から上への長さ＞
			width;		// 幅＜半分＞

		SquareChara() :Base(), height(0.0f), width(0.0f) {}
		~SquareChara() {}

		virtual void Render() {}
	};

}

class Collision
{
public:
	// レイピック
	static void Raypic(BasePlayer *player, Vector3 *move); // Squareで判定
	static float CheckMove(BasePlayer *pPlayer, const Vector3 &vMove);

	// プレイヤーの判定
	static void PlayerCollision(PlayerManager* pPlayerMgr, ShotManager *pShotMgr);

	// 形状で判定
	static bool HitCheck(CollisionShape::Circle* c1, CollisionShape::Circle* c2);
	static bool HitCheck(CollisionShape::Square* s1, CollisionShape::Square* s2);
	static bool HitCheck(CollisionShape::Square* s1, CollisionShape::SquareChara* s2);
	static bool HitCheck(CollisionShape::SquareChara* s1, CollisionShape::SquareChara* s2);
	static bool HitCheck(CollisionShape::Circle* c, CollisionShape::Square* s);

	static void SetStage(Stage::Base *pStage){ m_pStage = pStage; }
private:

	static Stage::Base *m_pStage;	// どっからでもアクセスしたいので持つ

	// 玉用の簡易版レイピック
	static bool RaypicShot(Stage::Base *obj, Shot::Base *shot);

	// レイピック
	static void RaypicDown(Stage::Base *obj, BasePlayer *player, Vector3 *move);
	static void RaypicLeft(Stage::Base *obj, BasePlayer *player, Vector3 *move, float fTargetDist);
	static void RaypicRight(Stage::Base *obj, BasePlayer *player, Vector3 *move, float fTargetDist);

	static void CollisionPlayerAttack(BasePlayer *my, BasePlayer *you,  HIT_DAMAGE_INFO **OutDamageInfo);
	static bool CollisionStandAttack(Stand::Base *pStand, BasePlayer *pYou);
	//static bool CollisionThrowAttack(BasePlayer *my, BasePlayer *you, Stage::Base *pStage);
	static bool CollisionShot(Shot::Base *shot, BasePlayer *you);

	// めり込み
	static void Sinking(BasePlayer *pPlayer1, BasePlayer *pPlayer2);

	static void SendHitMessage(BasePlayer *pAttackPlayer, BasePlayer *pDamagePlayer, HIT_DAMAGE_INFO *pHitDamageInfo);
};