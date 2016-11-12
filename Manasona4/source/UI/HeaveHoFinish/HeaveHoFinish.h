#pragma once
#include "TDNLIB.h"

//+-----------------------------
//	�q�[�z�[�t�B�j�b�V���̉��o
//+-----------------------------

class HeaveHoFinish
{
private:
	// �V���O���g���̍�@
	HeaveHoFinish();
	~HeaveHoFinish();
	HeaveHoFinish(const HeaveHoFinish&);
	HeaveHoFinish &operator=(const HeaveHoFinish&) {}

	// 1�����Ȃ�����
	static HeaveHoFinish *m_pInstance;

public:
	// ���̎擾
	static HeaveHoFinish *GetInstance()
	{
		if (!m_pInstance) m_pInstance = new HeaveHoFinish;
		return m_pInstance;
	}
	static void Rerease() {
		SAFE_DELETE(m_pInstance);
	}

	void Update();
	void Render();
	void Action();

	// �����̃t���O�������Ă�����Q�[���̓������~�߂Ă�������
	bool IsAction() { return m_bActionFlag; }

private:
	tdn2DAnim* m_pHeaveho;
	tdn2DAnim* m_pHeavehoShink;
	tdn2DAnim* m_pBlueRing;
	tdn2DAnim* m_pBlueRingNext;
	tdn2DAnim* m_pLight;
	tdn2DAnim* m_pStarDust;
	Vector2	   m_pStarDustPos;

	bool m_bActionFlag;
	int m_iFrame;

	float m_fPicAngle;

	bool m_bRippleFlag;

	void  Trigger();
};

#define HeaveHoFinishUI (HeaveHoFinish::GetInstance())