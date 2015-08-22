
#ifndef COLLISION_H
#define COLLISION_H

#include "iextreme.h"

class BasePlayer;

namespace Collision_shape
{
	class Base
	{
	public:

		Vector3 pos; // ’†S

		Base() :pos(0, 0, 0){}
		~Base(){}
	};

	class Circle : public Base
	{
	public:

		float radius; // ”¼Œa

		Circle() :Base(), radius(0.0f){}
		~Circle(){}
	};

	class Square : public Base
	{
	public:

		float height, // ‚‚³ƒ”¼•ª„
			width; // •ƒ”¼•ª„

		Square() :Base(), height(0.0f), width(0.0f){}
		~Square(){}
	};
}

namespace Collision
{
	void Raypic(iexMesh2 *obj, BasePlayer *player); // ‰~Œ`‚Ì”»’è
};

#endif // !COLLISION_H
