#pragma once
#include "TDNLIB.h"

//// �^�C���Ǘ�<Singleton>
//class TimeManager
//{
//private:
//	int m_frame;		// �����̃t���[��
//	tdn2DAnim* m_pic;	// �摜
//	Vector2 m_vPos;
//
//	// �V���O���g���̍�@
//	TimeManager();
//	TimeManager(const TimeManager&){}
//	TimeManager &operator=(const TimeManager&){}
//
//public:
//	static TimeManager *GetInstance(){ static TimeManager i; return &i; }
//
//	// ������
//	void  Init();
//
//	// ���
//	void  Release() { SAFE_DELETE(m_pic); };
//
//	// ������(�����́u�b�P�ʁv�Ŏw�肷��)
//	void Reset(int second){ m_frame = second * 60; }
//
//	// �X�V(�^�C�����Z����&�I�������Ԃ�)
//	bool Update();
//
//	// �`��
//	void Render();
//
//	// �u�t���[���P�ʁv�擾
//	int GetFrame(){ return m_frame; }
//
//	// �u�b�P�ʁv�擾
//	int GetSecond(){ return m_frame / 60; }
//};
//
//#define TimeMgr (TimeManager::GetInstance())

//+-------------------------------------------
//	new �^�C�}�[
//+-------------------------------------------

class Timer
{
public:
	Timer();
	~Timer();

	// �X�V (�^�C�����Z����&�I�������Ԃ�)
	void Update();
	
	// �`��
	void Render();
	
	// ���o���s (�����́u�b�P�ʁv�Ŏw�肷��)
	void Action(int delayTimer = 0);

	// ���ԏ����� 
	void Reset();

	// ���Ԃ𓮂���
	void Start();

	// ���Ԃ��~�߂�
	void Stop();

	// �A�N�Z�T
	bool isTimeUp() { return (m_iFrame <= 0); }
	int GetElapsedTime() { return m_iElapsedTime; }

private:
	int m_iFrame;		// �t���[��
	tdn2DAnim* m_pPic;	// �摜
	Vector2 m_vPos;		// �ꏊ
	bool m_bStop;		// ���Ԃ��~�߂�t���O

	int m_iElapsedTime;			// �ꎎ���Œ�o�ߎ���
	int m_iElapsedFrame;		// �o�߃t���[��
	int m_iRoundElapsedTime;	// ���E���h���̌o�ߎ���
};
