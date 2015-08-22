
#ifndef CHARACTER_LOADER_H
#define CHARACTER_LOADER_H

#include "text_loader.h"

class BasePlayer;

class Character_loader : public Text_loader
{
private:
	Character_loader(const Character_loader&){}
	void operator = (const Character_loader&){}

public:
	Character_loader() :Text_loader(){}
	~Character_loader(){}

	bool Load(BasePlayer *character);
};

#endif // !CHARACTER_LOADER_H
