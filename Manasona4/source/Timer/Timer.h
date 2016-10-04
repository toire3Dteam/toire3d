#pragma once

// �^�C���Ǘ�<Singleton>
class TimeManager
{
private:
	int m_frame;	// �����̃t���[��

	// �V���O���g���̍�@
	TimeManager():m_frame(0){}
	TimeManager(const TimeManager&){}
	TimeManager &operator=(const TimeManager&){}

public:
	static TimeManager *GetInstance(){ static TimeManager i; return &i; }

	// ������(�����́u�b�P�ʁv�Ŏw�肷��)
	void Reset(int second){ m_frame = second * 60; }

	// �X�V(�^�C�����Z����&�I�������Ԃ�)
	bool Update()
	{
		bool bEnd(--m_frame <= 0);
		return bEnd;
	}

	// �u�t���[���P�ʁv�擾
	int GetFrame(){ return m_frame; }

	// �u�b�P�ʁv�擾
	int GetSecond(){ return m_frame / 60; }
};

#define TimeMgr (TimeManager::GetInstance())