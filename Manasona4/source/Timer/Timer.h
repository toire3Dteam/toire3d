#pragma once
#include "TDNLIB.h"

// �^�C���Ǘ�<Singleton>
class TimeManager
{
private:
	int m_frame;		// �����̃t���[��
	tdn2DAnim* m_pic;	// �摜
	Vector2 m_vPos;

	// �V���O���g���̍�@
	TimeManager();
	TimeManager(const TimeManager&){}
	TimeManager &operator=(const TimeManager&){}

public:
	static TimeManager *GetInstance(){ static TimeManager i; return &i; }

	// ������
	void  Init();

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