#include "Airou.h"

Airou::Airou(int deviceID) :BasePlayer(deviceID)
{
	// �A�C���[���b�V��
	m_pObj = new iex3DObj("DATA/CHR/Airou/airou.IEM");
}