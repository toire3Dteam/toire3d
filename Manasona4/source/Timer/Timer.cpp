#include "Timer.h"

//+-----------------------
//	�^�C�}�[
//+-----------------------

TimeManager::TimeManager()
{
	m_frame = 0;

}

void  TimeManager::Init()
{
	m_vPos.x = 574;
	m_vPos.y = 58;

	m_pic = new tdn2DAnim("Data/UI/Game/timer.png");
	m_pic->OrderShrink(12, 1, 1.5);
	m_pic->Action();
};

// �X�V(�^�C�����Z����&�I�������Ԃ�)
bool TimeManager::Update()
{
	m_pic->Update();

	bool bEnd(--m_frame <= 0);
	return bEnd;

}

// �`��
void TimeManager::Render()
{
	const int second = m_frame / 60,
		minutes = m_frame / 60;

	m_pic->Render((int)m_vPos.x, (int)m_vPos.y, 64, 64, (second / 10) * 64, 0, 64, 64);			// ��
	m_pic->Render((int)m_vPos.x + 64, (int)m_vPos.y, 64, 64, (second % 10) * 64, 0, 64, 64);	// 
	

}
