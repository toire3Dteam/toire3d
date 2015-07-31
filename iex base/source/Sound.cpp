#include	"iextreme.h"

#include	"Sound.h"


Sound::~Sound()
{
		SAFE_DELETE(manager);
}