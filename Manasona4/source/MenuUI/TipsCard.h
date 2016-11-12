#pragma once
#include "TDNLIB.h"

//+---------------------
//	�q���g�J�[�h�N���X
//+---------------------

// �T�v
// �F�X���b�Z�\�W�Ƃ��Ă��g��
// �����@���@��������Ƃ��ɏ�������


class TipsCard
{
public:
	TipsCard(LPSTR string);
	~TipsCard();

	void Update();
	void Render();

	void Action();
	void End();
private:
	LPSTR m_pString;
	tdn2DObj* m_pCardPic;

	bool m_bRender;

};

