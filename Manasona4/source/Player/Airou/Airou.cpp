#include "Airou.h"

Airou::Airou(int deviceID) :BasePlayer(deviceID)
{
	// �L�����ŗL�̏��̐ݒ�
	m_Gravity = .14f;
	m_maxSpeed = 1.0f;

	// �A�C���[���b�V��
	m_pObj = new iex3DObj("DATA/CHR/Airou/airou_motion.IEM");
}