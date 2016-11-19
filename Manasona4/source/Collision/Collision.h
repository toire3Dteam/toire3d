#pragma once

// OϋιΎ
class BasePlayer;
class PlayerManager;
class ShotManager;
namespace Stand{class Base;}
namespace Stage{class Base;}
namespace Shot{class Base;}
struct HIT_DAMAGE_INFO;

// `σ
namespace CollisionShape
{
	class Base
	{
	public:

		Vector3 pos; // S

		Base() :pos(0, 0, 0){}
		virtual ~Base(){}

		virtual void Render(){}
	};

	class Circle : public Base
	{
	public:

		float radius; // Όa

		Circle() :Base(), radius(0.0f){}
		~Circle(){}
	};

	class Square : public Base
	{
	public:

		float height, // ³Όͺ
			width; // Όͺ

		Square() :Base(), height(0.0f), width(0.0f){}
		~Square(){}

		virtual void Render(){}
	};
}

class Collision
{
public:
	// CsbN
	static void Raypic(Stage::Base *obj, BasePlayer *player, Vector3 *move); // SquareΕ»θ
	//void RaypicUp(Stage::Base *obj, BasePlayer *player, Vector3 *move);

	// vC[Μ»θ
	static void PlayerCollision(PlayerManager* pPlayerMgr, ShotManager *pShotMgr, Stage::Base *pStage);

	// `σΕ»θ
	static bool HitCheck(CollisionShape::Circle* c1, CollisionShape::Circle* c2);
	static bool HitCheck(CollisionShape::Square* s1, CollisionShape::Square* s2);
	static bool HitCheck(CollisionShape::Circle* c, CollisionShape::Square* s);

private:

	// ΚpΜΘΥΕCsbN
	static bool RaypicShot(Stage::Base *obj, Shot::Base *shot);

	// CsbN
	static void RaypicDown(Stage::Base *obj, BasePlayer *player, Vector3 *move);
	static void RaypicLeft(Stage::Base *obj, BasePlayer *player, Vector3 *move, float fTargetDist);
	static void RaypicRight(Stage::Base *obj, BasePlayer *player, Vector3 *move, float fTargetDist);

	static void CollisionPlayerAttack(BasePlayer *my, BasePlayer *you,  HIT_DAMAGE_INFO **OutDamageInfo);
	static bool CollisionStandAttack(Stand::Base *pStand, BasePlayer *pYou);
	static bool CollisionThrowAttack(BasePlayer *my, BasePlayer *you, Stage::Base *pStage);
	static bool CollisionShot(Shot::Base *shot, BasePlayer *you);

	// ίθέ
	static void Sinking(Stage::Base *obj, BasePlayer *pPlayer1, BasePlayer *pPlayer2);

	static void SendHitMessage(BasePlayer *pAttackPlayer, BasePlayer *pDamagePlayer, HIT_DAMAGE_INFO *pHitDamageInfo);
};