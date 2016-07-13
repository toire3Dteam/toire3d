#pragma once

// OϋιΎ
namespace Fighter
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
	void Raypic(iexMesh *obj, Fighter::Base *player); // SquareΕ»θ
	void RaypicUp(iexMesh *obj, Fighter::Base *player);
	void RaypicDown(iexMesh *obj, Fighter::Base *player);
	void RaypicLeft(iexMesh *obj, Fighter::Base *player);
	void RaypicRight(iexMesh *obj, Fighter::Base *player);

	// `σΕ»θ
	bool HitCheck(CollisionShape::Circle* c1, CollisionShape::Circle* c2);
	bool HitCheck(CollisionShape::Square* s1, CollisionShape::Square* s2);
	bool HitCheck(CollisionShape::Circle* c, CollisionShape::Square* s);
};