#pragma once

// OϋιΎ
class BasePlayer;
namespace Stage
{
	class Base;
}

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

namespace Collision
{
	// CsbN
	void Raypic(Stage::Base *obj, BasePlayer *player, Vector3 *move); // SquareΕ»θ
	//void RaypicUp(Stage::Base *obj, BasePlayer *player, Vector3 *move);
	void RaypicDown(Stage::Base *obj, BasePlayer *player, Vector3 *move);
	void RaypicLeft(Stage::Base *obj, BasePlayer *player, Vector3 *move);
	void RaypicRight(Stage::Base *obj, BasePlayer *player, Vector3 *move);

	// ίθέ
	void Sinking(Stage::Base *obj, BasePlayer *pPlayer1, BasePlayer *pPlayer2);

	// `σΕ»θ
	bool HitCheck(CollisionShape::Circle* c1, CollisionShape::Circle* c2);
	bool HitCheck(CollisionShape::Square* s1, CollisionShape::Square* s2);
	bool HitCheck(CollisionShape::Circle* c, CollisionShape::Square* s);
};