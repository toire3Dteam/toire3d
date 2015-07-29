
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
	Vector3 pos, move;

public:
	Character();
	~Character();

	void Set_object(iex3DObj2 *in);

	void Set_pos(const Vector3 &in);
	const Vector3 &Get_pos();
};

#endif // !CHARACTER_H
