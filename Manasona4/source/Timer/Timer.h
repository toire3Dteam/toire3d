#pragma once
#include "TDNLIB.h"

// �^�C���Ǘ�<Singleton>
class TimeManager
{
private:
	int m_frame;		// �����̃t���[��
	tdn2DAnim* m_pic;	// �摜

	// �V���O���g���̍�@
	TimeManager():m_frame(0){}
	TimeManager(const TimeManager&){}
	TimeManager &operator=(const TimeManager&){}

public:
	static TimeManager *GetInstance(){ static TimeManager i; return &i; }

	// ������
	void  Init() 
	{
		m_pic = new tdn2DAnim("Data/UI/Game/timer.png");
		m_pic->OrderNone();
		m_pic->Action();
	};

	// ���
	void  Release() { SAFE_DELETE(m_pic); };

	// ������(�����́u�b�P�ʁv�Ŏw�肷��)
	void Reset(int second){ m_frame = second * 60; }

	// �X�V(�^�C�����Z����&�I�������Ԃ�)
	bool Update();

	// �`��
	void Render();

	// �u�t���[���P�ʁv�擾
	int GetFrame(){ return m_frame; }

	// �u�b�P�ʁv�擾
	int GetSecond(){ return m_frame / 60; }
};

#define TimeMgr (TimeManager::GetInstance())