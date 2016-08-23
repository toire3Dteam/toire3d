#pragma once

// �O���錾
class BasePlayer;


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
}

namespace Collision
{
	// ���C�s�b�N
	void Raypic(iexMesh *obj, BasePlayer *player, Vector3 *move); // Square�Ŕ���
	void RaypicUp(iexMesh *obj, BasePlayer *player, Vector3 *move);
	void RaypicDown(iexMesh *obj, BasePlayer *player, Vector3 *move);
	void RaypicLeft(iexMesh *obj, BasePlayer *player, Vector3 *move);
	void RaypicRight(iexMesh *obj, BasePlayer *player, Vector3 *move);

	// �߂荞��
	void Sinking(iexMesh *obj, BasePlayer *pPlayer1, BasePlayer *pPlayer2);

	// �`��Ŕ���
	bool HitCheck(CollisionShape::Circle* c1, CollisionShape::Circle* c2);
	bool HitCheck(CollisionShape::Square* s1, CollisionShape::Square* s2);
	bool HitCheck(CollisionShape::Circle* c, CollisionShape::Square* s);
};