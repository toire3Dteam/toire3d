#include "Airou.h"

Airou::Airou(int deviceID) :BasePlayer(deviceID)
{
	// �L�����ŗL�̏��̐ݒ�
	m_maxSpeed = 1.25f;

	// �A�C���[���b�V��
	m_pObj = new iex3DObj("DATA/CHR/Airou/airou_motion.IEM");
}