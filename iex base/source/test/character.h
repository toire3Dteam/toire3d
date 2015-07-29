
#ifndef CHARACTER_H
#define CHARACTER_H

#include <assert.h>
#include "../../IEX/iextreme.h"

class iex3DObj2;

class Character
{
protected:
	iex3DObj2 *obj = nullptr;

	float angle = 0;
	float speed = 0;
	Vector3 pos, move, accelerator;

	enum
	{
		LOWER, UPPER
	};

	float size_side = 0, size_up = 0, size_down = 0;

public:
	Character();
	~Character();

	virtual void Update();
	virtual void Update_pos();

	void Set_object(iex3DObj2 *in);

	void Set_pos(const Vector3 &in);
	const Vector3 &Get_pos();

	const Vector3 &Get_move();

	virtual void Col_vs_stage(const Vector3 &updown, const Vector3 &side){};
};

#endif // !CHARACTER_H
