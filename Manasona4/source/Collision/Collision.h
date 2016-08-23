#pragma once

// 前方宣言
class BasePlayer;


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
	void Raypic(iexMesh *obj, BasePlayer *player, Vector3 *move); // Squareで判定
	void RaypicUp(iexMesh *obj, BasePlayer *player, Vector3 *move);
	void RaypicDown(iexMesh *obj, BasePlayer *player, Vector3 *move);
	void RaypicLeft(iexMesh *obj, BasePlayer *player, Vector3 *move);
	void RaypicRight(iexMesh *obj, BasePlayer *player, Vector3 *move);

	// めり込み
	void Sinking(iexMesh *obj, BasePlayer *pPlayer1, BasePlayer *pPlayer2);

	// 形状で判定
	bool HitCheck(CollisionShape::Circle* c1, CollisionShape::Circle* c2);
	bool HitCheck(CollisionShape::Square* s1, CollisionShape::Square* s2);
	bool HitCheck(CollisionShape::Circle* c, CollisionShape::Square* s);
};