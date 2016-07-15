#include "Airou.h"

Airou::Airou(int deviceID) :BasePlayer(deviceID)
{
	// アイルーメッシュ
	m_pObj = new iex3DObj("DATA/CHR/Airou/airou.IEM");
}