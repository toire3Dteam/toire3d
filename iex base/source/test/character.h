
#ifndef CHARACTER_H
#define CHARACTER_H

#include <assert.h>

class iex3DObj2;

class Character
{
protected:
	iex3DObj2 *obj = nullptr;
public:
	void Set_object(iex3DObj2 *in){ assert(in != nullptr); delete obj; obj = in; };
};

#endif // !CHARACTER_H
