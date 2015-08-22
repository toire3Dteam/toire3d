
#ifndef COLLISION_H
#define COLLISION_H

#include "iextreme.h"

class BasePlayer;

namespace Collision_shape
{
	class Base
	{
	public:

		Vector3 pos; // ���S

		Base() :pos(0, 0, 0){}
		~Base(){}
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
	};
}

namespace Collision
{
	void Raypic(iexMesh2 *obj, BasePlayer *player); // �~�`�̔���
};

#endif // !COLLISION_H
