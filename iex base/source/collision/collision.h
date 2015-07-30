
#ifndef COLLISION_H
#define COLLISION_H

class iexMesh;
class Character;
class TEST_airou;
class Move_block;

class Collision
{
private:
public:

	void Check(iexMesh *obj, Character *chara);
	void Check(iexMesh *obj, TEST_airou *chara);

	void Check(Move_block *block, TEST_airou *chara);
};

#endif // !COLLISION_H
