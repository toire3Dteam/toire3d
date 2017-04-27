#pragma once

// �O���錾
class BasePlayer;
class PlayerManager;
class ShotManager;
namespace Stand{class Base;}
namespace Stage{class Base;}
namespace Shot{class Base;}
struct HIT_DAMAGE_INFO;

// �`��
namespace CollisionShape
{
	class Base
	{
	public:

		Vector3 pos; // ���S

		Base() :pos(0, 0, 0){}
		virtual ~Base(){}

		virtual void Render(){}
	};

	class Circle : public Base
	{
	public:

		float radius; // ���a

		Circle() :Base(), radius(0.0f){}
		~Circle(){}
	};

	class Square : public Base
	{
	public:

		float height, // ������������
			width; // ����������

		Square() :Base(), height(0.0f), width(0.0f){}
		~Square(){}

		virtual void Render(){}
	};

	// �L�����N�^�[�p�̓����蔻��
	class SquareChara : public Base
	{
	public:

		float height,	// �����������������ł͂Ȃ��A�������ւ̒�����
			width;		// ����������

		SquareChara() :Base(), height(0.0f), width(0.0f) {}
		~SquareChara() {}

		virtual void Render() {}
	};

}

class Collision
{
public:
	// ���C�s�b�N
	static void Raypic(BasePlayer *player, Vector3 *move); // Square�Ŕ���
	static float CheckMove(BasePlayer *pPlayer, const Vector3 &vMove);

	// �v���C���[�̔���
	static void PlayerCollision(PlayerManager* pPlayerMgr, ShotManager *pShotMgr);

	// �`��Ŕ���
	static bool HitCheck(CollisionShape::Circle* c1, CollisionShape::Circle* c2);
	static bool HitCheck(CollisionShape::Square* s1, CollisionShape::Square* s2);
	static bool HitCheck(CollisionShape::Square* s1, CollisionShape::SquareChara* s2);
	static bool HitCheck(CollisionShape::SquareChara* s1, CollisionShape::SquareChara* s2);
	static bool HitCheck(CollisionShape::Circle* c, CollisionShape::Square* s);

	static void SetStage(Stage::Base *pStage){ m_pStage = pStage; }
private:

	static Stage::Base *m_pStage;	// �ǂ�����ł��A�N�Z�X�������̂Ŏ���

	// �ʗp�̊ȈՔŃ��C�s�b�N
	static bool RaypicShot(Stage::Base *obj, Shot::Base *shot);

	// ���C�s�b�N
	static void RaypicDown(Stage::Base *obj, BasePlayer *player, Vector3 *move);
	static void RaypicLeft(Stage::Base *obj, BasePlayer *player, Vector3 *move, float fTargetDist);
	static void RaypicRight(Stage::Base *obj, BasePlayer *player, Vector3 *move, float fTargetDist);

	static void CollisionPlayerAttack(BasePlayer *my, BasePlayer *you,  HIT_DAMAGE_INFO **OutDamageInfo);
	static bool CollisionStandAttack(Stand::Base *pStand, BasePlayer *pYou);
	//static bool CollisionThrowAttack(BasePlayer *my, BasePlayer *you, Stage::Base *pStage);
	static bool CollisionShot(Shot::Base *shot, BasePlayer *you);

	// �߂荞��
	static void Sinking(BasePlayer *pPlayer1, BasePlayer *pPlayer2);

	static void SendHitMessage(BasePlayer *pAttackPlayer, BasePlayer *pDamagePlayer, HIT_DAMAGE_INFO *pHitDamageInfo);
};