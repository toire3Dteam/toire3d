
#ifndef COLLISION_H
#define COLLISION_H

class iexMesh2;
class Character;
class TEST_airou;
class Move_block;

class Collision
{
private:
public:

	void Check(iexMesh2 *obj, Character *chara);
	void Check(iexMesh2 *obj, TEST_airou *chara);

	void Check(Move_block *block, TEST_airou *chara);
};

#endif // !COLLISION_H
