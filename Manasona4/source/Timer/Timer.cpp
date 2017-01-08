#include "Timer.h"
#include "Data\SelectData.h"

////+-----------------------
////	�^�C�}�[
////+-----------------------
//
//TimeManager::TimeManager()
//{
//	m_frame = 0;
//
//}
//
//void  TimeManager::Init()
//{
//	m_vPos.x = 574;
//	m_vPos.y = 58;
//
//	m_pic = new tdn2DAnim("Data/UI/Game/timer.png");
//	m_pic->OrderShrink(12, 1, 1.5);
//	m_pic->Action();
//};
//
//// �X�V(�^�C�����Z����&�I�������Ԃ�)
//bool TimeManager::Update()
//{
//	m_pic->Update();
//
//	bool bEnd(--m_frame <= 0);
//	return bEnd;
//
//}
//
//// �`��
//void TimeManager::Render()
//{
//	const int second = m_frame / 60,
//		minutes = m_frame / 60;
//
//	m_pic->Render((int)m_vPos.x, (int)m_vPos.y, 64, 64, (second / 10) * 64, 0, 64, 64);			// ��
//	m_pic->Render((int)m_vPos.x + 64, (int)m_vPos.y, 64, 64, (second % 10) * 64, 0, 64, 64);	// 
//	
//
//}


//+-------------------------------------------
//	new�^�C�}�[
//+-------------------------------------------

Timer::Timer()
{
	m_iFrame = 0;
	m_iMaxFrame = 99;

	m_pPic = new tdn2DAnim("Data/UI/Game/timer.png");
	m_pPic->OrderShrink(12, 1, 2.5);

	m_vPos = Vector2(574, 58);
	m_bStop = true;

	m_iElapsedTime = 0;
	m_iElapsedFrame = 0;
	m_iRoundElapsedTime = 0;

	m_bInfinity = false;
	m_pInfinityPic = new tdn2DAnim("Data/UI/Game/timerInfinity.png");
	m_pInfinityPic->OrderShrink(12, 1, 2.5);

} 

Timer::~Timer()
{
	SAFE_DELETE(m_pPic);
	SAFE_DELETE(m_pInfinityPic);
}

//+-------------------------------------------
//	�X�V�E�`��
//+-------------------------------------------
void Timer::Update()
{
	// �~�߂�t���O�������Ă��Ȃ�������
	if (m_bStop == false)
	{
		// ���t���O�������Ă���^�C�}�[���X�V�����Ȃ�
		if (m_bInfinity == false)
		{
			m_iFrame--;
			if (m_iFrame <= 0)m_iFrame = 0;
		}

		// �����ł��낤�ƌo�߃t���[���͐}��܂�

		// �o�߃t���[��
		m_iElapsedFrame++;
		// ���݂̌o�ߎ���
		m_iRoundElapsedTime = m_iElapsedFrame / 60;

		// �ߋ��Œ�̃^�C����ۑ�
		if (m_iElapsedTime < m_iRoundElapsedTime)
		{
			m_iElapsedTime = m_iRoundElapsedTime;
		}

	}





	// �A�j���[�V����
	m_pPic->Update();
	m_pInfinityPic->Update();
}

void Timer::Render()
{
	// ���t���O�������Ă�����
	if (m_bInfinity == true)
	{
		// ����`��
		m_pInfinityPic->Render((int)m_vPos.x, (int)m_vPos.y);
	}
	else
	{
		const int second = m_iFrame / 60;

		if (m_iFrame <= 15 * 60)
		{
			m_pPic->SetARGB(256, 256, 0, 0);
		}

		m_pPic->Render((int)m_vPos.x, (int)m_vPos.y, 64, 64, (second / 10) * 64, 0, 64, 64);			// �b
		m_pPic->Render((int)m_vPos.x + 64, (int)m_vPos.y, 64, 64, (second % 10) * 64, 0, 64, 64);	// 

	}

}

//+-------------------------------------------
//	�J�����o 
//+-------------------------------------------
void Timer::Action(int delayFrame)
{
	Reset(); // �^�C�}�[������
	m_pPic->Action(delayFrame);
	m_pInfinityPic->Action(delayFrame);

}

//+-------------------------------------------
//	���Ԃ𓮂����E�~�߂�
//+-------------------------------------------
void Timer::Start()
{
	m_bStop = false;
}

void Timer::Stop()
{
	m_bStop = true;
}

void Timer::SetTimerType(ROUND_TIME_TYPE eTimeType)
{
	m_bInfinity = false;

	switch (eTimeType)
	{
	case ROUND_TIME_TYPE::SEC_30:
		m_iMaxFrame = 30;
		break;
	case ROUND_TIME_TYPE::SEC_45:
		m_iMaxFrame = 45;
		break;
	case ROUND_TIME_TYPE::SEC_60:
		m_iMaxFrame = 60;
		break;
	case ROUND_TIME_TYPE::SEC_75:
		m_iMaxFrame = 75;
		break;
	case ROUND_TIME_TYPE::SEC_99:
		m_iMaxFrame = 99;
		break;
	case ROUND_TIME_TYPE::SEC_INFINITY:
		m_iMaxFrame = 99;
		m_bInfinity = true;
		break;
	default:
		break;
	}

	Reset();
}

void  Timer::Reset()
{
	/* (TODO) �c�莞�Ԃ̓f�[�^����擾 */
	m_iFrame = m_iMaxFrame * 60;

	//m_iElapsedTime = 0;
	m_iElapsedFrame = 0;
	m_iRoundElapsedTime = 0;

}

