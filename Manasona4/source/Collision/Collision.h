#pragma once

// 前方宣言
namespace Fighter
{
	class Base;
}


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
}

namespace Collision
{
	// レイピック
	void Raypic(iexMesh *obj, Fighter::Base *player); // Squareで判定
	void RaypicUp(iexMesh *obj, Fighter::Base *player);
	void RaypicDown(iexMesh *obj, Fighter::Base *player);
	void RaypicLeft(iexMesh *obj, Fighter::Base *player);
	void RaypicRight(iexMesh *obj, Fighter::Base *player);

	// 形状で判定
	bool HitCheck(CollisionShape::Circle* c1, CollisionShape::Circle* c2);
	bool HitCheck(CollisionShape::Square* s1, CollisionShape::Square* s2);
	bool HitCheck(CollisionShape::Circle* c, CollisionShape::Square* s);
};